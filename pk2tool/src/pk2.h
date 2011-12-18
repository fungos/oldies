#ifndef __PK2__
#define __PK2__

#include "defs.h"
#include <boost/filesystem/fstream.hpp>
//#include <boost/filesystem/operations.hpp>
//#include <iostream>

namespace fs = boost::filesystem;
using namespace std;

class PK2
{
	private:
		int pk2handler;
		string package;
		fs::path importPath;
		fs::path exportPath;

	public:
		PK2(string &package);
		~PK2();

		void SetImportPath(string &path);
		void SetExportPath(string &path);

		/* File functions */
		int OpenFile(string &filename);
		bool CloseFile(int file);
		int GetFileSize(int file);
		bool ReadFile(int file, unsigned char *buffer, int size);

		/* Export some file from package to filesystem */
		bool Export(string &filename);

		/* Import some file from filesystem to the package */
		bool Import(string &filename);
};

#endif

