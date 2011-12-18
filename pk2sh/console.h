#ifndef __CONSOLE__
#define __CONSOLE__

#include "defs.h"

class IFilesystem;
class Console;

typedef int (*CommandProc)(int argc, char **argv, Console *env);

struct CommandEntry 
{
	const char *pcName;
	CommandProc pfnProc;
};

class Console
{
	private:
		FILE *_pStdErr;
		FILE *_pStdOut;
		FILE *_pStdIn;
		
		IFilesystem *pclFilesystem;
		int iQuit;
		int iLastReturnCode;
		char *pcCurrentMount;
		char *pcPrompt;


	public:
		static int ls(int argc, char **argv, Console *env);
		static int pwd(int argc, char **argv, Console *env);
		static int cat(int argc, char **argv, Console *env);
		static int bcat(int argc, char **argv, Console *env);
		static int exit(int argc, char **argv, Console *env);
		static int help(int argc, char **argv, Console *env);
		static int chdir(int argc, char **argv, Console *env);
		static int mount(int argc, char **argv, Console *env);
		static int umount(int argc, char **argv, Console *env);
		static int extract(int argc, char **argv, Console *env);
		static int defrag(int argc, char **argv, Console *env);
		static int import(int argc, char **argv, Console *env);

	public:
		Console(IFilesystem *fs);
		~Console();

		void Run();
		int iExecute(int argc, char **argv);

		void Quit();
		void SetPrompt(char *prompt);
		void SetCurrentMount(char *mount);
		void SetFilesystem(IFilesystem *fs);
		
		IFilesystem *pclGetFilesystem() const
		{
			return this->pclFilesystem;
		}

		const char *pcGetCurrentMount() const
		{
			return this->pcCurrentMount;
		}

		FILE *pStdErr() const
		{
			return this->_pStdErr;
		}

		FILE *pStdOut() const
		{
			return this->_pStdOut;
		}
};

#endif

