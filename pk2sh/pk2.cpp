#include <boost/filesystem/path.hpp>
#include <queue>

#include "util.h"
#include "pk2.h"
#include "empty.h"
#include "dir.h"
#include "file.h"
#include "blowfish.h"

PK2::PK2()
{
	PTR_INIT(pRoot);
	PTR_INIT(pCurrent);
	PTR_INIT(pFile);

	this->pBf = new CBlowFish();
	this->pBf->Initialize((unsigned char *)PK2_KEY, strlen(PK2_KEY));
}

PK2::~PK2()
{
	this->Close();
	PTR_DELETE(this->pBf);
}

bool PK2::oOpen(const string filename, bool dump)
{
	pRoot = new Directory(NULL, "/", 0, 0x100, 0);

	pFile = fopen(filename.c_str(), "r+");
	if (!pFile)
		return FALSE;

	if (dump)
	{
		fs::path file(filename);
		string dump_name(file.leaf().string() + ".dump");
		pLog.open(dump_name.c_str());

		pLog << "             [Offset    ] [Position  ] [Size      ] [Next      ] Filename" << endl;
	}

	this->pCurrent = this->pRoot;
	this->Recurse(0x100, this->pRoot, dump);

	return TRUE;
}

void PK2::Close()
{
	if (this->pFile)
		fclose(pFile);

	if (!pLog.bad())
	{
		pLog.flush();
		pLog.close();
	}

	PTR_DELETE(pRoot);
	PTR_INIT(pFile);
	PTR_INIT(pCurrent);
}

bool PK2::oRebuild(const string filename) 
{
	if (fs::exists(filename))
		return FALSE;

	fs::ofstream file(filename);
	file.close();

	return TRUE;
}

void PK2::WritePK2Entry(Entry *entry)
{
	unsigned char raw[PK2_ENTRY_SIZE];
	unsigned char data[PK2_ENTRY_SIZE];

	this->SerializeEntry(entry, raw);
	this->pBf->Encode(raw, data, PK2_ENTRY_SIZE);
		
	fseek(this->pFile, entry->getOffset(), SEEK_SET);
	fwrite(data, PK2_ENTRY_SIZE, 1, this->pFile);
	fflush(this->pFile);
}

void PK2::Recurse(unsigned long start, Directory *parent, bool dump)
{
	queue<Entry *> queue;

	Directory *oldParent;
	unsigned char raw[PK2_ENTRY_SIZE];
	unsigned char data[PK2_ENTRY_SIZE];

	Entry *entry;
	oldParent = this->pCurrent;
	this->pCurrent = parent;

	if (dump)
		pLog << "[BLOCK BEGIN]" << endl;

	for (int i = 0; i<PK2_ENTRIES; i++)
	{
		unsigned long current = start+(i*PK2_ENTRY_SIZE);
		fseek(pFile, current, SEEK_SET);
		fread(raw, PK2_ENTRY_SIZE, 1, pFile);
		this->pBf->Decode(raw, data, PK2_ENTRY_SIZE);

		entry = this->pNextEntry(data, current);

		if (dump)
			pLog << "Entry: ";
		
		if (entry)
		{
			if (dump)
			{
				if (entry->eGetType() == PK2_GARBAGE)
					pLog << "EMPTY ";
				else if (entry->eGetType() == PK2_FILE)
					pLog << " FILE ";
				else if (entry->eGetType() == PK2_DIR)
					pLog << "  DIR ";

				pLog << "[" << setw(10) << right << entry->getOffset() << "] ";
				pLog << "[" << setw(10) << right << entry->getPosition() << "] ";
				pLog << "[" << setw(10) << right << entry->getSize() << "] ";
				pLog << "[" << setw(10) << right << entry->getNext() << "] ";
				pLog << entry->getName();
			}

			this->pCurrent->Add(entry);

			if (entry->eGetType() == PK2_DIR && entry->getPosition() != current && entry->getName() != "..")
				queue.push(entry);
			else 
			{
				if (entry->getNext()) 
					queue.push(entry);
			}
		}

		if (dump)
			pLog << endl;
	}

	if (dump)
	{
		pLog << "[BLOCK END]" << endl;
		pLog << "==FILE DATA==" << endl;
		pLog.flush();
	}

	while (queue.size())
	{
		Entry *entry = (queue.front());

		if (entry->getNext())
		{
			this->Recurse(entry->getNext(), this->pCurrent, dump);
		}
		else
		{
			this->Recurse(entry->getPosition(), (Directory *)entry, dump);
			this->pCurrent = oldParent;
		}
		queue.pop();
	}
}

void PK2::List(int flags) const
{
	if (this->pCurrent)
	{
		if (HAS_FLAG(flags, LIST_COMPLETE))
			cout << "[Offset  ] [Position] [Next    ] ";

		cout << "[Size    ] [Full path]" << endl;
		this->pCurrent->List(flags);
	}
}

string PK2::Change(string name)
{
	if (this->pCurrent)
	{
		if (name == ".")
			return this->pCurrent->getName();

		if (name == "..")
		{
			this->pCurrent = this->pCurrent->getParent();

			if (this->pCurrent == NULL)
				this->pCurrent = this->pRoot;

			return this->pCurrent->getName();
		}
	
		if (this->pCurrent->oHasEntry(name))
		{
			this->pCurrent = (Directory *)this->pCurrent->pGet(name);
			return name;
		}
	}

	return "";
}

string PK2::getCurrentPath() const
{
	if (this->pCurrent)
		return this->pCurrent->FullPath();	

	return NULL;
}

bool PK2::oDump(const string file) const
{
	if (this->pFile && pCurrent->oHasEntry(file))
	{
		Entry *entry = pCurrent->pGet(file);

		if (entry->eGetType() != PK2_FILE)
			return false;

		fseek(pFile, entry->getPosition(), SEEK_SET);
		
		unsigned char *buffer;
		int length = entry->getSize() + 1;

		buffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
		memset(buffer, '\0', length);
		fread(buffer, entry->getSize(), 1, pFile);
		fprintf(stdout, "%s\n", buffer);

		PTR_FREE(buffer);

		return true;
	}
	return false;
}

bool PK2::oHexDump(const string file) const
{
	if (this->pFile && pCurrent->oHasEntry(file))
	{
		Entry *entry = pCurrent->pGet(file);

		if (entry->eGetType() != PK2_FILE)
			return false;

		fseek(pFile, entry->getPosition(), SEEK_SET);

		unsigned char *buffer;
		int length = entry->getSize();
		buffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
		fread(buffer, length, 1, pFile);
		for (int i=0; i<length; i++)
		{
			for (int j=0; j<16 && i<length; j++, i++)
				fprintf(stdout, "%02x ", buffer[i]);
			fprintf(stdout, "\n");
		}
		PTR_FREE(buffer);
		return true;
	}
	return false;
}

bool PK2::oFileExtract(const string file) const
{
	if (this->pFile && pCurrent->oHasEntry(file))
	{
		Entry *entry = pCurrent->pGet(file);

		if (entry->eGetType() != PK2_FILE)
			return false;

		fseek(pFile, entry->getPosition(), SEEK_SET);

		unsigned char *buffer;
		int length = entry->getSize();
		buffer = (unsigned char *)malloc(sizeof(unsigned char) * length);
		fread(buffer, length, 1, pFile);

		FILE *fp = fopen(file.c_str(), "w+");
		fwrite(buffer, length, 1, fp);
		fflush(fp);
		fclose(fp);

		PTR_FREE(buffer);
		return true;
	}
	return false;
}

int PK2::iFileImport(const string dest, const string input)
{
	if (this->pFile && pCurrent->oHasEntry(dest))
	{
		fs::path finput(input);
		if (!fs::exists(finput))
			return -1; // Input file not found/doesnt exist.

		if (fs::is_directory(finput))
			return -2; // Input not a file.

		boost::intmax_t size = fs::file_size(finput);

		Entry *entry = pCurrent->pGet(dest);
		if (entry->eGetType() != PK2_FILE)
			return -4; // Destination not a file.

		FILE *fp = fopen(input.c_str(), "r+");
		unsigned char *buffer;
		buffer = (unsigned char *)malloc(sizeof(unsigned char) * size);
		if (buffer == NULL)
		{
			cout << "not enought memory to allocate file." << endl;
			return -2;
		}
		fread(buffer, size, 1, fp);
		fclose(fp);

		int esize = entry->getSize();
		entry->setSize(size);
		if (size <= esize)
		{
			cout << "file fit in current slot." << endl;

			fseek(this->pFile, entry->getPosition(), SEEK_SET);
			fwrite(buffer, size, 1, this->pFile);

			int diff = esize - size;
			if (diff)
			{
				PTR_FREE(buffer);
				buffer = (unsigned char *)malloc(sizeof(unsigned char) * diff);
				memset(buffer, '\0', diff);
				fwrite(buffer, diff, 1, this->pFile);
			}
		}
		else
		{
			cout << "file doesnt fit in current slot." << endl;

			fseek(this->pFile, 0, SEEK_END);
			unsigned long pos = ftell(this->pFile);

			fwrite(buffer, size, 1, this->pFile);

			memset(buffer, '\0', esize);
			fseek(this->pFile, entry->getPosition(), SEEK_SET);
			fwrite(buffer, esize, 1, this->pFile);

			entry->setPosition(pos);
		}
		fflush(this->pFile);
		PTR_FREE(buffer);
		this->WritePK2Entry(entry);

		cout << "done." << endl;
		return EXIT_SUCCESS;
	}
	
	return -3; // Destination file not found/doesnt exist.
}

Entry *PK2::pNextEntry(unsigned char *data, unsigned long offset) const
{
	unsigned char type;
	char name[89];
	unsigned long *pos;
	unsigned long *size;
	unsigned long *next;
	
	memset(name, '\0', 89);

	type = data[0];
	memcpy(name, &data[1], 89);
	pos = (unsigned long *)(data+106);
	size = (unsigned long *)(data+114); 
	next = (unsigned long *)(data+118);
	string nameStr = string(name);

	Entry *entry = NULL;
	if (type == PK2_DIR)
		entry = new Directory(this->pCurrent, nameStr, *pos, *next, offset);
	else if (type == PK2_FILE) 
		entry = new File(this->pCurrent, nameStr, *pos, *size, *next, offset);
	else
		entry = new Empty(this->pCurrent, *pos, *size, *next, offset);

	return entry;
}

void PK2::SerializeEntry(Entry *entry, unsigned char *buffer)
{
	memset(buffer, '\0', PK2_ENTRY_SIZE);

	buffer[0] = entry->eGetType();
	memcpy((buffer+1), (unsigned char *)entry->getName().c_str(), entry->getName().size());
	write_long(entry->getPosition(), buffer+106);
	write_long(entry->getSize(), buffer+114);
	write_long(entry->getNext(), buffer+118);
}


