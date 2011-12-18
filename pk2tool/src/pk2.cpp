#include "pk2.h"
#include "pk2lib.h"
#include "log.h"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>

namespace fs = boost::filesystem;

PK2::PK2(string &package)
{
	Log::Info("Opening %s.\n", package.c_str());
	this->package = package;
	int ret = PK2Init(package.c_str(), &this->pk2handler);
	
	if (!ret)
	{
		Log::Error("Failed to open '%s' package.\n", package.c_str());
		this->pk2handler = 0;
	}
}

PK2::~PK2()
{
}

void PK2::SetImportPath(string &path)
{
	this->importPath = fs::path(path);
	if (!fs::exists(this->importPath))
	{
		this->importPath = fs::initial_path()/path;
		if (!fs::exists(this->importPath))
		{
			Log::Error("Import path '%s' could not be found, using current.\n", this->importPath.string().c_str());
			this->importPath = fs::initial_path();
		}
	}
}

void PK2::SetExportPath(string &path)
{
	this->exportPath = fs::path(path);
	if (!fs::exists(this->exportPath))
	{
		this->exportPath = fs::initial_path()/path;
		if (!fs::exists(this->exportPath))
		{
			Log::Error("Export path '%s' could not be found, using current.\n", this->exportPath.string().c_str());
			this->exportPath = fs::initial_path();
		}
	}
}

int PK2::OpenFile(string &filename)
{
	if (this->pk2handler == 0)
		return 0;

	return 1;
}

bool PK2::CloseFile(int file)
{
	if (this->pk2handler == 0)
		return false;

	return true;
}

int PK2::GetFileSize(int file)
{
	if (this->pk2handler == 0)
		return 0;

	return 1;
}

bool PK2::ReadFile(int file, unsigned char *buffer, int size)
{
	if (this->pk2handler == 0)
		return false;

	int nothing;
	PK2ReadFile(this->pk2handler, file, buffer, size, &nothing);

	return true;
}

bool PK2::Export(string &filename)
{
	if (this->pk2handler == 0)
		return false;

	int nothing;

	fs::path to = (this->exportPath/filename);
	Log::Info("Exporting [%s]%s to %s\n", package.c_str(), filename.c_str(), to.string().c_str());

	to = to.branch_path();
	if (!fs::exists(to))
	{
		Log::Error("Directory %s doesnt exists, creating it.\n", to.string().c_str());
		fs::create_directory(to);
	}

	fs::path from(filename);
	string file = from.leaf().string();
	from = from.branch_path();
	
	Log::Error("from: %s\nTo: %s\nFile: %s\n", from.string().c_str(), to.string().c_str(), file.c_str());

	int result = PK2WriteFileToFilesystem(this->pk2handler, to.string().c_str(), from.string().c_str(), file.c_str(), &nothing);

	if (!result)
	{
		Log::Error("Export: Something is wrong. Sorry, try again or read the documentation.\n");
		return false;
	}

	return true;
}

bool PK2::Import(string &filename)
{
	if (this->pk2handler == 0)
		return false;

	int nothing;

	fs::path from = (this->importPath/filename);
	if (!fs::exists(from))
	{
		Log::Error("File %s not found.\n", from.string().c_str());
		return false;
	}
	Log::Info("Importing %s to [%s]%s.\n", from.string().c_str(), package.c_str(), filename.c_str());

	from = from.branch_path();
	fs::path to(filename);
	int result = PK2ReadFileFromFilesystem(this->pk2handler, from.string().c_str(), to.string().c_str(), to.leaf().c_str(), &nothing);

	if (!result)
	{
		Log::Error("Import: Something is wrong. Sorry, try again or read the documentation.\n");
		return false;
	}

	return true;
}

