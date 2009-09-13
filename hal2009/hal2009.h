/*
 * This file is part of FreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
 * http://freehal.org
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#define FULL_NAME "FreeHAL"
#define FULL_VERSION 00000

#ifndef __cplusplus
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#else
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cstring>
#include <stdarg.h>
#endif

#ifdef __unix__
    #include <sys/types.h>
    #include <sys/stat.h>
    #define MODUS ,0711)
#elif __WIN32__ || _MS_DOS_
    #include <sys/stat.h>
    #include <dir.h>
    #define MODUS )
#else
    #include <direct.h>  /* Visual C++ */
    #define MODUS )
#endif

#define EVERYTHING 3

#include <pthread.h>

#define halwrite(a, b, c, d) { if (a && d)      { fwrite(a, b, c, d); } }
#define halclose(a)          { if (a)           { fclose(a);          } }

#ifndef USE_CXX
void *malloc (size_t);
void free (void*);
void *calloc (size_t, size_t);
void *realloc (void*, size_t);
#endif

#if defined (__MINGW) || defined(__MINGW32__)
#include <windows.h>
#define sleep(x) Sleep(1000*(x))
#define usleep(x) Sleep(x)
#define _set_stdout(x) { if (x) { _iob[STDOUT_FILENO] = *(x) ; } else { 1; } }
#else
#define _set_stdout(x) stdout = x
#endif

#ifndef HAL2009
#define HAL2009 1

#ifdef __cplusplus
extern "C" {
#endif
#ifdef USE_CXX
extern "C" {
#endif

#ifndef __cplusplus
#ifndef USE_CXX
extern FILE* output_handle_pipe;
#endif
#endif
void lock_stdout();
void unlock_stdout();
FILE* output();
FILE* get_output();
FILE* set_output(FILE*);
void set_output_fd(int);
FILE* unset_output();
FILE* get_output_pipe();
FILE* set_output_pipe(FILE*);
FILE* unset_output_pipe();
#define reset_stdout() (1)

#ifdef __cplusplus
}
#endif
#ifdef USE_CXX
}
#endif

#define matchstr(a, b) (0 == strcmp(( a ),( b )))

#define NO_CONNECTION (254)
#define INVALID (253)
#define RECORD_NORMAL (0)
#define RECORD_IF (1)

#define SINGLE 1
#define MULTI 2

#ifdef USE_CXX
extern "C"
#endif
char* sql_engine;

#define MAX_CLAUSES 10
#define LINE_SIZE 4096
#define FACT struct RECORD
#define SHORTFACT struct RECORD_short
#define factsize sizeof(FACT)+32
struct RECORD;
struct RECORD {
    char verb[LINE_SIZE];
    char subjects[LINE_SIZE];
    char objects[LINE_SIZE];
    char adverbs[LINE_SIZE];
    char from[LINE_SIZE];
    char extra[LINE_SIZE];
    char questionword[LINE_SIZE];
    char context[LINE_SIZE];
    char pkey[LINE_SIZE];
    int prio;
    void* clauses[MAX_CLAUSES];
    int num_clauses;
    short type;
    int hash_clauses;
    double truth;
    
    short verb_flag_want;
    short verb_flag_must;
    short verb_flag_can;
    short verb_flag_may;
    short verb_flag_should;
    
    short everything_q;
};
struct RECORD_short {
    char verb[50];
    char subjects[50];
    char objects[50];
    char adverbs[50];
    char from[50];
    char extra[50];
    char questionword[50];
    char context[50];
    char pkey[50];
    int prio;
    void* clauses[MAX_CLAUSES];
    short type;
    int hash_clauses;
    double truth;
    
    short verb_flag_want;
    short verb_flag_must;
    short verb_flag_can;
    short verb_flag_may;
    short verb_flag_should;
};
struct DATASET {
    char*** data;
    long int size;
    long int column_count;
    long int timeout;
    int err;
};

static int num_facts_added_during_this_run = 0;
static int num_facts_not_added_during_this_run_because_exist = 0;
static int num_facts_not_added_during_this_run_because_other_error = 0;

/************* API functions **************/
#ifdef __cplusplus
extern "C" {
#endif
    pthread_t hal2009_answer(char*, char*, char*, char*, int, short);
    void hal2009_clean();
    void hal2009_init(char* planguage, char* tlanguage, char* base_dir);
    void* hal2009_answer_thread(void*);
    pthread_t hal2009_start_signal_handler(char*, char*, short);
    void hal2009_handle_signal(void*);
    /* execute functions */
    void execute_perl6(char* filename);
    void execute_perl5(char* filename);
    int hal2009_add_pro_file (char* filename);
    struct DATASET hal2009_get_csv(char* csv_request);
    int hal2009_add_link (char* link, int key_1, int key_2);
    const char* hal2009_make_csv(struct DATASET* set);
    void* hal2009_answer_thread(void* parameters);
    int hal2009_execute_file(char* file, char* planguage);
    int sql_sqlite_set_filename(const char* filename);
    void* halfilecalloc( long s, long si, char* f );
    void* halfilefree( void* s );
    int sql_begin();
    int sql_end();
    const char* engine();
    const char* is_engine(const char*);
    char* sql_del_record(struct RECORD* r);
    int fact_delete_from_source(const char* source);
#ifdef __cplusplus
}
#endif
/************* END API functions **********/

/************* INTERNAL STUFF *************/

#define JOIN        1
#define NOT_JOIN    0


static int halmallocs = 0;
#ifndef USE_CXX
#define halfree( p )            { { free( p ); p = 0; --halmallocs; } /*printf("(free)      -- halmallocs: %i\t->\t%i\n", (int)(halmallocs)+1, (int)(halmallocs));*/ }
#define halfreef( p, f )        { { free( p ); p = 0; --halmallocs; } /*printf("(free)      -- halmallocs: %i\t->\t%i (new from: %s)\n", (int)(halmallocs)+1, (int)(halmallocs), f);*/ }
#define halmalloc( s, f )       calloc( s, 1 ); {++halmallocs;  /*printf("(halmalloc) ++ halmallocs: %i\t->\t%i (new from: %s)\n", (int)(halmallocs)-1, (int)(halmallocs), f);*/ }
#define zero_p( p, s )          { long i; for (i = 0; i < ( s ) - 1 ; ++i) { ( p ) [i] = 0; } }
#define line_size 4096
#else
#define cxxhalfree( p )            { { free( p ); p = 0; --halmallocs; } /*printf("(free)      -- halmallocs: %i\t->\t%i\n", (int)(halmallocs)+1, (int)(halmallocs));*/ }
#define cxxhalfreef( p, f )        { { free( p ); p = 0; --halmallocs; } /*printf("(free)      -- halmallocs: %i\t->\t%i (new from: %s)\n", (int)(halmallocs)+1, (int)(halmallocs), f);*/ }
#define cxxhalmalloc( s, f )       calloc( s, 1 ); { ++halmallocs;  /*printf("(halmalloc) ++ halmallocs: %i\t->\t%i (new from: %s)\n", (int)(halmallocs)-1, (int)(halmallocs), f);*/ }
#define cxxzero_p( p, s )          { long i; for (i = 0; i < ( s ) - 1 ; ++i) { ( p ) [i] = 0; } }
#define line_size 4096
#endif
struct _impl_halstring {
    char* s;
    short do_free;
};
#define halstring struct _impl_halstring
halstring* replace(halstring*, const char*, const char*);
halstring* replace_once(halstring*, const char*, const char*);
halstring **stringtoarray(halstring*, char, int*);

#ifndef NOT_HEADER_ONLY
short locked_stdout = 0;
void lock_stdout() {
    locked_stdout = 1;
}
void unlock_stdout() {
    locked_stdout = 0;
}
FILE* output_handle_pipe;
FILE* output() {
    while (locked_stdout)
        usleep(100);
    return stdout;
}
FILE* get_output() {
 	return stdout;
}

#if defined (__MINGW) || defined(__MINGW32__)
#ifdef stdout
#undef stdout
#define stdout	(&_iob[STDOUT_FILENO])
#endif
#endif
FILE* set_output(FILE* f) {
    _set_stdout(f);
 	return stdout;
}
void set_output_fd(int fd) {
    if (dup2(fd, 1) == -1)
        1;
    else if ((close(fd)==-1))
        1;
}
FILE* unset_output() {
   FILE* nullf = 0;
    _set_stdout(nullf);
 	return stdout;
}
FILE* get_output_pipe() {
 	return output_handle_pipe;
}
FILE* set_output_pipe(FILE* f) {
    output_handle_pipe = f;
 	return output_handle_pipe;
}
FILE* unset_output_pipe() {
    output_handle_pipe = 0;
 	return output_handle_pipe;
}


#undef IS_INVALID_FD
#undef INVALID_FD
#ifdef WIN32
# define IS_INVALID_FD(f) (!(f))
# define INVALID_FD 0
# undef close
# define close CloseHandle
#else
# define IS_INVALID_FD(f) ( (f) < 0)
# define INVALID_FD -1
#endif



#define PerlIO_stdout stdout
#define Perl_PerlIO_stdout stdout
#include "hal2009-sql.c"
#include "hal2009.c"
#endif /* NOT_HEADER_ONLY */

/************* END INTERNAL STUFF *********/


#endif /* HAL2009 */

