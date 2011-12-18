#ifndef __PK2__
#define __PK2__

#include "defs.h"
#include "ifilesystem.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>

#define 	PK2_ENTRY_SIZE	0x80
#define		PK2_ENTRIES	0x14

#define		PK2_KEY		"164891"
//unsigned char pk2Key[] = {0x31, 0x36, 0x34, 0x38, 0x39, 0x31};

class Directory;
class Entry;
class CBlowFish;

namespace fs = boost::filesystem;
namespace io = boost::iostreams;
using namespace std;


class PK2 : public IFilesystem
{
	private:
		FILE *pFile;

		Directory *pRoot;
		Directory *pCurrent;
		CBlowFish *pBf;

		std::ofstream pLog;

		void Recurse(unsigned long start, Directory *parent, bool dump);
		Entry *pNextEntry(unsigned char *data, unsigned long offset) const;
		void SerializeEntry(Entry *entry, unsigned char *buff);
		void WritePK2Entry(Entry *entry);

	public:
		PK2();
		virtual ~PK2();
		
		virtual void Close();
		virtual void List(int flags) const;

		virtual bool oOpen(const string file, bool dump);
		virtual bool oDump(const string file) const;
		virtual bool oHexDump(const string file) const;
		virtual bool oFileExtract(const string file) const;
		virtual bool oRebuild(const string file);

		virtual int  iFileImport(const string dest, const string input);

		virtual string Change(string name);
		virtual string getCurrentPath() const;
};

#endif

