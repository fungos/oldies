#include "entry.h"
#include "file.h"
#include "dir.h"

#include <iostream>
using namespace std;


Entry::Entry(Directory *parent, string name, unsigned long pos, unsigned long size, unsigned long next, unsigned long offset) 
	: pParent(parent), iPosition(pos), iSize(size), iNext(next), iOffset(offset)
{
	this->name = name;
}

Entry::~Entry()
{
	PTR_INIT(pParent);
	iPosition = 0;
	iSize = 0;
	iNext = 0;
}

void Entry::setName(string name)
{
	this->name = name;
}

string Entry::getName() const
{
	return this->name;
}

void Entry::setPosition(unsigned long pos)
{
	this->iPosition = pos;
}

unsigned long Entry::getPosition() const
{
	return this->iPosition;
}

void Entry::setSize(unsigned long size)
{
	this->iSize = size;
}

unsigned long Entry::getSize() const
{
	return this->iSize;
}

void Entry::setNext(unsigned long next)
{
	this->iNext = next;
}

unsigned long Entry::getNext() const
{
	return this->iNext;
}

void Entry::setParent(Directory *parent)
{
	this->pParent = parent;
}

Directory *Entry::getParent() const
{
	return this->pParent;
}

void Entry::setOffset(unsigned long offset)
{
	this->iOffset = offset;
}

unsigned long Entry::getOffset() const
{
	return this->iOffset;
}


string Entry::FullPath() const
{
	if (this->pParent != NULL)
		return this->pParent->FullPath() + "/" + this->name;
	else
		return string("");
}

