#ifndef __PROGRAM__
#define __PROGRAM__

class Console;

class Program
{
	private:
		char *pcName;
		Console::CommandProc pfnProc;

		friend class Console;
	public:
		Program(const char *, Console::CommandProc);
		~Program();

		const char *pcGetName() const
		{
			return pcName;
		}

		void Execute(int argc, char **argv, Console *env) const;
}


#endif

