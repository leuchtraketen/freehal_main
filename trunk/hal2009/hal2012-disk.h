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

#include "hal2012.h"

BEGIN_EXTERN_C /* GENERATED */
#ifndef HAL2009_DISK
#define HAL2009_DISK 1

#include "sqlite3.h"

static struct flist*** disk_net = 0;

// #define QUERY_CACHE_SIZE 99
#define QUERY_CACHE_SIZE 9
struct query_cache_entry {
    char*** rawfacts;
    int position;
    int hash_n_1;
    int hash_n_2;
    int hash_n_3;
};
static struct query_cache_entry** query_cache_list;

static sqlite3* sqlite_connection = 0;
static const char* sqlite_filename = 0;
static char*** sqlite_results;
static long int num_of_records[2];
static int sqlite3async_started = 0;

#define NOT_UNIQUE  7
#define TABLE_ERROR 8

struct request_get_facts_for_words {
    struct word*** words;
    struct fact** facts;
    int limit;
    int* position;
    int rel;

    int hash_n_1;
    int hash_n_2;
    int hash_n_3;
    short make_rawfacts;
    short debug_facts;
    int i_right;
};

struct request_string_pair {
    struct string_pair** facts;
    int limit;
    int* position;
    int rel;
};

static const char* sqlite_sql_create_table = ""
"CREATE TABLE IF NOT EXISTS `word` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`word` varchar(10), UNIQUE(`word`) );"
"INSERT OR IGNORE INTO word (`id`, `word`) VALUES (0, \"\");"
"INSERT OR IGNORE INTO word (`id`, `word`) VALUES (1, \"_\");"
""
"CREATE TABLE IF NOT EXISTS `synonym` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`a` integer(10), `a_last` integer(10), "
"`b` integer(10), `b_last` integer(10), "
"`fileid` INTEGER, `line` INTEGER, "
"UNIQUE(`b`, `a`) );"
""
"CREATE TABLE IF NOT EXISTS `facts` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`fileid` INTEGER, `line` INTEGER, `verb` INTEGER, `subjects` INTEGER, `objects` INTEGER, "
"`adverbs` INTEGER, `questionword` INTEGER, `prio` varchar(10), `rel` integer(10), `type` integer(10), `truth` double(10), "
"`only_logic` integer(10) );"
""
"CREATE INDEX IF NOT EXISTS `idx_facts_rel` ON `facts` (`rel`);"
"CREATE INDEX IF NOT EXISTS `idx_facts_truth` ON `facts` (`truth`);"
"CREATE INDEX IF NOT EXISTS `idx_facts_verb` ON `facts` (`verb`); "
"CREATE INDEX IF NOT EXISTS `idx_facts_subjects` ON `facts` (`subjects`); "
"CREATE INDEX IF NOT EXISTS `idx_facts_objects` ON `facts` (`objects`); "
"CREATE INDEX IF NOT EXISTS `idx_facts_adverbs` ON `facts` (`adverbs`); "
"CREATE INDEX IF NOT EXISTS `idx_facts_only_logic` ON `facts` (`only_logic`); "
"CREATE INDEX IF NOT EXISTS `idx_facts_fileid` ON `facts` (`fileid`); "

"CREATE TABLE IF NOT EXISTS `cache_facts` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`fileid` INTEGER, `line` INTEGER, `verb` INTEGER, `subjects` INTEGER, `objects` INTEGER, "
"`adverbs` INTEGER, `questionword` INTEGER, `prio` varchar(10), `rel` integer(10), `type` integer(10), `truth` double(10), "
"`only_logic` integer(10) );"
""
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_rel` ON `cache_facts` (`rel`);"
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_truth` ON `cache_facts` (`truth`);"
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_verb` ON `cache_facts` (`verb`); "
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_subjects` ON `cache_facts` (`subjects`); "
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_objects` ON `cache_facts` (`objects`); "
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_adverbs` ON `cache_facts` (`adverbs`); "
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_only_logic` ON `cache_facts` (`only_logic`); "
"CREATE INDEX IF NOT EXISTS `idx_cache_facts_fileid` ON `cache_facts` (`fileid`); "

"CREATE TABLE IF NOT EXISTS `clauses` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`fileid` INTEGER, `line` INTEGER, `verb` INTEGER, `subjects` INTEGER, `objects` INTEGER, "
"`adverbs` INTEGER, `questionword` INTEGER, `prio` varchar(10), `rel` integer(10), `type` integer(10), `truth` double(10), "
"`only_logic` integer(10) );"
""
"CREATE INDEX IF NOT EXISTS `idx_clauses_rel` ON `clauses` (`rel`);"
"CREATE INDEX IF NOT EXISTS `idx_clauses_truth` ON `clauses` (`truth`);"
"CREATE INDEX IF NOT EXISTS `idx_clauses_verb` ON `clauses` (`verb`); "
"CREATE INDEX IF NOT EXISTS `idx_clauses_subjects` ON `clauses` (`subjects`); "
"CREATE INDEX IF NOT EXISTS `idx_clauses_objects` ON `clauses` (`objects`); "
"CREATE INDEX IF NOT EXISTS `idx_clauses_adverbs` ON `clauses` (`adverbs`); "
"CREATE INDEX IF NOT EXISTS `idx_clauses_fileid` ON `cache_facts` (`fileid`); "

"CREATE TABLE IF NOT EXISTS `linking` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`f1` INTEGER, `f2` INTEGER, `link` varchar(10), "
"UNIQUE(`f1`, `f2`, `link`));"
"CREATE INDEX IF NOT EXISTS `idx_link_f1` ON `linking` (`f1`);"
"CREATE INDEX IF NOT EXISTS `idx_link_f2` ON `linking` (`f2`);"

"CREATE TABLE IF NOT EXISTS `rel_fact_flag` (`fact` INTEGER PRIMARY KEY, "
"verb_flag_want char(2),verb_flag_must char(2),verb_flag_can char(2),verb_flag_may char(2),verb_flag_should char(2),"
"UNIQUE(`fact`));"
"CREATE INDEX IF NOT EXISTS `idx_fact_rel_fact_flag` ON `rel_fact_flag` (`fact`);"

"CREATE TABLE IF NOT EXISTS `rel_clause_flag` (`fact` INTEGER PRIMARY KEY, "
"verb_flag_want char(2),verb_flag_must char(2),verb_flag_can char(2),verb_flag_may char(2),verb_flag_should char(2),"
"UNIQUE(`fact`));"
"CREATE INDEX IF NOT EXISTS `idx_fact_rel_clause_flag` ON `rel_clause_flag` (`fact`);"

"CREATE TABLE IF NOT EXISTS `rel_verb_verb` (`v1` varchar(10), `v2` varchar(10), `type` varchar(10), "
"UNIQUE(`v1`, `v2`, `type`));"
"CREATE INDEX IF NOT EXISTS `idx_v1_rel_verb_verb` ON `rel_verb_verb` (`v1`);"
"CREATE INDEX IF NOT EXISTS `idx_v2_rel_verb_verb` ON `rel_verb_verb` (`v2`);"

"CREATE TABLE IF NOT EXISTS `cache_indices` (`i` integer);"
"CREATE INDEX IF NOT EXISTS `idx_i_cache_indices` ON `cache_indices` (`i`);"

"CREATE TABLE IF NOT EXISTS `cache_ids` (`id` INTEGER PRIMARY KEY AUTOINCREMENT);"
"CREATE TABLE IF NOT EXISTS `files` (`id` INTEGER PRIMARY KEY AUTOINCREMENT, `filename` varchar(10), UNIQUE(`filename`) );"

"CREATE TABLE IF NOT EXISTS `db_index`.`rel_word_fact` (`word` INTEGER, `fact` INTEGER, `table` INTEGER, UNIQUE(`word`, `fact`, `table`));"
"CREATE INDEX IF NOT EXISTS `db_index`.`idx_rel_word_fact_word` ON `rel_word_fact` (`word`);"
"CREATE INDEX IF NOT EXISTS `db_index`.`idx_rel_word_fact_fact` ON `rel_word_fact` (`fact`);"

"";

// functions begin
char* disk_del_record(const char* key);
char* disk_get_source(const char* key);
char* gen_sql_add_entry(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic);
char* gen_sql_add_filename(const char* filename);
char* gen_sql_add_verb_flags(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic);
char* gen_sql_add_word_fact_relations(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword);
char* gen_sql_delete_everything_from(const char* filename);
char* gen_sql_get_clauses_for_rel(int rel, struct fact** facts, int limit, int* position);
char* gen_sql_get_double_facts();
char* gen_sql_get_facts_for_words(struct word*** words, struct fact** facts, int limit, int* position, int* hash_n_1, int* hash_n_2, int* hash_n_3);
char* no_underscore(const char* word);
const char* disk_get_thesaurus_synonyms(const char* key, const char** keys, struct string_pair** facts, int limit, int* position, int level, short reverse);
int detect_words(int* num_of_words, char** words, const char* r_verbs, const char* r_subjects, const char* r_objects, const char* r_adverbs, const char* r_extra);
int disk_add_filename(const char* filename);
int disk_add_link (const char* link, int key_1, int key_2);
int disk_begin(const char* modes);
int disk_clear_cache();
int disk_delete_everything_from(const char* filename);
int disk_end();
int disk_free_wordlist(int i, int k);
int disk_net_go_way_conditional_questionword(struct request* fact, struct fact** facts, int limit, int* position);
int disk_re_index();
int disk_search_double_facts(struct word*** words, struct fact** facts, int limit, int* position);
int disk_search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position);
int disk_set_to_invalid_value(void** p);
int find_query_cache_entry (struct request_get_facts_for_words* req, short if_needed_create_entry);
int get_last_pk(int rel);
int get_last_wordid();
int is_in(void** unique, int first, int last, void* o);
int sql_execute(char* sql, int (*callback)(void*,int,char**,char**), void* arg);
int sql_get_word(char* sql, const char* word);
int sql_get_word_glob(char* sql, const char* word);
int sql_get_words(char* sql, const char** words);
int sql_reset();
int sql_select_word(char* sql, const char* word);
int sql_set_word(char* sql, const char* word);
int sql_sqlite_set_filename(const char* filename);
static int callback_get_facts(void* arg, int argc, char **argv, char **azColName);
static int callback_re_index(void* arg, int argc, char **argv, char **azColName);
static int callback_string_pair(void* arg, int argc, char **argv, char **azColName);
static int select_primary_key(void* key, int argc, char **argv, char **azColName);
struct fact* disk_add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should);
struct fact* disk_add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword);
struct fact** disk_search_clauses(int rel);
struct word* disk_get_word(const char* name);
struct word* disk_set_word(const char* name);
void hashes(struct word** unique_words, int* hash_n_1, int* hash_n_2, int* hash_n_3);
void** unique(void*** words);
// functions end
#endif /* HAL2009_DISK */
END_EXTERN_C /* GENERATED */
