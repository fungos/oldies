#ifndef __UTIL__
#define __UTIL__

#include "defs.h"

struct ltstr
{
	bool operator()(const char *s1, const char *s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

char *strdupcat(const char *dup, const char *cat);

int cli_tokens(char *line, char **args);
void path_token(const char *str);
void write_long(unsigned long, unsigned char *);

#ifdef WIN32
char *readline(const char *prompt);		// Need free return ptr!
#else
#include <readline/readline.h>
#endif

#endif

