#include "util.h"

#define COMAND_LINE_SIZE 2048


char *strdupcat(const char *dup, const char *cat)
{
	char *ret;
	int len = strlen(dup) + strlen(cat) + 1;

	ret = (char *)malloc(sizeof(char) * len);
	memset(ret, '\0', len);

	strcat(ret, dup);
	strcat(ret, cat);

	return ret;
}


#ifdef WIN32
char *readline(const char *prompt)
{
	char *buf, ch = 0;
	int length = 0;
	
	buf = (char *)malloc(sizeof(char) * COMAND_LINE_SIZE);
	memset(buf, '\0', COMAND_LINE_SIZE);
	fflush(stdin);

	fprintf(stdout, prompt);
	while (ch != '\n')
	{
		ch = fgetc(stdin);
		buf[length++] = ch;
		
		if (length == COMAND_LINE_SIZE)
			break;
	}
	
	return buf;
}
#endif


int cli_tokens(char *line, char **args)
{
	char ch;
	int in_string = 0, arg = 0, size;
	
	args[arg] = line;
	size = strlen(line);
	for (int i=0; i<size; i++)
	{
		ch = line[i];
		
		switch (ch)
		{
			case '"':
				if (in_string)
				{
					in_string = 0;
					line[i--]=' ';
				}
				else
				{
					args[arg] = &line[i+1];
					in_string = 1;
				}
				break;
						
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if (!in_string)
				{
					line[i] = '\0';
					while (line[i+1]==' '||line[i+1]=='\t'||line[i+1]=='\n'||line[i+1]=='\r') line[++i] = '\0';
					if (strlen(&line[i+1])>0)
						args[++arg] = &line[i+1];
				}
				break;
				
			default:
				break;
		}
	}
	return ++arg;
}


void path_token(const char *str)
{
	int start = 0, i = 0;
	char *fname;
	bool is_file = false;

	while (!is_file)
	{
		for (; str[i]!='/' && str[i]!='\0'; )
			++i;

		if (str[i]=='\0')
			is_file = true;
		
		fname = (char *)malloc(sizeof(char)* (i-start)+1);
		memset(fname, '\0', (i-start)+1);
		strncpy(fname, (str+start), (i-start)+1);
		fname[i-start] = '\0';
		if (is_file)
			fprintf(stdout, "cat %s\n", fname);
		else
			fprintf(stdout, "cd %s\n", fname);

		free(fname);

		i++;
		start = i;
	}
}


void write_long(unsigned long val, unsigned char *buff)
{
	buff[3] = (val >> 24);
	buff[2] = (val >> 16) & 0xff;
	buff[1] = (val >> 8) & 0xff;
	buff[0] = (val & 0xff);
}


/*
int main(int argc, char **arg)
{
	char *buf, **args;
	int arc = 0;

	args = (char **)malloc(sizeof(char *) * 256);
 	memset(args, '\0', sizeof(char *) * 256);

	buf = readline("/$ ");
	arc = cli_tokens(buf, args);
	free(args);
	free(buf);

	path_token("/mnt/jorge/Danny/Silkroad/Music.pk2");
    	return EXIT_SUCCESS;
}
*/


