/*
 * This file is part of FreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
 * http://freehal.org
 * 
 * This progdisk is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This progdisk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this progdisk; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef HAL2009_DISK_H
#define HAL2009_DISK_H 1

#include "sqlite3.h"

int disk_begin();
int disk_end();
struct word* disk_get_word(const char* name);
int insert_fact_by_list_into_net(struct word** list, struct fact* fact);
struct fact* disk_add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should);
struct fact* disk_add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should);
struct word* disk_set_word(const char* name);
int disk_search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position);
struct fact** disk_search_clauses(int rel);


static struct list*** disk_net = 0;

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
};


static char* sqlite_sql_create_table = ""
"CREATE TABLE `facts` (`pk` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`from` varchar(250), `verb` varchar(50), `verbgroup` varchar(50), `subjects` varchar(50), `objects` varchar(50), `adverbs` varchar(50), `mix_1` varchar(150), `questionword` varchar(50), `prio` varchar(50), `rel` integer(50), `type` integer(50), `truth` double(50), `hash_clauses` integer(50), "
"UNIQUE(`verb`, `subjects`, `objects`, `adverbs`, `truth`, `from`));"
"CREATE INDEX `idx_facts_rel` ON `facts` (`rel`);"
"CREATE INDEX `idx_facts_truth` ON `facts` (`truth`);"
"CREATE INDEX `idx_facts_verb` ON `facts` (`verb`); "
"CREATE INDEX `idx_facts_verbgroup` ON `facts` (`verbgroup`); "
"CREATE INDEX `idx_facts_subjects` ON `facts` (`subjects`); "
"CREATE INDEX `idx_facts_objects` ON `facts` (`objects`); "
"CREATE INDEX `idx_facts_adverbs` ON `facts` (`adverbs`); "
"CREATE INDEX `idx_facts_mix_1` ON `facts` (`mix_1`); "

"CREATE TABLE `cache_facts` (`pk` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`from` varchar(250), `verb` varchar(50), `verbgroup` varchar(50), `subjects` varchar(50), `objects` varchar(50), `adverbs` varchar(50), `mix_1` varchar(150), `questionword` varchar(50), `prio` varchar(50), `rel` integer(50), `type` integer(50), `truth` double(50), `hash_clauses` integer(50)"
");"
"CREATE INDEX `idx_cache_facts_rel` ON `cache_facts` (`rel`);"
"CREATE INDEX `idx_cache_facts_truth` ON `cache_facts` (`truth`);"
"CREATE INDEX `idx_cache_facts_verb` ON `cache_facts` (`verb`); "
"CREATE INDEX `idx_cache_facts_verbgroup` ON `cache_facts` (`verbgroup`); "
"CREATE INDEX `idx_cache_facts_subjects` ON `cache_facts` (`subjects`); "
"CREATE INDEX `idx_cache_facts_objects` ON `cache_facts` (`objects`); "
"CREATE INDEX `idx_cache_facts_adverbs` ON `cache_facts` (`adverbs`); "
"CREATE INDEX `idx_cache_facts_mix_1` ON `cache_facts` (`mix_1`); "

"CREATE TABLE `clauses` (`pk` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`from` varchar(250), `verb` varchar(50), `verbgroup` varchar(50), `subjects` varchar(50), `objects` varchar(50), `adverbs` varchar(50), `mix_1` varchar(150), `questionword` varchar(50), `prio` varchar(50), `rel` integer(50), `type` integer(50), `truth` double(50), `hash_clauses` integer(50), "
"UNIQUE(`verb`, `subjects`, `objects`, `adverbs`, `truth`, `rel`, `truth`));"
"CREATE INDEX `idx_clauses_rel` ON `clauses` (`rel`);"
"CREATE INDEX `idx_clauses_truth` ON `clauses` (`truth`);"
"CREATE INDEX `idx_clauses_verb` ON `clauses` (`verb`); "
"CREATE INDEX `idx_clauses_verbgroup` ON `clauses` (`verbgroup`); "
"CREATE INDEX `idx_clauses_subjects` ON `clauses` (`subjects`); "
"CREATE INDEX `idx_clauses_objects` ON `clauses` (`objects`); "
"CREATE INDEX `idx_clauses_adverbs` ON `clauses` (`adverbs`); "
"CREATE INDEX `idx_clauses_mix_1` ON `clauses` (`mix_1`); "

"CREATE TABLE `linking` (`pk` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`f1` INTEGER, `f2` INTEGER, `link` varchar(50), "
"UNIQUE(`f1`, `f2`, `link`));"
"CREATE INDEX `idx_link_pk` ON `linking` (`pk`);"
"CREATE INDEX `idx_link_f1` ON `linking` (`f1`);"
"CREATE INDEX `idx_link_f2` ON `linking` (`f2`);"

"CREATE TABLE `rel_word_fact` (`word` varchar(50), `fact` INTEGER, `table` varchar(50), "
"UNIQUE(`word`, `fact`));"
"CREATE INDEX `idx_rel_word_fact_word` ON `rel_word_fact` (`word`);"
"CREATE INDEX `idx_rel_word_fact_fact` ON `rel_word_fact` (`fact`);"

"CREATE TABLE `rel_fact_flag` (`fact` INTEGER, "
"verb_flag_want char(2),verb_flag_must char(2),verb_flag_can char(2),verb_flag_may char(2),verb_flag_should char(2),"
"UNIQUE(`fact`));"
"CREATE INDEX `idx_fact_rel_fact_flag` ON `rel_fact_flag` (`fact`);"

"CREATE TABLE `rel_clause_flag` (`fact` INTEGER, "
"verb_flag_want char(2),verb_flag_must char(2),verb_flag_can char(2),verb_flag_may char(2),verb_flag_should char(2),"
"UNIQUE(`fact`));"
"CREATE INDEX `idx_fact_rel_clause_flag` ON `rel_clause_flag` (`fact`);"

"CREATE TABLE `rel_verb_verb` (`v1` varchar(50), `v2` varchar(50), `type` varchar(50), "
"UNIQUE(`v1`, `v2`, `type`));"
"CREATE INDEX `idx_v1_rel_verb_verb` ON `rel_verb_verb` (`v1`);"
"CREATE INDEX `idx_v2_rel_verb_verb` ON `rel_verb_verb` (`v2`);"
"";



#endif
