#include "log.h"
#include <stdarg.h>

FILE *Log::fpErr = stderr;
FILE *Log::fpOut = stdout;

void Log::SetError(FILE *fp)
{
	Log::fpErr = fp;
}

void Log::SetInfo(FILE *fp)
{
	Log::fpOut = fp;
}

void Log::Error(const char *str, ...)
{
	char buf[LOG_MAX_SIZE];
	va_list ap;

	if (!fpErr)
		return;

	va_start(ap, str);
	vsnprintf(buf, LOG_MAX_SIZE, str, ap);
	va_end(ap);

	fputs(buf, Log::fpErr);
}

void Log::Info(const char *str, ...)
{
	char buf[LOG_MAX_SIZE];
	va_list ap;

	if (!fpOut)
		return;

	va_start(ap, str);
	vsnprintf(buf, LOG_MAX_SIZE, str, ap);
	va_end(ap);

	fputs(buf, Log::fpOut);
}

