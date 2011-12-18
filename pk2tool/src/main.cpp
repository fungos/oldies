#include <boost/program_options.hpp>
#include <string>
#include <iostream>
#include <fstream>

#include "defs.h"
#include "log.h"
#include "pk2.h"

#define VERSION	"0.1.0"

namespace po = boost::program_options;
using namespace std;


void header()
{
	Log::Info("PK2Tool %s by Bucaneiros\n", VERSION);
	Log::Info("Fungos Bauux (danny.cabelo at gmail.com)\n");
	Log::Info("http://danny.bitcrafters.com.br/\n");
	Log::Info("use '--help' to see more commands.\n\n");
}

int main(int argc, char **argv)
{
	po::options_description desc("Command Line Options");
	desc.add_options()
		("help,h", "produce help message")
		("import,i", po::value<string>(), "import a file to package")
		("export,e", po::value<string>(), "export a file from package")
		("package,p", po::value<string>(), "package to use");

	po::options_description conf("Configuration Options");
	conf.add_options()
		("export_path", po::value<string>(), "where to save extracted files.")
		("import_path", po::value<string>(), "from where to import files.")
		("package", po::value<string>(), "default package to use"); 

	po::options_description options("Allowed options");
	options.add(desc).add(conf);

	ifstream file("pk2tool.cfg");
	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::store(po::parse_config_file(file, conf), vm);
		po::notify(vm);
	}
	catch (po::unknown_option o)
	{
		cout << o.what() << endl;
		return EXIT_FAILURE;
	}

	header();
	if (vm.count("help"))
	{
		cout << options << endl;
		return EXIT_FAILURE;
	}

	string import_path = "import";
	if (vm.count("import_path"))
		import_path = vm["import_path"].as<string>();

	string extract_path = "export";
	if (vm.count("export_path"))
		extract_path = vm["export_path"].as<string>();

	string pk2;
	if (vm.count("package"))
		pk2 = vm["package"].as<string>();
	else
	{
		Log::Error("'package' option is missing, you can pass one by command line or by configuration file.\n");
		return EXIT_FAILURE;
	}

	if (vm.count("import"))
	{
		string file(vm["import"].as<string>());
		PK2 *package = new PK2(pk2);

		package->SetImportPath(import_path);
		if (package->Import(file))
		{
			Log::Info("Done.\n");
			return EXIT_SUCCESS;
		}

		Log::Error("Import failed or file doesnt exist.\n");
		return EXIT_FAILURE;
	}

	if (vm.count("export"))
	{
		string file(vm["export"].as<string>());
		PK2 *package = new PK2(pk2);

		package->SetExportPath(extract_path);
		if (package->Export(file))
		{
			Log::Info("Done.\n");
			return EXIT_SUCCESS;
		}

		Log::Error("Export failed or file doesnt exist.\n");
		return EXIT_FAILURE;
}

	return EXIT_SUCCESS;
}

