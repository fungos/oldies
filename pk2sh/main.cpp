#include <boost/program_options.hpp>
#include <string>
#include <iostream>

#include "defs.h"
#include "ifilesystem.h"
#include "pk2.h"
#include "console.h"

#define VERSION	"0.9.5"

namespace po = boost::program_options;
using namespace std;


void header()
{
	fprintf(stdout, "PK2Shell %s by Funghi Kingdom\n", VERSION);
	fprintf(stdout, "Fungos Bauux (danny.cabelo at gmail.com)\n");
}


int main(int argc, char **argv)
{
	po::options_description desc("Command Line Options");
	desc.add_options()
		("help,h", "produce help message")
		("version,v", "show version")
		("dump,d", po::value<string>(), "dump package data")
		("mount,m", po::value<string>(), "mount package");

	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch (po::unknown_option o)
	{
	}

	if (vm.count("version"))
	{
		header();
		return EXIT_SUCCESS;
	}
	
	if (vm.count("help"))
	{
		header();
		cout << desc << endl;
		return EXIT_FAILURE;
	}

	IFilesystem *package = new PK2();
	if (vm.count("dump"))
	{
		package->oOpen(vm["dump"].as<string>(), TRUE);
		package->Close();
		return EXIT_SUCCESS;
	}

	header();
	fprintf(stdout, "run 'help' to see more commands.\n\n");
	Console *shell = new Console(package);

	if (vm.count("mount"))
	{
		package->oOpen(vm["mount"].as<string>(), FALSE);
		shell->SetCurrentMount(strdup(vm["mount"].as<string>().c_str()));
	}

	shell->Run();
	PTR_DELETE(shell);
	PTR_DELETE(package);

	return EXIT_SUCCESS;
}

