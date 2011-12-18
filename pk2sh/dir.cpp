#include "ifilesystem.h"
#include "file.h"
#include "dir.h"

Directory::Directory(Directory *parent, string name, unsigned long position, unsigned long next, unsigned long offset) 
	: Entry(parent, name, position, 0xa00, next, offset)
{
}


Directory::~Directory()
{
	PTR_INIT(pParent);
}


void Directory::Add(Entry *entry)
{
	entries.insert(entry);
}


void Directory::List(int flags) const
{
	//foreach(item, this->entries, EntryCollectionIterator)
	if (HAS_FLAG(flags, LIST_SORT_POSITION))
	{
		foreach_by(BY_POSITION, item, this->entries, EntryByPositionIterator)
			this->Print((*item), flags);
		return;
	}

	if (HAS_FLAG(flags, LIST_SORT_SIZE))
	{
		foreach_by(BY_SIZE, item, this->entries, EntryBySizeIterator)
			this->Print((*item), flags);
		return;
	}

	foreach_by(BY_OFFSET, item, this->entries, EntryByOffsetIterator)
		this->Print((*item), flags);
}


void Directory::Print(Entry *entry, int flags) const
{
	if (entry->eGetType() == PK2_GARBAGE && !HAS_FLAG(flags, LIST_SHOW_EMPTY))
		return;

	if (entry->eGetType() == PK2_FILE && !HAS_FLAG(flags, LIST_SHOW_FILE))
		return;

	if (HAS_FLAG(flags, LIST_RECURSIVE))
	{
		if ((entry->eGetType() == PK2_DIR && HAS_FLAG(flags, LIST_SHOW_DIR)) || entry->eGetType() != PK2_DIR)
		{	
			if (HAS_FLAG(flags, LIST_COMPLETE))
				entry->FullPrint();
			else
				entry->Print();
		}

		if (entry->eGetType() == PK2_DIR)
			((Directory *)entry)->List(flags);
	}
	else
	{
		if (entry->eGetType() == PK2_DIR && !HAS_FLAG(flags, LIST_SHOW_DIR))
			return;

		if (HAS_FLAG(flags, LIST_COMPLETE))
			entry->FullPrint();
		else
			entry->Print();
	}
}


void Directory::Print() const
{
	cout << setw(10) << right << this->iSize;
	cout << " " << this->name << "/" << endl;
}


void Directory::FullPrint() const
{
	cout << setw(10) << right << this->iOffset;
	cout << " ";
	cout << setw(10) << right << this->iPosition;
	cout << " ";
	cout << setw(10) << right << this->iNext;
	cout << " ";
	cout << setw(10) << right << this->iSize;
	cout << " ";
	cout << this->FullPath();
	cout << "/";
	cout << endl;
}


bool Directory::oHasEntry(const string name) const
{
	EntryByNameIterator elem = this->entries.get<BY_NAME>().find(name), end = this->entries.get<BY_NAME>().end();
	return (elem != end);
}


Entry *Directory::pGet(const string name) const
{
	EntryByNameIterator it = this->entries.get<BY_NAME>().find(name);
	return (*it);
}


Directory *Directory::pGetParent() const
{
	return this->pParent;
}

