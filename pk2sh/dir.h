#ifndef __PK2_DIR__
#define __PK2_DIR__

#include "defs.h"
#include "entry.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>

using boost::multi_index_container;
using namespace ::boost::multi_index;

typedef multi_index_container<
	Entry *, 
	indexed_by<
		ordered_non_unique< const_mem_fun<Entry, unsigned long, &Entry::getSize> >,
		ordered_non_unique< const_mem_fun<Entry, unsigned long, &Entry::getOffset> >,
		ordered_non_unique< const_mem_fun<Entry, unsigned long, &Entry::getPosition> >,
		hashed_unique< const_mem_fun<Entry, string, &Entry::getName> >
	>
> EntryCollection;

#define BY_SIZE 0
#define BY_OFFSET 1
#define BY_POSITION 2
#define BY_NAME 3

typedef EntryCollection::nth_index<BY_SIZE>::type EntryBySize;
typedef EntryBySize::iterator EntryBySizeIterator;

typedef EntryCollection::nth_index<BY_OFFSET>::type EntryByOffset;
typedef EntryByOffset::iterator EntryByOffsetIterator;

typedef EntryCollection::nth_index<BY_POSITION>::type EntryByPosition;
typedef EntryByPosition::iterator EntryByPositionIterator;

typedef EntryCollection::nth_index<BY_NAME>::type EntryByName;
typedef EntryByName::iterator EntryByNameIterator;

typedef EntryCollection::const_iterator EntryCollectionIterator;


class Directory : public Entry
{
	private:
		EntryCollection entries;

		void Print(Entry *entry, int flags) const;
	public:
		Directory(Directory *parent, string name, unsigned long position, unsigned long next, unsigned long offset);
		~Directory();

		void Add(Entry *entry);
		void List(int flags) const;

		Entry *pGet(const string name) const;
		Directory *pGetParent() const;
		
		bool oHasEntry(const string name) const;

		virtual void Print() const;
		virtual void FullPrint() const;

		virtual entryType eGetType() const
		{
			return PK2_DIR;
		}
};

#endif

