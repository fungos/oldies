#ifndef __PK2_EMPTY__
#define __PK2_EMPTY__

#include "defs.h"
#include "entry.h"

class Directory;

class Empty : public Entry
{
	public:
		static int counter;
	private:
		Directory *pParent;
	public:
		Empty(Directory *parent, unsigned long iPosition, unsigned long iSize, unsigned long iNext, unsigned long iOffset);
		~Empty();

		virtual void Print() const;
		virtual void FullPrint() const;

		virtual entryType eGetType() const
		{
			return PK2_GARBAGE;
		}
};

#endif

