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

#include "hal2009-disk.h"

int disk_cache_clauses = -1;
int disk_cache_facts = -1;

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
    
    disk_cache_clauses = -1;
    disk_cache_facts   = -1;

    // initialize sqlite
    if (0 == sqlite_connection) {
        printf("%s%s\n", "Open SQLite connection to file: ", sqlite_filename);
        char db_main[999];
        sprintf(db_main, "%s.main", sqlite_filename);

        short create_tables = 0;
        {
            struct stat stbuf;
            stat(db_main, &stbuf);
            int file_size = &stbuf ? stbuf.st_size : 0;
            if (file_size < 1000) {
                create_tables = 1;
            }
        }

        if (sqlite3_open(db_main, &sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            sqlite_connection = 0;
            return NO_CONNECTION;
        }

        char attach_indices[999];
        sprintf(attach_indices, "ATTACH DATABASE \"%s.index\" AS db_index;", sqlite_filename);
        char attach_cache[999];
        sprintf(attach_cache, "ATTACH DATABASE \"%s.cache\" AS db_cache;", sqlite_filename);

        char* err;
        sqlite3_exec(sqlite_connection, attach_indices, NULL, NULL, &err);
        if (err) printf("SQL error: %s\n", err);
        sqlite3_free(err);
        sqlite3_exec(sqlite_connection, attach_cache, NULL, NULL, &err);
        if (err) printf("SQL error: %s\n", err);
        sqlite3_free(err);

        sqlite3_close(sqlite_connection);
        sqlite_connection = 0;

        if (sqlite3_open(db_main, &sqlite_connection)) {
            printf("%s%s\n", "Could not open SQLite connection to file: ", sqlite_filename);
            sqlite_connection = 0;
            return NO_CONNECTION;
        }

        sqlite3_exec(sqlite_connection, attach_indices, NULL, NULL, &err);
        if (err) printf("SQL error: %s\n", err);
        sqlite3_free(err);
        sqlite3_exec(sqlite_connection, attach_cache, NULL, NULL, &err);
        if (err) printf("SQL error: %s\n", err);
        sqlite3_free(err);

        if (create_tables) {
            printf("create tables...\n");
            sqlite3_exec(sqlite_connection, sqlite_sql_create_table, NULL, NULL, &err);
            if (err) printf("SQL error: %s\n", err);
            printf("done.\n");
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
    if (rel) {
        if (disk_cache_clauses > -1) {
            ++disk_cache_clauses;
            return disk_cache_clauses;
        }
    }
    else {
        if (disk_cache_facts > -1) {
            ++disk_cache_facts;
            return disk_cache_facts;
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
        disk_cache_clauses = to_number(key);
    }
    else {
        disk_cache_facts = to_number(key);
    }
    
    return (rel ? disk_cache_clauses : disk_cache_facts);
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
    
    buffer = strtok(verbs, " ;.)(-,_");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,_");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,_");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    
    buffer = strtok(subj, " ;.)(-,_");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,_");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,_");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    buffer = strtok( obj, " ;.)(-,_");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,_");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,_");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    buffer = strtok(advs, " ;.)(-,_");
    while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
        if (is_a_trivial_word(buffer)) {
            buffer = strtok(NULL, " ;.)(-,_");
            continue;
        }
        words[*num_of_words] = strdup(buffer);
        buffer = strtok(NULL, " ;.)(-,_");
        ++(*num_of_words);
        if (*num_of_words >= 500) break;
    }
    
    if (extra) {
        buffer = strtok(extra, " ;.)(-,_");
        while (buffer && strlen(buffer) && strcmp(buffer, "0")) {
            if (is_a_trivial_word(buffer)) {
                buffer = strtok(NULL, " ;.)(-,_");
                continue;
            }
            words[*num_of_words] = strdup(buffer);
            buffer = strtok(NULL, " ;.)(-,_");
            ++(*num_of_words);
            if (*num_of_words >= 500) break;
        }
    }

    --(*num_of_words);
    
    free(verbs);
    free(subj);
    free(obj);
    free(advs);
    free(extra);
}

char* gen_sql_add_entry(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic) {
    
    const char* last_subject_word = subjects;
    if (subjects && strlen(subjects)) {
        const char* found;
        while (found = strstr(last_subject_word, " ")) {
            if (found && strlen(found) > 1) {
                last_subject_word = found+1;
            }
            else {
                ++last_subject_word;
            }
        }
    }
    const char* last_object_word = objects;
    if (objects && strlen(objects)) {
        const char* found;
        while (found = strstr(last_object_word, " ")) {
            if (found && strlen(found) > 1) {
                last_object_word = found+1;
            }
            else {
                ++last_object_word;
            }
        }
    }
    
    if (0 == sql) {
        sql = malloc(102400);
        *sql = 0;
    }
    strcat(sql, "INSERT INTO ");
    strcat(sql, rel ? "clauses" : "facts");
    strcat(sql, " (`pk`, `mix_1`, `verb`, `verbgroup`, `subjects`, `objects`, "
    "`last_subject_word`, `last_object_word`, `adverbs`, `questionword`, `prio`, "
    "`fileid`, `line`, `rel`, `truth`, `only_logic`, `can_be_synonym`) VALUES (");
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
    if (last_subject_word[0] != ' ')
        strcat(sql, last_subject_word);
    strcat(sql, "\", \"");
    if (last_object_word[0] != ' ')
        strcat(sql, last_object_word);
    strcat(sql, "\", \"");
    if (adverbs[0] != ' ')
        strcat(sql, adverbs);
    strcat(sql, "\", \"");
    if (questionword)    strcat(sql, questionword);
    else                 strcat(sql, "NULL");
    strcat(sql, "\", 50, ");
    strcat(sql, from_number(fileid(filename)));
    strcat(sql, ", ");
    strcat(sql, line);
    strcat(sql, ", ");
    if (rel) strcat(sql, from_number(rel));
    else     strcat(sql, "-1");
    strcat(sql, ", ");
    char truth_str[10];
    snprintf(truth_str, 9, "%f", truth);
    strcat(sql, truth_str);
    strcat(sql, ", ");
    strcat(sql, only_logic?"1":"0");
    strcat(sql, ", ");
    strcat(sql, strstr(filename, "fa-")?"0":"1");
    strcat(sql, ");\n");
    
    return sql;
}

char* gen_sql_add_verb_flags(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic) {
    
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

char* gen_sql_add_word_fact_relations(char* sql, int pk, int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword) {

    char key[101];
    snprintf(key, 100, "%d", pk);
    
    if (0 == sql) {
        sql = malloc(102400);
        *sql = 0;
    }
    
    int num_of_words = 0;
    char** words = calloc(501*sizeof(char*), 1);
    detect_words(&num_of_words, words, verbs, subjects, objects, adverbs, "");
    
    if (has_conditional_questionword) {
        ++num_of_words;
        words[num_of_words] = strdup("$$has_conditional_questionword$$");
    }
    
    while (num_of_words >= 0) {
        if (words[num_of_words]) {
            if (words[num_of_words][0] != '0') {
                /*{
                    strcat(sql, "INSERT OR IGNORE INTO db_index`.`rel_word_fact__general (`word`, `fact`, `table`) VALUES (");
                    strcat(sql, "\n\"");
                    strcat(sql, words[num_of_words]);
                    strcat(sql, "\", \n");
                    strcat(sql, key);
                    strcat(sql, ", \n\"");
                    strcat(sql, rel ? "clauses" : "facts");
                    strcat(sql, "\"");
                    strcat(sql, ");");
                }*/

                char* smid = small_identifier(words[num_of_words]);
                {
                    strcat(sql, "INSERT OR IGNORE INTO `db_index`.`rel_word_fact__");
                    strcat(sql, smid);
                    strcat(sql, "` (`word`, `fact`, `table`) VALUES (");
                    strcat(sql, "\n\"");
                    strcat(sql, words[num_of_words]);
                    strcat(sql, "\", \n");
                    strcat(sql, key);
                    strcat(sql, ", \n\"");
                    strcat(sql, rel ? "c" : "f");
                    strcat(sql, "\"");
                    strcat(sql, ");");
                }
                free(smid);

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
    
    strcat(sql, "SELECT DISTINCT -1, "
    "`nmain`.`verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, "
    "`nmain`.`subjects`, `nmain`.`objects`, `nmain`.`adverbs`, `nmain`.`questionword`, `nmain`.`fileid`, `nmain`.`line`, `nmain`.`truth`, 0 ");
    strcat(sql, " FROM clauses AS nmain JOIN rel_clause_flag AS rff ON nmain.pk = rff.fact WHERE nmain.rel = ");
    char rel_str[10];
    snprintf(rel_str, 9, "%d", rel);
    strcat(sql, rel_str);
    strcat(sql, " UNION ALL ");
    strcat(sql, "SELECT DISTINCT -1, "
    "`nmain`.`verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, "
    "`nmain`.`subjects`, `nmain`.`objects`, `nmain`.`adverbs`, `nmain`.`questionword`, `nmain`.`fileid`, `nmain`.`line`, `nmain`.`truth`, 0 ");
    strcat(sql, " FROM facts AS nmain JOIN rel_fact_flag AS rff ON nmain.pk = rff.fact WHERE nmain.pk IN ");
    strcat(sql, " (SELECT f2 FROM `linking` WHERE f1 = ");
    strcat(sql, rel_str);
    strcat(sql, " );");
    
    return sql;
}

char* gen_sql_get_double_facts() {

    char* sql = malloc(102400);
    *sql = 0;
    
    strcat(sql, "SELECT `nmain`.`pk`, "
    "`nmain`.`verb` || \"00000\", "
    "`nmain`.`subjects`, `nmain`.`objects`, `nmain`.`adverbs`, `nmain`.`questionword`, `nmain`.`fileid`, `nmain`.`line`, `nmain`.`truth`, `nmain`.`only_logic` ");
    strcat(sql, " FROM facts WHERE mix_1||verb IN ( SELECT mix_1||verb AS a FROM facts GROUP BY a HAVING count(pk) >= 2) order by mix_1, verb;");
    
    return sql;
}

char* gen_sql_add_filename(const char* filename) {

    char* sql = malloc(1024*30);
    *sql = 0;

    strcat(sql, "INSERT OR IGNORE INTO `files` (`id`, `filename`) VALUES ( ");
    strcat(sql, from_number(fileid(filename)));
    strcat(sql, ", \"");
    strcat(sql, filename);
    strcat(sql, "\");\n");

    return sql;
}

char* gen_sql_delete_everything_from(const char* filename) {

    char* sql = malloc(1024*30);
    *sql = 0;

    printf("Clean index...\n");

    strcat(sql, "delete from cache_ids ; INSERT OR IGNORE INTO cache_ids SELECT `pk` FROM facts WHERE `fileid` = ");
    strcat(sql, from_number(fileid(filename)));
    strcat(sql, ";\n");

    int error = sql_execute(sql, NULL, NULL);

    int i;
    for (i = 'a'; i <= 'z'; ++i) {
        int error1 = sql_execute("COMMIT;", NULL, NULL);
        int error2 = sql_execute("BEGIN;", NULL, NULL);
        
        printf("\r%fl\t\t\t", (100.0 / (float)(('z'-'a')*('z'-'a')) * (float)(('z'-'a')*(i-'a')) ));
        fflush(stdout);
        *sql = 0;

        int k;
        for (k = 'a'; k <= 'z'; ++k) {
            strcat(sql, "DELETE FROM `db_index`.`rel_word_fact__");
            char z[3];
            z[0] = i;
            z[1] = k;
            z[2] = 0;
            strcat(sql, z);
            strcat(sql, "` WHERE fact IN ( SELECT id FROM cache_ids );\n");
        }

        int error = sql_execute(sql, NULL, NULL);
    }
    
    *sql = 0;
    strcat(sql, "DELETE FROM facts WHERE `fileid` = ");
    strcat(sql, from_number(fileid(filename)));
    strcat(sql, ";");
   

    printf("\r%s\n", sql);
    return sql;
}

char* disk_get_source(const char* key) {
    printf("disk_get_source: %s\n", key);
    if (!key || !key[0])
        return (char*)1;
    
    char* source = calloc(512, 1);
    {
        char* _where = malloc(1024);
        *_where = 0;
        if (0 == strcmp(key, "a")) {
            strcat(_where, " WHERE pk = (SELECT pk FROM facts ORDER BY pk DESC LIMIT 1) ");
        }
        else {
            strcat(_where, " WHERE pk = ");
            strcat(_where, key);
            strcat(_where, " ");
        }
        
        char* sql = malloc(1024);
        *sql = 0;
        
        strcat(sql, "SELECT filename||\":\"||(SELECT line FROM facts ");
        strcat(sql, _where);
        strcat(sql, ") from files where id = (select fileid FROM facts ");
        strcat(sql, _where);
        strcat(sql, ");");
        free(_where);
        
        int error = sql_execute(sql, select_primary_key, source);
        free(sql);
    }
    printf("source: %s\n", source);
    
    return source;
}

char* disk_get_thesaurus_synonyms(const char* key, struct string_pair** facts, int limit, int* position, int level, short reverse) {
    printf("disk_get_thesaurus_synonyms: %s\n", key);
    if (!key || !key[0])
        return (char*)1;
    
    struct request_string_pair req;
    req.facts    = facts;
    req.limit    = limit;
    req.position = position;
    req.rel      = 0;
    
    {
        char* sql = malloc(1024);
        *sql = 0;
        if (reverse) {
            if (level == 1) {
                strcat(sql, "select e.last_subject_word, e.objects from facts as e where e.verb = \"=\" and e.objects = \"");
                strcat(sql, key);
                strcat(sql, "\" and `can_be_synonym` = 1;");
            }
            else if (level == 2) {
                strcat(sql, "CREATE TABLE IF NOT EXISTS `_tmp_thesaurus_synonyms` "
                "(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `word` varchar(250));"
                "CREATE INDEX IF NOT EXISTS `idx__tmp_thesaurus_synonyms_word` ON `_tmp_thesaurus_synonyms` (`word`); "
                "delete from `_tmp_thesaurus_synonyms`;");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) values (\"");
                strcat(sql, key);
                strcat(sql, "\");");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) "
                "select distinct u.subjects from facts as u where u.verb = \"=\" and u.objects = \"");
                strcat(sql, key);
                strcat(sql, "\" and u.`can_be_synonym` = 1;");
                
                strcat(sql, "select e.last_subject_word, e.objects from facts as e "
                "where e.verb IN (\"=\", \"is-a\", \"ist\") and e.`can_be_synonym` = 1 "
                "and exists (select 1 from `_tmp_thesaurus_synonyms` where e.last_object_word = `word`);");
            }
            else {
                strcat(sql, "CREATE TABLE IF NOT EXISTS `_tmp_thesaurus_synonyms` "
                "(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `word` varchar(250)); "
                "CREATE INDEX IF NOT EXISTS `idx__tmp_thesaurus_synonyms_word` ON `_tmp_thesaurus_synonyms` (`word`); "
                "delete from `_tmp_thesaurus_synonyms`;");
                strcat(sql, "CREATE TABLE IF NOT EXISTS `_tmp_thesaurus_synonyms_2` "
                "(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `word` varchar(250)); "
                "CREATE INDEX IF NOT EXISTS `idx__tmp_thesaurus_synonyms_2_word` ON `_tmp_thesaurus_synonyms_2` (`word`); "
                "delete from `_tmp_thesaurus_synonyms_2`;");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) values (\"");
                strcat(sql, key);
                strcat(sql, "\");");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) "
                "select distinct u.subjects from facts as u where u.verb = \"=\" and u.objects = \"");
                strcat(sql, key);
                strcat(sql, "\" and u.`can_be_synonym` = 1;");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms_2` (`word`) "
                "select distinct e.subjects from facts as e "
                "where e.verb IN (\"=\", \"is-a\", \"ist\") and e.`can_be_synonym` = 1 "
                "and exists (select 1 from `_tmp_thesaurus_synonyms` where e.last_object_word = `word`);");
                strcat(sql, "select e.last_subject_word, e.objects from facts as e "
                "where e.verb IN (\"=\", \"is-a\", \"ist\") and e.`can_be_synonym` = 1 "
                "and exists (select 1 from `_tmp_thesaurus_synonyms_2` where e.last_object_word = `word`);");
            }
        }
        else {
            if (level == 1) {
                strcat(sql, "select e.last_object_word, e.subjects from facts as e where e.verb = \"=\" and e.subjects = \"");
                strcat(sql, key);
                strcat(sql, "\" and `can_be_synonym` = 1;");
            }
            else if (level == 2) {
                strcat(sql, "CREATE TABLE IF NOT EXISTS `_tmp_thesaurus_synonyms` "
                "(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `word` varchar(250)); "
                "CREATE INDEX IF NOT EXISTS `idx__tmp_thesaurus_synonyms_word` ON `_tmp_thesaurus_synonyms` (`word`); "
                "delete from `_tmp_thesaurus_synonyms`;");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) values (\"");
                strcat(sql, key);
                strcat(sql, "\");");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) "
                "select distinct u.objects from facts as u where u.verb = \"=\" and u.subjects = \"");
                strcat(sql, key);
                strcat(sql, "\" and u.`can_be_synonym` = 1;");
                strcat(sql, "select e.last_object_word, e.subjects from facts as e "
                "where e.verb IN (\"=\", \"is-a\", \"ist\") and e.`can_be_synonym` = 1 "
                "and exists (select 1 from `_tmp_thesaurus_synonyms` where e.last_subject_word = `word`);");
            }
            else {
                strcat(sql, "CREATE TABLE IF NOT EXISTS `_tmp_thesaurus_synonyms` "
                "(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `word` varchar(250)); "
                "CREATE INDEX IF NOT EXISTS `idx__tmp_thesaurus_synonyms_word` ON `_tmp_thesaurus_synonyms` (`word`); "
                "delete from `_tmp_thesaurus_synonyms`;");
                strcat(sql, "CREATE TABLE IF NOT EXISTS `_tmp_thesaurus_synonyms_2` "
                "(`id` INTEGER PRIMARY KEY AUTOINCREMENT, `word` varchar(250)); "
                "CREATE INDEX IF NOT EXISTS `idx__tmp_thesaurus_synonyms_2_word` ON `_tmp_thesaurus_synonyms_2` (`word`); "
                "delete from `_tmp_thesaurus_synonyms_2`;");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) values (\"");
                strcat(sql, key);
                strcat(sql, "\");");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms` (`word`) "
                "select distinct u.objects from facts as u where u.verb = \"=\" and u.subjects = \"");
                strcat(sql, key);
                strcat(sql, "\" and u.`can_be_synonym` = 1;");
                strcat(sql, "insert into `_tmp_thesaurus_synonyms_2` (`word`) "
                "select distinct e.objects from facts as e "
                "where e.verb IN (\"=\", \"is-a\", \"ist\") and e.`can_be_synonym` = 1 "
                "and exists (select 1 from `_tmp_thesaurus_synonyms` where e.object_word = `word`);");
                strcat(sql, "select e.last_object_word, e.subjects from facts as e "
                "where e.verb IN (\"=\", \"is-a\", \"ist\") and e.`can_be_synonym` = 1 "
                "and exists (select 1 from `_tmp_thesaurus_synonyms_2` where e.last_subject_word = `word`);");
            }
        }
        printf("%s\n", sql);
        
        int error = sql_execute(sql, callback_string_pair, &req);
        free(sql);
    }
}

char* disk_del_record(const char* key) {
    if (!key || !key[0])
        return (char*)1;
    
    char* source = disk_get_source(key);
    
    char* sql = malloc(1024);
    *sql = 0;
    if (0 == strcmp(key, "a")) {
        strcat(sql, "DELETE FROM facts WHERE pk = (SELECT pk FROM facts ORDER BY pk DESC LIMIT 1);");
    }
    else {
        strcat(sql, "DELETE FROM facts WHERE pk = ");
        strcat(sql, key);
        strcat(sql, ";");
    }
    printf("pkey (in hal2009-disk.c): %s, SQL: %s\n", key, sql);
    
    int error = sql_execute(sql, NULL, NULL);

    int i;
    for (i = 'a'; i <= 'z'; ++i) {
        int k;
        for (k = 'a'; k <= 'z'; ++k) {
            *sql = 0;
            strcat(sql, "DELETE FROM `db_index`.`rel_word_fact__");
            char z[3];
            z[0] = i;
            z[1] = k;
            z[2] = 0;
            strcat(sql, z);
            strcat(sql, "` WHERE fact = ");
            strcat(sql, key);
            strcat(sql, ";");
            int error = sql_execute(sql, NULL, NULL);
        }
    }

    free(sql);
    
    return source;
}

char* gen_sql_get_facts_for_words(struct word*** words, struct fact** facts, int limit, int* position, int* hash_n_1, int* hash_n_2, int* hash_n_3) {

    char* sql = malloc(512000);
    *sql = 0;
    int n, m, q;
    
    if (0 == can_be_a_pointer(words[0])) {
        return sql;
    }

    strcat(sql, "delete from cache_facts ; delete from cache_indices ");

    int in_bracket = 0;
    debugf("Generating SQL for searching facts for words (at %p).\n", words);
    char* last_smid = 0;
    for (n = 0; words[n]; ++n) {
        if (!can_be_a_pointer(words[n])) {
            continue;
        }
        
        int is_new = 1;
        if (can_be_a_pointer(words[n][0])) {
            for (q = 0; words[q] && q+1 < n; ++q) {
                if (can_be_a_pointer(words[q][0]) && words[n][0] == words[q][0]) {
                    is_new = 0;
                    break;
                }
            }
        }
        if (can_be_a_pointer(words[n]) && can_be_a_pointer(words[n][0])) {
            debugf("synonym no %d: %d, %p, %s\n",
                n,
                can_be_a_pointer(words[n]),
                can_be_a_pointer(words[n]) ? words[n][0] : 0,
                can_be_a_pointer(words[n]) && can_be_a_pointer(words[n][0]) ? words[n][0]->name : "(null)"
            );
        }
        if (!is_new) {
            debugf("not new.\n");
            continue;
        }
        
        for (m = 0; words[n][m]; ++m) {
            if (!(can_be_a_pointer(words[n][m]) && words[n][m]->name && words[n][m]->name[0])) {
                continue;
            }
            if (!is_important_word(words[n][m]->name) && is_a_trivial_word(words[n][m]->name)) {
                continue;
            }
            if (!can_be_a_synonym(words[n][m]->name)) {
                continue;
            }
            if (is_bad(words[n][m]->name)) {
                continue;
            }

            {
                *hash_n_1 += words[n][m]->name[0];
                *hash_n_2 += strlen(words[n][m]->name);
                if (strlen(words[n][m]->name) >= 3) {
                    *hash_n_3 += words[n][m]->name[2]*3 + words[n][m]->name[2]*2 - words[n][m]->name[0];
                }
            }

            char* smid = small_identifier(words[n][m]->name);
            if ((!last_smid || strcmp(last_smid, smid)) && smid) {
                if (in_bracket) {
                    strcat(sql, ")");
                    in_bracket = 0;
                }
                strcat(sql, " ; \nINSERT OR IGNORE INTO cache_indices SELECT fact FROM ");
                
                strcat(sql, " `db_index`.`rel_word_fact__");
                strcat(sql, smid);
                strcat(sql, "` AS rel_word_fact ");
                
                strcat(sql, " WHERE 0 ");
            }
            if (last_smid) {
                free(last_smid);
            }
            last_smid = strdup(smid);
            free(smid);

            
            if (words[n][m]->name[0] && words[n][m]->name[0] == '*') {
                if (strstr(words[n][m]->name+1, "*")) {
                    strcat(sql, "OR rel_word_fact.word GLOB \"");
                    strcat(sql, words[n][m]->name+1);
                    strcat(sql, "\" ");
                }
                else {
                    strcat(sql, "OR rel_word_fact.word = \"");
                    strcat(sql, words[n][m]->name+1);
                    strcat(sql, "\" ");
                }
            }
            else if (strstr(words[n][m]->name, "*")) {
                strcat(sql, "OR rel_word_fact.word GLOB \"");
                strcat(sql, words[n][m]->name);
                strcat(sql, "\" ");
            }
            else {
                strcat(sql, "OR rel_word_fact.word = \"");
                strcat(sql, words[n][m]->name);
                strcat(sql, "\" ");
            }
        }
    }
    if (last_smid)
        free(last_smid);
    if (in_bracket) {
        strcat(sql, ")");
    }
    strcat(sql, ";");
    strcat(sql, " ; INSERT OR IGNORE INTO cache_facts "
    "(pk, fileid, line, verb, verbgroup, subjects, objects, adverbs, "
    "mix_1, questionword, prio, rel, type, truth, hash_clauses, only_logic, can_be_synonym) "
    "SELECT "
    "pk, fileid, line, verb, verbgroup, subjects, objects, adverbs, "
    "mix_1, questionword, prio, rel, type, truth, hash_clauses, only_logic, can_be_synonym "
    "FROM facts WHERE pk in (SELECT i FROM cache_indices);");
                
    strcat(sql, "SELECT DISTINCT "
    "`nmain`.`pk`, "
    "`nmain`.`verb` || rff.verb_flag_want || rff.verb_flag_must || rff.verb_flag_can || rff.verb_flag_may || rff.verb_flag_should, "
    "`nmain`.`subjects`, `nmain`.`objects`, `nmain`.`adverbs`, "
    "`nmain`.`questionword`, `nmain`.`fileid`, `nmain`.`line`, `nmain`.`truth`, `nmain`.`only_logic` ");
    strcat(sql, " FROM cache_facts AS nmain LEFT JOIN rel_fact_flag AS rff ON nmain.pk = rff.fact");
    strcat(sql, ";");
    printf(sql);
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
        if (strstr(err, "no such table")) {
            printf("SQL Error:\n------------\n%s\n\n", err, sql);
            sqlite3_free(err);
            
            if (sqlite3_exec(sqlite_connection, sqlite_sql_create_table, NULL, NULL, &err)) {
                printf("Table creation: SQL Error:\n------------\n%s\n\n", err, sql);
//                printf("SQL Error:\n------------\n%s\n------------\n%s\n------------\n\n", err, sql);
                error_to_return = TABLE_ERROR;
                break;
            }
            continue;
        }
        else {
            printf("SQL Error:\n------------\n%s\n------------\n%s\n------------\n\n", err, sql);
            --num_facts_added_during_this_run;
            ++num_facts_not_added_during_this_run_because_other_error;
            printf("SQL Error:\n------------\n%s\n------------\n%s\n------------\n\n", err, sql);
            break;
        }
    }
    sqlite3_free(err);
    
    if (error_to_return) {
        disk_cache_clauses = -1;
        disk_cache_facts   = -1;
    }
    
    return error_to_return;
}

struct fact* disk_add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    if ((is_bad(subjects) && is_bad(objects) && is_bad(verbs) && !(verb_flag_want || verb_flag_must || verb_flag_can || verb_flag_may || verb_flag_should)) || (questionword && questionword[0] == ')')) {
        return 0;
    }

    int pk = get_last_pk(rel)+1;

    {
        char* sql = 0;
        sql = gen_sql_add_entry(sql, pk, rel, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, 0);
        sql = gen_sql_add_verb_flags(sql, pk, rel, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, 0);
        int error = sql_execute(sql, NULL, NULL);
        free(sql);
        
        sql = 0;
        sql = gen_sql_add_word_fact_relations(sql, pk, rel, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, 0, 0);
        error = sql_execute(sql, NULL, NULL);
        free(sql);
    }

    return 0;
}

struct fact* disk_add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword) {
    if ((is_bad(subjects) && is_bad(objects) && is_bad(verbs) && !(verb_flag_want || verb_flag_must || verb_flag_can || verb_flag_may || verb_flag_should)) || (questionword && questionword[0] == ')')) {
        return 0;
    }

    int pk = get_last_pk(0)+1;
    int error = 0;
    {
        char* sql = 0;
        sql = gen_sql_add_entry(sql, pk, 0, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, only_logic);
        sql = gen_sql_add_verb_flags(sql, pk, 0, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, only_logic);
        error = sql_execute(sql, NULL, NULL);
        free(sql);
        
        sql = 0;
        sql = gen_sql_add_word_fact_relations(sql, pk, 0, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, only_logic, has_conditional_questionword);
        error = sql_execute(sql, NULL, NULL);
        free(sql);
    }
    
    if (error) {
        printf("Error in disk_add_fact.\n");
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

static int callback_string_pair(void* arg, int argc, char **argv, char **azColName) {
    struct request_string_pair* req = arg;
    
    if (*req->position >= req->limit) {
        return 1;
    }
    
    if (argc < 2) {
        return 0;
    }
    
    struct string_pair* fact  = calloc(sizeof(struct string_pair), 1);
    fact->subjects     = strdup(argv[0] ? argv[0] : "");
    fact->objects      = strdup(argv[1] ? argv[1] : "");
    
    req->facts[*req->position] = fact;
    if (!argv[1] || !strstr(argv[1], ">>>")) {
        debugf("Added string pair no %d at %p (%s, %s).\n", *req->position, req->facts[*req->position], fact->subjects, fact->objects);
    }
    ++(*req->position);
    
    return 0;
}

int find_query_cache_entry (struct request_get_facts_for_words* req, short if_needed_create_entry) {
    if (!query_cache_list) {
        query_cache_list = calloc(sizeof(struct query_cache_entry*), QUERY_CACHE_SIZE+1);
    }
    short need_to_create = 1;
    int i_right = INVALID_POINTER;
    int i = 0;
    // check whether there is a matching entry
        printf("search in query_cache: %d - %d - %d\n", req->hash_n_1, req->hash_n_2, req->hash_n_3);
    while (query_cache_list[i] && i < QUERY_CACHE_SIZE) {
        struct query_cache_entry* entry = query_cache_list[i];
        if (entry->hash_n_1 == req->hash_n_1 && entry->hash_n_2 == req->hash_n_2 && entry->hash_n_2 == req->hash_n_2) {
            need_to_create = 0;
            i_right = i;
        }
            printf("found in query_cache:  %d - %d - %d\n", entry->hash_n_1, entry->hash_n_2, entry->hash_n_3);
        ++i;
    }
    if (i >= QUERY_CACHE_SIZE) {
        // free the whole list
        i = 0;
        while (query_cache_list[i] && i < QUERY_CACHE_SIZE) {
            int b;
            for (b = 0; b < query_cache_list[i]->position; ++b) {
                free(query_cache_list[i]->rawfacts[b]);
                query_cache_list[i]->rawfacts[b] = 0;
            }
            free(query_cache_list[i]->rawfacts);
            free(query_cache_list[i]);
            query_cache_list[i] = 0;
            ++i;
        }

        need_to_create = 1;
        i = 0;
        i_right = INVALID_POINTER;
    }

    if (if_needed_create_entry) {
        if (need_to_create) {
            // add new entry
            query_cache_list[i] = calloc(sizeof(struct query_cache_entry), 1);
            query_cache_list[i]->hash_n_1 = req->hash_n_1;
            query_cache_list[i]->hash_n_2 = req->hash_n_2;
            query_cache_list[i]->hash_n_3 = req->hash_n_3;
            query_cache_list[i]->rawfacts = calloc(sizeof(char**), req->limit);
            query_cache_list[i]->position = 0;
            i_right = i;
        }
        return i_right;
    }
    else {
        return i_right;
    }
}

static int callback_get_facts(void* arg, int argc, char **argv, char **azColName) {
    struct request_get_facts_for_words* req = arg;
    
    if (*req->position >= req->limit) {
        return 1;
    }
    
    if (argc <= 5 || (!argv[1] || !strlen(argv[1])) || ((!argv[2] || !strlen(argv[2])) && (!argv[3] || !strlen(argv[3])) && (!argv[4] || !strlen(argv[4])) && (!argv[5] || !strlen(argv[5])))) {
        return 0;
    }
    

    if (req->make_rawfacts && (req->hash_n_1 || req->hash_n_2 || req->hash_n_3)) {
        int i_right = req->i_right;

        // printf("if (%d < %d)\n", query_cache_list[i_right]->position, req->limit);
        if (query_cache_list[i_right]->position < req->limit) {
            char** rawfact = calloc(sizeof(char*), 11);
            int k;
            for (k = 0; k <= 9; ++k) {
                rawfact[k] = argv[k] ? strdup(argv[k]) : 0;
                // printf("rawfact[%d]: %s\n", k, rawfact[k] ? rawfact[k] : "(null)");
            }
            rawfact[10] = 0;

            // add new fact into entry
            query_cache_list[i_right]->rawfacts[query_cache_list[i_right]->position] = rawfact;
            // printf("=> %d\n", query_cache_list[i_right]->position);
            ++(query_cache_list[i_right]->position);
        }
    }

    struct fact* fact  = calloc(sizeof(struct fact), 1);
    fact->pk           = to_number(argv[0] ? argv[0] : "0");
    fact->verbs        = divide_words(argv[1] ? argv[1] : "");
    fact->subjects     = divide_words(argv[2] ? argv[2] : "");
    fact->objects      = divide_words(argv[3] ? argv[3] : "");
    fact->adverbs      = divide_words(argv[4] ? argv[4] : "");
    fact->extra        = divide_words("");
    fact->questionword = strdup(argv[5] ? argv[5] : "");
    fact->filename     = strdup(argv[6] ? argv[6] : "");
    fact->line         = strdup(argv[7] ? argv[7] : "");
    fact->truth        = (argv[8] && argv[8][0] && argv[8][0] == '1') ? 1.0 : ((argv[8] && argv[8][0] && argv[8][0] && argv[8][1] && argv[8][2] != '0') ? 0.5 : 0.0);
    fact->only_logic   = argv[9] && argv[9][0] && argv[9][0] == '1' ? 1 : 0;
    
    req->facts[*req->position] = fact;
    if (!argv[1] || !strstr(argv[1], ">>>")) {
        if (DEBUG__LONG_LISTINGS) {
            debugf("added fact no %d at %p:  %s, %s, %s, %s\n", *req->position, req->facts[*req->position], argv[1], argv[2], argv[3], argv[4]);
        }
    }
    ++(*req->position);
    
    return 0;
}

int disk_net_go_way_conditional_questionword(struct request* fact, struct fact** facts, int limit, int* position) {
    struct word*** words = calloc(1, 2*sizeof(char**));
    words[0] = calloc(1, 2*sizeof(char**));
    char* has_conditional_questionword = strdup("$$has_conditional_questionword$$");
    words[0][0] = set_word(has_conditional_questionword);
    search_facts_for_words_in_net(words, facts, limit, position);
    free(words[0]);
    free(words);
    free(has_conditional_questionword);
    return 0;
}

int disk_search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position) {
    struct request_get_facts_for_words req;
    req.words         = words;
    req.facts         = facts;
    req.limit         = limit;
    req.position      = position;
    req.rel           = 0;
    req.make_rawfacts = 1;
    req.debug_facts   = 1;

    {
        req.hash_n_1 = 0;
        req.hash_n_2 = 0;
        req.hash_n_3 = 0;
        char* sql = gen_sql_get_facts_for_words(words, facts, limit, position, &req.hash_n_1, &req.hash_n_2, &req.hash_n_3);

        // check whether in cache
        int i_right = find_query_cache_entry(&req, 0);

        /*
        printf("%s\n", sql);
        
        printf("%s\n", "\n");
        printf("hash_n_1: %d\n", req.hash_n_1);
        printf("hash_n_2: %d\n", req.hash_n_2);
        printf("hash_n_3: %d\n", req.hash_n_3);
        printf("i_right:  %d\n", i_right);
        printf("%s\n", "\n");
        */

        if (i_right >= 0 && query_cache_list && query_cache_list[i_right] && query_cache_list[i_right]->rawfacts) {
            req.make_rawfacts = 0;
            req.debug_facts = 0;
            req.hash_n_1 = 0;
            req.hash_n_2 = 0;
            req.hash_n_3 = 0;

            int k;
            for (k = 0; k < query_cache_list[i_right]->position; ++k) {
                char** argv = query_cache_list[i_right]->rawfacts[k];
                if (argv) {
                    callback_get_facts(&req, 10, argv, 0);
                }
            }
        }
        else {
            req.i_right = find_query_cache_entry(&req, 1);
            //printf("i_right (created):  %d\n", req.i_right);

            int error = sql_execute(sql, callback_get_facts, &req);
        }

        free(sql);
    }
    
    if (*req.position > limit - 10) {
        return TOOMUCH;
    }
    
    return 0;
}

int disk_search_double_facts(struct word*** words, struct fact** facts, int limit, int* position) {
    struct request_get_facts_for_words req;
    req.words    = words;
    req.facts    = facts;
    req.limit    = limit;
    req.position = position;
    req.rel      = 0;
    req.make_rawfacts = 0;
    req.debug_facts = 0;
    
    {
        req.hash_n_1 = 0;
        req.hash_n_2 = 0;
        req.hash_n_3 = 0;

        char* sql = gen_sql_get_double_facts();
        printf("%s\n", sql);
        int error = sql_execute(sql, callback_get_facts, &req);
        free(sql);
    }
    
    if (*req.position > limit - 10) {
        return TOOMUCH;
    }
    
    return 0;
}

int re_index_size_sql = 0;
int re_index_pos_sql = 0;
int re_index_in_facts = 1;

static int callback_re_index(void* arg, int argc, char **argv, char **azColName) {
    char* re_index_sql = *((void**)(arg));
    
    char** verbs       = divide_string(argv[1] ? argv[1] : "");
    char** subjects    = divide_string(argv[2] ? argv[2] : "");
    char** objects     = divide_string(argv[3] ? argv[3] : "");
    char** adverbs     = divide_string(argv[4] ? argv[4] : "");
    
    int stage;
    for (stage = 1; stage <= 4; ++stage) {
        
        char** words;
        if (stage == 1) words = verbs;
        if (stage == 2) words = subjects;
        if (stage == 3) words = objects;
        if (stage == 4) words = adverbs;
        
        int i;
        for (i = 0; words[i]; ++i) {
            char* smid = small_identifier(words[i]);
            {
                char sql[1000];
                strcat(sql, "INSERT OR IGNORE INTO `db_index`.`rel_word_fact__");
                strcat(sql, smid);
                strcat(sql, "` (`word`, `fact`, `table`) VALUES (");
                strcat(sql, "\n\"");
                strcat(sql, words[i]);
                strcat(sql, "\", \n");
                strcat(sql, argv[0]);
                strcat(sql, ", \n\"");
                strcat(sql, re_index_in_facts ? "f" : "c");
                strcat(sql, "\"");
                strcat(sql, ");");
                
                int size_sql = strlen(sql);
                if (!(re_index_pos_sql + size_sql < re_index_size_sql - 10)) {
                    re_index_size_sql += 10000;
                    *((void**)(arg)) = realloc(*((void**)(arg)), re_index_size_sql);
                    re_index_sql = *((void**)(arg));
                }
                re_index_pos_sql += size_sql;
                strcat(re_index_sql, sql);
            }
            free(smid);
            free(words[i]);
        }
        
        free(words);
    }
    
    return 0;
}

int disk_re_index() {
    {
        int error1 = sql_execute("COMMIT;", NULL, NULL);
        int error2 = sql_execute("BEGIN;", NULL, NULL);
        printf("Delete old index...\n");
        int error3 = sql_execute("delete from `db_index`.`rel_word_fact__aa`; delete from `db_index`.`rel_word_fact__ab`; delete from `db_index`.`rel_word_fact__ac`; delete from `db_index`.`rel_word_fact__ad`; delete from `db_index`.`rel_word_fact__ae`; delete from `db_index`.`rel_word_fact__af`; delete from `db_index`.`rel_word_fact__ag`; delete from `db_index`.`rel_word_fact__ah`; delete from `db_index`.`rel_word_fact__ai`; delete from `db_index`.`rel_word_fact__aj`; delete from `db_index`.`rel_word_fact__ak`; delete from `db_index`.`rel_word_fact__al`; delete from `db_index`.`rel_word_fact__am`; delete from `db_index`.`rel_word_fact__an`; delete from `db_index`.`rel_word_fact__ao`; delete from `db_index`.`rel_word_fact__ap`; delete from `db_index`.`rel_word_fact__aq`; delete from `db_index`.`rel_word_fact__ar`; delete from `db_index`.`rel_word_fact__as`; delete from `db_index`.`rel_word_fact__at`; delete from `db_index`.`rel_word_fact__au`; delete from `db_index`.`rel_word_fact__av`; delete from `db_index`.`rel_word_fact__aw`; delete from `db_index`.`rel_word_fact__ax`; delete from `db_index`.`rel_word_fact__ay`; delete from `db_index`.`rel_word_fact__az`; delete from `db_index`.`rel_word_fact__a_`; delete from `db_index`.`rel_word_fact__ba`; delete from `db_index`.`rel_word_fact__bb`; delete from `db_index`.`rel_word_fact__bc`; delete from `db_index`.`rel_word_fact__bd`; delete from `db_index`.`rel_word_fact__be`; delete from `db_index`.`rel_word_fact__bf`; delete from `db_index`.`rel_word_fact__bg`; delete from `db_index`.`rel_word_fact__bh`; delete from `db_index`.`rel_word_fact__bi`; delete from `db_index`.`rel_word_fact__bj`; delete from `db_index`.`rel_word_fact__bk`; delete from `db_index`.`rel_word_fact__bl`; delete from `db_index`.`rel_word_fact__bm`; delete from `db_index`.`rel_word_fact__bn`; delete from `db_index`.`rel_word_fact__bo`; delete from `db_index`.`rel_word_fact__bp`; delete from `db_index`.`rel_word_fact__bq`; delete from `db_index`.`rel_word_fact__br`; delete from `db_index`.`rel_word_fact__bs`; delete from `db_index`.`rel_word_fact__bt`; delete from `db_index`.`rel_word_fact__bu`; delete from `db_index`.`rel_word_fact__bv`; delete from `db_index`.`rel_word_fact__bw`; delete from `db_index`.`rel_word_fact__bx`; delete from `db_index`.`rel_word_fact__by`; delete from `db_index`.`rel_word_fact__bz`; delete from `db_index`.`rel_word_fact__b_`; delete from `db_index`.`rel_word_fact__ca`; delete from `db_index`.`rel_word_fact__cb`; delete from `db_index`.`rel_word_fact__cc`; delete from `db_index`.`rel_word_fact__cd`; delete from `db_index`.`rel_word_fact__ce`; delete from `db_index`.`rel_word_fact__cf`; delete from `db_index`.`rel_word_fact__cg`; delete from `db_index`.`rel_word_fact__ch`; delete from `db_index`.`rel_word_fact__ci`; delete from `db_index`.`rel_word_fact__cj`; delete from `db_index`.`rel_word_fact__ck`; delete from `db_index`.`rel_word_fact__cl`; delete from `db_index`.`rel_word_fact__cm`; delete from `db_index`.`rel_word_fact__cn`; delete from `db_index`.`rel_word_fact__co`; delete from `db_index`.`rel_word_fact__cp`; delete from `db_index`.`rel_word_fact__cq`; delete from `db_index`.`rel_word_fact__cr`; delete from `db_index`.`rel_word_fact__cs`; delete from `db_index`.`rel_word_fact__ct`; delete from `db_index`.`rel_word_fact__cu`; delete from `db_index`.`rel_word_fact__cv`; delete from `db_index`.`rel_word_fact__cw`; delete from `db_index`.`rel_word_fact__cx`; delete from `db_index`.`rel_word_fact__cy`; delete from `db_index`.`rel_word_fact__cz`; delete from `db_index`.`rel_word_fact__c_`; delete from `db_index`.`rel_word_fact__da`; delete from `db_index`.`rel_word_fact__db`; delete from `db_index`.`rel_word_fact__dc`; delete from `db_index`.`rel_word_fact__dd`; delete from `db_index`.`rel_word_fact__de`; delete from `db_index`.`rel_word_fact__df`; delete from `db_index`.`rel_word_fact__dg`; delete from `db_index`.`rel_word_fact__dh`; delete from `db_index`.`rel_word_fact__di`; delete from `db_index`.`rel_word_fact__dj`; delete from `db_index`.`rel_word_fact__dk`; delete from `db_index`.`rel_word_fact__dl`; delete from `db_index`.`rel_word_fact__dm`; delete from `db_index`.`rel_word_fact__dn`; delete from `db_index`.`rel_word_fact__do`; delete from `db_index`.`rel_word_fact__dp`; delete from `db_index`.`rel_word_fact__dq`; delete from `db_index`.`rel_word_fact__dr`; delete from `db_index`.`rel_word_fact__ds`; delete from `db_index`.`rel_word_fact__dt`; delete from `db_index`.`rel_word_fact__du`; delete from `db_index`.`rel_word_fact__dv`; delete from `db_index`.`rel_word_fact__dw`; delete from `db_index`.`rel_word_fact__dx`; delete from `db_index`.`rel_word_fact__dy`; delete from `db_index`.`rel_word_fact__dz`; delete from `db_index`.`rel_word_fact__d_`; delete from `db_index`.`rel_word_fact__ea`; delete from `db_index`.`rel_word_fact__eb`; delete from `db_index`.`rel_word_fact__ec`; delete from `db_index`.`rel_word_fact__ed`; delete from `db_index`.`rel_word_fact__ee`; delete from `db_index`.`rel_word_fact__ef`; delete from `db_index`.`rel_word_fact__eg`; delete from `db_index`.`rel_word_fact__eh`; delete from `db_index`.`rel_word_fact__ei`; delete from `db_index`.`rel_word_fact__ej`; delete from `db_index`.`rel_word_fact__ek`; delete from `db_index`.`rel_word_fact__el`; delete from `db_index`.`rel_word_fact__em`; delete from `db_index`.`rel_word_fact__en`; delete from `db_index`.`rel_word_fact__eo`; delete from `db_index`.`rel_word_fact__ep`; delete from `db_index`.`rel_word_fact__eq`; delete from `db_index`.`rel_word_fact__er`; delete from `db_index`.`rel_word_fact__es`; delete from `db_index`.`rel_word_fact__et`; delete from `db_index`.`rel_word_fact__eu`; delete from `db_index`.`rel_word_fact__ev`; delete from `db_index`.`rel_word_fact__ew`; delete from `db_index`.`rel_word_fact__ex`; delete from `db_index`.`rel_word_fact__ey`; delete from `db_index`.`rel_word_fact__ez`; delete from `db_index`.`rel_word_fact__e_`; delete from `db_index`.`rel_word_fact__fa`; delete from `db_index`.`rel_word_fact__fb`; delete from `db_index`.`rel_word_fact__fc`; delete from `db_index`.`rel_word_fact__fd`; delete from `db_index`.`rel_word_fact__fe`; delete from `db_index`.`rel_word_fact__ff`; delete from `db_index`.`rel_word_fact__fg`; delete from `db_index`.`rel_word_fact__fh`; delete from `db_index`.`rel_word_fact__fi`; delete from `db_index`.`rel_word_fact__fj`; delete from `db_index`.`rel_word_fact__fk`; delete from `db_index`.`rel_word_fact__fl`; delete from `db_index`.`rel_word_fact__fm`; delete from `db_index`.`rel_word_fact__fn`; delete from `db_index`.`rel_word_fact__fo`; delete from `db_index`.`rel_word_fact__fp`; delete from `db_index`.`rel_word_fact__fq`; delete from `db_index`.`rel_word_fact__fr`; delete from `db_index`.`rel_word_fact__fs`; delete from `db_index`.`rel_word_fact__ft`; delete from `db_index`.`rel_word_fact__fu`; delete from `db_index`.`rel_word_fact__fv`; delete from `db_index`.`rel_word_fact__fw`; delete from `db_index`.`rel_word_fact__fx`; delete from `db_index`.`rel_word_fact__fy`; delete from `db_index`.`rel_word_fact__fz`; delete from `db_index`.`rel_word_fact__f_`; delete from `db_index`.`rel_word_fact__ga`; delete from `db_index`.`rel_word_fact__gb`; delete from `db_index`.`rel_word_fact__gc`; delete from `db_index`.`rel_word_fact__gd`; delete from `db_index`.`rel_word_fact__ge`; delete from `db_index`.`rel_word_fact__gf`; delete from `db_index`.`rel_word_fact__gg`; delete from `db_index`.`rel_word_fact__gh`; delete from `db_index`.`rel_word_fact__gi`; delete from `db_index`.`rel_word_fact__gj`; delete from `db_index`.`rel_word_fact__gk`; delete from `db_index`.`rel_word_fact__gl`; delete from `db_index`.`rel_word_fact__gm`; delete from `db_index`.`rel_word_fact__gn`; delete from `db_index`.`rel_word_fact__go`; delete from `db_index`.`rel_word_fact__gp`; delete from `db_index`.`rel_word_fact__gq`; delete from `db_index`.`rel_word_fact__gr`; delete from `db_index`.`rel_word_fact__gs`; delete from `db_index`.`rel_word_fact__gt`; delete from `db_index`.`rel_word_fact__gu`; delete from `db_index`.`rel_word_fact__gv`; delete from `db_index`.`rel_word_fact__gw`; delete from `db_index`.`rel_word_fact__gx`; delete from `db_index`.`rel_word_fact__gy`; delete from `db_index`.`rel_word_fact__gz`; delete from `db_index`.`rel_word_fact__g_`; delete from `db_index`.`rel_word_fact__ha`; delete from `db_index`.`rel_word_fact__hb`; delete from `db_index`.`rel_word_fact__hc`; delete from `db_index`.`rel_word_fact__hd`; delete from `db_index`.`rel_word_fact__he`; delete from `db_index`.`rel_word_fact__hf`; delete from `db_index`.`rel_word_fact__hg`; delete from `db_index`.`rel_word_fact__hh`; delete from `db_index`.`rel_word_fact__hi`; delete from `db_index`.`rel_word_fact__hj`; delete from `db_index`.`rel_word_fact__hk`; delete from `db_index`.`rel_word_fact__hl`; delete from `db_index`.`rel_word_fact__hm`; delete from `db_index`.`rel_word_fact__hn`; delete from `db_index`.`rel_word_fact__ho`; delete from `db_index`.`rel_word_fact__hp`; delete from `db_index`.`rel_word_fact__hq`; delete from `db_index`.`rel_word_fact__hr`; delete from `db_index`.`rel_word_fact__hs`; delete from `db_index`.`rel_word_fact__ht`; delete from `db_index`.`rel_word_fact__hu`; delete from `db_index`.`rel_word_fact__hv`; delete from `db_index`.`rel_word_fact__hw`; delete from `db_index`.`rel_word_fact__hx`; delete from `db_index`.`rel_word_fact__hy`; delete from `db_index`.`rel_word_fact__hz`; delete from `db_index`.`rel_word_fact__h_`; delete from `db_index`.`rel_word_fact__ia`; delete from `db_index`.`rel_word_fact__ib`; delete from `db_index`.`rel_word_fact__ic`; delete from `db_index`.`rel_word_fact__id`; delete from `db_index`.`rel_word_fact__ie`; delete from `db_index`.`rel_word_fact__if`; delete from `db_index`.`rel_word_fact__ig`; delete from `db_index`.`rel_word_fact__ih`; delete from `db_index`.`rel_word_fact__ii`; delete from `db_index`.`rel_word_fact__ij`; delete from `db_index`.`rel_word_fact__ik`; delete from `db_index`.`rel_word_fact__il`; delete from `db_index`.`rel_word_fact__im`; delete from `db_index`.`rel_word_fact__in`; delete from `db_index`.`rel_word_fact__io`; delete from `db_index`.`rel_word_fact__ip`; delete from `db_index`.`rel_word_fact__iq`; delete from `db_index`.`rel_word_fact__ir`; delete from `db_index`.`rel_word_fact__is`; delete from `db_index`.`rel_word_fact__it`; delete from `db_index`.`rel_word_fact__iu`; delete from `db_index`.`rel_word_fact__iv`; delete from `db_index`.`rel_word_fact__iw`; delete from `db_index`.`rel_word_fact__ix`; delete from `db_index`.`rel_word_fact__iy`; delete from `db_index`.`rel_word_fact__iz`; delete from `db_index`.`rel_word_fact__i_`; delete from `db_index`.`rel_word_fact__ja`; delete from `db_index`.`rel_word_fact__jb`; delete from `db_index`.`rel_word_fact__jc`; delete from `db_index`.`rel_word_fact__jd`; delete from `db_index`.`rel_word_fact__je`; delete from `db_index`.`rel_word_fact__jf`; delete from `db_index`.`rel_word_fact__jg`; delete from `db_index`.`rel_word_fact__jh`; delete from `db_index`.`rel_word_fact__ji`; delete from `db_index`.`rel_word_fact__jj`; delete from `db_index`.`rel_word_fact__jk`; delete from `db_index`.`rel_word_fact__jl`; delete from `db_index`.`rel_word_fact__jm`; delete from `db_index`.`rel_word_fact__jn`; delete from `db_index`.`rel_word_fact__jo`; delete from `db_index`.`rel_word_fact__jp`; delete from `db_index`.`rel_word_fact__jq`; delete from `db_index`.`rel_word_fact__jr`; delete from `db_index`.`rel_word_fact__js`; delete from `db_index`.`rel_word_fact__jt`; delete from `db_index`.`rel_word_fact__ju`; delete from `db_index`.`rel_word_fact__jv`; delete from `db_index`.`rel_word_fact__jw`; delete from `db_index`.`rel_word_fact__jx`; delete from `db_index`.`rel_word_fact__jy`; delete from `db_index`.`rel_word_fact__jz`; delete from `db_index`.`rel_word_fact__j_`; delete from `db_index`.`rel_word_fact__ka`; delete from `db_index`.`rel_word_fact__kb`; delete from `db_index`.`rel_word_fact__kc`; delete from `db_index`.`rel_word_fact__kd`; delete from `db_index`.`rel_word_fact__ke`; delete from `db_index`.`rel_word_fact__kf`; delete from `db_index`.`rel_word_fact__kg`; delete from `db_index`.`rel_word_fact__kh`; delete from `db_index`.`rel_word_fact__ki`; delete from `db_index`.`rel_word_fact__kj`; delete from `db_index`.`rel_word_fact__kk`; delete from `db_index`.`rel_word_fact__kl`; delete from `db_index`.`rel_word_fact__km`; delete from `db_index`.`rel_word_fact__kn`; delete from `db_index`.`rel_word_fact__ko`; delete from `db_index`.`rel_word_fact__kp`; delete from `db_index`.`rel_word_fact__kq`; delete from `db_index`.`rel_word_fact__kr`; delete from `db_index`.`rel_word_fact__ks`; delete from `db_index`.`rel_word_fact__kt`; delete from `db_index`.`rel_word_fact__ku`; delete from `db_index`.`rel_word_fact__kv`; delete from `db_index`.`rel_word_fact__kw`; delete from `db_index`.`rel_word_fact__kx`; delete from `db_index`.`rel_word_fact__ky`; delete from `db_index`.`rel_word_fact__kz`; delete from `db_index`.`rel_word_fact__k_`; delete from `db_index`.`rel_word_fact__la`; delete from `db_index`.`rel_word_fact__lb`; delete from `db_index`.`rel_word_fact__lc`; delete from `db_index`.`rel_word_fact__ld`; delete from `db_index`.`rel_word_fact__le`; delete from `db_index`.`rel_word_fact__lf`; delete from `db_index`.`rel_word_fact__lg`; delete from `db_index`.`rel_word_fact__lh`; delete from `db_index`.`rel_word_fact__li`; delete from `db_index`.`rel_word_fact__lj`; delete from `db_index`.`rel_word_fact__lk`; delete from `db_index`.`rel_word_fact__ll`; delete from `db_index`.`rel_word_fact__lm`; delete from `db_index`.`rel_word_fact__ln`; delete from `db_index`.`rel_word_fact__lo`; delete from `db_index`.`rel_word_fact__lp`; delete from `db_index`.`rel_word_fact__lq`; delete from `db_index`.`rel_word_fact__lr`; delete from `db_index`.`rel_word_fact__ls`; delete from `db_index`.`rel_word_fact__lt`; delete from `db_index`.`rel_word_fact__lu`; delete from `db_index`.`rel_word_fact__lv`; delete from `db_index`.`rel_word_fact__lw`; delete from `db_index`.`rel_word_fact__lx`; delete from `db_index`.`rel_word_fact__ly`; delete from `db_index`.`rel_word_fact__lz`; delete from `db_index`.`rel_word_fact__l_`; delete from `db_index`.`rel_word_fact__ma`; delete from `db_index`.`rel_word_fact__mb`; delete from `db_index`.`rel_word_fact__mc`; delete from `db_index`.`rel_word_fact__md`; delete from `db_index`.`rel_word_fact__me`; delete from `db_index`.`rel_word_fact__mf`; delete from `db_index`.`rel_word_fact__mg`; delete from `db_index`.`rel_word_fact__mh`; delete from `db_index`.`rel_word_fact__mi`; delete from `db_index`.`rel_word_fact__mj`; delete from `db_index`.`rel_word_fact__mk`; delete from `db_index`.`rel_word_fact__ml`; delete from `db_index`.`rel_word_fact__mm`; delete from `db_index`.`rel_word_fact__mn`; delete from `db_index`.`rel_word_fact__mo`; delete from `db_index`.`rel_word_fact__mp`; delete from `db_index`.`rel_word_fact__mq`; delete from `db_index`.`rel_word_fact__mr`; delete from `db_index`.`rel_word_fact__ms`; delete from `db_index`.`rel_word_fact__mt`; delete from `db_index`.`rel_word_fact__mu`; delete from `db_index`.`rel_word_fact__mv`; delete from `db_index`.`rel_word_fact__mw`; delete from `db_index`.`rel_word_fact__mx`; delete from `db_index`.`rel_word_fact__my`; delete from `db_index`.`rel_word_fact__mz`; delete from `db_index`.`rel_word_fact__m_`; delete from `db_index`.`rel_word_fact__na`; delete from `db_index`.`rel_word_fact__nb`; delete from `db_index`.`rel_word_fact__nc`; delete from `db_index`.`rel_word_fact__nd`; delete from `db_index`.`rel_word_fact__ne`; delete from `db_index`.`rel_word_fact__nf`; delete from `db_index`.`rel_word_fact__ng`; delete from `db_index`.`rel_word_fact__nh`; delete from `db_index`.`rel_word_fact__ni`; delete from `db_index`.`rel_word_fact__nj`; delete from `db_index`.`rel_word_fact__nk`; delete from `db_index`.`rel_word_fact__nl`; delete from `db_index`.`rel_word_fact__nm`; delete from `db_index`.`rel_word_fact__nn`; delete from `db_index`.`rel_word_fact__no`; delete from `db_index`.`rel_word_fact__np`; delete from `db_index`.`rel_word_fact__nq`; delete from `db_index`.`rel_word_fact__nr`; delete from `db_index`.`rel_word_fact__ns`; delete from `db_index`.`rel_word_fact__nt`; delete from `db_index`.`rel_word_fact__nu`; delete from `db_index`.`rel_word_fact__nv`; delete from `db_index`.`rel_word_fact__nw`; delete from `db_index`.`rel_word_fact__nx`; delete from `db_index`.`rel_word_fact__ny`; delete from `db_index`.`rel_word_fact__nz`; delete from `db_index`.`rel_word_fact__n_`; delete from `db_index`.`rel_word_fact__oa`; delete from `db_index`.`rel_word_fact__ob`; delete from `db_index`.`rel_word_fact__oc`; delete from `db_index`.`rel_word_fact__od`; delete from `db_index`.`rel_word_fact__oe`; delete from `db_index`.`rel_word_fact__of`; delete from `db_index`.`rel_word_fact__og`; delete from `db_index`.`rel_word_fact__oh`; delete from `db_index`.`rel_word_fact__oi`; delete from `db_index`.`rel_word_fact__oj`; delete from `db_index`.`rel_word_fact__ok`; delete from `db_index`.`rel_word_fact__ol`; delete from `db_index`.`rel_word_fact__om`; delete from `db_index`.`rel_word_fact__on`; delete from `db_index`.`rel_word_fact__oo`; delete from `db_index`.`rel_word_fact__op`; delete from `db_index`.`rel_word_fact__oq`; delete from `db_index`.`rel_word_fact__or`; delete from `db_index`.`rel_word_fact__os`; delete from `db_index`.`rel_word_fact__ot`; delete from `db_index`.`rel_word_fact__ou`; delete from `db_index`.`rel_word_fact__ov`; delete from `db_index`.`rel_word_fact__ow`; delete from `db_index`.`rel_word_fact__ox`; delete from `db_index`.`rel_word_fact__oy`; delete from `db_index`.`rel_word_fact__oz`; delete from `db_index`.`rel_word_fact__o_`; delete from `db_index`.`rel_word_fact__pa`; delete from `db_index`.`rel_word_fact__pb`; delete from `db_index`.`rel_word_fact__pc`; delete from `db_index`.`rel_word_fact__pd`; delete from `db_index`.`rel_word_fact__pe`; delete from `db_index`.`rel_word_fact__pf`; delete from `db_index`.`rel_word_fact__pg`; delete from `db_index`.`rel_word_fact__ph`; delete from `db_index`.`rel_word_fact__pi`; delete from `db_index`.`rel_word_fact__pj`; delete from `db_index`.`rel_word_fact__pk`; delete from `db_index`.`rel_word_fact__pl`; delete from `db_index`.`rel_word_fact__pm`; delete from `db_index`.`rel_word_fact__pn`; delete from `db_index`.`rel_word_fact__po`; delete from `db_index`.`rel_word_fact__pp`; delete from `db_index`.`rel_word_fact__pq`; delete from `db_index`.`rel_word_fact__pr`; delete from `db_index`.`rel_word_fact__ps`; delete from `db_index`.`rel_word_fact__pt`; delete from `db_index`.`rel_word_fact__pu`; delete from `db_index`.`rel_word_fact__pv`; delete from `db_index`.`rel_word_fact__pw`; delete from `db_index`.`rel_word_fact__px`; delete from `db_index`.`rel_word_fact__py`; delete from `db_index`.`rel_word_fact__pz`; delete from `db_index`.`rel_word_fact__p_`; delete from `db_index`.`rel_word_fact__qa`; delete from `db_index`.`rel_word_fact__qb`; delete from `db_index`.`rel_word_fact__qc`; delete from `db_index`.`rel_word_fact__qd`; delete from `db_index`.`rel_word_fact__qe`; delete from `db_index`.`rel_word_fact__qf`; delete from `db_index`.`rel_word_fact__qg`; delete from `db_index`.`rel_word_fact__qh`; delete from `db_index`.`rel_word_fact__qi`; delete from `db_index`.`rel_word_fact__qj`; delete from `db_index`.`rel_word_fact__qk`; delete from `db_index`.`rel_word_fact__ql`; delete from `db_index`.`rel_word_fact__qm`; delete from `db_index`.`rel_word_fact__qn`; delete from `db_index`.`rel_word_fact__qo`; delete from `db_index`.`rel_word_fact__qp`; delete from `db_index`.`rel_word_fact__qq`; delete from `db_index`.`rel_word_fact__qr`; delete from `db_index`.`rel_word_fact__qs`; delete from `db_index`.`rel_word_fact__qt`; delete from `db_index`.`rel_word_fact__qu`; delete from `db_index`.`rel_word_fact__qv`; delete from `db_index`.`rel_word_fact__qw`; delete from `db_index`.`rel_word_fact__qx`; delete from `db_index`.`rel_word_fact__qy`; delete from `db_index`.`rel_word_fact__qz`; delete from `db_index`.`rel_word_fact__q_`; delete from `db_index`.`rel_word_fact__ra`; delete from `db_index`.`rel_word_fact__rb`; delete from `db_index`.`rel_word_fact__rc`; delete from `db_index`.`rel_word_fact__rd`; delete from `db_index`.`rel_word_fact__re`; delete from `db_index`.`rel_word_fact__rf`; delete from `db_index`.`rel_word_fact__rg`; delete from `db_index`.`rel_word_fact__rh`; delete from `db_index`.`rel_word_fact__ri`; delete from `db_index`.`rel_word_fact__rj`; delete from `db_index`.`rel_word_fact__rk`; delete from `db_index`.`rel_word_fact__rl`; delete from `db_index`.`rel_word_fact__rm`; delete from `db_index`.`rel_word_fact__rn`; delete from `db_index`.`rel_word_fact__ro`; delete from `db_index`.`rel_word_fact__rp`; delete from `db_index`.`rel_word_fact__rq`; delete from `db_index`.`rel_word_fact__rr`; delete from `db_index`.`rel_word_fact__rs`; delete from `db_index`.`rel_word_fact__rt`; delete from `db_index`.`rel_word_fact__ru`; delete from `db_index`.`rel_word_fact__rv`; delete from `db_index`.`rel_word_fact__rw`; delete from `db_index`.`rel_word_fact__rx`; delete from `db_index`.`rel_word_fact__ry`; delete from `db_index`.`rel_word_fact__rz`; delete from `db_index`.`rel_word_fact__r_`; delete from `db_index`.`rel_word_fact__sa`; delete from `db_index`.`rel_word_fact__sb`; delete from `db_index`.`rel_word_fact__sc`; delete from `db_index`.`rel_word_fact__sd`; delete from `db_index`.`rel_word_fact__se`; delete from `db_index`.`rel_word_fact__sf`; delete from `db_index`.`rel_word_fact__sg`; delete from `db_index`.`rel_word_fact__sh`; delete from `db_index`.`rel_word_fact__si`; delete from `db_index`.`rel_word_fact__sj`; delete from `db_index`.`rel_word_fact__sk`; delete from `db_index`.`rel_word_fact__sl`; delete from `db_index`.`rel_word_fact__sm`; delete from `db_index`.`rel_word_fact__sn`; delete from `db_index`.`rel_word_fact__so`; delete from `db_index`.`rel_word_fact__sp`; delete from `db_index`.`rel_word_fact__sq`; delete from `db_index`.`rel_word_fact__sr`; delete from `db_index`.`rel_word_fact__ss`; delete from `db_index`.`rel_word_fact__st`; delete from `db_index`.`rel_word_fact__su`; delete from `db_index`.`rel_word_fact__sv`; delete from `db_index`.`rel_word_fact__sw`; delete from `db_index`.`rel_word_fact__sx`; delete from `db_index`.`rel_word_fact__sy`; delete from `db_index`.`rel_word_fact__sz`; delete from `db_index`.`rel_word_fact__s_`; delete from `db_index`.`rel_word_fact__ta`; delete from `db_index`.`rel_word_fact__tb`; delete from `db_index`.`rel_word_fact__tc`; delete from `db_index`.`rel_word_fact__td`; delete from `db_index`.`rel_word_fact__te`; delete from `db_index`.`rel_word_fact__tf`; delete from `db_index`.`rel_word_fact__tg`; delete from `db_index`.`rel_word_fact__th`; delete from `db_index`.`rel_word_fact__ti`; delete from `db_index`.`rel_word_fact__tj`; delete from `db_index`.`rel_word_fact__tk`; delete from `db_index`.`rel_word_fact__tl`; delete from `db_index`.`rel_word_fact__tm`; delete from `db_index`.`rel_word_fact__tn`; delete from `db_index`.`rel_word_fact__to`; delete from `db_index`.`rel_word_fact__tp`; delete from `db_index`.`rel_word_fact__tq`; delete from `db_index`.`rel_word_fact__tr`; delete from `db_index`.`rel_word_fact__ts`; delete from `db_index`.`rel_word_fact__tt`; delete from `db_index`.`rel_word_fact__tu`; delete from `db_index`.`rel_word_fact__tv`; delete from `db_index`.`rel_word_fact__tw`; delete from `db_index`.`rel_word_fact__tx`; delete from `db_index`.`rel_word_fact__ty`; delete from `db_index`.`rel_word_fact__tz`; delete from `db_index`.`rel_word_fact__t_`; delete from `db_index`.`rel_word_fact__ua`; delete from `db_index`.`rel_word_fact__ub`; delete from `db_index`.`rel_word_fact__uc`; delete from `db_index`.`rel_word_fact__ud`; delete from `db_index`.`rel_word_fact__ue`; delete from `db_index`.`rel_word_fact__uf`; delete from `db_index`.`rel_word_fact__ug`; delete from `db_index`.`rel_word_fact__uh`; delete from `db_index`.`rel_word_fact__ui`; delete from `db_index`.`rel_word_fact__uj`; delete from `db_index`.`rel_word_fact__uk`; delete from `db_index`.`rel_word_fact__ul`; delete from `db_index`.`rel_word_fact__um`; delete from `db_index`.`rel_word_fact__un`; delete from `db_index`.`rel_word_fact__uo`; delete from `db_index`.`rel_word_fact__up`; delete from `db_index`.`rel_word_fact__uq`; delete from `db_index`.`rel_word_fact__ur`; delete from `db_index`.`rel_word_fact__us`; delete from `db_index`.`rel_word_fact__ut`; delete from `db_index`.`rel_word_fact__uu`; delete from `db_index`.`rel_word_fact__uv`; delete from `db_index`.`rel_word_fact__uw`; delete from `db_index`.`rel_word_fact__ux`; delete from `db_index`.`rel_word_fact__uy`; delete from `db_index`.`rel_word_fact__uz`; delete from `db_index`.`rel_word_fact__u_`; delete from `db_index`.`rel_word_fact__va`; delete from `db_index`.`rel_word_fact__vb`; delete from `db_index`.`rel_word_fact__vc`; delete from `db_index`.`rel_word_fact__vd`; delete from `db_index`.`rel_word_fact__ve`; delete from `db_index`.`rel_word_fact__vf`; delete from `db_index`.`rel_word_fact__vg`; delete from `db_index`.`rel_word_fact__vh`; delete from `db_index`.`rel_word_fact__vi`; delete from `db_index`.`rel_word_fact__vj`; delete from `db_index`.`rel_word_fact__vk`; delete from `db_index`.`rel_word_fact__vl`; delete from `db_index`.`rel_word_fact__vm`; delete from `db_index`.`rel_word_fact__vn`; delete from `db_index`.`rel_word_fact__vo`; delete from `db_index`.`rel_word_fact__vp`; delete from `db_index`.`rel_word_fact__vq`; delete from `db_index`.`rel_word_fact__vr`; delete from `db_index`.`rel_word_fact__vs`; delete from `db_index`.`rel_word_fact__vt`; delete from `db_index`.`rel_word_fact__vu`; delete from `db_index`.`rel_word_fact__vv`; delete from `db_index`.`rel_word_fact__vw`; delete from `db_index`.`rel_word_fact__vx`; delete from `db_index`.`rel_word_fact__vy`; delete from `db_index`.`rel_word_fact__vz`; delete from `db_index`.`rel_word_fact__v_`; delete from `db_index`.`rel_word_fact__wa`; delete from `db_index`.`rel_word_fact__wb`; delete from `db_index`.`rel_word_fact__wc`; delete from `db_index`.`rel_word_fact__wd`; delete from `db_index`.`rel_word_fact__we`; delete from `db_index`.`rel_word_fact__wf`; delete from `db_index`.`rel_word_fact__wg`; delete from `db_index`.`rel_word_fact__wh`; delete from `db_index`.`rel_word_fact__wi`; delete from `db_index`.`rel_word_fact__wj`; delete from `db_index`.`rel_word_fact__wk`; delete from `db_index`.`rel_word_fact__wl`; delete from `db_index`.`rel_word_fact__wm`; delete from `db_index`.`rel_word_fact__wn`; delete from `db_index`.`rel_word_fact__wo`; delete from `db_index`.`rel_word_fact__wp`; delete from `db_index`.`rel_word_fact__wq`; delete from `db_index`.`rel_word_fact__wr`; delete from `db_index`.`rel_word_fact__ws`; delete from `db_index`.`rel_word_fact__wt`; delete from `db_index`.`rel_word_fact__wu`; delete from `db_index`.`rel_word_fact__wv`; delete from `db_index`.`rel_word_fact__ww`; delete from `db_index`.`rel_word_fact__wx`; delete from `db_index`.`rel_word_fact__wy`; delete from `db_index`.`rel_word_fact__wz`; delete from `db_index`.`rel_word_fact__w_`; delete from `db_index`.`rel_word_fact__xa`; delete from `db_index`.`rel_word_fact__xb`; delete from `db_index`.`rel_word_fact__xc`; delete from `db_index`.`rel_word_fact__xd`; delete from `db_index`.`rel_word_fact__xe`; delete from `db_index`.`rel_word_fact__xf`; delete from `db_index`.`rel_word_fact__xg`; delete from `db_index`.`rel_word_fact__xh`; delete from `db_index`.`rel_word_fact__xi`; delete from `db_index`.`rel_word_fact__xj`; delete from `db_index`.`rel_word_fact__xk`; delete from `db_index`.`rel_word_fact__xl`; delete from `db_index`.`rel_word_fact__xm`; delete from `db_index`.`rel_word_fact__xn`; delete from `db_index`.`rel_word_fact__xo`; delete from `db_index`.`rel_word_fact__xp`; delete from `db_index`.`rel_word_fact__xq`; delete from `db_index`.`rel_word_fact__xr`; delete from `db_index`.`rel_word_fact__xs`; delete from `db_index`.`rel_word_fact__xt`; delete from `db_index`.`rel_word_fact__xu`; delete from `db_index`.`rel_word_fact__xv`; delete from `db_index`.`rel_word_fact__xw`; delete from `db_index`.`rel_word_fact__xx`; delete from `db_index`.`rel_word_fact__xy`; delete from `db_index`.`rel_word_fact__xz`; delete from `db_index`.`rel_word_fact__x_`; delete from `db_index`.`rel_word_fact__ya`; delete from `db_index`.`rel_word_fact__yb`; delete from `db_index`.`rel_word_fact__yc`; delete from `db_index`.`rel_word_fact__yd`; delete from `db_index`.`rel_word_fact__ye`; delete from `db_index`.`rel_word_fact__yf`; delete from `db_index`.`rel_word_fact__yg`; delete from `db_index`.`rel_word_fact__yh`; delete from `db_index`.`rel_word_fact__yi`; delete from `db_index`.`rel_word_fact__yj`; delete from `db_index`.`rel_word_fact__yk`; delete from `db_index`.`rel_word_fact__yl`; delete from `db_index`.`rel_word_fact__ym`; delete from `db_index`.`rel_word_fact__yn`; delete from `db_index`.`rel_word_fact__yo`; delete from `db_index`.`rel_word_fact__yp`; delete from `db_index`.`rel_word_fact__yq`; delete from `db_index`.`rel_word_fact__yr`; delete from `db_index`.`rel_word_fact__ys`; delete from `db_index`.`rel_word_fact__yt`; delete from `db_index`.`rel_word_fact__yu`; delete from `db_index`.`rel_word_fact__yv`; delete from `db_index`.`rel_word_fact__yw`; delete from `db_index`.`rel_word_fact__yx`; delete from `db_index`.`rel_word_fact__yy`; delete from `db_index`.`rel_word_fact__yz`; delete from `db_index`.`rel_word_fact__y_`; delete from `db_index`.`rel_word_fact__za`; delete from `db_index`.`rel_word_fact__zb`; delete from `db_index`.`rel_word_fact__zc`; delete from `db_index`.`rel_word_fact__zd`; delete from `db_index`.`rel_word_fact__ze`; delete from `db_index`.`rel_word_fact__zf`; delete from `db_index`.`rel_word_fact__zg`; delete from `db_index`.`rel_word_fact__zh`; delete from `db_index`.`rel_word_fact__zi`; delete from `db_index`.`rel_word_fact__zj`; delete from `db_index`.`rel_word_fact__zk`; delete from `db_index`.`rel_word_fact__zl`; delete from `db_index`.`rel_word_fact__zm`; delete from `db_index`.`rel_word_fact__zn`; delete from `db_index`.`rel_word_fact__zo`; delete from `db_index`.`rel_word_fact__zp`; delete from `db_index`.`rel_word_fact__zq`; delete from `db_index`.`rel_word_fact__zr`; delete from `db_index`.`rel_word_fact__zs`; delete from `db_index`.`rel_word_fact__zt`; delete from `db_index`.`rel_word_fact__zu`; delete from `db_index`.`rel_word_fact__zv`; delete from `db_index`.`rel_word_fact__zw`; delete from `db_index`.`rel_word_fact__zx`; delete from `db_index`.`rel_word_fact__zy`; delete from `db_index`.`rel_word_fact__zz`;", NULL, NULL);
        int error4 = sql_execute("COMMIT;", NULL, NULL);
        printf("Done.\n");
        printf("Run vacuum...\n");
        int error5 = sql_execute("VACUUM;", NULL, NULL);
        printf("Done.\n");
        printf("Create new index...\n");
        
        re_index_size_sql = 10001;
        re_index_pos_sql = 10001;
        char* sql = calloc(re_index_size_sql, 1);
        
        re_index_in_facts = 1;
        {
            char sql[5120];
            *sql = 0;
            strcat(sql, "SELECT count(pk) from facts;");
            printf("%s\n", sql);
            char key[99];
            int error = sql_execute(sql, select_primary_key, key);
            int count = to_number(key ? key : "0");
            
            int k = 0;
            while (k < count) {
                *sql = 0;
                strcat(sql, "SELECT `nmain`.`pk`, `nmain`.`verb` || \"00000\", `nmain`.`subjects`, `nmain`.`objects`, `nmain`.`adverbs`, `nmain`.`questionword`, `nmain`.`fileid`, `nmain`.`line`, `nmain`.`truth`");
                strcat(sql, " FROM facts AS nmain LIMIT ");
                char _k_1[40];
                snprintf(_k_1, 39, "%d", k);
                char _k_2[40];
                snprintf(_k_2, 39, "%d", k+20000);
                strcat(sql, _k_1);
                strcat(sql, ", ");
                strcat(sql, _k_2);
                strcat(sql, ";");
                printf("%s\n", sql);
                
                int error = sql_execute(sql, callback_re_index, key);
            }
        }
        
        printf("Done.\n");
        int error6 = sql_execute("BEGIN;", NULL, NULL);
    }
    return 0;
}

struct fact** disk_search_clauses(int rel) {
    int limit = 8000;
    if (strcmp("1", check_config("limit-amount-of-answers", "1"))) {
        limit = 100000;
    }
    struct fact** clauses = calloc(1, sizeof(struct fact*)*(limit+1));
    int position = 0;

    struct request_get_facts_for_words req;
    req.words    = 0;
    req.facts    = clauses;
    req.limit    = limit;
    req.position = &position;
    req.rel      = rel;
    req.make_rawfacts = 0;
    req.debug_facts = 0;
    
    {
        req.hash_n_1 = 0;
        req.hash_n_2 = 0;
        req.hash_n_3 = 0;

        char* sql = gen_sql_get_clauses_for_rel(rel, clauses, limit, &position);
        //printf("%s\n", sql);
        int error = sql_execute(sql, callback_get_facts, &req);
        free(sql);
    }
    
    return clauses;
}

int disk_delete_everything_from(const char* filename) {
    {
        char* sql = gen_sql_delete_everything_from(filename);
        int error = sql_execute(sql, NULL, NULL);
        free(sql);
        return error;
    }
    return INVALID_POINTER;
}

int disk_add_filename(const char* filename) {
    {
        char* sql = gen_sql_add_filename(filename);
        int error = sql_execute(sql, NULL, NULL);
        free(sql);
        return error;
    }
    return INVALID_POINTER;
}

int disk_set_to_invalid_value(void** p) {
    if (!p) return 1;
    if (*p && can_be_a_pointer(*p)) free(*p);
    *p = INVALID_POINTER;
    return 0;
}

int disk_add_link (const char* link, int key_1, int key_2) {
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
    
    int error = sql_execute(sql, NULL, NULL);
    return error;
}


