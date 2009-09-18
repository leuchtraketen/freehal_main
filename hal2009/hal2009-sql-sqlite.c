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

#include "sqlite3.h"

static sqlite3* traditional_sqlite_connection = 0;
static char*** traditional_sqlite_results;
static long int traditional_num_of_records[2];


static char* traditional_sqlite_sql_create_table = ""
"CREATE TABLE `facts` (`pk` INTEGER PRIMARY KEY AUTOINCREMENT, "
"`from` varchar(250), `verb` varchar(50), `verbgroup` varchar(50), `subjects` varchar(50), `objects` varchar(50), `adverbs` varchar(50), `mix_1` varchar(150), `questionword` varchar(50), `prio` varchar(50), `rel` integer(50), `type` integer(50), `truth` double(50), `hash_clauses` integer(50), "
"UNIQUE(`verb`, `subjects`, `objects`, `adverbs`, `truth`, `hash_clauses`));"
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
"verb_flag_want char(2),verb_flag_must char(2),verb_flag_can char(2),verb_flag_may char(2),verb_flag_should char(2),"
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

static int callback_clause(void* arg, int argc, char **argv, char **azColName) {
    char** record = *(char***)arg;
    --record;
    
    while (record[0]) ++record;
    
    int i;
    for(i=0; i<argc; ++i){
        printf("Entity: %s\n", argv[i]);
        if (argv[i] && strlen(argv[i])) {
            record[i] = malloc(strlen(argv[i])+1);
            strcpy(record[i], argv[i]);
        }
        else {
            record[i] = malloc(5);
            strcpy(record[i], "NULL");
        }
    }
    
    return 0;
}

static int callback_synonyms (void* arg, int argc, char **argv, char **azColName) {
    if (!argv[0] || 0 == strcmp(argv[0], "*")) {
        return 0;
    }
    
    char** synonyms = *(char***)(arg);
    int n = 0;
    while (synonyms[n] && n < 20000) {
        ++n;
    }
    
    if (!synonyms[n] && (n < 1 || strcmp(synonyms[n-1], argv[0]))) {
        synonyms[n] = strdup(argv[0]);
        if (synonyms[n]) {
            if (synonyms[n][0] == 'a' && synonyms[n][1] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+2);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'a' && synonyms[n][1] == 'n' && synonyms[n][2] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+3);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'e' && synonyms[n][1] == 'i' && synonyms[n][2] == 'n' && synonyms[n][3] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+4);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'e' && synonyms[n][1] == 'i' && synonyms[n][2] == 'n' && synonyms[n][4] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+5);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'e' && synonyms[n][1] == 'i' && synonyms[n][2] == 'n' && synonyms[n][5] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+6);
                synonyms[n][0] = '*';
            }
        }
        if (n+1 < 20000) {
            synonyms[n+1] = 0;
        }

        if (n+1 < 20000) {
            printf("%d: %s\n", n, synonyms[n]);
        }
        else {
            return 1;
        }
        
        if (n >= 1 && 0 == strcmp(synonyms[n-1], synonyms[n])) {
            if (synonyms[n])
                free(synonyms[n]);
            synonyms[n] = 0;
        }
    }
    
    return 0;
}


static int callback_synonyms_trio (void* arg, int argc, char **argv, char **azColName) {
    if (!argv[0] || !argv[1] || !argv[2] || 0 == strcmp(argv[0], "*")) {
        return 0;
    }
    
    char** synonyms = *(char***)(arg);
    int n = 0;
    while (synonyms[n] && n < 20000) {
        ++n;
    }
    
    if (!synonyms[n] && (n < 1 || strcmp(synonyms[n-1], argv[0]))) {
        synonyms[n] = malloc(5*sizeof(char**));
        char** arr = synonyms[n];
        arr[0] = strdup(argv[0]);
        arr[1] = strdup(argv[1]);
        arr[2] = strdup(argv[2]);
        
        if (synonyms[n]) {
            if (synonyms[n][0] == 'a' && synonyms[n][1] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+2);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'a' && synonyms[n][1] == 'n' && synonyms[n][2] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+3);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'e' && synonyms[n][1] == 'i' && synonyms[n][2] == 'n' && synonyms[n][3] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+4);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'e' && synonyms[n][1] == 'i' && synonyms[n][2] == 'n' && synonyms[n][4] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+5);
                synonyms[n][0] = '*';
            }
            if (synonyms[n][0] == 'e' && synonyms[n][1] == 'i' && synonyms[n][2] == 'n' && synonyms[n][5] == ' ') {
                strcpy(synonyms[n]+1, synonyms[n]+6);
                synonyms[n][0] = '*';
            }
        }
        if (n+1 < 20000) {
            synonyms[n+1] = 0;
        }

        if (n+1 < 20000) {
            printf("%d: %s zzz %s zzz %s\n", n, ((char**)synonyms[n])[0], ((char**)synonyms[n])[1], ((char**)synonyms[n])[2]);
        }
        else {
            return 1;
        }
        
        if (n >= 1 && 0 == strcmp(synonyms[n-1], synonyms[n])) {
            if (synonyms[n])
                free(synonyms[n]);
            synonyms[n] = 0;
        }
    }
    
    return 0;
}

static int callback(void* arg, int argc, char **argv, char **azColName) {
    struct DATASET* sqlite_current_data = (struct DATASET*)(arg);
    
    if (sqlite_current_data->size > 99997) {
        return 1;
    }
    if (0 == sqlite_current_data->data) {
        sqlite_current_data->data = calloc(99999, 1);
    }
    
    sqlite_current_data->column_count = argc - 1 + 5*(MAX_CLAUSES - 1);
    
    int i;
    char** record = calloc(sizeof(char**)*(argc+2)*MAX_CLAUSES+1, 1);
    if (argv[0]) {
        size_t size = strlen(argv[0]);
        ++size;
        record[0] = calloc(size, 1);
        strcpy(record[0], argv[0]);
    }
    for(i=0; i < argc-2; ++i){
        if (argv[i+2] && strlen(argv[i+2])) {
            size_t size = strlen(argv[i+2]);
            ++size;
            record[i+1] = calloc(size, 1);
            strcpy(record[i+1], argv[i+2]);
        }
        else {
            record[i+1] = malloc(5);
            strcpy(record[i+1], "NULL");
        }
    }
    
    short there_are_sub_clauses = (argv[1] && 0 == strcmp(argv[1], "-1"));
    
    if (there_are_sub_clauses) printf("There are sub clauses. Ok.\n");
    else                       printf("There aren't any sub clauses. Ok.\n");
    
    /// Fetch subclauses.
    if (there_are_sub_clauses) {
        char* sql = malloc(5120);
        *sql = 0;
        strcat(sql, "SELECT `verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, `subjects`, `objects`, `adverbs`, `questionword`, `truth` FROM `clauses` ");
        strcat(sql, " LEFT JOIN rel_fact_flag AS rff ON rff.fact = pk WHERE `rel` = ");
        strcat(sql, argv[0]);
        strcat(sql, " UNION ALL SELECT `verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, `subjects`, `objects`, `adverbs`, `questionword`, `truth` FROM `facts` ");
        strcat(sql, " LEFT JOIN rel_fact_flag AS rff ON rff.fact = pk ");
        strcat(sql, " WHERE `pk` = (SELECT f2 FROM `linking` WHERE f1 = ");
        strcat(sql, argv[0]);
        strcat(sql, ");");
        
        printf("%s\n", sql);
        
        void* offset = record + argc - 1;
        
        char* err;
        while (sqlite3_exec(traditional_sqlite_connection, sql, callback_clause, &offset, &err)) {
            if (strstr(err, "are not unique")) {
                break;
            }
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            break;
        }
        sqlite3_free(err);
        free(sql);
    }

    sqlite_current_data->data[sqlite_current_data->size] = record;

    ++sqlite_current_data->size;
    time_t time_now;
    time(&time_now);
    printf("Timeout is: %li\n", time_now);
    printf("Time    is: %li\n", sqlite_current_data->timeout);
    if (sqlite_current_data->timeout < time_now) {
        if (sqlite_current_data->size >= 1) {
            return 1;
        }
    }
    return 0;
}


char* sql_sqlite_mask_sql(char* sql) {
    halstring ssql;
    ssql.s = sql;
    ssql.do_free = 1;
    halstring* ssql_ref = &ssql;
    ssql_ref = replace(ssql_ref, " a ", "*");
    ssql_ref = replace(ssql_ref, "\"a ", "\"*");
    ssql_ref = replace(ssql_ref, " a\"", "*\"");
    ssql_ref = replace(ssql_ref, "\"a\"", "\"*\"");
    ssql_ref = replace(ssql_ref, " b ", "*");
    ssql_ref = replace(ssql_ref, "\"b ", "\"*");
    ssql_ref = replace(ssql_ref, " b\"", "*\"");
    ssql_ref = replace(ssql_ref, "\"b\"", "\"*\"");
    ssql_ref = replace(ssql_ref, " c ", "*");
    ssql_ref = replace(ssql_ref, "\"c ", "\"*");
    ssql_ref = replace(ssql_ref, " c\"", "*\"");
    ssql_ref = replace(ssql_ref, "\"c\"", "\"*\"");
    ssql_ref = replace(ssql_ref, " d ", "*");
    ssql_ref = replace(ssql_ref, "\"d ", "\"*");
    ssql_ref = replace(ssql_ref, " d\"", "*\"");
    ssql_ref = replace(ssql_ref, "\"d\"", "\"*\"");
    ssql_ref = replace(ssql_ref, " e ", "*");
    ssql_ref = replace(ssql_ref, "\"e ", "\"*");
    ssql_ref = replace(ssql_ref, " e\"", "*\"");
    ssql_ref = replace(ssql_ref, "\"e\"", "\"*\"");
    ssql_ref = replace(ssql_ref, " f ", "*");
    ssql_ref = replace(ssql_ref, "\"f ", "\"*");
    ssql_ref = replace(ssql_ref, " f\"", "*\"");
    ssql_ref = replace(ssql_ref, "\"f\"", "\"*\"");
    strcpy(sql, ssql_ref->s);
    
    return sql;
}

int sql_sqlite_begin() {
    if (0 == traditional_sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        if (sqlite3_open(sqlite_filename, &traditional_sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            traditional_sqlite_connection = 0;
            return NO_CONNECTION;
        }
    }

    char* err;
    sqlite3_exec(traditional_sqlite_connection, "BEGIN;", NULL, NULL, &err);
    sqlite3_free(err);
}

int sql_sqlite_end() {
    if (0 == traditional_sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        if (sqlite3_open(sqlite_filename, &traditional_sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            traditional_sqlite_connection = 0;
            return NO_CONNECTION;
        }
    }
    
    char* err;
    sqlite3_exec(traditional_sqlite_connection, "COMMIT;", NULL, NULL, &err);
    sqlite3_free(err);
    sqlite3_close(traditional_sqlite_connection);
    traditional_sqlite_connection = 0;
}

int sql_sqlite_add_link (const char* link, int key_1, int key_2) {
    if (0 == traditional_sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        if (sqlite3_open(sqlite_filename, &traditional_sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            traditional_sqlite_connection = 0;
            return NO_CONNECTION;
        }
    }
    
    if (!link) {
        return INVALID;
    }

    char sql[5120];
    *sql = 0;
    strcat(sql, "INSERT INTO linking (`link`, `f1`, `f2`) VALUES (\"");

    char str_fact_1[40];
    snprintf(str_fact_1, 39, "%d", key_1);
    char str_fact_2[40];
    snprintf(str_fact_2, 39, "%d", key_2);

    strcat(sql, link);
    strcat(sql, "\", ");
    strcat(sql, str_fact_1);
    strcat(sql, ", ");
    strcat(sql, str_fact_2);
    strcat(sql, ");");
    
    strcpy(sql, sql_sqlite_mask_sql(sql));
    
    char* err;
    while (sqlite3_exec(traditional_sqlite_connection, sql, NULL, NULL, &err)) {
        if (strstr(err, "are not unique")) {
            /// Link is not unique - it already exists in the database
            break;
        }
        printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
        if (strstr(err, "no such table")) {
            if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            }
        }
        else {
            break;
        }
    }
    sqlite3_free(err);
}

int is_unimportant_word(const char* word) {
    return (
        (0 == strcmp(word, "der"))                 ? 1 :
        (0 == strcmp(word, "die"))                 ? 1 :
        (0 == strcmp(word, "das"))                 ? 1 :
        (0 == strcmp(word, "nicht"))               ? 1 :
        (0 == strcmp(word, "in"))                  ? 1 :
        (0 == strcmp(word, "an"))                  ? 1 :
        0
    );
}

int traditional_detect_words(int* num_of_words, char** words, struct RECORD* r) {
    char* subj  = strdup(r->subjects ? r->subjects : "");
    char* obj   = strdup(r->objects  ? r->objects  : "");
    char* advs  = strdup(r->adverbs  ? r->adverbs  : "");
    char* extra = strdup(r->extra    ? r->extra    : "");
    char* buffer;
    
    *num_of_words = 1;
    words[0] = strdup("0");

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
    
    buffer = strtok(subj, " ;.)(-,");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_unimportant_word(buffer)) {
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
        if (is_unimportant_word(buffer)) {
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
        if (is_unimportant_word(buffer)) {
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
            if (is_unimportant_word(buffer)) {
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

char* sql_sqlite_del_record(struct RECORD* r) {
    if (!r->pkey || !r->pkey[0])
        return 1;
    
    char* source;
    {
        char* sql = malloc(1024);
        *sql = 0;
        if (0 == strcmp(r->pkey, "a")) {
            strcat(sql, "SELECT `from` FROM facts WHERE pk = (SELECT pk FROM facts ORDER BY pk DESC LIMIT 1);");
        }
        else {
            strcat(sql, "SELECT `from` FROM facts WHERE pk = ");
            strcat(sql, r->pkey);
            strcat(sql, " LIMIT 1;");
        }

        // Fetch source of fact
        char** sources = calloc(4*sizeof(char*), 1);
        char* err;
        while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms, &sources, &err)) {
            if (strstr(err, "are not unique")) {
                break;
            }
            if (strstr(err, "callback requested query abort")) {
                break;
            }
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            if (strstr(err, "no such table")) {
                sqlite3_free(err);
                if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                    printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                }
            }
            else {
                break;
            }
        }
        
        if (sources[0]) {
            source = strdup(sources[0]);
        }
        else {
            source = 0;
        }
        
        sqlite3_free(err);    
        free(sql);
        free(sources);
    }
    
    char* sql = malloc(1024);
    *sql = 0;
    
    if (0 == strcmp(r->pkey, "a")) {
        strcat(sql, "DELETE FROM facts WHERE pk = (SELECT pk FROM facts ORDER BY pk DESC LIMIT 1);");
    }
    else {
        strcat(sql, "DELETE FROM facts WHERE pk = ");
        strcat(sql, r->pkey);
        strcat(sql, ";");
    }
    printf("pkey (in hal2009-sql-sqlite): %s\n", r->pkey);
    
    char* err;
    while (sqlite3_exec(traditional_sqlite_connection, sql, NULL, NULL, &err)) {
        printf("Error while executing SQL: %s\n%s\n", sql, err);
        if (strstr(err, "no such table")) {
            sqlite3_free(err);
            if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                printf("Error while executing SQL: %s\n%s\n", sql, err);
            }
            else {
                sqlite3_exec(traditional_sqlite_connection, sql, NULL, NULL, &err);
            }
        }
        else {
            break;
        }
    }
    sqlite3_free(err);
    free(sql);
    
    return source;
}

int sql_sqlite_add_record(struct RECORD* r, const char* relation_to) {
    if (0 == traditional_sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        if (sqlite3_open(sqlite_filename, &traditional_sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            traditional_sqlite_connection = 0;
            return NO_CONNECTION;
        }
    }
    
    if (!r) {
        printf ("!(1)");
        return INVALID;
    }
    if ( *r->verb == ' ' && !relation_to ) {
        printf ("!(3)");
        return INVALID;
    }
    if (    (!r->subjects     || !strlen(r->subjects))
         && (!r->objects      || !strlen(r->objects))
         && (!r->adverbs      || !strlen(r->adverbs))
         && (!r->verb         || !strlen(r->verb))
         && (!r->questionword || !strlen(r->questionword) || 1<=strlen(r->questionword) || r->questionword[0] == ' ' || r->questionword[0] == '\r' || r->questionword[0] == '\n')
        ) {
            
        return INVALID;
    }
    
    if (r->verb) {
        char* override_verb = halmalloc(strlen(r->verb)*2+1, "sql_sqlite_add_record");
        *override_verb = 0;
        char* buffer;
        buffer = strtok(r->verb, " ");
        while (buffer) {
            int size_of_buffer = strlen(buffer);
            short there_was_a_flag_change = 1;
            if (size_of_buffer > 7 && 0 == strcmp(buffer+size_of_buffer-7, "/should")) {
                r->verb_flag_should = 1;
            }
            else if (size_of_buffer > 4 && 0 == strcmp(buffer+size_of_buffer-4, "/may")) {
                r->verb_flag_may = 1;
            }
            else if (size_of_buffer > 4 && 0 == strcmp(buffer+size_of_buffer-4, "/can")) {
                r->verb_flag_can = 1;
            }
            else if (size_of_buffer > 5 && 0 == strcmp(buffer+size_of_buffer-5, "/must")) {
                r->verb_flag_must = 1;
            }
            else if (size_of_buffer > 5 && 0 == strcmp(buffer+size_of_buffer-5, "/want")) {
                r->verb_flag_want = 1;
            }
            else {            
                strcat(override_verb, buffer);
                there_was_a_flag_change = 0;
            }
            buffer = strtok(NULL, " ");
            
            if (!buffer) buffer = strtok(NULL, " ");
            if (!buffer) buffer = strtok(NULL, " ");
            
            if (!there_was_a_flag_change && buffer) strcat(override_verb, " ");
        }
        strncpy(r->verb, override_verb, LINE_SIZE);
        halfree(override_verb);
    }
    
    if (1 || traditional_num_of_records[relation_to?1:0] == 0) {
        char sql[5120];
        *sql = 0;
        strcat(sql, "SELECT pk from ");
        if (relation_to) {
            strcat(sql, "clauses");
        }
        else {
            strcat(sql, "facts");
        }
        strcat(sql, " order by 1 desc limit 1;");
    
        char key[99];
        char* err;
        if (sqlite3_exec(traditional_sqlite_connection, sql, select_primary_key, key, &err)) {
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
        }
        sqlite3_free(err);
        
        if (key && key[0]) {
            traditional_num_of_records[relation_to?1:0] = atol(key);
        }
    }
    ++(traditional_num_of_records[relation_to?1:0]);

    char* sql = malloc(102400);
    *sql = 0;
    strcat(sql, "INSERT INTO ");
    if (relation_to) {
        strcat(sql, "clauses");
    }
    else {
        strcat(sql, "facts");
    }
    strcat(sql, " (`pk`, `hash_clauses`, `mix_1`, `verb`, `verbgroup`, `subjects`, `objects`, `adverbs`, `questionword`, `prio`, `from`, `rel`, `truth`) VALUES (");
    char traditional_num_of_records_str[10];
    snprintf(traditional_num_of_records_str, 9, "%d", traditional_num_of_records[relation_to?1:0]);
    strcat(sql, traditional_num_of_records_str);
    strcat(sql, ", ");
    char hash_clauses_str[10];
    snprintf(hash_clauses_str, 9, "%d", r->hash_clauses);
    strcat(sql, hash_clauses_str);
    strcat(sql, ", \"");
    strcat(sql, " ");
    strcat(sql, r->subjects);
    strcat(sql, " ");
    strcat(sql, r->objects);
    strcat(sql, " ");
    strcat(sql, r->adverbs);
    strcat(sql, " ");
    strcat(sql, "\", \"");
    strcat(sql, r->verb);
    strcat(sql, "\", \"");
    if (0==strcmp(r->verb, "=") || 0==strcmp(r->verb, "ist") || 0==strcmp(r->verb, "bist") || 0==strcmp(r->verb, "bin") || 0==strcmp(r->verb, "sind") || 0==strcmp(r->verb, "sein") || 0==strcmp(r->verb, "heisst") || 0==strcmp(r->verb, "heisse") || 0==strcmp(r->verb, "heissen") || 0==strcmp(r->verb, "war") || 0==strcmp(r->verb, "is") || 0==strcmp(r->verb, "are") || 0==strcmp(r->verb, "am") || 0==strcmp(r->verb, "was")) {
        strcat(sql, "be");
    }
    if (0==strcmp(r->verb, "haben") || 0==strcmp(r->verb, "habe") || 0==strcmp(r->verb, "hat") || 0==strcmp(r->verb, "hast") || 0==strcmp(r->verb, "hab") || 0==strcmp(r->verb, "have") || 0==strcmp(r->verb, "has")) {
        strcat(sql, "have");
    }
    strcat(sql, "\", \"");
    if (r->subjects[0] != ' ')
        strcat(sql, r->subjects);
    strcat(sql, "\", \"");
    if (r->objects[0] != ' ')
        strcat(sql, r->objects);
    strcat(sql, "\", \"");
    if (r->adverbs[0] != ' ')
        strcat(sql, r->adverbs);
    strcat(sql, "\", \"");
    if (r->questionword) strcat(sql, r->questionword);
    else                 strcat(sql, "NULL");
    strcat(sql, "\", ");
    char prio_str[10];
    snprintf(prio_str, 9, "%d", r->prio?r->prio:50);
    strcat(sql, prio_str);
    strcat(sql, ", \"");
    strcat(sql, r->from);
    strcat(sql, "\", ");
    short there_are_sub_clauses = r->clauses && r->clauses[0] && !relation_to;
    if (relation_to)                strcat(sql, relation_to);
    else if (there_are_sub_clauses) strcat(sql, "-1");
    else                            strcat(sql, "-1");
    strcat(sql, ", ");
    char truth[10];
    snprintf(truth, 9, "%f", r->truth);
    strcat(sql, truth);
    strcat(sql, ");\n");

    strcpy(sql, sql_sqlite_mask_sql(sql));

    if (traditional_num_of_records_str && !relation_to) {
        FILE* target = fopen("_input_key", "w+b");
        if (target) {
            fwrite(traditional_num_of_records_str, 1, strlen(traditional_num_of_records_str), target);
            fclose(target);
        }
    }

    {
        char* key = traditional_num_of_records_str;

        if (key && key[0]) {
            /// Add fact-flag relations

            strcat(sql, "INSERT INTO rel_");
            if (relation_to) {
                strcat(sql, "clause");
            }
            else {
                strcat(sql, "fact");
            }
            strcat(sql, "_flag (`fact`, `verb_flag_want`, `verb_flag_must`, `verb_flag_can`, `verb_flag_may`, `verb_flag_should`) VALUES (");
            strcat(sql, key);
            strcat(sql, ", ");
            strcat(sql, r->verb_flag_want?"1":"0");
            strcat(sql, ", ");
            strcat(sql, r->verb_flag_must?"1":"0");
            strcat(sql, ", ");
            strcat(sql, r->verb_flag_can?"1":"0");
            strcat(sql, ", ");
            strcat(sql, r->verb_flag_may?"1":"0");
            strcat(sql, ", ");
            strcat(sql, r->verb_flag_should?"1":"0");
            strcat(sql, ");\n");

            
            int num_of_words = 0;
            char** words = calloc(501*sizeof(char*), 1);
            strcpy(r->extra, "");
            traditional_detect_words(&num_of_words, words, r);
            
            while (num_of_words >= 0) {
                if (words[num_of_words]) {
                    if (words[num_of_words][0] != '0') {
                        strcat(sql, "INSERT OR IGNORE INTO rel_word_fact (`word`, `fact`, `table`, `verb_flag_want`, `verb_flag_must`, `verb_flag_can`, `verb_flag_may`, `verb_flag_should`) VALUES (");
                        strcat(sql, "\n\"");
                        strcat(sql, words[num_of_words]);
                        strcat(sql, "\", \n");
                        strcat(sql, key);
                        strcat(sql, ", \n\"");
                        if (relation_to) {
                            strcat(sql, "clauses");
                        }
                        else {
                            strcat(sql, "facts");
                        }
                        strcat(sql, "\"");
                        strcat(sql, ", ");
                        strcat(sql, r->verb_flag_want?"1":"0");
                        strcat(sql, ", ");
                        strcat(sql, r->verb_flag_must?"1":"0");
                        strcat(sql, ", ");
                        strcat(sql, r->verb_flag_can?"1":"0");
                        strcat(sql, ", ");
                        strcat(sql, r->verb_flag_may?"1":"0");
                        strcat(sql, ", ");
                        strcat(sql, r->verb_flag_should?"1":"0");
                        strcat(sql, ");");

                        free(words[num_of_words]);
                        words[num_of_words] = 0;
                    }
                }
                --num_of_words;
            }
            free(words);

            if (!relation_to) {
                FILE* target = fopen("_input_key", "w+b");
                if (target) {
                    fwrite(key, 1, strlen(key), target);
                    fclose(target);
                }
            }
        }

        if (r->clauses && r->clauses[0] && !relation_to) {
            int n;
            int broken = 0;
            for (n = 0; n <= r->num_clauses && n+1 < MAX_CLAUSES && r->clauses && r->clauses[n]; ++n) {
                if (broken) {
                    continue;
                }
                if (sql_sqlite_add_record(r->clauses[n], key)) {
                    broken = 1;
                }
            }
        }
    }
    
    char* sqlite_filename_sql = calloc(9999, 1);
    strcpy(sqlite_filename_sql, sqlite_filename);
    strcat(sqlite_filename_sql, ".sql");
    FILE* database_sql = fopen(sqlite_filename_sql, "a");
    if (!database_sql) {
        database_sql = fopen(sqlite_filename_sql, "a");
    }
    if (!database_sql) {
        free(sql);
        return 0;
    }
    fprintf(database_sql, "%s", sql);
    fclose(database_sql);
    free(sqlite_filename_sql);

    char* err;
    while (sqlite3_exec(traditional_sqlite_connection, sql, NULL, NULL, &err)) {
        if (strstr(err, " not unique") && !strstr(err, "PRIMARY KEY must be unique")) {
            /// Fact is not unique - it already exists in the database
            --(traditional_num_of_records[relation_to?1:0]);
            if (!relation_to) {
                FILE* target = fopen("_input_key", "w+b");
                if (target) {
                    fclose(target);
                }
            }
            --num_facts_added_during_this_run;
            ++num_facts_not_added_during_this_run_because_exist;

            break;
        }
        else if (strstr(err, "PRIMARY KEY must be unique")) {
            ++(traditional_num_of_records[relation_to?1:0]);
            sql_sqlite_add_record(r, relation_to);
            break;
        }
        printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
        if (strstr(err, "no such table")) {
            sqlite3_free(err);
            if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            }
            else {
                sqlite3_exec(traditional_sqlite_connection, sql, NULL, NULL, &err);
            }
        }
        else {
            --num_facts_added_during_this_run;
            ++num_facts_not_added_during_this_run_because_other_error;
            break;
        }
    }
    sqlite3_free(err);
    free(sql);
    
    return 0;
}

char* replace_in_select(const char* var, char** syn, char** rep) {
    if (!rep) {
        return var;
    }
    if (0 == strlen(rep)) {
        return var;
    }
    
    char res[20000];
    strcpy(res, "");
    int n = 0;
    while (syn[n] && n < 20000) {
        strcat(res, "replace(");
        ++n;
    }
    strcat(res, var);
    
    if (n >= 20) {
        return var;
    }
    
    n = 0;
    while (syn[n] && n < 20000) {
        if (strlen(syn[n])) {
            char* synon = strdup(syn[n]);
            if (synon[0] == "*") {
                ++synon;
            }
            
            strcat(res, ", \"");
            strcat(res, synon);
            strcat(res, "\", \"");
            strcat(res, rep);
            strcat(res, "\") ");
            strcat(res, "");
            free(synon);
        }
        ++n;
    }

    return res;
}

struct DATASET sql_sqlite_get_records(struct RECORD* r) {
    if (0 == traditional_sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        if (sqlite3_open(sqlite_filename, &traditional_sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            traditional_sqlite_connection = 0;
            struct DATASET set;
            set.err = NO_CONNECTION;
            return set;
        }
    }

    char* sql = malloc(512000+sizeof(r));
    *sql = 0;
    short need_and;


    // Fetch subject before the other parameters because of the SQL join statement
    char* extras_buffer = r->extra;
    char* subjects_buffer = 0;
    int subject_is_extra = 0;
    if (r->subjects && *r->subjects != '0' && strlen(r->subjects)) {
        subjects_buffer = r->subjects;
        extras_buffer   = r->extra;
    }
    else if (r->extra && *r->extra != '0' && strlen(r->extra) && strcmp(r->context, "q_how") && strcmp(r->context, "q_what_prep")) {
        subjects_buffer = r->extra;
        subject_is_extra = 1;
    }
    
    char* objects_buffer = 0;
    int object_is_extra = 0;
    if (r->objects && *r->objects != '0' && strlen(r->objects)) {
        objects_buffer = r->objects;
        if (!extras_buffer) {
            extras_buffer = r->extra;
        }
    }
    else if (r->extra && *r->extra != '0' && strlen(r->extra) && strcmp(r->context, "q_how") ) {
        objects_buffer = r->extra;
        object_is_extra = 1;
    }


    // Fetch subject synonyms (maximum: 20000)
    char** subject_synonyms = calloc(20000*sizeof(char*), 1);
    char*** subject_synonyms_trio = calloc(20000*sizeof(char**), 1);
    if (subjects_buffer) {
        char* buf = 0;
        if (subjects_buffer) buf = strdup(subjects_buffer);
        char* bbuf = buf;
        int size = strlen(buf);
        int j;
        for (j = 0; j < size; ++j) {
            if (buf[j] == ' ') {
                buf[j] = '\0';
                buf += j+1;
                size = strlen(buf);
                j = 0;
                printf("New buf (matched by ' '): %s\n", buf);
            }
        }
        
        if (subject_is_extra) {
            strcat(sql, "SELECT objects, pk, `from`  FROM facts WHERE truth = 1 AND verbgroup = \"be\" AND (subjects ");
        }
        else {
            strcat(sql, "SELECT objects, pk, `from`  FROM facts WHERE truth = 1 AND verbgroup = \"be\" AND (NOT objects GLOB \"ein *\" AND NOT objects GLOB \"eine *\") AND (subjects ");
        }
        
        if (buf && strstr(buf, "*"))    strcat(sql, " GLOB ");
        else                            strcat(sql, " = ");
        strcat(sql, " \"");
        if (buf) {
            if (strlen(buf) <= 2) {
                strcat(sql, buf);
            }
            else if (buf[0] == '*') {
                strcat(sql, "* ");
                strcat(sql, buf+1);
            }
            else {
                strcat(sql, buf);
            }
        }
        strcat(sql, "\" OR subjects ");
        if (buf && strstr(buf, "*"))    strcat(sql, " GLOB ");
        else                            strcat(sql, " = ");
        strcat(sql, " \"");
        if (buf) {
            if (strlen(buf) <= 2) {
                strcat(sql, buf);
            }
            else if (buf[0] == '*') {
                strcat(sql, "");
                strcat(sql, buf+1);
            }
            else {
                strcat(sql, buf);
            }
        }
        strcat(sql, "\" OR (1 ");
        
        if (buf && !strstr(buf, "*")) {
            strcat(sql, "AND subjects ");
            strcat(sql, "=");
            strcat(sql, " \"");
            strcat(sql, buf);
            strcat(sql, "\"");
        }
        else if (buf) {
            int size = strlen(buf);
            int j;
            for (j = 0; j < size; ++j) {
                if (buf[j] == '*') {
                    buf[j] = '\0';

                    printf("New buf (matched by '*'): %s*\n", buf);
                    if (size && strlen(buf)) {
                        strcat(sql, "AND subjects ");
                        strcat(sql, "GLOB");
                        strcat(sql, " \"* ");
                        strcat(sql, buf);
                        strcat(sql, "\"");
                    }
                    buf[j] = '*';
                    if (j-1 >= 0) buf[j-1] = '\0';

                    if (j-1 >= 0) buf += j;
                    else          buf += j+1;
                    size = strlen(buf);
                    j = 0;
                }
            }

            if (size) {
                strcat(sql, "AND subjects ");
                strcat(sql, "GLOB");
                strcat(sql, " \"* ");
                strcat(sql, buf);
                strcat(sql, "\"");
            }
        }
        
        strcat(sql, ") ) AND objects <> \"*\" AND subjects <> \"*\" AND adverbs = \"\"");
        
        if (bbuf) free(bbuf);
        
        printf("%s\n", sql);
        
        char* err;
        while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms, &subject_synonyms, &err)) {
            if (strstr(err, "are not unique")) {
                break;
            }
            if (strstr(err, "callback requested query abort")) {
                break;
            }
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            if (strstr(err, "no such table")) {
                sqlite3_free(err);
                if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                    printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                }
            }
            else {
                break;
            }
        }
        sqlite3_free(err);
    
        while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms_trio, &subject_synonyms_trio, &err)) {
            if (strstr(err, "are not unique")) {
                break;
            }
            if (strstr(err, "callback requested query abort")) {
                break;
            }
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            if (strstr(err, "no such table")) {
                sqlite3_free(err);
                if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                    printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                }
            }
            else {
                break;
            }
        }
        sqlite3_free(err);
    
        free(sql);
        sql = malloc(512000+sizeof(r));
        *sql = 0;
    }
    

    // Fetch object synonyms (maximum: 20000)
    char** object_synonyms = calloc(20000*sizeof(char*), 1);
    char*** object_synonyms_trio = calloc(20000*sizeof(char**), 1);
    if (objects_buffer && strcmp(objects_buffer, "*")) {
        char* buf = 0;
        if (objects_buffer) buf = strdup(objects_buffer);
        char* bbuf = buf;
        int size = strlen(buf);
        int j;
        for (j = 0; j < size; ++j) {
            if (buf[j] == ' ') {
                buf[j] = '\0';
                buf += j+1;
                size = strlen(buf);
                j = 0;
                printf("New buf: %s\n", buf);
            }
        }
        
        if (object_is_extra) {
            strcat(sql, "SELECT subjects, pk, `from`  FROM facts WHERE truth = 1 AND verbgroup = \"be\" AND (objects ");
        }
        else {
            strcat(sql, "SELECT subjects, pk, `from`  FROM facts WHERE truth = 1 AND verbgroup = \"be\" AND ((NOT subjects GLOB \"ein *\" AND NOT subjects GLOB \"eine *\") OR ((subjects GLOB \"ein *\" OR subjects GLOB \"eine *\") AND (objects GLOB \"ein *\" OR objects GLOB \"eine *\"))) AND (objects ");
        }
        
        if (buf && strstr(buf, "*"))    strcat(sql, " GLOB ");
        else                            strcat(sql, " = ");
        strcat(sql, " \"");
        if (buf) {
            if (strlen(buf) <= 2) {
                strcat(sql, buf);
            }
            else if (buf[0] == '*') {
                strcat(sql, "* ");
                strcat(sql, buf+1);
            }
            else {
                strcat(sql, buf);
            }
        }
        strcat(sql, "\" OR objects ");
        strcat(sql, " GLOB ");
        strcat(sql, " \"");
        if (buf) {
            if (strlen(buf) <= 2) {
                strcat(sql, buf);
            }
            else if (buf[0] == '*') {
                strcat(sql, "");
                strcat(sql, buf+1);
            }
            else {
                strcat(sql, "* ");
                strcat(sql, buf);
            }
        }
        strcat(sql, "\" OR (0 ");
        
        if (buf && strstr(buf, "|")) {
            int size = strlen(buf);
            int j;
            for (j = 0; j < size; ++j) {
                if (buf[j] == '|') {
                    buf[j] = '\0';

                    printf("New buf (matched by '|'): %s*\n", buf);
                    if (size) {
                        strcat(sql, "OR objects ");
                        if (strstr(buf, "*")) {
                            strcat(sql, "GLOB");
                        }
                        else {
                            strcat(sql, "=");
                        }
                        strcat(sql, " \"");
                        if (buf) {
                            if (strlen(buf) <= 2) {
                                strcat(sql, buf);
                            }
                            else if (buf[0] == '*') {
                                strcat(sql, "* ");
                                strcat(sql, buf+1);
                            }
                            else {
                                strcat(sql, buf);
                            }
                        }
                        strcat(sql, "\"");
                        
                        // add to synonyms
                        {
                                int n = 0;
                                while (object_synonyms[n] && n < 20000) {
                                    ++n;
                                }
                                object_synonyms[n] = strdup(buf);
                        }
                    }
                    buf[j] = '\0';
                    if (j-1 >= 0) ++buf;
                    if (j-1 >= 0) buf[j-1] = '\0';

                    if (j-1 >= 0) buf += j;
                    else          buf += j+1;
                    size = strlen(buf);
                    j = 0;
                }
            }

            if (size) {
                strcat(sql, "OR objects ");
                if (strstr(buf, "*")) {
                    strcat(sql, "GLOB");
                }
                else {
                    strcat(sql, "=");
                }
                strcat(sql, " \"");
                if (buf) {
                    if (strlen(buf) <= 2) {
                        strcat(sql, buf);
                    }
                    else if (buf[0] == '*') {
                        strcat(sql, "* ");
                        strcat(sql, buf+1);
                    }
                    else {
                        strcat(sql, buf);
                    }
                }
                strcat(sql, "\"");
                
                // add to synonyms
                {
                        int n = 0;
                        while (object_synonyms[n] && n < 20000) {
                            ++n;
                        }
                        object_synonyms[n] = strdup(buf);
                }
            }
        }
        
        strcat(sql, ") ) AND objects <> \"*\" AND subjects <> \"*\" AND adverbs = \"\"");
        
        if (bbuf) free(bbuf);
        
        printf("%s\n", sql);
        
        char* err;
        while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms, &object_synonyms, &err)) {
            if (strstr(err, "are not unique")) {
                break;
            }
            if (strstr(err, "callback requested query abort")) {
                break;
            }
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            if (strstr(err, "no such table")) {
                sqlite3_free(err);
                if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                    printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                }
            }
            else {
                break;
            }
        }
        sqlite3_free(err);
    
        while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms_trio, &object_synonyms_trio, &err)) {
            if (strstr(err, "are not unique")) {
                break;
            }
            if (strstr(err, "callback requested query abort")) {
                break;
            }
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            if (strstr(err, "no such table")) {
                sqlite3_free(err);
                if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                    printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                }
            }
            else {
                break;
            }
        }
        sqlite3_free(err);
    
        free(sql);
        sql = malloc(512000+sizeof(r));
        *sql = 0;
    }
    
    // Fetch extra synonyms (maximum: 20000)
    char** extra_synonyms = calloc(20000*sizeof(char*), 1);
    char*** extra_synonyms_trio = calloc(20000*sizeof(char**), 1);
    if (extras_buffer && strcmp(extras_buffer, "*") && strcmp(extras_buffer, "0")) {
        char* buf = 0;
        if (extras_buffer) buf = strdup(extras_buffer);
        char* bbuf = buf;
        int size = strlen(buf);
        int j;
        for (j = 0; j < size; ++j) {
            if (buf[j] == ' ') {
                buf[j] = '\0';
                buf += j+1;
                size = strlen(buf);
                j = 0;
                printf("New buf: %s\n", buf);
            }
        }
        if (strcmp(buf, "*")) {
            strcat(sql, "SELECT objects, pk, `from` FROM facts WHERE truth = 1 AND verbgroup = \"be\" AND (subjects ");
            if (buf && strstr(buf, "*")) {
                strcat(sql, "GLOB");
            }
            else {
                strcat(sql, "=");
            }
            strcat(sql, " \"");
            if (buf) {
                if (strlen(buf) <= 2) {
                    strcat(sql, buf);
                }
                else if (buf[0] == '*') {
                    strcat(sql, "* ");
                    strcat(sql, buf+1);
                }
                else {
                    strcat(sql, buf);
                }
            }
            strcat(sql, "\") AND subjects <> \"*\" AND objects <> \"*\" AND adverbs = \"\"");
            strcat(sql, " UNION ALL ");
            strcat(sql, "SELECT subjects, pk, `from` FROM facts WHERE truth = 1 AND verbgroup = \"be\" AND (objects ");
            strcat(sql, "GLOB");
            strcat(sql, " \" ");
            if (buf) {
                if (strlen(buf) <= 2) {
                    strcat(sql, buf);
                }
                else if (buf[0] == '*') {
                    strcat(sql, buf+1);
                }
                else {
                    strcat(sql, buf);
                }
            }
            strcat(sql, "\" OR objects ");
            strcat(sql, "GLOB");
            strcat(sql, " \"");
            if (buf) {
                if (strlen(buf) <= 2) {
                    strcat(sql, buf);
                }
                else if (buf[0] == '*') {
                    strcat(sql, "* ");
                    strcat(sql, buf+1);
                }
                else {
                    strcat(sql, buf);
                }
            }
            strcat(sql, "\") AND subjects <> \"*\" AND objects <> \"*\" AND adverbs = \"\"");
            
            if (bbuf) free(bbuf);
            
            printf("%s\n", sql);
            
            char* err;
            while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms, &extra_synonyms, &err)) {
                if (strstr(err, "are not unique")) {
                    break;
                }
                if (strstr(err, "callback requested query abort")) {
                    break;
                }
                printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                if (strstr(err, "no such table")) {
                    sqlite3_free(err);
                    if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                        printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                    }
                }
                else {
                    break;
                }
            }
            sqlite3_free(err);
        
            while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms_trio, &extra_synonyms_trio, &err)) {
                if (strstr(err, "are not unique")) {
                    break;
                }
                if (strstr(err, "callback requested query abort")) {
                    break;
                }
                printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                if (strstr(err, "no such table")) {
                    sqlite3_free(err);
                    if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                        printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                    }
                }
                else {
                    break;
                }
            }
            sqlite3_free(err);
        
            free(sql);
            sql = malloc(512000+sizeof(r));
            *sql = 0;
        }
    }
    
    /// if the object was assigned to extra and the subject is null, this was done to get the object-synonyms of extra.
    /// now swap:
    if (object_is_extra && !subjects_buffer) {
        void* temp;
        
        temp = object_synonyms_trio;
        object_synonyms_trio = subject_synonyms_trio;
        subject_synonyms_trio = temp;
        
        temp = object_synonyms;
        object_synonyms = subject_synonyms;
        subject_synonyms = temp;
        
        temp = subjects_buffer;
        subjects_buffer = objects_buffer;
        objects_buffer = temp;
        
        int z;
        z = object_is_extra;
        object_is_extra = subject_is_extra;
        subject_is_extra = z;
    }
    
    int num_of_synonyms;
    int _num_of_synonyms;
    // Flow Chart Log
    {
        num_of_synonyms = 0;
        while (subject_synonyms_trio[num_of_synonyms] && num_of_synonyms < 20000) {
            ++num_of_synonyms;
        }

        _num_of_synonyms = 0;
        while (object_synonyms_trio[_num_of_synonyms] && _num_of_synonyms < 20000) {
            ++_num_of_synonyms;
        }
        
        if (num_of_synonyms + _num_of_synonyms > 0) {
            if (0 == num_of_synonyms)
                num_of_synonyms = 1;
            if (0 == _num_of_synonyms)
                _num_of_synonyms = 1;

            FILE* logfile = fopen("flowchart.log", "a");
            if (logfile) {
                fprintf(logfile, "begin box\n");
                fprintf(logfile, "bckgrndcolr D0FFFF\n");
                fprintf(logfile, "bordercolor D0FFFF\n");
                fprintf(logfile, "linesoftext %d\n", num_of_synonyms+_num_of_synonyms+3);
                fprintf(logfile, "draw\n");
                fprintf(logfile, "textcontent 000000 Synonyms (Subject):\n");
                int n = 0;
                while (subject_synonyms_trio[n] && n < 20000) {
                    char** subject_synonym_buf = subject_synonyms_trio[n];
                    
                    fprintf(logfile, "linkcontent 000000 \t - %s", subject_synonym_buf[0]);
                    int u;
                    for (u = 0; u < 25-strlen(subject_synonym_buf[0]) && u < 26; ++u) {
                        fprintf(logfile, " ");
                    }
                    fprintf(logfile, " (");
                    fprintf(logfile, "%s)\n", subject_synonym_buf[2]);
                    fprintf(logfile, "property    pk     %s\n", subject_synonym_buf[1]);
                    
                    ++n;
                }
                if (0 == n) {
                    fprintf(logfile, "textcontent 000000 \t none\n");
                }
                fclose(logfile);
            }
        }
    }
    
    // Flow Chart Log
    {
        if (num_of_synonyms + _num_of_synonyms > 0) {
            FILE* logfile = fopen("flowchart.log", "a");
            if (logfile) {
                fprintf(logfile, "textcontent 000000 \n");
                fprintf(logfile, "textcontent 000000 Synonyms (Object):\n");
                int n = 0;
                while (object_synonyms_trio[n] && n < 20000) {
                    char** object_synonym_buf = object_synonyms_trio[n];
                    
                    fprintf(logfile, "linkcontent 000000 \t - %s", object_synonym_buf[0]);
                    int u;
                    for (u = 0; u < 25-strlen(object_synonym_buf[0]) && u < 26; ++u) {
                        fprintf(logfile, " ");
                    }
                    fprintf(logfile, " (");
                    fprintf(logfile, "%s)\n", object_synonym_buf[2]);
                    fprintf(logfile, "property    pk     %s\n", object_synonym_buf[1]);
                    
                    
                    ++n;
                }
                if (0 == n) {
                    fprintf(logfile, "textcontent 000000 \t none\n");
                }
                fprintf(logfile, "end box\n");
                fclose(logfile);
            }
        }
    }
    
    char** important_records = calloc(20000*sizeof(char*), 1);
    {
        {
            int num_of_words = 0;
            char** words = calloc(501*sizeof(char*), 1);
            traditional_detect_words(&num_of_words, words, r);
            strcat(sql, "SELECT fact FROM rel_word_fact WHERE (");
            int n = 0;
            while (subject_synonyms[n] && n < 20000) {
                char* subject_synonym_buf = subject_synonyms[n];
                if (subject_synonym_buf[0] == '*')
                    subject_synonym_buf += 1;
                
                if (strstr(subject_synonym_buf, "*")) {
                    strcat(sql, "rel_word_fact.word GLOB \"");
                    strcat(sql, subject_synonym_buf);
                    strcat(sql, "\" OR ");
                }
                else {
                    strcat(sql, "rel_word_fact.word = \"");
                    strcat(sql, subject_synonym_buf);
                    strcat(sql, "\" OR ");
                }
                ++n;
            }
            while (num_of_words >= 0) {
                if (words[num_of_words]) {
                    if (words[num_of_words][0] != '0') {
                        if (strstr(words[num_of_words], "*")) {
                            strcat(sql, "rel_word_fact.word GLOB \"");
                            strcat(sql, words[num_of_words]);
                            strcat(sql, "\" ");
                        }
                        else {
                            strcat(sql, "rel_word_fact.word = \"");
                            strcat(sql, words[num_of_words]);
                            strcat(sql, "\" ");
                        }
                        free(words[num_of_words]);

                        --num_of_words;

                        if (num_of_words>=0) {
                            if (num_of_words >= 0) strcat(sql, "OR ");
                        }
                    }
                    else {
                        strcat(sql, " 0 ");
                        --num_of_words;
                    }
                }
                else {
                    --num_of_words;
                }
            }
            strcat(sql, ")");
            free(words);
        }
        if (r->verb_flag_want)
            strcat(sql, " AND EXISTS(SELECT 1 FROM rel_fact_flag AS rff WHERE rff.fact = rel_word_fact.fact AND verb_flag_want   = 1) ");
        if (r->verb_flag_must)
            strcat(sql, " AND EXISTS(SELECT 1 FROM rel_fact_flag AS rff WHERE rff.fact = rel_word_fact.fact AND verb_flag_must   = 1) ");
        if (r->verb_flag_can)
            strcat(sql, " AND EXISTS(SELECT 1 FROM rel_fact_flag AS rff WHERE rff.fact = rel_word_fact.fact AND verb_flag_can    = 1) ");
        if (r->verb_flag_may)
            strcat(sql, " AND EXISTS(SELECT 1 FROM rel_fact_flag AS rff WHERE rff.fact = rel_word_fact.fact AND verb_flag_may    = 1) ");
        if (r->verb_flag_should)
            strcat(sql, " AND EXISTS(SELECT 1 FROM rel_fact_flag AS rff WHERE rff.fact = rel_word_fact.fact AND verb_flag_should = 1) ");

        strcat(sql, " UNION ALL ");
        {
            int num_of_words = 0;
            char** words = calloc(501*sizeof(char*), 1);
            traditional_detect_words(&num_of_words, words, r);
            strcat(sql, "SELECT facts.rel FROM rel_word_fact JOIN facts ON facts.pk = rel_word_fact.fact WHERE facts.rel <> -1 AND (");
            int n = 0;
            while (subject_synonyms[n] && n < 20000) {
                char* subject_synonym_buf = subject_synonyms[n];
                if (subject_synonym_buf[0] == '*')
                    subject_synonym_buf += 1;
                if (strstr(subject_synonym_buf, "*")) {
                    strcat(sql, "rel_word_fact.word GLOB \"");
                    strcat(sql, subject_synonym_buf);
                    strcat(sql, "\" OR ");
                }
                else {
                    strcat(sql, "rel_word_fact.word = \"");
                    strcat(sql, subject_synonym_buf);
                    strcat(sql, "\" OR ");
                }
                ++n;
            }
            while (num_of_words >= 0) {
                if (words[num_of_words]) {
                    if (words[num_of_words][0] != '0') {
                        if (strstr(words[num_of_words], "*")) {
                            strcat(sql, "rel_word_fact.word GLOB \"");
                            strcat(sql, words[num_of_words]);
                            strcat(sql, "\" ");
                        }
                        else {
                            strcat(sql, "rel_word_fact.word = \"");
                            strcat(sql, words[num_of_words]);
                            strcat(sql, "\" ");
                        }
                        free(words[num_of_words]);

                        --num_of_words;

                        if (num_of_words>=0) {
                            if (num_of_words >= 0) strcat(sql, "OR ");
                        }
                    }
                    else {
                        strcat(sql, " 0 ");
                        --num_of_words;
                    }
                }
                else {
                    --num_of_words;
                }
            }
            strcat(sql, ")");
            free(words);
        }
        if (r->verb_flag_want)
            strcat(sql, " AND (rel_word_fact.verb_flag_want   = 1) ");
        if (r->verb_flag_must)
            strcat(sql, " AND (rel_word_fact.verb_flag_must   = 1) ");
        if (r->verb_flag_can)
            strcat(sql, " AND (rel_word_fact.verb_flag_can    = 1) ");
        if (r->verb_flag_may)
            strcat(sql, " AND (rel_word_fact.verb_flag_may    = 1) ");
        if (r->verb_flag_should)
            strcat(sql, " AND (rel_word_fact.verb_flag_should = 1) ");
        strcat(sql, " UNION ALL ");
        {
            int num_of_words = 0;
            char** words = calloc(501*sizeof(char*), 1);
            traditional_detect_words(&num_of_words, words, r);
            strcat(sql, "SELECT facts.pk FROM rel_word_fact JOIN facts ON facts.rel = rel_word_fact.fact WHERE facts.rel <> -1 AND (");
            int n = 0;
            while (subject_synonyms[n] && n < 20000) {
                char* subject_synonym_buf = subject_synonyms[n];
                if (subject_synonym_buf[0] == '*')
                    subject_synonym_buf += 1;
                if (strstr(subject_synonym_buf, "*")) {
                    strcat(sql, "rel_word_fact.word GLOB \"");
                    strcat(sql, subject_synonym_buf);
                    strcat(sql, "\" OR ");
                }
                else {
                    strcat(sql, "rel_word_fact.word = \"");
                    strcat(sql, subject_synonym_buf);
                    strcat(sql, "\" OR ");
                }
                ++n;
            }
            while (num_of_words >= 0) {
                if (words[num_of_words]) {
                    if (words[num_of_words][0] != '0') {
                        if (strstr(words[num_of_words], "*")) {
                            strcat(sql, "rel_word_fact.word GLOB \"");
                            strcat(sql, words[num_of_words]);
                            strcat(sql, "\" ");
                        }
                        else {
                            strcat(sql, "rel_word_fact.word = \"");
                            strcat(sql, words[num_of_words]);
                            strcat(sql, "\" ");
                        }
                        free(words[num_of_words]);

                        --num_of_words;

                        if (num_of_words>=0) {
                            if (num_of_words >= 0) strcat(sql, "OR ");
                        }
                    }
                    else {
                        strcat(sql, " 0 ");
                        --num_of_words;
                    }
                }
                else {
                    --num_of_words;
                }
            }
            strcat(sql, ")");
            free(words);
        }
        if (r->verb_flag_want)
            strcat(sql, " AND (rel_word_fact.verb_flag_want   = 1) ");
        if (r->verb_flag_must)
            strcat(sql, " AND (rel_word_fact.verb_flag_must   = 1) ");
        if (r->verb_flag_can)
            strcat(sql, " AND (rel_word_fact.verb_flag_can    = 1) ");
        if (r->verb_flag_may)
            strcat(sql, " AND (rel_word_fact.verb_flag_may    = 1) ");
        if (r->verb_flag_should)
            strcat(sql, " AND (rel_word_fact.verb_flag_should = 1) ");

        strcat(sql, " ORDER BY 1");

        printf("%s\n", sql);
        
        char* err;
        while (sqlite3_exec(traditional_sqlite_connection, sql, callback_synonyms, &important_records, &err)) {
            if (strstr(err, "are not unique")) {
                break;
            }
            if (strstr(err, "callback requested query abort")) {
                break;
            }
            printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            if (strstr(err, "no such table")) {
                sqlite3_free(err);
                if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                    printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
                }
            }
            else {
                break;
            }
        }
        sqlite3_free(err);
    
        free(sql);
        sql = malloc(512000+sizeof(r));
        *sql = 0;
    }
    
    strcat(sql, "delete from cache_facts;");
    {
        strcat(sql, "INSERT INTO cache_facts SELECT * from facts WHERE pk IN (0");
        int w = 0;
        while (important_records[w] && w < 20000) {

            strcat(sql, ", ");
            strcat(sql, important_records[w]);

            ++w;
        }
        strcat(sql, ");");
    }


    /////////////////////////// First, select the normal facts ///////////////////////////
    strcat(sql, "SELECT `nmain`.`pk`, `nmain`.`rel`, `nmain`.`verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, `nmain`.`subjects`, ");
    strcat(sql, replace_in_select("`nmain`.`objects`", object_synonyms, objects_buffer));
    strcat(sql, ", `nmain`.`adverbs`, `nmain`.`prio`, `nmain`.`from`, `nmain`.`truth` FROM `cache_facts` AS nmain "
                    " LEFT JOIN rel_fact_flag AS rff ON rff.fact = nmain.pk");
    
    if (r->everything_q != EVERYTHING) {
    
        strcat(sql, " WHERE ");
        if (important_records[0]) {
            strcat(sql, " ( ");
            int w = 0;
            while (important_records[w] && w < 20000) {
                ++w;
            }
            --w;
            if (w < 0)
                w = 0;
            strcat(sql, " nmain.pk >= ");
            strcat(sql, important_records[0]);
            strcat(sql, " AND nmain.pk <= ");
            strcat(sql, important_records[w]);
            strcat(sql, " ) AND ");
        }
        strcat(sql, " nmain.rel = -1 AND (((nmain.objects <> \"\" OR nmain.adverbs <> \"\")) OR NOT (nmain.verbgroup = \"be\"))");
        need_and = 1;
        
        /// Match the primary key
        if (r->pkey && *r->pkey != '0' && *r->pkey != ' ' && strlen(r->pkey)) {
            if (need_and) strcat(sql, " AND");
            else          strcat(sql, "WHERE");
            strcat(sql, " (nmain.pk IN (SELECT f2 FROM linking WHERE f1 = ");
            strcat(sql, r->pkey);
            strcat(sql, " AND f1 <> f2) OR nmain.pk IN (SELECT f1 FROM linking WHERE f2 = 0 - ");
            strcat(sql, r->pkey);
            strcat(sql, " AND f1 <> f2))");
        }
        
        char** similar_verbs;
        if (r->verb && *r->verb != '0' && *r->verb != ' ' && strlen(r->verb)) {
            char* sql_rel_verb_verb = malloc(512000);
            *sql_rel_verb_verb = 0;
            strcat(sql_rel_verb_verb, "SELECT v2 FROM rel_verb_verb WHERE v1 = \"");
            strcat(sql_rel_verb_verb, r->verb);
            strcat(sql_rel_verb_verb, "\";");

            similar_verbs = calloc(20000*sizeof(char*), 1);
            printf("%s\n", sql_rel_verb_verb);
            
            char* err;
            while (sqlite3_exec(traditional_sqlite_connection, sql_rel_verb_verb, callback_synonyms, &similar_verbs, &err)) {
                if (strstr(err, "are not unique")) {
                    break;
                }
                if (strstr(err, "callback requested query abort")) {
                    break;
                }
                printf("Error while executing SQL: %s\n\n%s\n\n", sql_rel_verb_verb, err);
                if (strstr(err, "no such table")) {
                    sqlite3_free(err);
                    if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                        printf("Error while executing SQL: %s\n\n%s\n\n", sql_rel_verb_verb, err);
                    }
                }
                else {
                    break;
                }
            }
            sqlite3_free(err);
            free(sql_rel_verb_verb);
        }

        if (r->verb && *r->verb != '0' && *r->verb != ' ' && strlen(r->verb)) {
            
            // If we do NOT have a WHERE phrase
            // temporarily disabled
            //if (0 != strcmp(r->context, "q_where")) {
            
            {
                if (need_and) strcat(sql, " AND");
                else          strcat(sql, "WHERE");
                strcat(sql, "( 1 ");
                char* buffers_all_verbs = malloc(strlen(r->verb)+2);
                strcpy(buffers_all_verbs, r->verb);
                if (buffers_all_verbs) {
                    char* buffer = strtok(buffers_all_verbs, " ");
                    char* buffers[10];
                    int l = 0;
                    while (buffer && l < 10) {
                        buffers[l] = buffer;
                        buffer = strtok(NULL, " ");
                        ++l;
                    }
                    --l;
                    
                    short flag_is_divided_verb = 1;
                    if (l == 0) {
                        flag_is_divided_verb = 0;
                    }
                    
                    while (l >= 0) {
                        buffer = buffers[l];
                        char* buffers = malloc(strlen(r->verb)+2);
                        strcpy(buffers, buffer);
                        if (buffers && !strstr(buffers, "*")) {
                            char* buffer = strtok(buffers, "|");
                            if (buffer) {
                                if (need_and) strcat(sql, " AND");
                                else          strcat(sql, "WHERE");
                                strcat(sql, " ( nmain.verb = \"");
                                strcat(sql, buffer);
                                strcat(sql, "\"");
                                if (flag_is_divided_verb) {
                                    strcat(sql, " OR nmain.verb GLOB \"");
                                    strcat(sql, buffer);
                                    strcat(sql, " *\"");
                                    strcat(sql, " OR nmain.verb GLOB \"* ");
                                    strcat(sql, buffer);
                                    strcat(sql, "\"");
                                }
                                while (buffer = strtok(NULL, "|")) {
                                    strcat(sql, " OR nmain.verb = \"");
                                    strcat(sql, buffer);
                                    strcat(sql, "\"");
                                    if (flag_is_divided_verb) {
                                        strcat(sql, " OR nmain.verb GLOB \"* ");
                                        strcat(sql, buffer);
                                        strcat(sql, "\"");
                                        strcat(sql, " OR nmain.verb GLOB \"");
                                        strcat(sql, buffer);
                                        strcat(sql, " *\"");
                                    }
                                }
                                strcat(sql, " )");
                                need_and = 1;
                            }
                        }
                        free(buffers);
                        --l;
                    }
                }
            //}
                strcat(sql, ")");
            }
            /// similar verbs:
            if (similar_verbs && similar_verbs[0]) {
                strcat(sql, " OR ( ");
                int w = 0;
                while (similar_verbs[w]) {
                    char* buffer = similar_verbs[w];
                    char* buffers[10];
                    int l = 0;
                    while (buffer && l < 10) {
                        buffers[l] = buffer;
                        buffer = strtok(NULL, " ");
                        ++l;
                    }
                    --l;
                    
                    short flag_is_divided_verb = 1;
                    if (l == 0) {
                        flag_is_divided_verb = 0;
                    }
                    
                    while (l >= 0) {
                        buffer = buffers[l];
                        char* buffers = malloc(strlen(similar_verbs[w])+2);
                        strcpy(buffers, buffer);
                        if (buffers && !strstr(buffers, "*")) {
                            char* buffer = strtok(buffers, "|");
                            if (buffer) {
                                {
                                    strcat(sql, " ( nmain.verb = \"");
                                    strcat(sql, buffer);
                                    strcat(sql, "\"");
                                    if (flag_is_divided_verb) {
                                        strcat(sql, " OR nmain.verb GLOB \"");
                                        strcat(sql, buffer);
                                        strcat(sql, " *\"");
                                        strcat(sql, " OR nmain.verb GLOB \"* ");
                                        strcat(sql, buffer);
                                        strcat(sql, "\"");
                                    }
                                    while (buffer = strtok(NULL, "|")) {
                                        strcat(sql, " OR nmain.verb = \"");
                                        strcat(sql, buffer);
                                        strcat(sql, "\"");
                                        if (flag_is_divided_verb) {
                                            strcat(sql, " OR nmain.verb GLOB \"* ");
                                            strcat(sql, buffer);
                                            strcat(sql, "\"");
                                            strcat(sql, " OR nmain.verb GLOB \"");
                                            strcat(sql, buffer);
                                            strcat(sql, " *\"");
                                        }
                                    }
                                    strcat(sql, " )");
                                }
                            }
                        }
                        free(buffers);
                        --l;
                    }
                    ++w;
                }
                strcat(sql, " ) ");
            }
        }
        if (subjects_buffer) {
            if (need_and) strcat(sql, " AND");
            else          strcat(sql, "WHERE");
            
            if (strlen(subjects_buffer)) {
                if (strstr(subjects_buffer, "*") || 0 == strcmp(subjects_buffer, "das")) {
                    strcat(sql, "\n ( nmain.subjects GLOB \"");
                    strcat(sql, subjects_buffer);
                    strcat(sql, "\" OR nmain.objects GLOB \"* ");
                    strcat(sql, subjects_buffer);
                    strcat(sql, "\" OR nmain.subjects GLOB \"* ");
                    strcat(sql, subjects_buffer);
                    strcat(sql, "\" ");
                }
                else {
                    strcat(sql, "\n ( nmain.subjects GLOB \"");
                    strcat(sql, subjects_buffer);
                    strcat(sql, "\" OR nmain.subjects GLOB \"* ");
                    strcat(sql, subjects_buffer);
                    strcat(sql, "\" OR nmain.subjects GLOB \"");
                    strcat(sql, subjects_buffer);
                    strcat(sql, " *\" OR nmain.objects GLOB \"*");
                    strcat(sql, subjects_buffer);
                    strcat(sql, "\" OR nmain.subjects GLOB \"");
                    strcat(sql, subjects_buffer);
                    strcat(sql, "\" ");
                }
            }
            else {
                    strcat(sql, "\n ( 0 ");
            }
            
            int n = 0;
            while (subject_synonyms[n] && n < 20000) {
                if (strlen(subject_synonyms[n])) {
                    if (strstr(subject_synonyms[n], "*")) {
                        strcat(sql, "\n OR nmain.subjects GLOB \"");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" OR nmain.objects GLOB \"* ");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" OR nmain.subjects GLOB \"* ");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" ");
                    }
                    else {
                        strcat(sql, "\n OR nmain.subjects GLOB \"");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" OR nmain.subjects GLOB \"* ");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" OR nmain.subjects GLOB \"");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, " *\" OR nmain.objects GLOB \"*");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" OR nmain.subjects GLOB \"");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" ");
                    }
                }
                else {
                        strcat(sql, "\n ");
                }
                ++n;
            }
            strcat(sql, ")");
            need_and = 1;
        }
        if (objects_buffer && (0 == subjects_buffer || strcmp(objects_buffer, subjects_buffer))) {
            if (need_and) strcat(sql, " AND");
            else          strcat(sql, "WHERE");
            strcat(sql, "\n ( ");
            if (r->context && *r->context != 'd' && strlen(r->context) && !strcmp(r->context, "default")) {
                strcat(sql, "nmain.objects = \"*\"");
            }
            strcat(sql, " nmain.objects GLOB \"");
            strcat(sql, objects_buffer);
            if (objects_buffer == subjects_buffer && objects_buffer == r->extra) {
                strcat(sql, "\" OR nmain.objects = \"\" OR nmain.objects = \" ");
            }
            strcat(sql, "\" ");
            strcat(sql, " OR nmain.objects GLOB \"* ");
            strcat(sql, objects_buffer);
            strcat(sql, "\" ");

            int n = 0;
            while (object_synonyms[n] && n < 20000) {
                if (strlen(object_synonyms[n]) && (!subjects_buffer || strcmp(object_synonyms[n], subjects_buffer))) {
                    strcat(sql, "\n OR nmain.objects GLOB \"");
                    strcat(sql, object_synonyms[n]);
                    strcat(sql, "\" ");
                    strcat(sql, "OR nmain.objects GLOB \"* ");
                    strcat(sql, object_synonyms[n]);
                    strcat(sql, "\" ");
                }
                ++n;
            }
            strcat(sql, " ) ");

            need_and = 1;
        }
        if (r->context && *r->context != 'd' && strlen(r->context) && !strcmp(r->context, "default")) {
            printf("Context is '%s'.\n", r->context);
        }
        if (r->context && *r->context != 'd' && strlen(r->context) && strcmp(r->context, "default")) {
            printf("Context is '%s' (verb = '%s').\n", r->context, (r->verb && *r->verb != '0' && *r->verb != ' ') ? r->verb : "" );

            char* buffers = calloc(5000, 1);
            *buffers = 0;
            short flag_should_contain = 1;
            
            if (0 == strcmp(r->context, "q_what_weakly")) {
                if (r->verb && *r->verb != '0' && *r->verb != ' ' && (strstr(r->verb, "ist") == r->verb || strstr(r->verb, "war") == r->verb)) {
                    strcpy(buffers, "* ein*");
                }
            }
            if (0 == strcmp(r->context, "q_who")) {
                if (r->verb && *r->verb != '0' && *r->verb != ' ' && (strstr(r->verb, "ist") == r->verb || strstr(r->verb, "war") == r->verb)) {
                    strcpy(buffers, "ein*");
                    flag_should_contain = 0;
                    need_and = 1;
                }
            }
            if (0 == strcmp(r->context, "q_where")) {
                if (r->verb && *r->verb != '0' && *r->verb != ' ' && strstr(r->verb, "|ist|")) {
                    strcpy(buffers, "* in *;* an *;* from *;* at *;* auf *;* aus *;in *;an *;from *;at *;auf *;aus *");
                }
                else {
                    strcpy(buffers, "* in *;* an *;* from *;* at *;* auf *;* von *;* aus *;in *;an *;from *;at *;auf *;von *;aus *");
                }
            }
            if (0 == strcmp(r->context, "q_how")) {
                if (!strstr(r->verb, "|ist|")) {
                    flag_should_contain = 0;
                    strcpy(buffers, "* in *;* im *;* an *;* from *;* at *;* auf *;* von *;* aus *;in *;im *;an *;from *;at *;auf *;von *;aus *");
                }
            }
            if (0 == strcmp(r->context, "q_from_where")) {
                strcpy(buffers, "aus *;von *;from *;aus *;durch *;* aus *;* von *;* from *;* aus *;* durch *");
            }
            
            if (buffers) {
                char* buffer = strtok(buffers, ";");
                if (buffer) {
                    if (need_and) strcat(sql, " AND");
                    else          strcat(sql, "WHERE");
                    strcat(sql, " ( ");
                    if (flag_should_contain) {
                        strcat(sql, " (nmain.mix_1 GLOB \"");
                        strcat(sql, buffer);
                        strcat(sql, "\")");
                    }
                    else {
                        strcat(sql, " ((NOT(nmain.mix_1 GLOB \"");
                        strcat(sql, buffer);
                        strcat(sql, "\")))");
                    }
                    
                    while (buffer = strtok(NULL, ";")) {
                        if (flag_should_contain) {
                            strcat(sql, " OR ( nmain.mix_1 GLOB \"");
                            strcat(sql, buffer);
                            strcat(sql, "\")");
                        }
                        else {
                            strcat(sql, " AND ( (NOT(nmain.mix_1 GLOB \"");
                            strcat(sql, buffer);
                            strcat(sql, "\")))");
                        }
                    }
                    strcat(sql, " ) ");

                    need_and = 1;
                }
            }
            halfree(buffers);
        }
        if (r->adverbs && *r->adverbs != '0' && strlen(r->adverbs)) {
            char* buffer;
            buffer = strtok(r->adverbs, " ;,)(-.");
            if (buffer) {
                if (need_and) strcat(sql, " AND");
                else          strcat(sql, "WHERE");
                strcat(sql, " ( nmain.mix_1 GLOB \"*");
                strcat(sql, buffer);
                strcat(sql, "*\")");
                
                while (buffer = strtok(NULL, " ;,)(-.")) {
                    strcat(sql, " AND");
                    strcat(sql, " ( nmain.mix_1 GLOB \"*");
                    strcat(sql, buffer);
                    strcat(sql, "*\")");
                }

                need_and = 1;
            }
        }
        if (r->extra && *r->extra != '0' && strlen(r->extra) && r->extra != objects_buffer && r->extra != subjects_buffer) {
            char* buffer = r->extra;
            if (need_and) strcat(sql, " AND");
            else          strcat(sql, "WHERE");
            strcat(sql, " ( nmain.mix_1 GLOB \"* ");
            strcat(sql, buffer);
            strcat(sql, "*\" OR nmain.mix_1 GLOB \"*");
            strcat(sql, buffer);
            strcat(sql, " *\" OR nmain.mix_1 GLOB \"* ");
            strcat(sql, buffer);
            strcat(sql, " *\"");

            int n = 0;
            while (extra_synonyms[n] && n < 20000) {
                char* extra_synonym_buf = extra_synonyms[n];
                
                if (extra_synonym_buf && extra_synonym_buf[0]) {
                    ++extra_synonym_buf;
                    if (strlen(extra_synonym_buf) >= 2) {
                        extra_synonym_buf[strlen(extra_synonym_buf)-1] = 0;
                        
                        strcat(sql, " OR nmain.mix_1 GLOB \"* ");
                        strcat(sql, extra_synonym_buf);
                        strcat(sql, "*\" OR nmain.mix_1 GLOB \"*");
                        strcat(sql, extra_synonym_buf);
                        strcat(sql, " *\" OR nmain.mix_1 GLOB \"* ");
                        strcat(sql, extra_synonym_buf);
                        strcat(sql, " *\"");
                    }
                }
                    
                ++n;
            }

            if (0 == strcmp(r->context, "q_what_prep")) {
                strcat(sql, " OR EXISTS(SELECT i.subjects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.subjects <> \"\" AND i.objects GLOB \"");
                strcat(sql, buffer);
                strcat(sql, "*\" AND nmain.adverbs GLOB \"* \"||replace(replace(replace(replace(replace(replace(replace(replace(i.subjects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\"))");
                strcat(sql, " OR EXISTS(SELECT i.subjects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.subjects <> \"\" AND i.objects GLOB \"");
                strcat(sql, buffer);
                strcat(sql, "*\" AND nmain.adverbs GLOB replace(replace(replace(replace(replace(replace(replace(replace(i.subjects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\")||\" *\")");
                strcat(sql, " OR EXISTS(SELECT i.objects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.objects <> \"\" AND i.subjects GLOB \"");
                strcat(sql, buffer);
                strcat(sql, "*\" AND nmain.adverbs GLOB \"* \"||replace(replace(replace(replace(replace(replace(replace(replace(i.objects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\"))");
                strcat(sql, " OR EXISTS(SELECT i.objects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.objects <> \"\" AND i.subjects GLOB \"");
                strcat(sql, buffer);
                strcat(sql, "*\" AND nmain.adverbs GLOB replace(replace(replace(replace(replace(replace(replace(replace(i.objects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\")||\" *\")");
            
                strcat(sql, " OR EXISTS(SELECT i.subjects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.subjects <> \"\" AND i.objects GLOB \"*");
                strcat(sql, buffer);
                strcat(sql, "\" AND nmain.adverbs GLOB \"* \"||replace(replace(replace(replace(replace(replace(replace(replace(i.subjects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\"))");
                strcat(sql, " OR EXISTS(SELECT i.subjects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.subjects <> \"\" AND i.objects GLOB \"*");
                strcat(sql, buffer);
                strcat(sql, "\" AND nmain.adverbs GLOB replace(replace(replace(replace(replace(replace(replace(replace(i.subjects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\")||\" *\")");
                strcat(sql, " OR EXISTS(SELECT i.objects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.objects <> \"\" AND i.subjects GLOB \"*");
                strcat(sql, buffer);
                strcat(sql, "\" AND nmain.adverbs GLOB \"* \"||replace(replace(replace(replace(replace(replace(replace(replace(i.objects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\"))");
                strcat(sql, " OR EXISTS(SELECT i.objects FROM facts AS i WHERE (i.verbgroup = \"be\") AND ");
                strcat(sql, "i.objects <> \"\" AND i.subjects GLOB \"*");
                strcat(sql, buffer);
                strcat(sql, "\" AND nmain.adverbs GLOB replace(replace(replace(replace(replace(replace(replace(replace(i.objects, \"ein \", \"\"), \"eine \", \"\"), \"a \", \"\"), \"an \", \"\"), \"der \", \"\"), \"die \", \"\"), \"das \", \"\"), \"the \", \"\")||\" *\")");
            }
            strcat(sql, ")");
            need_and = 1;
        }
        
        strcat(sql, " AND nmain.questionword NOT IN (\"wenn\", \"if\", \"falls\", \"when\")"
                    " AND NOT EXISTS(SELECT nclause.pk FROM facts AS nclause WHERE nclause.rel = nmain.pk AND ( nclause.questionword IN (\"wenn\", \"if\", \"falls\", \"when\") OR nclause.verb GLOB \"*=>*\" ))");
        
        /////////////////////////// Then, select facts by if statements ///////////////////////////
        
        // There must be no primary key
        if ( !( r->pkey && *r->pkey != '0' && *r->pkey != ' ' && strlen(r->pkey) ) ) {

            need_and = 0;

            strcat(sql, " \n\nUNION ALL "
                        "SELECT DISTINCT \n"
                        "fact.pk, fact.rel, fact.verb || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, fact.subjects, fact.objects, fact.adverbs, fact.prio, NULL, fact.truth \n"
                        "FROM facts AS main JOIN clauses AS clause ON ");
            if (important_records[0]) {
                strcat(sql, " ( ");
                int w = 0;
                while (important_records[w] && w < 20000) {
                    ++w;
                }
                --w;
                if (w < 0)
                    w = 0;
                strcat(sql, " main.pk >= ");
                strcat(sql, important_records[0]);
                strcat(sql, " AND main.pk <= ");
                strcat(sql, important_records[w]);
                strcat(sql, " ");
                strcat(sql, " ) AND ");
            }
            strcat(sql, " main.pk = clause.rel ");
            strcat(sql, "JOIN cache_facts AS fact ON main.truth = 1 AND \n"
                        "((clause.verbgroup <> \"have\" AND clause.verbgroup <> \"be\" AND fact.verb GLOB clause.verb) OR "
                        "((clause.verbgroup = \"have\") AND (fact.verbgroup = \"have\") ) OR  "
                        "((clause.verbgroup = \"be\") AND (fact.verbgroup = \"be\") )) ");
    //                    "JOIN rel_word_fact ON (rel_word_fact.fact = main.pk OR rel_word_fact.fact = clause.pk)\n\n");

            strcat(sql, " LEFT JOIN rel_fact_flag AS rff ON rff.fact = fact.pk ");
            {
                if (need_and) strcat(sql, " AND");
                else          strcat(sql, "WHERE");
                strcat(sql, " fact.questionword = \"\" AND fact.subjects <> \"*\" AND fact.objects <> \"*\"");
                need_and = 1;
                
                {
                    if (need_and) strcat(sql, " AND");
                    else          strcat(sql, "WHERE");

                    strcat(sql, "\n (((main.subjects = \"*\" OR clause.subjects = \"*\") AND (clause.subjects <> \"*\" OR main.subjects <> \"*\")) OR (main.subjects = \"\") OR (clause.subjects = \"\") OR (clause.subjects <> \"*\" AND fact.subjects GLOB clause.subjects) OR (clause.subjects = \"*\" AND fact.subjects GLOB \"");
                    if (r->subjects && *r->subjects != '0' && strlen(r->subjects)) {
                        strcat(sql, r->subjects);
                    }
                    strcat(sql, "\")");
                    int n = 0;
                    while (subject_synonyms[n] && n < 20000) {
                        strcat(sql, " OR fact.subjects GLOB \"");
                        strcat(sql, subject_synonyms[n]);
                        strcat(sql, "\" ");
                        ++n;
                    }
                    strcat(sql, " ) AND (clause.objects = \"\" OR fact.objects GLOB clause.objects OR fact.objects GLOB main.objects) AND (clause.adverbs = \"\" OR clause.adverbs = \"\" OR fact.adverbs GLOB clause.adverbs)"
                    "\n AND (((fact.objects <> \"\" OR fact.adverbs <> \"\") AND fact.objects <> \"\") OR NOT (fact.verbgroup = \"be\"))\n");
                }
            }

            if (r->verb && *r->verb != '0' && *r->verb != ' ' && strlen(r->verb)) {
                char* buffers = malloc(strlen(r->verb)+2);
                strcpy(buffers, r->verb);
                if (buffers && !strstr(buffers, "*")) {
                    char* buffer = strtok(buffers, "|");
                    if (buffer) {
                        if (need_and) strcat(sql, " AND");
                        else          strcat(sql, "WHERE");
                        strcat(sql, " ( main.verb = \"");
                        strcat(sql, buffer);
                        strcat(sql, "\"");
                        while (buffer = strtok(NULL, "|")) {
                            strcat(sql, " OR main.verb = \"");
                            strcat(sql, buffer);
                            strcat(sql, "\"");
                        }
                        strcat(sql, " )");
                        need_and = 1;
                    }
                }
                free(buffers);
            }
            if (r->context && *r->context != 'd' && strlen(r->context) && strcmp(r->context, "default")) {
                char* buffers = calloc(5000, 1);
                *buffers = 0;
                short flag_should_contain = 1;
                
                if (0 == strcmp(r->context, "q_what_weakly")) {
                    if (r->verb && *r->verb != '0' && *r->verb != ' ' && (strstr(r->verb, "ist") || strstr(r->verb, "war"))) {
                        if ( !(r->adverbs && *r->adverbs != '0' && strlen(r->adverbs))) {
                            strcpy(buffers, "ein*");
                        }
                    }
                }
                if (0 == strcmp(r->context, "q_who")) {
                    strcpy(buffers, "ein*");
                    flag_should_contain = 0;
                    if (need_and) strcat(sql, " AND");
                    else          strcat(sql, "WHERE");
                    strcat(sql, " ( main.adverbs = \"\" )");
                    need_and = 1;
                }
                if (0 == strcmp(r->context, "q_where")) {
                    strcpy(buffers, "*in ;*im ;*am ;*an ;* aus;* von;*from ;*at ;in *;im *;am *;an *;from *;at *;aus *;von *");
                }
                
                if (buffers) {
                    char* buffer = strtok(buffers, ";");
                    if (buffer) {
                        if (need_and) strcat(sql, " AND");
                        else          strcat(sql, "WHERE");
                        strcat(sql, " ( ");
                        if (0 == strcmp(r->context, "q_who")) {
                            strcat(sql, "( main.adverbs = \"\" ) AND main.truth > 0.9 AND ");
                        }
                        if (flag_should_contain) {
                            strcat(sql, " ( main.mix_1 GLOB \"");
                            strcat(sql, buffer);
                            strcat(sql, "\")");
                        }
                        else {
                            strcat(sql, " ( (NOT(main.mix_1 GLOB \"");
                            strcat(sql, buffer);
                            strcat(sql, "\")))");
                        }
                        
                        while (buffer = strtok(NULL, ";")) {
                            if (flag_should_contain) {
                                strcat(sql, " OR ( main.mix_1 GLOB \"");
                                strcat(sql, buffer);
                                strcat(sql, "\")");
                            }
                            else {
                                strcat(sql, " OR ( (NOT(main.mix_1 GLOB \"");
                                strcat(sql, buffer);
                                strcat(sql, "\")))");
                            }
                        }
                        strcat(sql, " ) ");

                        need_and = 1;
                    }
                }
                halfree(buffers);
            }
            if (r->extra && *r->extra != '0' && strlen(r->extra)) {
                char* buffer = r->extra;
                if (need_and) strcat(sql, " AND");
                else          strcat(sql, "WHERE");

                strcat(sql, " EXISTS(SELECT 1 FROM rel_word_fact WHERE word = \"");
                strcat(sql, buffer);
                strcat(sql, "\" AND fact IN (main.pk, clause.pk) ) ");
                
                need_and = 1;
            }
            if (r->subjects && *r->subjects != '0' && strlen(r->subjects)) {
                if (need_and) strcat(sql, " AND");
                else          strcat(sql, "WHERE");
                strcat(sql, "\n ( \n");
                
                strcat(sql, "       (       main.subjects = \"*\" AND (\n\"");
                strcat(sql, r->subjects);
                strcat(sql, "\" GLOB fact.subjects OR fact.subjects GLOB\n\"");
                strcat(sql, r->subjects);
                strcat(sql, "\" )\n");
                strcat(sql, "       )\n");
                strcat(sql, " OR    (       main.subjects <> \"*\" AND (\n\"");
                strcat(sql, r->subjects);
                strcat(sql, "\" GLOB main.subjects OR main.subjects GLOB \n\"");
                strcat(sql, r->subjects);
                strcat(sql, "\" )\n");
                strcat(sql, "       )\n");
                
                int n = 0;
                while (subject_synonyms[n] && n < 20000) {
                    strcat(sql, " OR    (       main.subjects = \"*\" AND (\n\"");
                    strcat(sql, subject_synonyms[n]);
                    strcat(sql, "\" GLOB fact.subjects OR fact.subjects GLOB\n\"");
                    strcat(sql, subject_synonyms[n]);
                    strcat(sql, "\" )\n");
                    strcat(sql, "       )\n");
                    strcat(sql, " OR    (       main.subjects <> \"*\" AND (\n\"");
                    strcat(sql, subject_synonyms[n]);
                    strcat(sql, "\" GLOB main.subjects OR main.subjects GLOB \n\"");
                    strcat(sql, subject_synonyms[n]);
                    strcat(sql, "\" )\n");
                    strcat(sql, "       )\n");
                    free(subject_synonyms[n]);
                    ++n;
                }
                strcat(sql, ")");
                need_and = 1;
            }
            if (r->objects && *r->objects != '0' && strlen(r->objects)) {
                if (need_and) strcat(sql, " AND");
                else          strcat(sql, "WHERE");
                strcat(sql, "\n ( \n");
                
                strcat(sql, "       (       (main.objects IN (\"*\",\"\")) AND (\n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" GLOB fact.objects OR \n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" GLOB fact.subjects OR fact.objects GLOB\n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" OR fact.subjects GLOB\n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" )\n");
                strcat(sql, "       )\n");
                strcat(sql, " OR    (       main.objects <> \"*\" AND main.objects <> \"\" AND (\n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" GLOB main.objects OR \n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" GLOB main.subjects OR main.objects GLOB \n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" OR main.subjects GLOB \n\"");
                strcat(sql, r->objects);
                strcat(sql, "\" )\n");
                strcat(sql, "       )\n");
                int n = 0;
                while (object_synonyms[n] && n < 20000) {
                    if (strlen(object_synonyms[n]) && (!subjects_buffer || strcmp(object_synonyms[n], subjects_buffer))) {
                        strcat(sql, " OR    (       (main.objects IN (\"*\",\"\")) AND (\n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" GLOB fact.objects OR \n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" GLOB fact.subjects OR fact.objects GLOB\n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" OR fact.subjects GLOB\n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" )\n");
                        strcat(sql, "       )\n");
                        strcat(sql, " OR    (       main.objects <> \"*\" AND main.objects <> \"\" AND (\n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" GLOB main.objects OR \n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" GLOB main.subjects OR main.objects GLOB \n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" OR main.subjects GLOB \n\"");
                        strcat(sql, object_synonyms[n]);
                        strcat(sql, "\" )\n");
                        strcat(sql, "       )\n");
                    }
                    free(object_synonyms[n]);
                    ++n;
                }
                
                strcat(sql, " ) \n");
                need_and = 1;
            }
            if (r->adverbs && *r->adverbs != '0' && strlen(r->adverbs)) {
                char* buffer;
                buffer = strtok(r->adverbs, " ;,)(-.");
                if (buffer) {
                    if (need_and) strcat(sql, " AND");
                    else          strcat(sql, "WHERE");
                    /// (main.subjects IN (\"*\", \"\")) OR (main.objects IN (\"*\", \"\") AND clause.subjects IN (\"*\", \"\")) OR 
                    strcat(sql, "(  (");
                    strcat(sql, " ( main.mix_1 GLOB \"*");
                    strcat(sql, buffer);
                    strcat(sql, "*\")");
                    strcat(sql, " AND ( main.subjects <> \"*\"");
                    strcat(sql, " OR main.mix_1 GLOB \"*");
                    strcat(sql, buffer);
                    strcat(sql, "*\")");
                    
                    while (buffer = strtok(NULL, " ;,)(-.")) {
                        strcat(sql, " AND");
                        strcat(sql, " ( main.mix_1 GLOB \"*");
                        strcat(sql, buffer);
                        strcat(sql, "*\")");
                        strcat(sql, " AND ( main.subjects <> \"*\"");
                        strcat(sql, " OR main.mix_1 GLOB \"*");
                        strcat(sql, buffer);
                        strcat(sql, "*\")");
                        }

                    strcat(sql, "))");
                    need_and = 1;
                }
            }
            /// There are no adverbs given in HAL request
            else if (!(r->extra && *r->extra != '0' && strlen(r->extra))) {
                if (need_and) strcat(sql, " AND");
                else          strcat(sql, "WHERE");
                strcat(sql, " ( main.adverbs = \"\" OR main.adverbs = \"*\" )");
                
                need_and = 1;
            }
        }
        
        /// The User asks for nothing, he accepts every fact
        if (0 == need_and) {
            strcat(sql, " WHERE `pk` >= (abs(random()) * (SELECT max(`pk`) FROM `facts`)) LIMIT 10");
        }
    }
        
    /// End of statement
    strcat(sql, ";");
    
    printf("%s\n", sql);
    
    struct DATASET sqlite_current_data;
    sqlite_current_data.size = 0;
    sqlite_current_data.data = 0;
    time_t time_now;
    time(&time_now);
    sqlite_current_data.timeout = time_now + 100;

    char* err;
    while (sqlite3_exec(traditional_sqlite_connection, sql, callback, &sqlite_current_data, &err)) {
        if (strstr(err, "are not unique")) {
            break;
        }
        printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
        if (strstr(err, "no such table")) {
            sqlite3_free(err);
            if (sqlite3_exec(traditional_sqlite_connection, traditional_sqlite_sql_create_table, NULL, NULL, &err)) {
                printf("Error while executing SQL: %s\n\n%s\n\n", sql, err);
            }
        }
        else {
            break;
        }
    }
    sqlite3_free(err);
    
    free(sql);
    
    return sqlite_current_data;
}

