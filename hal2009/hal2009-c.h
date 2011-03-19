/*
 * This file is part of FreeHAL 2010.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010 Tobias Schulz and contributors.
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
extern char* sql_engine;
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

char* fact_read_from_source (const char* source);
char* fact_read_from_source(const char* source);
char* hal2009_make_csv(struct DATASET* set);
char* sql_del_record(struct RECORD* r);
char* sql_get_source(struct RECORD* r);
char* check_config (const char* name, const char* _default);
const char* engine();
const char* hal2009_get_text_language();
const char* is_engine(const char*);
/* execute functions */
int _debugf(const char* c, ...);
int fact_delete_from_source (const char* source);
int fact_delete_from_source(const char* source);
int fact_replace_in_source (const char* source, const char* _replacement);
int fact_replace_in_source(const char* source, const char* replacement);
int hal2009_add_filename(const char* filename);
int hal2009_add_link (char* link, int key_1, int key_2);
int hal2009_delete_everything_from(const char* filename);
int hal2009_set_text_language(const char*);
int hal2009_set_text_language(const char* s);
int halsleep(double);
int halsleep(double seconds);
int halusleep(double);
int halusleep(double seconds);
int remove_negation (char* _line, double* truth_ref, int* only_logic);
int sql_begin(const char* begin);
int sql_end();
int sql_re_index();
int sql_set_quiet(int);
int sql_sqlite_set_filename(const char* filename);
static char* hal2009_text_language;
struct DATASET hal2009_get_csv(char* csv_request);
void* hal2009_answer_thread(void*);
void* hal2009_answer_thread(void* parameters);
void hal2009_clean();
void* hal2009_handle_signal(void*);
void* hal2009_signal_handler(void* parameters);
void* halfilecalloc( long s, long si, char* f );
void* halfilecalloc( long s, long si, char* str );
void* halfilefree( void* s );

char* sql_del_record(struct RECORD* r);
char* sql_get_source(struct RECORD* r);
const char* engine();
const char* is_engine(const char* m);
int sql_add_filename(const char* filename);
int sql_add_link (char* link, int key_1, int key_2);
int sql_add_record(struct RECORD* r);
int sql_begin(const char* modes);
int sql_delete_everything_from(const char* filename);
int sql_end();
int sql_re_index();
int sql_set_quiet(int i);


