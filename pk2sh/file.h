#ifndef __PK2_FILE__
#define __PK2_FILE__

#include "defs.h"
#include "entry.h"

#include <string>

using namespace std;


class Directory;

class File : public Entry
{
	private:
		Directory *pParent;
	public:
		File(Directory *parent, string pcName, unsigned long iPosition, unsigned long iSize, unsigned long iNext, unsigned long offset);
		~File();

		virtual void Print() const;
		virtual void FullPrint() const;
 
		virtual entryType eGetType() const
		{
			return PK2_FILE;
		}
};

#endif

