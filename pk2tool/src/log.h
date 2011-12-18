#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define LOG_MAX_SIZE	1024

class Log
{
	private:
		static FILE *fpErr;
		static FILE *fpOut;

	public:
		static void SetInfo(FILE *fp);
		static void SetError(FILE *fp);

		static void Error(const char *str, ...);
		static void Info(const char *str, ...);
};

#endif

