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

#include "hal2009-ram.h"


int disk_begin() {
    // initialize semantic ram_net
    disk_net = calloc(sizeof(void*)*(4+'z'-'a'), 1);
    int i;
    for (i = n('a'); i <= n('z'); ++i) {
        disk_net[i] = calloc(sizeof(void*)*(4+'z'-'a'), 1);
        
        int k;
        for (k = n('a'); k <= n('z'); ++k) {
            disk_net[i][k] = calloc(sizeof(struct list), 1);
            disk_net[i][k]->size = 0;
            disk_net[i][k]->list = 0;
        }
        
        k = WRONG;
        disk_net[i][k] = calloc(sizeof(struct list), 1);
        disk_net[i][k]->size = 0;
        disk_net[i][k]->list = 0;
    }
    i = WRONG;
    disk_net[i] = calloc(sizeof(void*)*(4+'z'-'a'), 1);
    int k;
    for (k = n('a'); k <= n('z'); ++k) {
        disk_net[i][k] = calloc(sizeof(struct list), 1);
        disk_net[i][k]->size = 0;
        disk_net[i][k]->list = 0;
    }
    k = WRONG;
    disk_net[i][k] = calloc(sizeof(struct list), 1);
    disk_net[i][k]->size = 0;
    disk_net[i][k]->list = 0;
    
    // initialize sqlite
    if (0 == sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        if (sqlite3_open(sqlite_filename, &sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            sqlite_connection = 0;
            return NO_CONNECTION;
        }
    }

    char* err;
    sqlite3_exec(sqlite_connection, "BEGIN;", NULL, NULL, &err);
    sqlite3_free(err);
    return 0;
}

int disk_free_wordlist(int i, int k) {
    int g;
    for (g = 0; g < disk_net[i][k]->size; ++g) {
        free(disk_net[i][k]->list[g]);
        disk_net[i][k]->list[g] = 0;
    }
}

int disk_end() {
    if (0 == sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        if (sqlite3_open(sqlite_filename, &sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            sqlite_connection = 0;
            return NO_CONNECTION;
        }
    }
    
    char* err;
    sqlite3_exec(sqlite_connection, "COMMIT;", NULL, NULL, &err);
    sqlite3_free(err);
    sqlite3_close(sqlite_connection);
    sqlite_connection = 0;
    
    int i;
    for (i = n('a'); i <= n('z'); ++i) {
        int k;
        for (k = n('a'); k <= n('z'); ++k) {
            disk_free_wordlist(i, k);
            free(disk_net[i][k]);
            disk_net[i][k] = 0;
        }
        
        k = WRONG;
        disk_free_wordlist(i, k);
        free(disk_net[i][k]);
        disk_net[i][k] = 0;
        
        free(disk_net[i]);
        disk_net[i] = 0;
    }
    i = WRONG;
    int k;
    for (k = n('a'); k <= n('z'); ++k) {
        disk_free_wordlist(i, k);
        free(disk_net[i][k]);
        disk_net[i][k] = 0;
    }
    k = WRONG;
    disk_free_wordlist(i, k);
    free(disk_net[i][k]);
    disk_net[i][k] = 0;
    free(disk_net[i]);
    disk_net[i] = 0;
    free(disk_net);
    disk_net = 0;
}

int sql_sqlite_set_filename(const char* filename) {
    if (sqlite_filename)
        free(sqlite_filename);
    sqlite_filename = 0;
    sqlite_filename = filename;
}

struct word* disk_get_word(const char* name) {
    if (0 == strlen(name)) {
        return 0;
    }
    int i = WRONG;
    if (strlen(name) >= 1) {
        i = n(name[0]);
    }
    int k = WRONG;
    if (strlen(name) >= 2) {
        k = n(name[1]);
    }
    
    int length = strlen(name);
    
    struct word** list = (struct word**)(disk_net[i][k]->list);
    
    if (0 == list) {
        //debugf("illegal list while searching %s.\n", name);
        return 0;
    }
    
    int g;
    for (g = 0; g < disk_net[i][k]->size; ++g) {
        if (length == list[g]->length && 0 == strcmp(list[g]->name, name)) {
            //debugf("found: %s = %p.\n", name, list[g]);
            return list[g];
        }
    }
    
    return 0;
}

static int select_primary_key(void* key, int argc, char **argv, char **azColName) {
    strcpy((char*)key, argv[0]);
    return 0;
}

int get_last_pk(int rel) {
    // cache
    static int cache_clauses = -1;
    static int cache_facts = -1;
    if (rel) {
        if (cache_clauses > -1) {
            ++cache_clauses;
            return cache_clauses;
        }
    }
    else {
        if (cache_facts > -1) {
            ++cache_facts;
            return cache_facts;
        }
    }
    
    // no cache
    char sql[5120];
    *sql = 0;
    strcat(sql, "SELECT pk from ");
    strcat(sql, rel ? "clauses" : "facts");
    strcat(sql, " order by 1 desc limit 1;");

    char key[99];
    int error = sql_execute(sql, select_primary_key, key);
    
    if (rel) {
        cache_clauses = to_number(key);
    }
    else {
        cache_facts = to_number(key);
        printf("cache_facts: %d\n", cache_facts);
    }
    
    return (rel?cache_clauses:cache_facts);
}

int detect_words(int* num_of_words, char** words, const char* r_verbs, const char* r_subjects, const char* r_objects, const char* r_adverbs, const char* r_extra) {
    char* verbs = strdup(r_verbs    ? r_verbs    : "");
    char* subj  = strdup(r_subjects ? r_subjects : "");
    char* obj   = strdup(r_objects  ? r_objects  : "");
    char* advs  = strdup(r_adverbs  ? r_adverbs  : "");
    char* extra = strdup(r_extra    ? r_extra    : "");
    char* buffer;
    
    *num_of_words = 1;
    words[0] = strdup("0");

    if (strcmp(subj, "0")) {
        words[*num_of_words] = strdup(verbs);
        ++(*num_of_words);
    }
    
    if (strcmp(subj, "0")) {
        words[*num_of_words] = strdup(subj);
        ++(*num_of_words);
    }
    
    if (strcmp( obj, "0")) {
        words[*num_of_words] = strdup( obj);
        ++(*num_of_words);
    }
    
    if (strcmp(advs, "0")) {
        words[*num_of_words] = strdup(advs);
        ++(*num_of_words);
    }
    
    buffer = strtok(verbs, " ;.)(-,");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    
    buffer = strtok(subj, " ;.)(-,");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    buffer = strtok( obj, " ;.)(-,");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    buffer = strtok(advs, " ;.)(-,");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    if (extra) {
        buffer = strtok(extra, " ;.)(-,");
        while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
            if (is_a_trivial_word(buffer)) {
                buffer = strtok(NULL, " ;.)(-,");
                continue;
            }
            words[*num_of_words] = strdup(buffer);
            buffer = strtok(NULL, " ;.)(-,");
            ++(*num_of_words);
            if (*num_of_words >= 500) break;
        }
    }

    --(*num_of_words);
    
    free(subj);
    free(obj);
    free(advs);
    free(extra);
}

char* gen_sql_add_entry(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    
    if (0 == sql) {
        sql = malloc(102400);
        *sql = 0;
    }
    strcat(sql, "INSERT INTO ");
    strcat(sql, rel ? "clauses" : "facts");
    strcat(sql, " (`pk`, `mix_1`, `verb`, `verbgroup`, `subjects`, `objects`, `adverbs`, `questionword`, `prio`, `from`, `rel`, `truth`) VALUES (");
    char num_of_records_str[10];
    snprintf(num_of_records_str, 9, "%d", pk);
    strcat(sql, num_of_records_str);
    strcat(sql, ", \"");
    strcat(sql, " ");
    strcat(sql, subjects);
    strcat(sql, " ");
    strcat(sql, objects);
    strcat(sql, " ");
    strcat(sql, adverbs);
    strcat(sql, " ");
    strcat(sql, "\", \"");
    strcat(sql, verbs);
    strcat(sql, "\", \"");
    if (0==strcmp(verbs, "=") || 0==strcmp(verbs, "ist") || 0==strcmp(verbs, "bist") || 0==strcmp(verbs, "bin") || 0==strcmp(verbs, "sind") || 0==strcmp(verbs, "sein") || 0==strcmp(verbs, "heisst") || 0==strcmp(verbs, "heisse") || 0==strcmp(verbs, "heissen") || 0==strcmp(verbs, "war") || 0==strcmp(verbs, "is") || 0==strcmp(verbs, "are") || 0==strcmp(verbs, "am") || 0==strcmp(verbs, "was")) {
        strcat(sql, "be");
    }
    if (0==strcmp(verbs, "haben") || 0==strcmp(verbs, "habe") || 0==strcmp(verbs, "hat") || 0==strcmp(verbs, "hast") || 0==strcmp(verbs, "hab") || 0==strcmp(verbs, "have") || 0==strcmp(verbs, "has")) {
        strcat(sql, "have");
    }
    strcat(sql, "\", \"");
    if (subjects[0] != ' ')
        strcat(sql, subjects);
    strcat(sql, "\", \"");
    if (objects[0] != ' ')
        strcat(sql, objects);
    strcat(sql, "\", \"");
    if (adverbs[0] != ' ')
        strcat(sql, adverbs);
    strcat(sql, "\", \"");
    if (questionword)    strcat(sql, questionword);
    else                 strcat(sql, "NULL");
    strcat(sql, "\", 50, \"");
    strcat(sql, from);
    strcat(sql, "\", ");
    if (rel) strcat(sql, from_number(rel));
    else     strcat(sql, "-1");
    strcat(sql, ", ");
    char truth_str[10];
    snprintf(truth_str, 9, "%f", truth);
    strcat(sql, truth_str);
    strcat(sql, ");\n");
    
    return sql;
}

char* gen_sql_add_verb_flags(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    
    char key[101];
    snprintf(key, 100, "%d", pk);
    
    if (0 == sql) {
        sql = malloc(102400);
        *sql = 0;
    }
    strcat(sql, "INSERT INTO rel_");
    strcat(sql, rel ? "clause" : "fact");
    strcat(sql, "_flag (`fact`, `verb_flag_want`, `verb_flag_must`, `verb_flag_can`, `verb_flag_may`, `verb_flag_should`) VALUES (");
    strcat(sql, key);
    strcat(sql, ", ");
    strcat(sql, verb_flag_want?"1":"0");
    strcat(sql, ", ");
    strcat(sql, verb_flag_must?"1":"0");
    strcat(sql, ", ");
    strcat(sql, verb_flag_can?"1":"0");
    strcat(sql, ", ");
    strcat(sql, verb_flag_may?"1":"0");
    strcat(sql, ", ");
    strcat(sql, verb_flag_should?"1":"0");
    strcat(sql, ");\n");
    
    return sql;
}

char* gen_sql_add_word_fact_relations(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {

    char key[101];
    snprintf(key, 100, "%d", pk);
    
    if (0 == sql) {
        sql = malloc(102400);
        *sql = 0;
    }
    
    int num_of_words = 0;
    char** words = calloc(501*sizeof(char*), 1);
    detect_words(&num_of_words, words, verbs, subjects, objects, adverbs, "");
    
    while (num_of_words >= 0) {
        if (words[num_of_words]) {
            if (words[num_of_words][0] != '0') {
                strcat(sql, "INSERT OR IGNORE INTO rel_word_fact (`word`, `fact`, `table`, `verb_flag_want`, `verb_flag_must`, `verb_flag_can`, `verb_flag_may`, `verb_flag_should`) VALUES (");
                strcat(sql, "\n\"");
                strcat(sql, words[num_of_words]);
                strcat(sql, "\", \n");
                strcat(sql, key);
                strcat(sql, ", \n\"");
                strcat(sql, rel ? "clauses" : "facts");
                strcat(sql, "\"");
                strcat(sql, ", ");
                strcat(sql, verb_flag_want?"1":"0");
                strcat(sql, ", ");
                strcat(sql, verb_flag_must?"1":"0");
                strcat(sql, ", ");
                strcat(sql, verb_flag_can?"1":"0");
                strcat(sql, ", ");
                strcat(sql, verb_flag_may?"1":"0");
                strcat(sql, ", ");
                strcat(sql, verb_flag_should?"1":"0");
                strcat(sql, ");");

                free(words[num_of_words]);
                words[num_of_words] = 0;
            }
        }
        --num_of_words;
    }
    free(words);

    strcat(sql, ";\n");
    
    return sql;
}

char* gen_sql_get_clauses_for_rel(int rel, struct fact** facts, int limit, int* position) {

    char* sql = malloc(102400);
    *sql = 0;
    
    strcat(sql, "SELECT `nmain`.`pk`, `nmain`.`verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, `nmain`.`subjects`, `nmain`.`objects`, `nmain`.`adverbs`, `nmain`.`questionword`, `nmain`.`from`, `nmain`.`truth` ");
    strcat(sql, " FROM clauses AS nmain JOIN rel_clause_flag AS rff ON nmain.pk = rff.fact WHERE nmain.rel = ");
    char rel_str[10];
    snprintf(rel_str, 9, "%d", rel);
    strcat(sql, rel_str);
    strcat(sql, ";");
    
    return sql;
}

char* gen_sql_get_facts_for_words(struct word*** words, struct fact** facts, int limit, int* position) {

    char* sql = malloc(512000);
    *sql = 0;
    
    if (0 == can_be_a_pointer(words[0])) {
        return sql;
    }
    
    strcat(sql, "delete from cache_facts ");

    int n, m, q;
    int in_bracket = 0;
    debugf("Generating SQL for searching facts for words (at %p).\n", words);
    for (n = 0; can_be_a_pointer(words[n]); ++n) {
        int is_new = 1;
        if (can_be_a_pointer(words[n][0])) {
            for (q = 0; words[q] && q+1 < n; ++q) {
                if (can_be_a_pointer(words[q][0]) && words[n][0] == words[q][0])
                    is_new = 0;
            }
        }
        debugf("synonym no %d: %d, %p, %s\n",
            n,
            can_be_a_pointer(words[n]),
            can_be_a_pointer(words[n]) ? words[n][0] : 0,
            can_be_a_pointer(words[n]) && can_be_a_pointer(words[n][0]) ? words[n][0]->name : "(null)"
        );
        if (!is_new) {
            debugf("not new.\n");
            continue;
        }
        
        for (m = 0; can_be_a_pointer(words[n][m]) && words[n][m]->name && words[n][m]->name[0]; ++m) {
            if (n == 0 || n % 100 == 0) {
                if (in_bracket) {
                    strcat(sql, ")");
                    in_bracket = 0;
                }
                strcat(sql, " ; INSERT OR IGNORE INTO cache_facts (pk, `from`, verb, verbgroup, subjects, objects, adverbs, mix_1, questionword, prio, rel, type, truth, hash_clauses) SELECT pk, `from`, verb, verbgroup, subjects, objects, adverbs, mix_1, questionword, prio, rel, type, truth, hash_clauses FROM facts WHERE pk in (SELECT fact FROM rel_word_fact WHERE 0 ");
                in_bracket = 1;
            }

            if (is_a_trivial_word(words[n][m]->name)) {
                continue;
            }
            
            if (words[n][m]->name[0] && words[n][m]->name[0] == '*') {
                if (strstr(words[n][m]->name+1, "*")) {
                    strcat(sql, "OR rel_word_fact.word GLOB \"");
                    strcat(sql, words[n][m]->name+1);
                    strcat(sql, "\"");
                }
                else {
                    strcat(sql, "OR rel_word_fact.word = \"");
                    strcat(sql, words[n][m]->name+1);
                    strcat(sql, "\"");
                }
            }
            else if (strstr(words[n][m]->name, "*")) {
                strcat(sql, "OR rel_word_fact.word GLOB \"");
                strcat(sql, words[n][m]->name);
                strcat(sql, "\"");
            }
            else {
                strcat(sql, "OR rel_word_fact.word = \"");
                strcat(sql, words[n][m]->name);
                strcat(sql, "\"");
            }
        }
    }
    if (in_bracket) {
        strcat(sql, ")");
    }
    strcat(sql, ";");
    strcat(sql, "SELECT DISTINCT `nmain`.`pk`, `nmain`.`verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, `nmain`.`subjects`, `nmain`.`objects`, `nmain`.`adverbs`, `nmain`.`questionword`, `nmain`.`from`, `nmain`.`truth` ");
    strcat(sql, " FROM cache_facts AS nmain LEFT JOIN rel_fact_flag AS rff ON nmain.pk = rff.fact");
    strcat(sql, ";");
    
    return sql;
}

int sql_execute(char* sql, int (*callback)(void*,int,char**,char**), void* arg) {
    char* err;
    int error_to_return = 0;
    while (sqlite3_exec(sqlite_connection, sql, callback, arg, &err)) {
        if (strstr(err, " not unique") && !strstr(err, "PRIMARY KEY must be unique")) {
            error_to_return = NOT_UNIQUE;
            break;
        }
        else if (strstr(err, "PRIMARY KEY must be unique")) {
            error_to_return = NOT_UNIQUE;
            break;
        }
        printf("SQL Error:\n------------\n%s\n------------\n%s\n------------\n\n", err, sql);
        if (strstr(err, "no such table")) {
            sqlite3_free(err);
            
            if (sqlite3_exec(sqlite_connection, sqlite_sql_create_table, NULL, NULL, &err)) {
                printf("SQL Error:\n------------\n%s\n------------\n%s\n------------\n\n", err, sql);
                error_to_return = TABLE_ERROR;
                break;
            }
            continue;
        }
        else {
            --num_facts_added_during_this_run;
            ++num_facts_not_added_during_this_run_because_other_error;
            break;
        }
    }
    sqlite3_free(err);
    
    return error_to_return;
}

struct fact* disk_add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    if ((is_bad(subjects) && is_bad(objects) && is_bad(verbs)) || (questionword && questionword[0] == ')')) {
        return 0;
    }

    int pk = get_last_pk(rel)+1;

    {
        char* sql = 0;
        sql = gen_sql_add_entry(sql, pk, rel, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
        sql = gen_sql_add_verb_flags(sql, pk, rel, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
        sql = gen_sql_add_word_fact_relations(sql, pk, rel, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
        int error = sql_execute(sql, NULL, NULL);
        free(sql);
    }

    return 0;
}

struct fact* disk_add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    if ((is_bad(subjects) && is_bad(objects) && is_bad(verbs)) || (questionword && questionword[0] == ')')) {
        return 0;
    }

    int pk = get_last_pk(0)+1;
    int error = 0;
    {
        char* sql = 0;
        sql = gen_sql_add_entry(sql, pk, 0, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
        sql = gen_sql_add_verb_flags(sql, pk, 0, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
        sql = gen_sql_add_word_fact_relations(sql, pk, 0, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
        //printf("%s\n", sql);
        error = sql_execute(sql, NULL, NULL);
        free(sql);
    }
    
    if (error) {
        return 0;
    }

    struct fact* fact = calloc(sizeof(struct fact), 1);
    fact->pk = pk;
    
    return fact;
}

struct word* disk_set_word(const char* name) {
    if (!name || 0 == strlen(name)) {
        return 0;
    }
    struct word* already_there = get_word(name);
    if (already_there) {
        return already_there;
    }
    int i = WRONG;
    if (strlen(name) >= 1) {
        i = n(name[0]);
    }
    int k = WRONG;
    if (strlen(name) >= 2) {
        k = n(name[1]);
    }
    
    if (0 == disk_net[i][k]->list) {
        // debugf("empty list wile inserting %s.\n", name);
    }
    else {
        0 && debugf("not empty list wile inserting %s: %p, %p entries, last entry = %s\n", name, disk_net[i][k]->list, disk_net[i][k]->size, ((struct word**)(disk_net[i][k]->list))[disk_net[i][k]->size-1]->name);
    }
    
    if (disk_net[i][k]->size == 0) {
        disk_net[i][k]->list = calloc(sizeof(struct word*), 11);
        disk_net[i][k]->allocated_until = 10;
    }
    else if (disk_net[i][k]->size >= disk_net[i][k]->allocated_until) {
        disk_net[i][k]->allocated_until += 10;
        disk_net[i][k]->list = realloc(disk_net[i][k]->list, sizeof(struct word*)*(disk_net[i][k]->allocated_until+1));
    }
    
    disk_net[i][k]->list[disk_net[i][k]->size] = calloc(1, sizeof(struct word));
    ((struct word**)(disk_net[i][k]->list))[disk_net[i][k]->size]->name   = strdup(name);
    ((struct word**)(disk_net[i][k]->list))[disk_net[i][k]->size]->length = strlen(name);
    0 && debugf("inserted: %s = %p, %p.\n", 
            ((struct word**)(disk_net[i][k]->list))[disk_net[i][k]->size]->name,
            disk_net[i][k]->list[disk_net[i][k]->size],
            disk_net[i][k]->size);
    ++(disk_net[i][k]->size);
    return disk_net[i][k]->list[disk_net[i][k]->size - 1];
}

static int callback_get_facts(void* arg, int argc, char **argv, char **azColName) {
    struct request_get_facts_for_words* req = arg;
    
    if (*req->position >= req->limit) {
        return 1;
    }
    
    if (argc <= 5 || (!argv[1] || !strlen(argv[1])) || ((!argv[2] || !strlen(argv[2])) && (!argv[3] || !strlen(argv[3])))) {
        return 0;
    }
    
    struct fact* fact  = calloc(sizeof(struct fact), 1);
    fact->pk           = to_number(argv[0] ? argv[0] : "0");
    fact->verbs        = divide_words(argv[1] ? argv[1] : "");
    fact->subjects     = divide_words(argv[2] ? argv[2] : "");
    fact->objects      = divide_words(argv[3] ? argv[3] : "");
    fact->adverbs      = divide_words(argv[4] ? argv[4] : "");
    fact->extra        = divide_words("");
    fact->questionword = strdup(argv[5] ? argv[5] : "");
    fact->from         = strdup(argv[6] ? argv[6] : "");
    fact->truth        = !argv[7] || !argv[7][0] || argv[7][0] == '0' ? 0.0 : 1.0;
    
    req->facts[*req->position] = fact;
    debugf("Added fact no %d at %p (%s, %s, %s, %s).\n", *req->position, req->facts[*req->position], argv[1], argv[2], argv[3], argv[4]);
    ++(*req->position);
    
    return 0;
}

int disk_search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position) {
    struct request_get_facts_for_words req;
    req.words    = words;
    req.facts    = facts;
    req.limit    = limit;
    req.position = position;
    req.rel      = 0;
    
    {
        char* sql = gen_sql_get_facts_for_words(words, facts, limit, position);
        printf("%s\n", sql);
        int error = sql_execute(sql, callback_get_facts, &req);
        free(sql);
    }
    
    return 0;
}

struct fact** disk_search_clauses(int rel) {
    int limit = 20000;
    struct fact** clauses = calloc(1, sizeof(struct fact*)*(limit+1));
    int position = 0;

    struct request_get_facts_for_words req;
    req.words    = 0;
    req.facts    = clauses;
    req.limit    = limit;
    req.position = &position;
    req.rel      = rel;
    
    {
        char* sql = gen_sql_get_clauses_for_rel(rel, clauses, limit, &position);
        //printf("%s\n", sql);
        int error = sql_execute(sql, callback_get_facts, &req);
        free(sql);
    }
    
    return clauses;
}
