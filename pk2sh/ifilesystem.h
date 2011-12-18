#ifndef __FILESYSTEM_INTERFACE__
#define __FILESYSTEM_INTERFACE__

#include <string>

#define LIST_COMPLETE		0x0001
#define LIST_RECURSIVE		0x0002
#define LIST_SHOW_EMPTY 	0x0004
#define LIST_SHOW_DIR		0x0008
#define LIST_SHOW_FILE		0x0010
#define LIST_SORT_OFFSET	0x1000
#define LIST_SORT_POSITION	0x2000
#define LIST_SORT_SIZE		0x4000

using namespace std;

class IFilesystem
{
	public:
		IFilesystem() {};
		virtual ~IFilesystem() {};

		virtual void Close() = 0;
		virtual void List(int flags) const = 0;
		
		virtual bool oOpen(const string filename, bool dump) = 0;
		virtual bool oDump(const string file) const = 0;
		virtual bool oHexDump(const string file) const = 0;
		virtual bool oFileExtract(const string file) const = 0;
		virtual bool oRebuild(const string filename) = 0;

		virtual int  iFileImport(const string dest, const string input) = 0;

		virtual string Change(string name) = 0;
		virtual string getCurrentPath() const = 0;
};

#endif

