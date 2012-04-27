/*
 * This file is part of FreeHAL 2012.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Tobias Schulz and contributors.
 * http://www.freehal.org
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


// C++
#ifndef CXX

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>

FILE* output();

#define halfree( p )            { { free( p ); p = 0; --halmallocs; } }
#define halfreef( p, f )        { { free( p ); p = 0; --halmallocs; } }
#define halmalloc( s, f )       calloc( s, 1 ); {++halmallocs; }
#define zero_p( p, s )          { long i; for (i = 0; i < ( s ) - 1 ; ++i) { ( p ) [i] = 0; } }
#define line_size 4096

#endif
// C

void *malloc (size_t);
void free (void*);
void *calloc (size_t, size_t);
void *realloc (void*, size_t);

#ifdef __unix__
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <math.h>
    #define MODUS ,0711)
#elif __WIN32__ || _MS_DOS_
    #include <sys/stat.h>
    #include <dir.h>
    #define MODUS )
#else
    #include <direct.h>  /* Visual C++ */
    #define MODUS )
#endif

#define NO_CONNECTION (254)
#define INVALID (253)
#define RECORD_NORMAL (0)
#define RECORD_IF (1)
#define SINGLE 1
#define MULTI 2
#define EVERYTHING 3
#define INVALID_POINTER (void*)(-1)
#define MAX_CLAUSES 10
#define LINE_SIZE 4096
#define FACT struct RECORD
#define SHORTFACT struct RECORD_short
#define factsize sizeof(FACT)+32
#define JOIN        1
#define NOT_JOIN    0

#define halwrite(a, b, c, d) { if (a && d)      { fwrite(a, b, c, d); } }
#define halclose(a)          { if (a)           { fclose(a);          } }
#define matchstr(a, b) (0 == strcmp(( a ),( b )))
#define output() (stdout)

/*
#   define _set_stdout(x) { if (x) { _iob[STDOUT_FILENO] = *(x) ; } else { 1; } }
#else
#   define _set_stdout(x) stdout = x
#   ifdef stdout
#       undef stdout
#       define stdout (&_iob[STDOUT_FILENO])
#   endif
#endif
*/

extern FILE* output_handle_pipe;
static int num_facts_added_during_this_run = 0;
static int num_facts_not_added_during_this_run_because_exist = 0;
static int num_facts_not_added_during_this_run_because_other_error = 0;
static int halmallocs = 0;

struct RECORD;
struct RECORD {
    char verb[LINE_SIZE];
    char subjects[LINE_SIZE];
    char objects[LINE_SIZE];
    char adverbs[LINE_SIZE];
    char filename[LINE_SIZE];
    char line[LINE_SIZE];
    char extra[LINE_SIZE];
    char questionword[LINE_SIZE];
    char context[LINE_SIZE];
    char pkey[LINE_SIZE];
    int prio;
    void* clauses[MAX_CLAUSES];
    int num_clauses;
    short type;
    int hash_clauses;
    int only_logic;
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
    char filename[50];
    char line[50];
    char extra[50];
    char questionword[50];
    char context[50];
    char pkey[50];
    int prio;
    void* clauses[MAX_CLAUSES];
    short type;
    int hash_clauses;
    int only_logic;
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


