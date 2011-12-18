#include "file.h"
#include "dir.h"

File::File(Directory *parent, string name, unsigned long pos, unsigned long size, unsigned long next, unsigned long offset) 
	: Entry(parent, name, pos, size, next, offset)
{
}

File::~File()
{
}

void File::Print() const
{
	cout << setw(10) << right << this->iSize;
	cout << " " << this->name << endl;
}


void File::FullPrint() const
{
	cout << setw(10) << right << this->iOffset;
	cout << " ";
	cout << setw(10) << right << this->iPosition;
	cout << " ";
	cout << setw(10) << right << this->iNext;
	cout << " ";
	cout << setw(10) << right << this->iSize;
	cout << " ";
	cout << this->FullPath() << endl;
}

