#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#if defined(MINGW) || defined(WIN32)

//for CaptureStackBacktrace
#include <windows.h>
#include <winbase.h>

#else

#include <execinfo.h>

#endif

void sighandler(int sig);
void fatalerror(const char *string);

static void __attribute__((constructor)) setup_sigsegv() {
	signal(SIGILL, sighandler);   // install our handler
	signal(SIGFPE, sighandler);   // install our handler
	signal(SIGSEGV, sighandler);   // install our handler
	signal(SIGTERM, sighandler);   // install our handler
//	signal(SIGBREAK, sighandler);   // install our handler
	signal(SIGABRT, sighandler);   // install our handler
}

void sighandler(int sig)
{
	char tmp[99];
	sprintf(tmp, "Signal error: %d\n",sig);
	fatalerror(tmp);
}

#if defined(LINUX) || defined(MACINTOSH) 
#include <execinfo.h>
#endif

void fatalerror(const char *string)
{
 FILE *ef;

ef=fopen("errors.txt","a");

if (ef) fprintf(ef, "%s\n",string);
else    fprintf(stderr, "%s\n",string);

#if defined(MINGW) || defined(WIN32)
{
void *array[63];
int i,num;

num=RtlCaptureStackBackTrace(0,63,array,0);

for (i = 0; i < num; i++)
	if (ef) fprintf(ef, "%s\n",(char *)array[i]);
	else    fprintf(stderr, "%s\n",(char *)array[i]);
}
#else
{
void *array[256];
size_t size;
char **strings;
size_t i;

size = backtrace (array, sizeof(array)/sizeof(void *));
strings = backtrace_symbols (array, size);

for (i = 0; i < size; i++) {
	if (ef) fprintf(ef, "%s\n",strings[i]);
	else    fprintf(stderr, "%s\n",strings[i]);
}

free (strings);
}
#endif

printf("%s",string);
exit(1);
}



