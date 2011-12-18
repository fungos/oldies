#ifndef __PK2_ENTRY__
#define __PK2_ENTRY__

#include "defs.h"

#include <string>

using namespace std;

enum entryType
{
	PK2_GARBAGE = 0x00,
	PK2_DIR = 0x01,
	PK2_FILE = 0x02,
};

class Directory;

class Entry
{
	protected:
		Directory *pParent;
		string name;

		unsigned long iPosition;
		unsigned long iSize;
		unsigned long iNext;
		unsigned long iOffset;

	public:
		Entry(Directory *pParent, string pcName, unsigned long iPosition, unsigned long iSize, unsigned long iNext, unsigned long iOffset);
	 	virtual ~Entry();

		void setName(string name);
		string getName() const;

		void setPosition(unsigned long pos);
		unsigned long getPosition() const;

		void setSize(unsigned long size);
		unsigned long getSize() const;

		void setNext(unsigned long next);
		unsigned long getNext() const;

		void setParent(Directory *pParent);
		Directory *getParent() const;

		void setOffset(unsigned long offset);
		unsigned long getOffset() const;

		string FullPath() const;

		virtual void Print() const = 0;
		virtual void FullPrint() const = 0;
		virtual entryType eGetType() const = 0;
};

#endif

