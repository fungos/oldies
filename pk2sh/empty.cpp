#include "empty.h"
#include "dir.h"

int Empty::counter = 0;

Empty::Empty(Directory *parent, unsigned long pos, unsigned long size, unsigned long next, unsigned long offset) 
	: Entry(parent, "", pos, size, next, offset)
{
	char count[20];
	snprintf(count, 20, "%d", Empty::counter++);
	string scount(count);
	this->name = "[empty-" + scount + "]";
}

Empty::~Empty()
{

}

void Empty::Print() const
{
	cout << setw(10) << right << this->iSize;
	cout << " " << this->name << endl;
}


void Empty::FullPrint() const
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

