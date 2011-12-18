#include <boost/program_options.hpp>
#include <string>

#include "ifilesystem.h"
#include "console.h"
#include "util.h"

namespace po = boost::program_options;
using namespace std;

static CommandEntry gCommandList[] =
{
	{"ls", &Console::ls},
	{"cd", &Console::chdir},
	{"pwd", &Console::pwd},
	{"cat", &Console::cat},
	{"help", &Console::help},
	{"exit", &Console::exit},
	{"bcat", &Console::bcat},
	{"mount", &Console::mount},
	{"umount", &Console::umount},
	{"export", &Console::extract},
	{"defrag", &Console::defrag},
	{"import", &Console::import},
	{NULL, NULL},
};


Console::Console(IFilesystem *fs) : pclFilesystem(fs)
{
	iQuit = 0;
	PTR_INIT(this->pcCurrentMount);
	this->pcPrompt = strdup("/$ ");
	this->_pStdErr = stderr;
	this->_pStdOut = stdout;
	this->_pStdIn = stdin;
}


Console::~Console()
{
	PTR_FREE(this->pcCurrentMount);
	PTR_FREE(this->pcPrompt);
	PTR_INIT(this->pclFilesystem);
	iQuit = 0;
}


void Console::Run()
{
	char *params[32], *command;
	int argc;
	
	while (!this->iQuit)
	{
		argc = -1;
		command = readline(this->pcPrompt);
		argc = cli_tokens(command, params); 
		this->iLastReturnCode = this->iExecute(argc, params);
		PTR_FREE(command);
	}
}


int Console::iExecute(int argc, char **params)
{
	if (argc<1 || !strlen(params[0]))
		return EXIT_SUCCESS;

	for (int i=0; gCommandList[i].pfnProc != NULL; i++)
	{
		if (strcmp(gCommandList[i].pcName, params[0]) == 0)
			return gCommandList[i].pfnProc(argc, params, this);
	}

	fprintf(this->pStdErr(), "pk2sh: %s: command not found\n", params[0]);

	return EXIT_FAILURE;
}


void Console::Quit()
{
	this->iQuit = 1;
}


void Console::SetPrompt(char *prompt)
{
	PTR_FREE(this->pcPrompt);
	this->pcPrompt = prompt;
}


void Console::SetCurrentMount(char *mount)
{
	PTR_FREE(this->pcCurrentMount);
	this->pcCurrentMount = mount;
}


void Console::SetFilesystem(IFilesystem *fs)
{
	PTR_DELETE(this->pclFilesystem);
	this->pclFilesystem = fs;
}


/*
 * Console commands
 *
 */
int Console::exit(int argc, char **argv, Console *env)
{
	env->Quit();
	return EXIT_SUCCESS;
}


int Console::help(int argc, char **argv, Console *env)
{
	fprintf(env->pStdOut(), "commands:\n");
	fprintf(env->pStdOut(), "  %s\t\tthis help.\n", argv[0]);
	fprintf(env->pStdOut(), "  cd  \t\tchange dir.\n");
	fprintf(env->pStdOut(), "  ls  \t\tlist dir, use -h to see options.\n");
	fprintf(env->pStdOut(), "  pwd \t\tdisplay current path.\n");
	fprintf(env->pStdOut(), "  cat\t\tdisplay file content in text mode.\n");
	fprintf(env->pStdOut(), "  bcat\t\tdisplay file content in hex mode.\n");
	fprintf(env->pStdOut(), "  mount\t\topen a pk2 file.\n");
	fprintf(env->pStdOut(), "  umount\tclose the current file.\n");
	fprintf(env->pStdOut(), "  export\textract a file from pk2.\n");
	fprintf(env->pStdOut(), "  rebuild\tgenerate a new pk2 from current mounted one.\n");
	fprintf(env->pStdOut(), "  exit\tquit pk2sh.\n");

	return EXIT_SUCCESS;
}


int Console::chdir(int argc, char **argv, Console *env)
{
	char *prompt;
	string dir;

	if (argc == 2)
	{
		if ((dir = env->pclGetFilesystem()->Change(argv[1])) != "")
		{
			if (dir != "")
				prompt = strdupcat(env->pclGetFilesystem()->getCurrentPath().c_str(), "$ ");
			else
				prompt = strdup("/$ ");
			
			env->SetPrompt(prompt);
		}
		else
			fprintf(env->pStdErr(), "pk2sh: %s: %s: No such directory\n", argv[0], argv[1]);
	}
	else
	{
		fprintf(env->pStdErr(), "USAGE: %s <directory>\n", argv[0]);
	}
	return EXIT_SUCCESS;
}


int Console::ls(int argc, char **argv, Console *env)
{
	int flags = LIST_SHOW_FILE | LIST_SHOW_DIR;

	po::options_description desc("USAGE: ls [OPTION]...\nList information about the current directory");
	desc.add_options()
		("help,h", "produce help message")
		("recursive,R", "list files recursively")
		("complete,c", "show full file information")
		("dir,d", "hide files")
		("file,f", "hide directories")
		("empty,e", "show empty entries")
		("offset,o", "sort by offset (default sort)")
		("position,p", "sort by position")
		("size,s", "sort by size");

	po::variables_map vm;
	try
	{
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch (po::unknown_option o)
	{
		fprintf(env->pStdErr(), "unknown option.\n");
		return EXIT_FAILURE;
	}

	if (vm.count("help"))
	{
		cout << desc << endl;
		return EXIT_SUCCESS;
	}

	if (vm.count("empty")) SET_FLAG(flags, LIST_SHOW_EMPTY); 
	if (vm.count("recursive")) SET_FLAG(flags, LIST_RECURSIVE);
	if (vm.count("complete")) SET_FLAG(flags, LIST_COMPLETE);
	if (vm.count("dir")) RESET_FLAG(flags, LIST_SHOW_FILE);
	if (vm.count("file")) RESET_FLAG(flags, LIST_SHOW_DIR);

	if (vm.count("offset"))
	{
		RESET_FLAG(flags, LIST_SORT_POSITION);
		SET_FLAG(flags, LIST_SORT_OFFSET);
		RESET_FLAG(flags, LIST_SORT_SIZE);
	}

	if (vm.count("position"))
	{
		SET_FLAG(flags, LIST_SORT_POSITION);
		RESET_FLAG(flags, LIST_SORT_OFFSET);
		RESET_FLAG(flags, LIST_SORT_SIZE);
	}

	if (vm.count("size"))
	{
		RESET_FLAG(flags, LIST_SORT_POSITION);
		RESET_FLAG(flags, LIST_SORT_OFFSET);
		SET_FLAG(flags, LIST_SORT_SIZE);
	}

	env->pclGetFilesystem()->List(flags);

	return EXIT_SUCCESS;
}


int Console::pwd(int argc, char **argv, Console *env)
{
	string path = env->pclGetFilesystem()->getCurrentPath();

	if (path.size()>0)
	{
		//cout << path << endl;
		fprintf(env->pStdOut(), "%s\n", path.c_str());
	}
	else
		//cout << "/" << endl;
		fprintf(env->pStdOut(), "/\n");

	return EXIT_SUCCESS;
}


int Console::cat(int argc, char **argv, Console *env)
{
	if (argc == 2)
	{
		if (env->pclGetFilesystem()->oDump(argv[1]))
			return EXIT_SUCCESS;
		else
		{
			fprintf(env->pStdErr(), "Not a file or doesnt exist\n");
			return EXIT_FAILURE;
		}
	}
	
	fprintf(env->pStdErr(), "USAGE: %s <filename>\n", argv[0]);
	return EXIT_SUCCESS;
}


int Console::bcat(int argc, char **argv, Console *env)
{
	if (argc == 2)
	{
		if (env->pclGetFilesystem()->oHexDump(argv[1]))
			return EXIT_SUCCESS;
		else
		{
			fprintf(env->pStdErr(), "Not a file or doesnt exist\n");
			return EXIT_FAILURE;
		}
	}
	
	fprintf(env->pStdErr(), "USAGE: %s <filename>\n", argv[0]);
	return EXIT_SUCCESS;
}

int Console::extract(int argc, char **argv, Console *env)
{
	if (argc == 2)
	{
		if (env->pclGetFilesystem()->oFileExtract(argv[1]))
			return EXIT_SUCCESS;
		else
		{
			fprintf(env->pStdErr(), "Not a file or doesnt exist\n");
			return EXIT_FAILURE;
		}
	}
	
	fprintf(env->pStdErr(), "USAGE: %s <filename>\n", argv[0]);
	return EXIT_SUCCESS;
}

int Console::mount(int argc, char **argv, Console *env)
{
	if (argc != 2)
	{
		fprintf(env->pStdErr(), "USAGE: %s <filename.pk2>\n", argv[0]);
		if (env->pcGetCurrentMount())
			fprintf(env->pStdOut(), "%s\n", env->pcGetCurrentMount());

		return EXIT_SUCCESS;
	}

	env->pclGetFilesystem()->Close();
	env->SetCurrentMount(NULL);

	if (env->pclGetFilesystem()->oOpen(argv[1], FALSE))
	{
		env->SetCurrentMount(strdup(argv[1]));
		return EXIT_SUCCESS;
	}
	else
	{
        	fprintf(env->pStdErr(), "File %s not found.\n", argv[1]);
	}

	return EXIT_FAILURE;
}


int Console::umount(int argc, char **argv, Console *env)
{
	if (env->pclGetFilesystem())
	{
		env->pclGetFilesystem()->Close();
		env->SetCurrentMount(NULL);
	}

	return EXIT_SUCCESS;

}


int Console::defrag(int argc, char **argv, Console *env)
{
	if (argc != 2)
	{
		fprintf(env->pStdErr(), "USAGE: %s <output filename>\n", argv[0]);
		if (env->pcGetCurrentMount())
			fprintf(env->pStdOut(), "%s.new\n", env->pcGetCurrentMount());

		return EXIT_SUCCESS;
	}

	if (env->pclGetFilesystem() && env->pclGetFilesystem()->oRebuild(argv[1]))
		return EXIT_SUCCESS;

	return EXIT_FAILURE;
}


int Console::import(int argc, char **argv, Console *env)
{
	if (argc == 3)
	{
		int errno = 0;

		if ((errno = env->pclGetFilesystem()->iFileImport(argv[1], argv[2])) == 0)
			return EXIT_SUCCESS;
		else
		{
			if (errno == -1 || errno == -2)
				fprintf(env->pStdErr(), "%s not a file or doesnt exist\n", argv[2]);
			else
				fprintf(env->pStdErr(), "%s not a file or doesnt exist\n", argv[1]);

			return EXIT_FAILURE;
		}
	}
	
	fprintf(env->pStdErr(), "USAGE: %s <dest filename> <input filename>\n", argv[0]);
	return EXIT_SUCCESS;
}

