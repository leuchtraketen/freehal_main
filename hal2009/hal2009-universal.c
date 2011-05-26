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

#include "hal2009-universal.h"

// variables
struct word** variable_replacements = 0;
int variable_replacements_c = 0;
short config_match_variables = 0;

// printf catcher
static char catched[20480];
static int size_catched = 20479;
static int len_catched = 0;
static int do_catchf = 0;
int catchf(const char* fmt, void* arg1) {
    char tmp[256];
    if (strstr(fmt, "%")) {
        snprintf(tmp, 255, fmt, arg1);
    }
    else {
        snprintf(tmp, 255, "%s", fmt);
    }
    if (len_catched + 256 < size_catched) {
        strcat(catched, tmp);
        len_catched += 256;
    }
    return 0;
}

const char* get_catchf() {
    return catched;
}
int begin_catchf() {
    do_catchf = 1;
}
int clear_catchf() {
    strcpy(catched, "");
    len_catched = 0;
}
int end_catchf() {
    do_catchf = 0;
}


int universal_begin(const char* modes) {
    int r = 0;
    if (is_engine("ram")) {
        r = ram_begin(modes);
    }
    else {
        r = disk_begin(modes);
    }

    return r;
}

int universal_end() {
    int r = 0;
    if (is_engine("ram")) {
        r = ram_end();
    }
    else {
        r = disk_end();
    }
    return r;
}

int universal_clear_cache() {
    int r = 0;
    if (is_engine("disk")) {
        r = disk_clear_cache();
    }
    return r;
}

struct word* get_word(const char* name) {
    if (is_engine("ram")) {
        return ram_get_word(name);
    }
    else {
        return disk_get_word(name);
    }
}

long can_be_a_pointer(void* _p) {
    long p = (long)_p;
    if (p < 0) p = -p;
    if (p > 100000) {
        return 1;
    }
    if (p == (long)INVALID_POINTER) {
        return 0;
    }
    return 0;
}

int set_to_invalid_value(void** p) {
    if (is_engine("ram")) {
        return ram_set_to_invalid_value(p);
    }
    else {
        return disk_set_to_invalid_value(p);
    }
    return 0;
}

int set_to_invalid_fact(struct fact** p) {
    if (is_engine("ram")) {
        return ram_set_to_invalid_value(p);
    }
    else {
        free_words_weak((*p)->verbs);
        free_words_weak((*p)->subjects);
        free_words_weak((*p)->objects);
        free_words_weak((*p)->adverbs);
        free_words_weak((*p)->extra);
        free((*p)->filename);
        free((*p)->line);

        return disk_set_to_invalid_value(p);
    }
    return 0;
}

static int max_synonym_count = 99;
static int max_entity_words_count = 9;

struct synonym* construct_synonym(const char* _exp, const char** _exps, struct word* word, struct word** words, int level, int direction) {
    if (is_bad(_exp) && !_exps && !word && !words) {
        return 0;
    }

    char* exp   = 0;
    char** exps = 0;

    if (_exp) {
        exp = strdup(_exp);
    }
    if (_exps) {
        exps = calloc(sizeof(char*), max_entity_words_count+1);
        int i = 0;
        while (_exps[i]) {
            exps[i] = strdup(_exps[i]);
            ++i;
        }
    }


    /// single value and array...
    if (exp && !exps) {
        exps = calloc(sizeof(char*), max_entity_words_count+1);
        exps[0] = strdup(exp);
        exp = 0;
        if (!word) {
            word = set_word(exps[0]);
        }

        words = calloc(sizeof(struct word*), max_entity_words_count+1);
        words[0] = word;
        word = 0;
    }

    if (!words) {
        words = calloc(sizeof(struct word*), max_entity_words_count+1);
        int i;
        for (i = 0; exps[i] && i < max_entity_words_count; ++i) {
            words[i] = set_word(exps[i]);
        }
    }

    if (exps) {
        free(exp);
        exp = 0;

        int i = 0;
        while (exps[i]) {
            free(exps[i]);
            exps[i] = 0;
            ++i;
        }
        exps = 0;
    }

    struct synonym* syn = calloc(sizeof(struct synonym), 1);
    syn->words = words;
    syn->level = level;
    syn->direction = direction;

    return syn;
}

int free_synonym(struct synonym* syn) {
    if (is_engine("ram")) {
        free(syn->words);
    }
    else {
        free_words_weak(syn->words);
    }
    syn->words = 0;

    free(syn);
    syn = 0;

    return 0;
}

int free_synonym_set(struct synonym_set* synonym_set) {
    int i;

    {
        for (i = 0; i < synonym_set->position_subjects; ++i) {
            if (synonym_set->subjects[i]) {
                free(synonym_set->subjects[i]);
                synonym_set->subjects[i] = 0;
            }
        }
        free(synonym_set->subjects);
        synonym_set->subjects = 0;
    }
    {
        for (i = 0; i < synonym_set->position_objects; ++i) {
            if (synonym_set->objects[i]) {
                free(synonym_set->objects[i]);
                synonym_set->objects[i] = 0;
            }
        }
        free(synonym_set->objects);
        synonym_set->objects = 0;
    }
    {
        for (i = 0; i < synonym_set->position_adverbs; ++i) {
            if (synonym_set->adverbs[i]) {
                free(synonym_set->adverbs[i]);
                synonym_set->adverbs[i] = 0;
            }
        }
        free(synonym_set->adverbs);
        synonym_set->adverbs = 0;
    }
    {
        for (i = 0; i < synonym_set->position_extra; ++i) {
            if (synonym_set->extra[i]) {
                free(synonym_set->extra[i]);
                synonym_set->extra[i] = 0;
            }
        }
        free(synonym_set->extra);
        synonym_set->extra = 0;
    }

    free(synonym_set);

    return 0;
}

int check_synonym(struct synonym* syn_a, struct synonym** synonyms, int a, int b) {
    int i;
    for (i = a; synonyms[i] && i < max_synonym_count && i < b; ++i) {
        struct synonym* syn_b = synonyms[i];

        if (syn_a->level < syn_b->level) {
            continue;
        }
        if (syn_a->direction != syn_b->direction) {
            continue;
        }

        short synonym_matches = 1;
        int k;
        for (k = 0; k < max_entity_words_count; ++k) {
            if (!syn_a->words[k] && !syn_b->words[k]) {
                break;
            }
            if (!syn_a->words[k] && syn_b->words[k]) {
                synonym_matches = 0;
                break;
            }
            if (syn_a->words[k] && !syn_b->words[k]) {
                synonym_matches = 0;
                break;
            }
            if (syn_a->words[k] && syn_b->words[k]) {
                if (strcmp(syn_a->words[k]->name, syn_b->words[k]->name)) {
                    synonym_matches = 0;
                    break;
                }
            }
        }

        if (synonym_matches) {
            return 1;
        }
    }

    return 0;
}

struct synonym** put_synonym_raw(struct synonym* syn, struct synonym** synonyms, int* position) {
    if (*position >= max_synonym_count) {
        free_synonym(syn);
        return synonyms;
    }


    synonyms[*position] = syn;
    ++(*position);

    debugf("    synonym at %d, level %d: '%s', '%s', '%s', '%s'\n", (*position)-1, syn->level,
            syn->words[0]                                                    ? syn->words[0]->name : "",
            syn->words[0] && syn->words[1]                                   ? syn->words[1]->name : "",
            syn->words[0] && syn->words[1] && syn->words[2]                  ? syn->words[2]->name : "",
            syn->words[0] && syn->words[1] && syn->words[2] && syn->words[3] ? syn->words[3]->name : ""
    );

    return synonyms;
}

struct synonym** put_synonym(struct synonym* syn, struct synonym** synonyms, int* position) {
    if (check_synonym(syn, synonyms, 0, *position)) {
        free_synonym(syn);
        return synonyms;
    }

    return put_synonym_raw(syn, synonyms, position);
}

struct synonym** add_synonyms_by_string(const char** exps, struct synonym** synonyms, int* position, const char* category) {

    debugf("  by string:\n");

    int e;
    for (e = 0; exps[e]; ++e) {
        const char* exp = exps[e];

        if (is_bad(exp)) {
            continue;
        }

        struct word** words = divide_words(exp);
        synonyms = put_synonym(construct_synonym(0, 0, 0, words, 0, FORWARD), synonyms, position);

        if (strlen(exp) > 1 && strstr(exp, " ") && !strstr(exp, "_")) {
            synonyms[*position] = calloc(sizeof(struct word*), strlen(exp)+10);

            char* _exp = calloc(1, strlen(exp)+10);
            strcpy(_exp, "_");
            strcat(_exp, exp);
            strcat(_exp, "_");
            int q;
            for (q = 0; q < strlen(_exp); ++q) {
                if (_exp[q] == ' ') {
                    while (q+1 < strlen(_exp) && _exp[q+1] == ' ') {
                        strcpy(_exp+q, _exp+q+1);
                    }
                    _exp[q] = '_';
                }
            }

            synonyms = put_synonym(construct_synonym(_exp, 0, 0, 0, 0, FORWARD), synonyms, position);
        }

        if (exp[0] == '*' && strlen(exp) > 1) {
            struct word** words = divide_words(exp+1);

            synonyms = put_synonym(construct_synonym(0, 0, 0, words, 0, FORWARD), synonyms, position);
        }

        if (exp[0] == '_' && strlen(exp) >= 3) {
            char* cpy_of_exp = strdup(exp+1);
            char* offset_of_last_underscore = strstr(cpy_of_exp+strlen(cpy_of_exp)-2, "_");
            if (offset_of_last_underscore) {
                offset_of_last_underscore[0] = '\0';
                int c;
                for (c = 0; cpy_of_exp[c]; ++c) {
                    if (cpy_of_exp[c] == '_') {
                        while (c+1 < strlen(cpy_of_exp) && cpy_of_exp[c+1] == '_') {
                            strcpy(cpy_of_exp+c, cpy_of_exp+c+1);
                        }
                        cpy_of_exp[c] = ' ';
                    }
                }
                struct word**  words    = divide_words(cpy_of_exp);

                synonyms = put_synonym(construct_synonym(0, 0, 0, words, 0, FORWARD), synonyms, position);
            }
            free(cpy_of_exp);
        }
    }

    return synonyms;
}

struct synonym** put_search_synonyms(const char* subj, const char* obj, const char* verb, const char* adverbs, int use_what, struct fact** facts, struct synonym** synonyms, int* position, int level, short direction) {
    if (facts == TOOMUCH_P) {
        return synonyms;
    }

    int f;
    for (f = 0; facts[f]; ++f) {
        if (can_be_a_pointer(facts[f]) && can_be_a_pointer(facts[f]->adverbs) && can_be_a_pointer(facts[f]->adverbs[0]) && can_be_a_pointer(facts[f]->adverbs[0]->name) && can_be_a_synonym(facts[f]->adverbs[0]->name)) {
            set_to_invalid_fact(&(facts[f]));
            debugf("-> invalid: has adverbs, cannot be a synonym: no %i.\n", f);
        }
        if (can_be_a_pointer(facts[f]) && facts[f]->truth < 0.5) {
            set_to_invalid_fact(&(facts[f]));
            debugf("-> invalid: not true, cannot be a synonym: no %i.\n", f);
        }
    }

    // add to synonyms list
    if (use_what == USE_SUBJECTS) {
        for (f = 0; facts[f]; ++f) {

            if (   can_be_a_pointer(facts[f])
                && can_be_a_pointer(facts[f]->subjects)
                && (
                (
                       can_be_a_pointer(facts[f]->subjects[0])
                    && can_be_a_pointer(facts[f]->subjects[0]->name)
                    && can_be_a_synonym(facts[f]->subjects[0]->name)
                    && !strstr(facts[f]->subjects[0]->name, obj)
                )
                || (
                       can_be_a_pointer(facts[f]->subjects[0])
                    && can_be_a_pointer(facts[f]->subjects[1])
                    && can_be_a_pointer(facts[f]->subjects[1]->name)
                    && can_be_a_synonym(facts[f]->subjects[1]->name)
                    && !strstr(facts[f]->subjects[1]->name, obj)
                )
                || (
                       can_be_a_pointer(facts[f]->subjects[0])
                    && can_be_a_pointer(facts[f]->subjects[1])
                    && can_be_a_pointer(facts[f]->subjects[2])
                    && can_be_a_pointer(facts[f]->subjects[2]->name)
                    && can_be_a_synonym(facts[f]->subjects[2]->name)
                    && !strstr(facts[f]->subjects[2]->name, obj)
                )
                )

            ) {

                int c;
                struct word** temp = calloc(sizeof(struct word*), max_entity_words_count+1);
                temp[0] = 0;
                for (c = 0; c < max_entity_words_count && can_be_a_pointer(facts[f]->subjects[c]) && can_be_a_pointer(facts[f]->subjects[c]->name) && is_good(facts[f]->subjects[c]->name); ++c) {
                    temp[c] = facts[f]->subjects[c];
                }
                temp[c] = 0;

                synonyms = put_synonym(construct_synonym(0, 0, 0, temp, level, direction), synonyms, position);
            }
        }
    }
    else if (use_what == USE_OBJECTS) {
        for (f = 0; facts[f]; ++f) {
            if (   can_be_a_pointer(facts[f])
                && can_be_a_pointer(facts[f]->objects)
                && can_be_a_pointer(facts[f]->objects[0])
                && can_be_a_pointer(facts[f]->objects[0]->name)
                && can_be_a_synonym(facts[f]->objects[0]->name)
                && !strstr(facts[f]->objects[0]->name, subj)
            ) {
                int c;
                struct word** temp = calloc(sizeof(struct word*), max_entity_words_count+1);
                temp[0] = 0;
                for (c = 0; max_entity_words_count && can_be_a_pointer(facts[f]->objects[c]) && can_be_a_pointer(facts[f]->objects[c]->name) && is_good(facts[f]->objects[c]->name); ++c) {
                    temp[c] = facts[f]->objects[c];
                }
                temp[c] = 0;

                synonyms = put_synonym(construct_synonym(0, 0, 0, temp, level, direction), synonyms, position);
            }
        }
    }

    return synonyms;
}

char** synonyms_to_expressions(struct synonym** synonyms, int a, int b) {
    int i = 0;
    char** entities = calloc(sizeof(char*), max_synonym_count+1);

    int u;
    for (u = a; u < b; ++u) {
        if (synonyms[u] && synonyms[u] != INVALID_POINTER) {
            char* req_synonym = join_words(synonyms[u]->words);

            entities[i] = req_synonym;
            ++i;
        }
    }

    return entities;
}

int free_facts_synonyms(struct fact** facts_synonyms) {
    if (is_engine("ram")) {
        free(facts_synonyms);
    }
    else {
        free_facts_deep(facts_synonyms);
    }
    return 0;
}

int free_expressions(char** exps) {
    int i;
    for (i = 0; exps[i] && i < max_synonym_count; ++i) {
        free(exps[i]);
    }
    free(exps);
    return 0;
}

struct synonym** add_synonyms_by_search(const char** exps, struct synonym** synonyms, int* position, const char* category) {

    debugf("  by search:\n");

    const char* synonym_verbs = "bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|=";

    int e;
    for (e = 0; exps[e]; ++e) {
        const char* exp = exps[e];

        if (!can_be_a_main_synonym(exp)) {
            continue;
        }
        printf("can_be_a_main_synonym: exp='%s'\n", exp);

        // level 1
        int begin_level_1 = *position;
        {
            if (begin_level_1 - 0 > 0) {
                char** exps = synonyms_to_expressions(synonyms, 0, begin_level_1);
                struct fact** facts_synonyms = search_facts_synonyms("", (const char **)exps, "", 0, synonym_verbs, "", "", "", "default");
                if (facts_synonyms != TOOMUCH_P) {
                    synonyms = put_search_synonyms(exp, "", synonym_verbs, "", USE_OBJECTS, facts_synonyms, synonyms, position, 1, FORWARD);
                    free_facts_synonyms(facts_synonyms);
                }
                free_expressions(exps);
            }
        }

        // level 2 forward
        int begin_level_2 = *position;
        if (begin_level_2-begin_level_1 < 39) {
            if (begin_level_2 - begin_level_1 > 0) {
                char** exps = synonyms_to_expressions(synonyms, begin_level_1, begin_level_2);
                struct fact** facts_synonyms = search_facts_synonyms("", (const char **)exps, "", 0, synonym_verbs, "", "", "", "default");
                if (facts_synonyms != TOOMUCH_P) {
                    int f;
                    for (f = 0; facts_synonyms[f]; ++f) {
                        if (can_be_a_pointer(facts_synonyms[f]) && can_be_a_pointer(facts_synonyms[f]->filename) && strstr(facts_synonyms[f]->filename, "ps-")) {
                            set_to_invalid_fact(&(facts_synonyms[f]));
                        }
                        if (can_be_a_pointer(facts_synonyms[f]) && can_be_a_pointer(facts_synonyms[f]->filename) && strstr(facts_synonyms[f]->filename, "fa-")) {
                            set_to_invalid_fact(&(facts_synonyms[f]));
                        }
                    }
                    synonyms = put_search_synonyms(exp, "", synonym_verbs, "", USE_OBJECTS, facts_synonyms, synonyms, position, 2, FORWARD);
                    free_facts_synonyms(facts_synonyms);
                }
                free_expressions(exps);
            }
        }

        // level 3 forward
        int begin_level_3 = *position;
        if (begin_level_3-begin_level_2 < 39) {
            if (begin_level_3 - begin_level_2 > 0) {
                char** exps = synonyms_to_expressions(synonyms, begin_level_2, begin_level_3);
                struct fact** facts_synonyms = search_facts_synonyms("", (const char**)exps, "", 0, synonym_verbs, "", "", "", "default");
                if (facts_synonyms != TOOMUCH_P) {
                    int f;
                    for (f = 0; facts_synonyms[f]; ++f) {
                        if (can_be_a_pointer(facts_synonyms[f]) && can_be_a_pointer(facts_synonyms[f]->filename) && strstr(facts_synonyms[f]->filename, "ps-")) {
                            set_to_invalid_fact(&(facts_synonyms[f]));
                        }
                        if (can_be_a_pointer(facts_synonyms[f]) && can_be_a_pointer(facts_synonyms[f]->filename) && strstr(facts_synonyms[f]->filename, "fa-")) {
                            set_to_invalid_fact(&(facts_synonyms[f]));
                        }
                    }
                    synonyms = put_search_synonyms(exp, "", synonym_verbs, "", USE_OBJECTS, facts_synonyms, synonyms, position, 3, FORWARD);
                    free_facts_synonyms(facts_synonyms);
                }
                free_expressions(exps);
            }
        }

        // level 2 reverse
        if (!strcmp(category, "extra") || !strcmp(category, "adverbs-parts")) {
            int limit = 20000;
            struct string_pair** facts = calloc(1, sizeof(struct string_pair*)*(limit+1));
            int _position = 0;
            get_thesaurus_synonyms(exp, facts, limit, &_position, 2, 1);

            int i = 0;
            while (facts[i]) {
                ++i;
            }

            i = 0;
            while (facts[i]) {
                struct word** words = divide_words(facts[i]->subjects);
                synonyms = put_synonym(construct_synonym(0, 0, 0, words, 2, REVERSE), synonyms, position);

                if (facts[i]->subjects) free(facts[i]->subjects);
                if (facts[i]-> objects) free(facts[i]-> objects);
                if (facts[i])           free(facts[i]);
                facts[i] = 0;
                ++i;
            }

            free(facts);
        }
    }

    return synonyms;
}

struct word*** filter_synonyms(struct synonym** synonyms, int level, short direction) {
    struct word*** filtered = calloc(sizeof(char*), max_synonym_count+1);

    int i;
    int k = 0;
    for (i = 0; i < max_synonym_count; ++i) {
        struct synonym* syn = synonyms[i];

        if (syn && syn != INVALID_POINTER) {
            //printf("level (%d) >= syn->level (%d) && direction (%d) == syn->direction (%d)\n", level, syn->level, direction, syn->direction);
            if (level >= syn->level && (direction == syn->direction || direction == REVERSE)) {
                filtered[k] = syn->words;
                ++k;
            }
        }
    }

    print_word_list_3rd_order(filtered);

    return filtered;
}

struct synonym** prepare_synonyms() {
    struct synonym** synonyms = calloc(sizeof(struct synonym*), max_synonym_count+1);

    return synonyms;
}

int except_article(const void* _a) {
    const struct word* a = (const struct word*) _a;
    return ( !a || !(a->name) || (
           strstr(a->name, "(a)")
        || strstr(a->name, "(the)")
    ) ? 0 : 1);
}

int copy_list(int a_1, int a_2, int b_1, int b_2, void** a, void** b, int(*except)(const void*)) {
    for (; (a_1 < a_2 || a_2 < 0) && (b_1 < b_2 || b_2 < 0) && b[b_1]; ++a_1, ++b_1) {
        if (!except(b[b_1])) {
            --a_1;
            continue;
        }
        a[a_1] = b[b_1];
    }
    return 0;
}

struct synonym** add_synonyms(const char* exp, const char** _exps, struct synonym** synonyms, int* position, const char* category) {

    debugf("synonyms: '%s'\n", category);

    /// single value and array...
    const char** exps = _exps;
    short free_exps =  0;
    if (exp && !exps) {
        exps = calloc(sizeof(const char*), max_synonym_count+1);
        exps[0] = exp;
        exp = 0;
        free_exps = 1;
    }

    // by string
    synonyms = add_synonyms_by_string((const char **)exps, synonyms, position, category);

    // by search
    synonyms = add_synonyms_by_search((const char **)exps, synonyms, position, category);

    // adverbs
    if (!strcmp(category, "adverbs")) {
        int z = 0;
        while (exps[z]) {
            char** splitted_exp = divide_by(' ', exps[z]);
            ++z;

            struct synonym*** part_synonyms = calloc(sizeof(struct synonym**), max_synonym_count+1);

            printf("1\n");
            int p;
            for (p = 0; splitted_exp[p]; ++p) {
                printf("1.%d\n", p);
                int part_position = 0;
                char** splitted_exps = calloc(sizeof(const char*), max_synonym_count+1);
                splitted_exps[0] = splitted_exp[p];
                char _tmp[4097];
                strcpy(_tmp, "(a) ");
                strcat(_tmp, splitted_exp[p]);
                splitted_exps[1] = _tmp;
                part_synonyms[p] = prepare_synonyms();
                part_synonyms[p] = add_synonyms_by_string((const char **)splitted_exps, part_synonyms[p], &part_position, "adverbs-parts");
                part_synonyms[p] = add_synonyms_by_search((const char **)splitted_exps, part_synonyms[p], &part_position, "adverbs-parts");
            }
            for (p = 0; splitted_exp[p]; ++p) {
                free(splitted_exp[p]);
            }
            free(splitted_exp);

            printf("2\n");
            struct synonym** last_new_synonyms = prepare_synonyms();
            if (part_synonyms[0]) {
                struct word** words = calloc(sizeof(struct word*), max_entity_words_count*max_entity_words_count +1);
                p = 0;
                last_new_synonyms = put_synonym_raw(construct_synonym(0, 0, 0, words, 1, FORWARD), last_new_synonyms, &p);
            }
            printf("3\n");
            int j;
            for (j = 0; part_synonyms[j]; ++j) {
                printf("3.%d\n", j);

                struct synonym** new_synonyms = prepare_synonyms();
                p = 0;
                int n;
                for (n = 0; last_new_synonyms[n]; ++n) {
                    int f;
                    for (f = 0; part_synonyms[j][f]; ++f) {
                        struct word** words = calloc(sizeof(struct word*), max_entity_words_count*max_entity_words_count +1);
                        copy_list(0, -1, 0, -1, (void **)words, (void **)last_new_synonyms[n]->words, except_article);
                        int k = next_in_list((void **)words);
                        copy_list(k, -1, 0, -1, (void **)words, (void **)part_synonyms[j][f]->words, except_article);
                        if (k == next_in_list((void **)words)) {
                            free(words);
                        }
                        else {
                            new_synonyms = put_synonym_raw(construct_synonym(0, 0, 0, words, last_new_synonyms[n]->level, last_new_synonyms[n]->direction), new_synonyms, &p);
                        }
                    }
                }
                printf("size: last_new_synonyms=%d\n", next_in_list((void **)last_new_synonyms));
                free(last_new_synonyms);
                last_new_synonyms = new_synonyms;

                printf("part_synonyms[%d]=%d\n", j, next_in_list((void **)part_synonyms[j]));
                printf("size: new_synonyms=%d\n", next_in_list((void **)new_synonyms));
            }
            printf("4\n");

            int d;
            for (d = 0; last_new_synonyms[d]; ++d) {
                synonyms = put_synonym(construct_synonym(0, 0, 0, last_new_synonyms[d]->words, last_new_synonyms[d]->level, last_new_synonyms[d]->direction), synonyms, position);
            }

            for (p = 0; part_synonyms[p]; ++p) {
                free(part_synonyms[p]);
            }
            free(part_synonyms);
            free(last_new_synonyms);
        }
    }

    if (free_exps) {
        free(exps);
    }

    // return
    return synonyms;
}

struct word** divide_words(const char* str) {
    str = (str && str[0] && str[0] != '0' && str[0] != ' ' ? str : "");

    struct word** list = calloc(sizeof(struct word*), 1);
    int count = 0;

    int a = 0;
    int e = 0;
    int p;
    int length = strlen(str);
    for (p = 0; p < length; ++p) {
        if (str[p] == ' ') {
            e = p;
            if (e - a > 0) {
                char* name = calloc(e-a+2, 1);
                strncpy(name, str+a, e-a);
                struct word* word = set_word(name);
                if (name) free(name);

                ++count;
                list = realloc(list, sizeof(struct word*)*(count+1));
                list[count] = 0;
                list[count-1] = word;
                //debugf("divided(1): %s\n", name);
                a = e+1;
            }
            while (p+1 < length && str[p] == ' ' && str[p+1] == ' ') {
                ++p;
                ++a;
            }
        }
    }

    e = p;
    if (e - a > 0) {
        char* name = calloc(e-a+2, 1);
        strncpy(name, str+a, e-a);
        if (is_good(name)) {
            // printf("e-a = %i -> name = '%s'\n", e-a, name);
            struct word* word = set_word(name);

            ++count;
            list = realloc(list, sizeof(struct word*)*(count+1));
            list[count] = 0;
            list[count-1] = word;
            //debugf("divided(2): %s\n", name);
            a = e;
        }
        if (name) free(name);
    }

    //debugf("name: %s (a = %p, e = %p), count = %p, list = %p\n", str && str[0] ? str : "(useless)", a, e, count, list);

    return list;
}

char** divide_string(const char* str) {
    str = (str && str[0] && str[0] != '0' && str[0] != ' ' ? str : "");

    char** list = calloc(sizeof(char*), 1);
    int count = 0;

    int a = 0;
    int e = 0;
    int p;
    int length = strlen(str);
    for (p = 0; p < length; ++p) {
        if ((str[p] == ' ' || str[p] == '-') && (str[p+1] == ' ' || str[p+1] == '-')) {
            ++p;
        }
        if (str[p] == ' ' || str[p] == '-') {
            e = p;
            if (e - a > 0) {
                char* name = calloc(e-a+2, 1);
                strncpy(name, str+a, e-a);

                ++count;
                list = realloc(list, sizeof(char*)*(count+1));
                list[count] = 0;
                list[count-1] = name;
                a = e+1;
            }
        }
    }

    e = p;
    if (e - a > 0) {
        char* name = calloc(e-a+2, 1);
        strncpy(name, str+a, e-a);
        if (is_good(name)) {

            ++count;
            list = realloc(list, sizeof(char*)*(count+1));
            list[count] = 0;
            list[count-1] = name;
            a = e;
        }
        else {
            free(name);
        }
    }

    return list;
}

char** divide_by(const char by, const char* str) {
    char** list = calloc(sizeof(struct word*), 1);
    int count = 0;

    int a = 0;
    int e = 0;
    int p;
    int length = strlen(str);
    for (p = 0; p < length; ++p) {
        if (str[p] == by) {
            e = p;
            if (e - a > 0) {
                char* name = calloc(e-a+2, 1);
                strncpy(name, str+a, e-a);

                ++count;
                list = realloc(list, sizeof(struct word*)*(count+1));
                list[count] = 0;
                list[count-1] = name;
                a = e+1;
            }
        }
    }

    e = p;
    if (e - a > 0) {
        char* name = calloc(e-a+2, 1);
        strncpy(name, str+a, e-a);

        ++count;
        list = realloc(list, sizeof(struct word*)*(count+1));
        list[count] = 0;
        list[count-1] = name;
        a = e;
    }

    return list;
}

struct flist* look_at_list(struct flist* l) {
    if (!l) {
        l = calloc(sizeof(struct flist), 1);
        l->list = 0;
        l->size = 0;
        l->allocated_until = 0;
    }

    return l;
}

int to_number(const char* str) {
    return (
        str && str[0]
            ? (
                str[0] == '-'
                    ? atoi(str+1)
                    : atoi(str)
            )
            : 0
    );
}

char* from_number(int i) {
    char str[101];
    strcpy(str, "");
    snprintf(str, 100, "%d", i);
    return strdup(str);
}

int is_bad(const char* s) {
    return (!can_be_a_pointer((void*)s) || !s || !s[0] || ((s[0] == ' ' || s[0] == '0' || s[0] == ')' || ((s[0] < 'a' || s[0] > 'Z') && s[0] != '_')) && strlen(s) < 1));
}
int is_good(const char* s) {
    return (!is_bad(s));
}

char* get_thesaurus_synonyms(const char* key, struct string_pair** facts, int limit, int* position, int level, short reverse) {
    if (is_engine("ram")) {
        return ram_get_thesaurus_synonyms(key, facts, limit, position, level, reverse);
    }
    else {
        return disk_get_thesaurus_synonyms(key, facts, limit, position, level, reverse);
    }
}

char* use_thesaurus_synonyms(const char* key) {
    int limit = 20000;
    struct string_pair** facts = 0;
    facts = calloc(1, sizeof(struct string_pair*)*(limit+1));
    int position = 0;

    get_thesaurus_synonyms(key, facts, limit, &position, 1, 0);
}

struct fact* add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    if (is_engine("ram")) {
        return ram_add_clause(rel, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
    }
    else {
        return disk_add_clause(rel, subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
    }
}

struct fact* add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword) {
    if (is_engine("ram")) {
        return ram_add_fact(subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, only_logic, has_conditional_questionword);
    }
    else {
        return disk_add_fact(subjects, objects, verbs, adverbs, extra, questionword, filename, line, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, only_logic, has_conditional_questionword);
    }
}

struct word* set_word(const char* name) {
    if (is_engine("ram")) {
        return ram_set_word(name);
    }
    else {
        return disk_set_word(name);
    }
}

struct DATASET search_facts_as_dataset(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    return as_dataset(search_facts(subjects, objects, verbs, adverbs, extra, questionword, context));
}
struct DATASET search_clauses_as_dataset(int rel) {
    return as_dataset(search_clauses(rel));
}

int fact_matches_truth(struct fact* fact, struct request* request) {
    int does_match = fact->truth > 0.1;

    return does_match;
}

int matches(const char* a, const char* b) {
    if (a[0] == b[0] && 0 == strcmp(a, b)) {
        return 1;
    }
    if (b[0] == '*' || strstr(b, "*")) {
        char _c[5123];
        char* c = _c;
        strncpy(c, b, 5120);
        if (c[0] == '*') {
            ++c;
            return matches(a, c);
        }
        else if (strstr(c, "*")) {
            int i;
            int size = strlen(c);
            int q = 0;
            for (i = 0; i < size || i == size; ++i) {
                if (c[i] == '*' || i == size) {
                    c[i] = '\0';
//                    catchf("Is '%s' in", a);
//                    catchf(" '%s'? ", c);
                    q = q || strstr(a, c);
                    if (q) {
//                        catchf("yes.\n", 0);
                    }
                    else {
//                        catchf("no.\n", 0);
                    }
                    if (i >= size-2) {
//                        catchf("break;\n", 0);
                        break;
                    }
                    else {
//                        catchf("going on...\n", 0);
                        c += i+1;
                        size -= i;
                        i = -1;
                    }
                }
            }
            return q;
        }
        else {
            return strstr(a, c) ? 1 : 0;
        }
    }
    if (b[0] == '_' || strstr(b, "_")) {
        char _c[5123];
        char* c = _c;
        strncpy(c, b, 5120);
        if (c[0] == '_') {
            ++c;
            return matches(a, c);
        }
        else if (strstr(c, "_")) {
            int i;
            int size = strlen(c);
            int q = 1;
            for (i = 0; i < size; ++i) {
                if (c[i] == '_') {
                    c[i] = '\0';
                    q = q && strstr(a, c);
                    c[i] = i >= size-2 ? '\0' : ' ';
                }
            }
            return q;
        }
        else {
            return strstr(a, c) ? 1 : 0;
        }
    }

    if (a[0] == '*' || strstr(a, "*")) {
        return matches(b, a) ? 1 : 0;
    }

    return 0;
}

int matches_with_pipe(const char* a, const char* b) {
    if (!strstr(b, "|")) {
        return matches(a, b);
    }

    char** splitted_b = divide_by('|', b);
    int p;
    int match = 0;
    for (p = 0; splitted_b[p]; ++p) {
        if (matches(a, splitted_b[p])) {
            match = 1;
            break;
        }
    }
    for (p = 0; splitted_b[p]; ++p) {
        free(splitted_b[p]);
    }
    free(splitted_b);

    return match;
}

int word_matches_word_array(struct word* word, struct word** words, int words_begin, int words_end, int flags) {

    if (word->name == strstr(word->name, "%")) {
        return 1;
    }
    if (strstr(word->name, "$$time$$")) {
        return 1;
    }

    int m;
    for (m = words_begin; m < words_end && words[m] && words[m]->name; ++m) {
        if (is_good(words[m]->name) && strlen(words[m]->name) >= 1) {
            //if (!is_a_trivial_word(words[m]->name)) {

                // || strstr(words[m]->name, "$$time$$")
                if (matches_with_pipe(word->name, words[m]->name) || (config_match_variables && is_a_variable(words[m]->name) && !is_a_variable(word->name))) {
                    char tmp[1000];
                    snprintf(tmp, 999, " ---     '%s' = '%s'\n", word->name, words[m]->name);
                    catchf(tmp, 0);

                    if (is_a_variable(words[m]->name) && to_number(word->name)) {
                        if (variable_replacements == 0) {
                            variable_replacements = calloc(1, sizeof(struct word*)*100);
                            variable_replacements_c = 0;
                        }
                        if (variable_replacements_c < 97) {
                            variable_replacements[variable_replacements_c] = words[m];
                            variable_replacements[variable_replacements_c+1] = word;
                            variable_replacements_c += 2;
                        }
                    }

                    return 1;
                }
                else {
                    char tmp[1000];
                    snprintf(tmp, 999, " ---     '%s' <> '%s'\n", word->name, words[m]->name);
                    catchf(tmp, 0);

                }
            //}
        }
    }

    return 0;
}

int _diff(int a, int b) {
    if (a > b) return a-b;
    if (b > a) return b-a;
    return 0;
}

int fact_matches_entity_by_entity(struct word** words, struct word*** request_words, int flags, const char* comment) {
    if (!request_words[0]) {
        catchf(comment, (void **)-1);
        return -1;
    }
    
    int words_last;
    for (words_last = 0; words[words_last]; ++words_last) {}
    int words_i;
    int words_begin = 0;
    int words_end = -1;
    for (words_i = 0; words_i <= words_last ; ++words_i) {
        if (words[words_i] && words[words_i]->name && strcmp(words[words_i]->name, "|")) {
            continue;
        }
        if (flags == WEAK && words[words_i] && words[words_i]->name && 0==strcmp(words[words_i]->name, "|")) {
            flags = EXACT;
        }

        words_begin = words_end+1;
        words_end = words_i;
        printf("%d to %d (0 to %d)\n", words_begin, words_end, words_last);
    
        int given_words_all                = 0;
        int given_words_all_only_important = 0;
        int given_words_all_also_trivial   = 0;
        int m;
        for (m = words_begin; m < words_end && words[m] && words[m]->name; ++m) {
            if (can_be_a_pointer(words[m])) {
                if (!is_a_trivial_word(words[m]->name)) {
                    ++given_words_all;
                }
                if (is_important_word(words[m]->name)) {
                    ++given_words_all_only_important;
                }
                ++given_words_all_also_trivial;
            }
        }

        for (m = 0; words[m] && words[m]->name; ++m) {
            if (is_good(words[m]->name) && strlen(words[m]->name) >= 1) {
                if (!is_a_trivial_word(words[m]->name)) {
                    if (strstr(words[m]->name, "genauso") || strstr(words[m]->name, "wie{{{adj}}}") || strstr(words[m]->name, "$$aswellas$$") || strstr(words[m]->name, "$$notaswellas$$")) {
                        flags = WEAK;
                    }
                }
            }
        }

        int count_requests = 0;

        int u;
        int does_match = 0;
        for (u = 0; request_words[u] && (INVALID_POINTER == request_words[u] || request_words[u][0]); ++u) {
            if (can_be_a_pointer(request_words[u])) {

                int request_words_all_also_trivial   = 0;
                int request_words_all_only_important = 0;
                int request_words_all = 0;
                
                int check_logic_operators = 0;

                int c;
                for (c = 0; request_words[u][c] && (INVALID_POINTER == request_words[u][c] || request_words[u][c]->name); ++c) {
                    if (can_be_a_pointer(request_words[u][c])) {
                        if (!is_a_trivial_word(request_words[u][c]->name)) {
                            ++request_words_all;
                        }
                        if (is_important_word(request_words[u][c]->name)) {
                            ++request_words_all_only_important;
                        }
                        ++request_words_all_also_trivial;
                        
                        if (0 == strcmp(request_words[u][c]->name, "&")) {
                            check_logic_operators = 1;
                        }
                    }
                }

                count_requests += (request_words_all_also_trivial) ? 1 : 0;

                // debugf("  next synonym.\n");
                int   does_match_this_synonym = 0;
                int  allow_match_this_synonym = 1;
                int should_match_this_synonym = 0;

                for (c = 0; request_words[u][c] && (INVALID_POINTER == request_words[u][c] || request_words[u][c]->name); ++c) {
                    if (can_be_a_pointer(request_words[u][c])) {
                        // if (is_a_variable(request_words[u][c]->name) && (request_words[u][c]->name != 'a' || c >= 0)) {
                        if (is_a_variable(request_words[u][c]->name)) {
                            flags = WEAK;
                            does_match_this_synonym   += 1;
                            should_match_this_synonym += 1;
                            continue;
                        }
                        if (!is_a_trivial_word(request_words[u][c]->name)) {
                            if (strstr(request_words[u][c]->name, "time_")) {
                                should_match_this_synonym -= 10;
                            }
                            else if (check_logic_operators && 0 == strcmp(request_words[u][c]->name, "&")) {
                                if (does_match_this_synonym == 0) {
                                    allow_match_this_synonym  = 0;
                                }
                                does_match_this_synonym   = 0;
                                should_match_this_synonym = 0;
                            }
                            else if (check_logic_operators && 0 == strcmp(request_words[u][c]->name, "|")) {
                                allow_match_this_synonym  = 1;
                                if (does_match_this_synonym) {
                                    break;
                                }
                            }
                            else {
                                does_match_this_synonym   += allow_match_this_synonym ? word_matches_word_array(request_words[u][c], words, words_begin, words_end, flags) : 0;
                                should_match_this_synonym += 1;
                            }
                        }
                    }
                }
                if (check_logic_operators && does_match_this_synonym == 0) {
                    
                }

                printf("flags == %s\n", flags == WEAK ? "WEAK" : flags == EXACT ? "EXACT" : flags == VERY_EXACT ? "VERY_EXACT" : "?");

                does_match = does_match || (should_match_this_synonym && does_match_this_synonym >= should_match_this_synonym);
                if (flags == WEAK) {
                    does_match = does_match || does_match_this_synonym;
                }
                if (flags == EXACT) {
                    does_match = does_match && (words_end - words_begin >= does_match_this_synonym || given_words_all_also_trivial < 2 || (_diff(given_words_all_also_trivial, request_words_all_also_trivial) < 2 && _diff(given_words_all_only_important, request_words_all_only_important) <= 0));
                }
                if (flags == VERY_EXACT) {
                    does_match = does_match && (given_words_all_also_trivial < 2 || (_diff(given_words_all_also_trivial, request_words_all_also_trivial) < 1 && _diff(given_words_all_only_important, request_words_all_only_important) <= 0));
                }
                if (does_match) {
                    break;
                }
            }
        }

        if (count_requests == 0) {
            catchf("  (the following match is '-1' because: count_requests = 0)\n", (void **)-1);
            catchf(comment, (void **)-1);
            return -1;
        }

        if (does_match)  {
            catchf(comment, (void **)does_match);
            return does_match;
        }
    }
    catchf(comment, (void **)0);
    return 0;
}

int fact_matches_subject_by_subject(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->subjects, weak, "");
    catchf("- subject by subject: %s\n", does_match?"OK":"FAIL");
    if (does_match == -1)
        does_match = 1;
    return does_match;
}
int fact_matches_object_by_subject(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->subjects, weak, "");
    catchf("-  object by subject:  %s\n", does_match?"OK":"FAIL");
    if (does_match == -1)
        does_match = 0;
    return does_match;
}
int fact_matches_subject_by_object(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->objects, weak, "");
    catchf("- subject by  object:  %s\n", does_match?"OK":"FAIL");
    if (does_match == -1) {
        if (0 == strcmp(request->context, "q_what_weakly")) {
            does_match = 0;
        }
        else {
            does_match = 1;
        }
    }
    return does_match;
}
int fact_matches_object_by_object(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->objects, weak, "");
    catchf("-  object by  object:   %s\n", does_match?"OK":"FAIL");
    if (does_match == -1)
        does_match = 1;
    return does_match;
}
int fact_matches_adverb_by_adverb(struct fact* fact, struct request* request, int weak) {
    if (request->adverbs_any) {
        catchf("-  any adverb...\n", 0);
        return 1;
    }
    int does_match = fact_matches_entity_by_entity(fact->adverbs, request->adverbs, weak, "");
    if (does_match == -1)
        does_match = 1;
    catchf("-  adverb by  adverb:   %s\n", does_match?"OK":"FAIL");
    return does_match;
}
int fact_matches_adverb_by_object(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->adverbs, request->objects, weak, "");
    if (does_match == -1)
        does_match = 0;
    catchf("-  adverb by  object:   %s\n", does_match?"OK":"FAIL");
    return does_match;
}
int fact_matches_object_by_adverb(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->adverbs, weak, "");
    if (does_match == -1)
        does_match = 0;
    catchf("-  object by  adverb:   %s\n", does_match?"OK":"FAIL");
    return does_match;
}
int fact_matches_anything_by_extra(struct fact* fact, struct request* request) {
    int does_match =
        (     fact_matches_entity_by_entity(fact->subjects, request->extra, WEAK, "fact->subjects, request->extra: %d\n") < 0
           && fact_matches_entity_by_entity(fact->objects,  request->extra, WEAK, "fact->objects,  request->extra: %d\n") < 0
           && fact_matches_entity_by_entity(fact->adverbs,  request->extra, WEAK, "fact->adverbs,  request->extra: %d\n") < 0
           && fact_matches_entity_by_entity(fact->extra,    request->extra, WEAK, "fact->extra,    request->extra: %d\n") < 0 )
        ||
           fact_matches_entity_by_entity(fact->subjects, request->extra, WEAK, "fact->subjects, request->extra: %d\n") > 0
        || fact_matches_entity_by_entity(fact->objects,  request->extra, WEAK, "fact->objects,  request->extra: %d\n") > 0
        || fact_matches_entity_by_entity(fact->adverbs,  request->extra, WEAK, "fact->adverbs,  request->extra: %d\n") > 0
        || fact_matches_entity_by_entity(fact->extra,    request->extra, WEAK, "fact->extra,    request->extra: %d\n") > 0
    ;
    // TODO: -1, see above
    catchf("- anything by extra:  %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_q_what_weakly(struct fact* fact, struct request* request) {
    int does_match = 1;

    catchf("-g q_what_weakly: %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_q_what_extra(struct fact* fact, struct request* request) {
    int does_match = 1;

    int i;
    if (can_be_a_pointer(fact->subjects)) {
        for (i = 0; can_be_a_pointer(fact->subjects[i]) && can_be_a_pointer(fact->subjects[i]->name); ++i) {
            if (0 == strcmp(fact->subjects[i]->name, "ein") || 0 == strcmp(fact->subjects[i]->name, "eine")) {
                does_match = 1;
            }
        }
    }
    if (can_be_a_pointer(fact->objects)) {
        for (i = 0; can_be_a_pointer(fact->objects[i]) && can_be_a_pointer(fact->objects[i]->name); ++i) {
            if (0 == strcmp(fact->objects[i]->name, "ein") || 0 == strcmp(fact->objects[i]->name, "eine")) {
                does_match = 1;
            }
        }
    }

    catchf("- q_what_extra: %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_q_what_exactly(struct fact* fact, struct request* request) {
    int does_match = 0;

    int i;
    if (can_be_a_pointer(fact->subjects)) {
        for (i = 0; can_be_a_pointer(fact->subjects[i]) && can_be_a_pointer(fact->subjects[i]->name); ++i) {
            if (0 == strcmp(fact->subjects[i]->name, "ein") || 0 == strcmp(fact->subjects[i]->name, "eine") || 0 == strcmp(fact->subjects[i]->name, "(a)")) {
                does_match = 1;
            }
        }
    }
    if (can_be_a_pointer(fact->objects)) {
        for (i = 0; can_be_a_pointer(fact->objects[i]) && can_be_a_pointer(fact->objects[i]->name); ++i) {
            if (0 == strcmp(fact->objects[i]->name, "ein") || 0 == strcmp(fact->objects[i]->name, "eine") || 0 == strcmp(fact->objects[i]->name, "(a)")) {
                does_match = 1;
            }
        }
    }

    catchf("- q_what_exactly: %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_q_who(struct fact* fact, struct request* request) {
    int does_match = 1;

    int i;
    if (can_be_a_pointer(fact->subjects)) {
        for (i = 0; can_be_a_pointer(fact->subjects[i]) && can_be_a_pointer(fact->subjects[i]->name); ++i) {
            if (0 == strcmp(fact->subjects[i]->name, "ein") || 0 == strcmp(fact->subjects[i]->name, "eine")) {
                does_match = 0;
            }
        }
    }
    if (can_be_a_pointer(fact->objects)) {
        for (i = 0; can_be_a_pointer(fact->objects[i]) && can_be_a_pointer(fact->objects[i]->name); ++i) {
            catchf("  - q_who: %s\n", fact->objects[i]->name);
            if (0 == strcmp(fact->objects[i]->name, "ein") || 0 == strcmp(fact->objects[i]->name, "eine")) {
                does_match = 0;
            }
        }
    }

    catchf("- q_who: %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_q_where(struct fact* fact, struct request* request) {
    int does_match = 0;

    int verb_is_be = 0;
    if (can_be_a_pointer(request->verbs) && can_be_a_pointer(request->verbs[0]) && can_be_a_pointer(request->verbs[0]->name)) {
        if (strstr(request->verbs[0]->name, "|ist|")) {
            verb_is_be = 1;
        }
    }

    int i;
    if (can_be_a_pointer(fact->adverbs)) {
        for (i = 0; can_be_a_pointer(fact->adverbs[i]) && can_be_a_pointer(fact->adverbs[i]->name); ++i) {
            catchf("  - adverb: %s\n", fact->adverbs[i]->name);
            if (    0 == strcmp(fact->adverbs[i]->name, "in")
                 || 0 == strcmp(fact->adverbs[i]->name, "an")
                 || 0 == strcmp(fact->adverbs[i]->name, "from")
                 || 0 == strcmp(fact->adverbs[i]->name, "at")
                 || 0 == strcmp(fact->adverbs[i]->name, "auf")
                 || 0 == strcmp(fact->adverbs[i]->name, "aus")
                 || (!verb_is_be && 0 == strcmp(fact->adverbs[i]->name, "von"))
            ) {
                does_match = 1;
            }
        }
    }

    if (can_be_a_pointer(fact->verbs)) {
        for (i = 0; can_be_a_pointer(fact->verbs[i]) && can_be_a_pointer(fact->verbs[i]->name); ++i) {
            printf("  - verb: %s\n", fact->verbs[i]->name);
            if (fact->verbs[i]->name == strstr(fact->verbs[i]->name, "is-part")) {
                does_match = 1;
            }
        }
    }

    catchf("- q_where: %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_q_how(struct fact* fact, struct request* request) {
    int does_match = 1;

    int verb_is_be = 0;
    if (can_be_a_pointer(request->verbs) && can_be_a_pointer(request->verbs[0]) && can_be_a_pointer(request->verbs[0]->name)) {
        if (strstr(request->verbs[0]->name, "|ist|")) {
            verb_is_be = 1;
        }
    }

    if (verb_is_be) {
        return 1;
    }

    int i;
    if (can_be_a_pointer(fact->adverbs)) {
        for (i = 0; can_be_a_pointer(fact->adverbs[i]) && can_be_a_pointer(fact->adverbs[i]->name); ++i) {

            if (    0 == strcmp(fact->adverbs[i]->name, "in")
                 || 0 == strcmp(fact->adverbs[i]->name, "an")
                 || 0 == strcmp(fact->adverbs[i]->name, "from")
                 || 0 == strcmp(fact->adverbs[i]->name, "at")
                 || 0 == strcmp(fact->adverbs[i]->name, "auf")
                 || 0 == strcmp(fact->adverbs[i]->name, "aus")
                 || 0 == strcmp(fact->adverbs[i]->name, "von")
            ) {
                does_match = 0;
            }
        }
    }

    catchf("- q_how: %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_q_from_where(struct fact* fact, struct request* request) {
    int does_match = 0;

    int i;
    if (can_be_a_pointer(fact->adverbs)) {
        for (i = 0; can_be_a_pointer(fact->adverbs[i]) && can_be_a_pointer(fact->adverbs[i]->name); ++i) {

            if (    0 == strcmp(fact->adverbs[i]->name, "aus")
                 || 0 == strcmp(fact->adverbs[i]->name, "von")
                 || 0 == strcmp(fact->adverbs[i]->name, "from")
                 || 0 == strcmp(fact->adverbs[i]->name, "durch")
                 || 0 == strcmp(fact->adverbs[i]->name, "von")
            ) {
                does_match = 1;
            }
        }
    }

    catchf("- q_from_where: %s\n", does_match?"OK":"FAIL");
    return does_match;
}

int fact_matches_questionword_rules_of_search_reasons(struct fact* fact, struct request* request) {
    int does_match = 0;

    int i;
    if (can_be_a_pointer(fact->subjects)) {
        for (i = 0; can_be_a_pointer(fact->subjects[i]) && can_be_a_pointer(fact->subjects[i]->name); ++i) {
            if (fact->subjects[i]->name[0] >= 'a' && fact->subjects[i]->name[0] <= 'g') {
                does_match = 1;
            }
        }
    }
    if (can_be_a_pointer(fact->objects)) {
        for (i = 0; can_be_a_pointer(fact->objects[i]) && can_be_a_pointer(fact->objects[i]->name); ++i) {
            if (fact->objects[i]->name[0] >= 'a' && fact->objects[i]->name[0] <= 'g') {
                does_match = 1;
            }
        }
    }

    return does_match;
}

int fact_matches_questionword_rules(struct fact* fact, struct request* request) {

    if (can_be_a_pointer(request->context)) {
        if (0 == strcmp(request->context, "q_what_weakly"))
            return fact_matches_questionword_rules_of_q_what_weakly(fact, request);
        if (0 == strcmp(request->context, "q_what_nowiki"))
            return fact_matches_questionword_rules_of_q_what_weakly(fact, request);
        if (0 == strcmp(request->context, "q_what_exactly"))
            return fact_matches_questionword_rules_of_q_what_exactly(fact, request);
        if (0 == strcmp(request->context, "q_what_extra"))
            return fact_matches_questionword_rules_of_q_what_extra(fact, request);
        if (0 == strcmp(request->context, "q_who"))
            return fact_matches_questionword_rules_of_q_who(fact, request);
        if (0 == strcmp(request->context, "q_where"))
            return fact_matches_questionword_rules_of_q_where(fact, request);
        if (0 == strcmp(request->context, "q_how"))
            return fact_matches_questionword_rules_of_q_how(fact, request);
        if (0 == strcmp(request->context, "q_from_where"))
            return fact_matches_questionword_rules_of_q_from_where(fact, request);
        if (0 == strcmp(request->context, "search_reasons"))
            return fact_matches_questionword_rules_of_search_reasons(fact, request);
    }

    return 1;
}
int fact_matches_verb(struct fact* fact, struct request* request) {
    if (!request->verbs[0]) {
        return 1;
    }
    int does_match = 1;
    //debugf("verb 0: %s\n", request->verbs[0]                                                                                  ? request->verbs[0]->name : "(null)");
    //debugf("verb 1: %s\n", request->verbs[0] && request->verbs[1]                                                       ? request->verbs[1]->name : "(null)");
    //debugf("verb 2: %s\n", request->verbs[0] && request->verbs[1] && request->verbs[2]                            ? request->verbs[2]->name : "(null)");
    //debugf("verb 3: %s\n", request->verbs[0] && request->verbs[1] && request->verbs[2] && request->verbs[3] ? request->verbs[3]->name : "(null)");

    int v;
    for (v = 0; request->verbs[v] && request->verbs[v]->name; ++v) {
        char** splitted_by_pipe = divide_by('|', request->verbs[v]->name);

        int does_match_here = 0;
        int p;
        for (p = 0; splitted_by_pipe[p]; ++p) {
            int m;
            for (m = 0; fact->verbs[m] && fact->verbs[m]->name; ++m) {
                //debugf("Does %s match %s?\n", fact->verbs[m]->name, splitted_by_pipe[p]);
                if (0 == strcmp(fact->verbs[m]->name, splitted_by_pipe[p]) || fact->verbs[m]->name == strstr(fact->verbs[m]->name, splitted_by_pipe[p])) { //(!strstr(fact->verbs[m]->name, " ") &&
                    does_match_here = 1;
                }
            }
        }

        /*if (v && does_match) {
            // don't change
        }
        if (v && !does_match && does_match_here) {
            does_match = does_match_here;
        }
        else {
            does_match = does_match_here;
        }*/         // uncommented: 04.02.2010

        does_match = does_match && does_match_here;

        for (p = 0; splitted_by_pipe[p]; ++p) {
            free(splitted_by_pipe[p]);
        }
        free(splitted_by_pipe);
    }

    return does_match;
}

struct fact* filter_fact_by_rules(struct fact* fact, struct request* request, int weak) {

    if (weak == EXACT) {
        weak = (0 == strcmp(request->context, "reasonof")) ? WEAK : EXACT;
    }

    int m;
    for (m = 0; fact->verbs[m] && fact->verbs[m]->name; ++m) {
        if (fact->verbs[m]->name == strstr(fact->verbs[m]->name, "is-property")) {
            weak = WEAK;
        }
    }

    if (!(
            fact_matches_verb                 (fact, request)
         && (
                (
                    fact_matches_subject_by_subject (fact, request, weak)
                 && (
                        fact_matches_object_by_object   (fact, request, weak)
                     || fact_matches_object_by_adverb   (fact, request, weak)
                     || fact_matches_adverb_by_object   (fact, request, weak)
                    )
                )
            ||  (
                    strcmp(request->context, "q_what_extra")
                 && fact_matches_object_by_subject (fact, request, weak)
                 && fact_matches_subject_by_object (fact, request, weak)
                )
            ||  (
                    request->context
                 && can_be_a_pointer(request->context)
                 && (0 == strcmp(request->context, "search_reasons"))
                 && fact_matches_questionword_rules_of_search_reasons(fact, request)
                )
            ||  (
                    (0 == strcmp(request->context, "just_verb"))
                )
            )
         && (
                fact_matches_adverb_by_adverb     (fact, request, weak)
             || fact_matches_adverb_by_object     (fact, request, weak)
             || fact_matches_object_by_adverb     (fact, request, weak)
            )
         && fact_matches_anything_by_extra    (fact, request)
         //&& fact_matches_questionword_rules   (fact, request)

         )) {

        set_to_invalid_fact(&fact);
    }


    return fact;
}

struct word* replace_variables_in_word(struct word* word) {
    int i;
    for (i = 0; i < variable_replacements_c; i += 2) {
        if (0==strcmp(word->name, variable_replacements[i]->name)) {
            word = variable_replacements[i+1];
        }
        else if (strstr(word->name, variable_replacements[i]->name)) {
            printf("word->name: %s\n", word->name);
            printf("variable_replacements[i]->name: %s\n", variable_replacements[i]->name);
            char* o_name = strdup(word->name);
            char* varpos = strstr(o_name, variable_replacements[i]->name);
            if (varpos >= 0) {
                varpos[0] = '\0';
                char* rest = varpos+strlen(variable_replacements[i]->name);
                char* new_name = calloc(1, strlen(variable_replacements[i]->name)+strlen(word->name)+3);
                snprintf(new_name, strlen(variable_replacements[i]->name)+strlen(word->name)+2, "%s%s%s", o_name, variable_replacements[i+1]->name, rest);
                word = set_word(new_name);
                free(new_name);
            }
            free(o_name);
            printf("word->name: %s\n\n", word->name);
        }
    }
    return word;
}
struct word** replace_variables_in_words(struct word** list) {
    int b;
    for (b = 0; list[b]; ++b) {
        list[b] = replace_variables_in_word(list[b]);
    }
    return list;
}
struct fact* replace_variables_in_fact(struct fact* fact) {
    fact->subjects = replace_variables_in_words(fact->subjects);
    fact->objects  = replace_variables_in_words(fact->objects);
    fact->adverbs  = replace_variables_in_words(fact->adverbs);
    return fact;
}
struct fact** replace_variables_in_list(struct fact** list) {
    int b;
    for (b = 0; list[b]; ++b) {

        if (list[b] != INVALID_POINTER) {
            list[b] = replace_variables_in_fact(list[b]);
        }
    }
    return list;
}


struct fact** filter_list_by_rules(struct fact** list, struct request* request, int weak) {
    printf("Starting fact filter!\n");

    if (list == TOOMUCH_P) {
        printf("Do not filter, too much data.\n");
        printf("Not filtered.\n");
        return TOOMUCH_P;
    }

    begin_catchf();
    clear_catchf();
    const char* catched = 0;
    static char last_catched[20480];
    strcpy(last_catched, "");
    short last_failed = -1;
    int lc, b, count_of_facts, count_of_true_facts, count_of_context_matching_facts;
    for (lc = 0, b = 0, count_of_facts = 0, count_of_true_facts = 0, count_of_context_matching_facts = 0; list[b]; ++b) {

        if (list[b] != INVALID_POINTER) {
            list[b] = filter_fact_by_rules(list[b], request, weak);
        }
        if (list[b] != INVALID_POINTER) {
            ++count_of_facts;
        }
        if (list[b] != INVALID_POINTER) {
            if (fact_matches_truth(list[b], request)) {
                ++count_of_true_facts;
            }
        }
        if (list[b] != INVALID_POINTER) {
            if (fact_matches_questionword_rules(list[b], request)) {
                ++count_of_context_matching_facts;
            }
        }

#define ALSO_FAILED "... and all others also failed ...\n"
#define ALSO_OK "... and all others also passed ...\n"

        catched = get_catchf();

        short failed = ( !can_be_a_pointer(list[b]) ) ? 1 : 0; // || strstr(catched, "FAIL")

        if (catched && strlen(catched)) {
            if (!strlen(last_catched)) {
                strcpy(last_catched, catched);
            }
            else if (failed && last_failed == failed) {
                if (strlen(catched) > strlen(last_catched)) {
                    strcpy(last_catched, catched);
                    strcat(last_catched, ALSO_FAILED);
                }
                last_failed = failed;
                clear_catchf();
            }
            else if (!failed && last_failed == failed) {
                if (strlen(catched) > strlen(last_catched)) {
                    strcpy(last_catched, catched);
                    strcat(last_catched, ALSO_OK);
                }
                clear_catchf();
            }
            else if (catched && strlen(catched)) {
                if (lc+1 < b) {
                    if (DEBUG__LONG_LISTINGS) printf("Filtering fact %i to %i.\n\n", lc+1, b);
                }
                else {
                    if (DEBUG__LONG_LISTINGS) printf("Filtering fact %i.\n\n", b);
                }

                if (!last_catched || !strlen(last_catched)) {
                    strcpy(last_catched, "... no output ...");
                }
                if (DEBUG__LONG_LISTINGS) printf("%s\n", last_catched);
                strcpy(last_catched, catched);
                last_failed = failed;
                if (last_failed) {
                    strcat(last_catched, ALSO_FAILED);
                }
                else {
                    strcat(last_catched, ALSO_OK);
                }
                clear_catchf();
                lc = b;
            }
        }
    }
    {
        if (lc+1 < b) {
            --b;
            if (DEBUG__LONG_LISTINGS) printf("Filtering fact %i to %i.\n\n", lc+1, b);
        }

        if (DEBUG__LONG_LISTINGS) printf("%s\n", last_catched);
        clear_catchf();
        lc = b;
    }
    end_catchf();

    if (count_of_true_facts >= 1 && count_of_facts >= 10 && (count_of_facts - count_of_true_facts) > (count_of_true_facts/2) ) {
        for (b = 0; list[b]; ++b) {
            if (list[b] != INVALID_POINTER) {
                if (!fact_matches_truth(list[b], request)) {
                    set_to_invalid_fact(&(list[b]));
                }
            }
        }
    }

    if (count_of_context_matching_facts >= 1) {
        for (b = 0; list[b]; ++b) {
            if (list[b] != INVALID_POINTER) {
                if (!fact_matches_questionword_rules(list[b], request)) {
                    set_to_invalid_fact(&(list[b]));
                }
            }
        }
    }

    printf("Filtered %d (true: %d, in context: %d) facts.\n", count_of_facts, count_of_true_facts, count_of_context_matching_facts);

    return list;
}

int universal_delete_everything_from(const char* filename) {
    if (is_engine("disk")) {
        return disk_delete_everything_from(filename);
    }
    return -1;
}

int universal_add_filename(const char* filename) {
    if (is_engine("disk")) {
        return disk_add_filename(filename);
    }
    return -1;
}

int search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position) {
    if (is_engine("ram")) {
        return ram_search_facts_for_words_in_net(words, facts, limit, position);
    }
    else {
        return disk_search_facts_for_words_in_net(words, facts, limit, position);
    }
}

const int COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART = 9;

void flowchart_write_list_of_synonym(struct word*** words) {
    FILE* logfile = fopen("flowchart.log", "a");
    if (!logfile) {
        return;
    }

    fprintf(logfile, "textcontent 000000 ");

    int count = 0;
    short first = 1;
    int n;
    for (n = 0; words[n]; ++n) {
        if (!can_be_a_pointer(words[n])) {
            continue;
        }

        int is_new = 1;
        if (can_be_a_pointer(words[n][0])) {
            int q;
            for (q = 0; words[q] && q+1 < n; ++q) {
                if (can_be_a_pointer(words[q][0]) && words[n][0] == words[q][0])
                    is_new = 0;
            }
        }
        if (!is_new) {
            continue;
        }

        int m;
        for (m = 0; words[n][m]; ++m) {
            if (!(can_be_a_pointer(words[n][m]) && words[n][m]->name && words[n][m]->name[0])) {
                continue;
            }
            if (is_a_trivial_word(words[n][m]->name)) {
                continue;
            }
            if (!can_be_a_synonym(words[n][m]->name)) {
                continue;
            }
            if (is_bad(words[n][m]->name)) {
                continue;
            }

            if (!first && !(count % COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART == 0)) {
                fprintf(logfile, ", ");
            }
            fprintf(logfile, "\"%s\"", words[n][m]->name);

            ++count;
            first = 0;

            if (count % COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART == 0) {
                fprintf(logfile, "\ntextcontent 000000 ");
            }
        }
    }
    fprintf(logfile, "\n");
    fclose(logfile);
}

int count_of_synonym(struct word*** words) {
    int count = 0;
    short first = 1;
    int n;
    for (n = 0; words[n]; ++n) {
        if (!can_be_a_pointer(words[n])) {
            continue;
        }

        int is_new = 1;
        if (can_be_a_pointer(words[n][0])) {
            int q;
            for (q = 0; words[q] && q+1 < n; ++q) {
                if (can_be_a_pointer(words[q][0]) && words[n][0] == words[q][0])
                    is_new = 0;
            }
        }
        if (!is_new) {
            continue;
        }

        int m;
        for (m = 0; words[n][m]; ++m) {
            if (!(can_be_a_pointer(words[n][m]) && words[n][m]->name && words[n][m]->name[0])) {
                continue;
            }
            if (is_a_trivial_word(words[n][m]->name)) {
                continue;
            }
            if (!can_be_a_synonym(words[n][m]->name)) {
                continue;
            }
            if (is_bad(words[n][m]->name)) {
                continue;
            }

            ++count;
            first = 0;
        }
    }

    return count;
}

int higher(int a, int b) {
    return (a > b ? a : b);
}

int divide_and_round_up(int a, int b) {
    double c = a;
    c /= b;
    return ((int)ceil(c));
}

int net_supports_way(const char* way, int value) {
    if (is_engine("disk")) {
        if (way && strstr(way, "has_conditional_questionword")) {
            if (value == 1) {
                return 1;
            }
        }
    }

    return 0;
}

int net_go_way(const char* way, int value, struct request* fact, struct fact** facts, int limit, int* position) {
    if (is_engine("disk")) {
        if (way && strstr(way, "has_conditional_questionword")) {
            if (value == 1) {
                return disk_net_go_way_conditional_questionword(fact, facts, limit, position);
            }
        }
    }

    return 0;
}

struct fact** search_in_net(struct request* fact, struct fact** list, const char* hints) {
    int limit = 10000;
    char* _config_value = check_config("limit-amount-of-answers", "1");
    if (strcmp("1", _config_value)) {
        limit = 100000;
    }
    free(_config_value);

    struct fact** facts = 0;
    if (list) {
        facts = list;
    }
    else {
        facts = calloc(1, sizeof(struct fact*)*(limit+1));
    }


    int position = 0;


    if (0 == strcmp(fact->context, "double_facts")) {
        if (is_engine("disk")) {
            int succ_1 = disk_search_double_facts(fact->subjects, facts, limit, &position);
            if (succ_1 == TOOMUCH) {
                printf("Too much double facts.\n");
                free(facts);
                return TOOMUCH_P;
            }
            printf("Found double facts.\n");
        }
        return facts;
    }

    int flowchart_lines = 1;
    int succ_1, succ_2, succ_3, succ_4, succ_5;

    if (net_supports_way("has_conditional_questionword", fact->has_conditional_questionword)) {
        net_go_way("has_conditional_questionword", fact->has_conditional_questionword, fact, facts, limit, &position);
    }
    else {
        succ_1 = search_facts_for_words_in_net(fact->subjects, facts, limit, &position);
        succ_2 = search_facts_for_words_in_net(fact->objects,  facts, limit, &position);
        succ_3 = search_facts_for_words_in_net(fact->adverbs,  facts, limit, &position);
        succ_4 = 0;
        succ_5 = 0;

        flowchart_lines += higher(count_of_synonym(fact->subjects), COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART);
        flowchart_lines += higher(count_of_synonym(fact->objects), COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART);
        flowchart_lines += higher(count_of_synonym(fact->adverbs), COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART);

        if (0 == strcmp(fact->context, "search_reasons") || !(fact->subjects[0] && fact->subjects[1] && fact->subjects[2])) {
            if (!strstr(hints, "no-extra")) {
                succ_4 = search_facts_for_words_in_net(fact->extra,    facts, limit, &position);
                flowchart_lines += higher(count_of_synonym(fact->extra), COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART);
            }
        }

        if (fact->verbs && fact->verbs[0] && fact->verbs[0]->name && 0 == strcmp(fact->verbs[0]->name, ">>>")) {
            struct word*** verbs = calloc(sizeof(struct word***), 2);
            verbs[0] = fact->verbs;
            verbs[1] = 0;

            succ_5 = search_facts_for_words_in_net(verbs, facts, limit, &position);

            free(verbs);
        }

        flowchart_lines = divide_and_round_up(flowchart_lines, COUNT_OF_SYNONYMS_PER_LINE_IN_FLOWCHART);

    }

    if (!fact->verbs || !fact->verbs[0] || fact->verbs[0]->name || strcmp(fact->verbs[0]->name, ">>>")) {
        {
            FILE* logfile = fopen("flowchart.log", "a");
            if (logfile) {
                fprintf(logfile, "begin box\n");
                fprintf(logfile, "bckgrndcolr D0FFFF\n");
                fprintf(logfile, "bordercolor D0FFFF\n");
                fprintf(logfile, "linesoftext %d\n", flowchart_lines);
                fprintf(logfile, "draw\n");
                fprintf(logfile, "textcontent 000000 Search:\n");
                fclose(logfile);
            }
        }
        flowchart_write_list_of_synonym(fact->subjects);
        flowchart_write_list_of_synonym(fact->objects);
        flowchart_write_list_of_synonym(fact->adverbs);
        if (0 == strcmp(fact->context, "search_reasons") || !(fact->subjects[0] && fact->subjects[1] && fact->subjects[2])) {
            flowchart_write_list_of_synonym(fact->extra);
        }
        {
            FILE* logfile = fopen("flowchart.log", "a");
            if (logfile) {
                fprintf(logfile, "end box\n");
                fclose(logfile);
            }
        }
    }

    if (succ_1 == TOOMUCH || succ_2 == TOOMUCH || succ_3 == TOOMUCH || succ_4 == TOOMUCH || succ_5 == TOOMUCH) {
        printf("(%i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH) = 1\n", succ_1, succ_2, succ_3, succ_4, succ_5);
        free(facts);
        return (struct fact**)TOOMUCH;
    }
    printf("(%i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH) = 0\n", succ_1, succ_2, succ_3, succ_4, succ_5);

    return facts;
}

void print_word_list_3rd_order(struct word*** list) {
    int i;
    for (i = 0; can_be_a_pointer(list[i]); ++i) {
        debugf(" - '");
        int j;
        for (j = 0; can_be_a_pointer(list[i][j]); ++j) {
            if (can_be_a_pointer(list[i][j]->name)) {
                if (j) {
                    debugf("', '", list[i][j]->name);
                }
                debugf("%s", list[i][j]->name);
            }
        }
        //if (j == 0) {
            //debugf("   array at %p: first = %p, second = %p\n", list[i], list[i][0], can_be_a_pointer(list[i][0])&&can_be_a_pointer(list[i][1])?list[i][1]:0);
        //}
        debugf("'\n");
    }
    debugf("\n");
}

int next_in_list(void** list) {
    int i;
    for (i = 0; list[i]; ++i) {}
    return i;
}

int count_list(void** list) {
    int i, size;
    for (i = 0, size = 0; list[i] || INVALID_POINTER == list[i]; ++i) {
        if (can_be_a_pointer(list[i]) && list[i] != INVALID_POINTER) {
            ++size;
        }
    }
    return size;
}

int delete_in_first_if_second(struct word*** list, const char* exp) {
    if (!strlen(exp)) {
        return 0;
    }

    int timeout = 50;
    int found_one = 1;
    while (found_one && timeout > 0) {
        --timeout;
        found_one = 0;

        int i;
        for (i = 0; !found_one && (INVALID_POINTER == list[i] || can_be_a_pointer(list[i])); ++i) {
            if (found_one) {
                break;
            }

            int j;
            for (j = 0; !found_one && (INVALID_POINTER == list[i] || can_be_a_pointer(list[i][j])); ++j) {
                if (found_one) {
                    break;
                }

                if (can_be_a_pointer(list[i][j]->name)) {
                    if (strstr(list[i][j]->name, exp) && strlen(list[i][j]->name)-3 <= strlen(exp)) {
                        printf("Deleting synonym: %s\n", list[i][j]->name);
                        int no_more_items = 1;
                        if (j) {
                            int k;
                            for (k = j+1; INVALID_POINTER == list[i] || can_be_a_pointer(list[i][k]); ++k) {
                                list[i][k-1] = list[i][k];
                                no_more_items = 0;
                            }
                            list[i][k-1] = list[i][k];
                            if (!can_be_a_pointer(list[i][j])) {
                                list[i][j] = 0;
                            }
                        }
                        if (!j || no_more_items) {
                            no_more_items = 1;
                            int k;
                            for (k = i+1; INVALID_POINTER == list[k] || can_be_a_pointer(list[k]); ++k) {
                                list[k-1] = list[k];
                                no_more_items = 0;
                            }
                            if (k && !can_be_a_pointer(list[k-1])) {
                                list[k-1] = 0;
                            }
                            if (!can_be_a_pointer(list[i])) {
                                list[i] = 0;
                            }
                        }
                        if (no_more_items) {
                            list[i] = 0;
                        }

                        found_one = 1;
                    }
                }
            }
        }
    }
    return 0;
}

int delete_in_first_if_in_second(struct word*** delete_in, struct word*** find_in) {
    int i;
    for (i = 0; find_in[i]; ++i) {
        if (can_be_a_pointer(find_in[i])) {
            char* joined = join_words(find_in[i]);
            if (joined) {
                delete_in_first_if_second(delete_in, joined);
                free(joined);
            }
        }
    }
}

int free_request(struct request* fact) {
    if (is_engine("ram")) {
        free(fact->verbs);
    }
    else {
        free_words_weak(fact->verbs);
    }

    free(fact->questionword);
    free(fact->context);
    free(fact->subjects);
    free(fact->objects);
    free(fact->adverbs);
    free(fact->extra);
    free(fact);

    return 0;
}

struct fact** search_facts_simple(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int level, int weak) {
    struct fact** list = 0;

    config_match_variables = 0;

    {
        if (0 == strcmp(context, "reasonof")) {
            level = 1;
        }

        struct request* fact = calloc(sizeof(struct request), 1);
        fact->adverbs_any  = 0;
        fact->has_conditional_questionword = 0;
        fact->subjects     = filter_synonyms(synonym_set->subjects, level, FORWARD);
        fact->objects      = filter_synonyms(synonym_set-> objects, level, FORWARD);
        fact->adverbs      = filter_synonyms(synonym_set-> adverbs, 1,     FORWARD);
        fact->extra        = filter_synonyms(synonym_set->   extra, level, REVERSE);
        fact->verbs        =    divide_words(verbs);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;

        delete_in_first_if_second(fact->objects, subjects);
        delete_in_first_if_second(fact->subjects, extra);

        debugf(
            "Searching simple answers for:\n"
            "    subjects (was: %s):\n", subjects);
        print_word_list_3rd_order(fact->subjects);
        debugf(
            "    objects (was: %s):\n" , objects);
        print_word_list_3rd_order(fact->objects);
        debugf(
            "    verbs:\n - %s\n"
            "    adverbs:\n",
            verbs
        );
        print_word_list_3rd_order(fact->adverbs);
        debugf(
            "    extra:\n");
        print_word_list_3rd_order(fact->extra);
        debugf(
            "    questionword:\n - %s\n",
            questionword);


        list = filter_list_by_rules (
            search_in_net(fact, list, "no-extra"),
            fact,
            weak
        );

        if (list == TOOMUCH_P) {
            free_request(fact);
            return TOOMUCH_P;
        }

        list = replace_variables_in_list(list);

        if (list == TOOMUCH_P) {
            free_request(fact);
            return TOOMUCH_P;
        }

        if (!can_be_a_pointer(list) || !count_list((void**)list)) {
            if (can_be_a_pointer(list)) {
                free(list);
                list = 0;
            }

            list = filter_list_by_rules (
                search_in_net(fact, list, ""),
                fact,
                weak
            );
        }

        if (list == TOOMUCH_P) {
            free_request(fact);
            return TOOMUCH_P;
        }

        struct word*** subject_syonyms_level_0 = 0;

        int l;
        for (l = 0; can_be_a_pointer(list[l]) || INVALID_POINTER == list[l]; ++l) {
            if (can_be_a_pointer(list[l])) {
                printf("list[l]->verbs: %d\n", (int)list[l]->verbs);
                if (can_be_a_pointer(list[l]->verbs) && can_be_a_pointer(list[l]->verbs[0]) && can_be_a_pointer(list[l]->verbs[0]->name)) {
                    printf("list[l]->verbs[0]->name: %s\n", list[l]->verbs[0]->name);
                    if (is_a_fact_verb(list[l]->verbs[0]->name) && strlen(verbs)) {

                        if (!subject_syonyms_level_0) {
                            subject_syonyms_level_0 = filter_synonyms(synonym_set->subjects, 1, FORWARD);
                        }

                        if (strstr(list[l]->filename, "thes")) {
                            printf("fact %d: from thesaurus => invalid\n", l);
                            set_to_invalid_fact(&(list[l]));
                        }
                        else if (strstr(list[l]->filename, "part_of_speech")) {
                            printf("fact %d: from part_of_speech => invalid\n", l);
                            set_to_invalid_fact(&(list[l]));
                        }
                        else if (strstr(list[l]->filename, "ps-") && fact_matches_entity_by_entity(list[l]->objects, subject_syonyms_level_0, EXACT, "")) {
                            printf("fact %d: from ps-... => invalid\n", l);
                            set_to_invalid_fact(&(list[l]));
                        }
                        else if (strstr(list[l]->filename, "fa-") && fact_matches_entity_by_entity(list[l]->objects, subject_syonyms_level_0, EXACT, "")) {
                            printf("fact %d: from fa-... => invalid\n", l);
                            set_to_invalid_fact(&(list[l]));
                        }
                        else {
                            printf("fact %d: => valid\n", l);
                            if (is_a_equal_verb(list[l]->verbs[0]->name)) {
                                free(list[l]->verbs);
                                list[l]->verbs = divide_words("equal");
                            }
                        }
                    }
                }
            }

            if (can_be_a_pointer(list[l])) {
                struct fact** clauses = search_clauses(list[l]->pk);
                if (can_be_a_pointer(clauses) && can_be_a_pointer(clauses[0])) {
                    printf("pk %d, clauses!\n", list[l]->pk);
                    int c;
                    for (c = 0; can_be_a_pointer(clauses[c]); ++c) {
                        printf("pk %d, clause %d, question word %s\n", list[l]->pk, c, clauses[c]->questionword);

                        if (is_conditional_questionword(clauses[c]->questionword)) {
                            set_to_invalid_fact(&(list[l]));
                            break;
                        }
                    }
                    for (c = 0; can_be_a_pointer(clauses[c]); ++c) {
                        set_to_invalid_fact(&(clauses[c]));
                    }
                }
                if (is_engine("disk")) {
                    free(clauses);
                }
            }
        }

        free_request(fact);
    }

    return list;
}

struct fact** search_facts_double_facts(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak) {
    struct fact** list = 0;

    {
        struct request* fact = calloc(sizeof(struct request), 1);
        fact->adverbs_any  = 0;
        fact->has_conditional_questionword = 0;
        fact->subjects     = filter_synonyms(synonym_set->subjects, 0, FORWARD);
        fact->objects      = filter_synonyms(synonym_set-> objects, 0, FORWARD);
        fact->adverbs      = filter_synonyms(synonym_set-> adverbs, 0, FORWARD);
        fact->extra        = filter_synonyms(synonym_set->   extra, 0, REVERSE);
        fact->verbs        =    divide_words(verbs);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;

        delete_in_first_if_second(fact->objects, subjects);
        delete_in_first_if_second(fact->subjects, extra);

        debugf(
            "Searching double facts for:\n"
            "    subjects (was: %s):\n", subjects);
        print_word_list_3rd_order(fact->subjects);
        debugf(
            "    objects (was: %s):\n" , objects);
        print_word_list_3rd_order(fact->objects);
        debugf(
            "    verbs:\n - %s\n"
            "    adverbs:\n",
            verbs
        );
        print_word_list_3rd_order(fact->adverbs);
        debugf(
            "    extra:\n");
        print_word_list_3rd_order(fact->extra);
        debugf(
            "    questionword:\n - %s\n",
            questionword);

        list = filter_list_by_rules (
            search_in_net(fact, list, ""),
            fact,
            weak
        );

        free_request(fact);
    }

    return list;
}

struct word*** fake_synonyms(const char* exp, const char** exp_array) {
    int k = 1;
    while (exp_array && exp_array[k] && k < max_synonym_count) {
        ++k;
    }

    struct word*** synonyms = calloc(sizeof(struct word**), k+1);

    if (!exp_array) {
        exp = exp && exp[0] && exp[0] != '0' && exp[0] != ' ' ? exp : "";
        if (strlen(exp)) {
            synonyms[0] = divide_words(exp);
        }
    }
    else {
        exp = exp_array[0];
        exp = exp && exp[0] && exp[0] != '0' && exp[0] != ' ' ? exp : "";
        synonyms[0] = divide_words(exp);

        int i = 1;
        while (exp_array[i]) {
            exp = exp_array[i];
            exp = exp && exp[0] && exp[0] != '0' && exp[0] != ' ' ? exp : "";
            synonyms[i] = divide_words(exp);
            ++i;
        }
    }

    return synonyms;
}

struct fact** search_facts_synonyms(const char* subjects, const char** subjects_array, const char* objects, const char** objects_array, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    struct fact** list = 0;

    config_match_variables = 0;

    {
        struct request* fact = calloc(sizeof(struct request), 1);

        fact->adverbs_any = 1;
        fact->has_conditional_questionword = 0;

        fact->subjects     = fake_synonyms(subjects, subjects_array);
        fact->objects      = fake_synonyms(objects, objects_array);
        fact->verbs        =    divide_words(verbs);
        fact->adverbs      = fake_synonyms(adverbs, 0);
        fact->extra        = fake_synonyms(extra, 0);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;

        delete_in_first_if_second(fact->objects, subjects);
        delete_in_first_if_second(fact->subjects, extra);

        debugf(
            "Searching synonym-answers for:\n"
            "    subjects (was: %s):\n", subjects);
        print_word_list_3rd_order(fact->subjects);
        debugf(
            "    objects (was: %s):\n" , objects);
        print_word_list_3rd_order(fact->objects);
        debugf(
            "    verbs:\n - %s\n\n"
            "    adverbs:\n",
            verbs
        );
        print_word_list_3rd_order(fact->adverbs);
        debugf(
            "    extra:\n");
        print_word_list_3rd_order(fact->extra);
        debugf(
            "    questionword:\n - %s\n",
            questionword);

        list = filter_list_by_rules (
            search_in_net(fact, list, ""),
            fact,
            VERY_EXACT
        );

        if (list != TOOMUCH_P) {
            struct word*** subject_syonyms_level_0 = 0;

            int l;
            for (l = 0; can_be_a_pointer(list[l]) || INVALID_POINTER == list[l]; ++l) {
                if (can_be_a_pointer(list[l])) {
                    if (can_be_a_pointer(list[l]->verbs) && can_be_a_pointer(list[l]->verbs[0]) && can_be_a_pointer(list[l]->verbs[0]->name)) {
                        if (can_be_a_pointer(list[l]->objects) && can_be_a_pointer(list[l]->objects[0]) && can_be_a_pointer(list[l]->objects[0]->name)) {
                            //printf("verb:     %s\n", list[l]->verbs[0]->name);
                            //printf("subjects: %s\n", list[l]->subjects[0]->name);
                            //printf("objects:  %s\n", list[l]->objects[0]->name);
                            if (strcmp(list[l]->verbs[0]->name, "is-a00000") && strcmp(list[l]->verbs[0]->name, "=00000")) {
                                set_to_invalid_fact(&(list[l]));
                            }
                        }
                    }
                }
            }
        }

        free_request(fact);
    }

    return list;
}


struct request* negotiate_deep_search(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, const struct fact* if_clause, const struct fact* then_clause, int level, int direction) {
    struct request* req = calloc(sizeof(struct request), 1);
    req->has_conditional_questionword = 0;

    const char* then_clause_has_variable_subjects = 0;
    const char* then_clause_has_variable_objects  = 0;
    const char* if_clause_has_variable_subjects   = 0;
    const char* if_clause_has_variable_objects    = 0;

    int i;
    if (can_be_a_pointer(then_clause->subjects)) {
        for (i = 0; can_be_a_pointer(then_clause->subjects[i]) && can_be_a_pointer(then_clause->subjects[i]->name); ++i) {
            if (can_be_a_pointer(then_clause->subjects[i]) && is_a_variable(then_clause->subjects[i]->name)) {
                then_clause_has_variable_subjects = then_clause->subjects[i]->name;
            }
        }
    }

    if (can_be_a_pointer(then_clause->objects)) {
        for (i = 0; can_be_a_pointer(then_clause->objects[i]) && can_be_a_pointer(then_clause->objects[i]->name); ++i) {
            if (can_be_a_pointer(then_clause->objects[i]) && is_a_variable(then_clause->objects[i]->name)) {
                then_clause_has_variable_objects  = then_clause->objects[i]->name;
            }
        }
    }

    if (can_be_a_pointer(if_clause->subjects)) {
        for (i = 0; can_be_a_pointer(if_clause->subjects[i]) && can_be_a_pointer(if_clause->subjects[i]->name); ++i) {
            if (can_be_a_pointer(if_clause->subjects[i]) && is_a_variable(if_clause->subjects[i]->name)) {
                if_clause_has_variable_subjects   = if_clause->subjects[i]->name;
            }
        }
    }

    if (can_be_a_pointer(if_clause->objects)) {
        for (i = 0; can_be_a_pointer(if_clause->objects[i]) && can_be_a_pointer(if_clause->objects[i]->name); ++i) {
            if (can_be_a_pointer(if_clause->objects[i]) && is_a_variable(if_clause->objects[i]->name)) {
                if_clause_has_variable_objects    = if_clause->objects[i]->name;
            }
        }
    }

    int then_clause_subjects_match_if_clause_subjects = 0;
    int then_clause_subjects_match_if_clause_objects = 0;
    int then_clause_objects_match_if_clause_subjects = 0;
    int then_clause_objects_match_if_clause_objects = 0;

    for (i = 0; can_be_a_pointer(then_clause->subjects[i]) && can_be_a_pointer(then_clause->subjects[i]->name); ++i) {
    }
    --i;
    if (i >= 0) {
        int i_then = i;

        if (can_be_a_pointer(if_clause->subjects)) {
            for (i = 0; can_be_a_pointer(if_clause->subjects[i]) && can_be_a_pointer(if_clause->subjects[i]->name); ++i) {
            }
            --i;
            if (i >= 0) {
                int i_if = i;

                if (strlen(then_clause->subjects[i_then]->name) > 1 && strlen(if_clause->subjects[i_if]->name) > 1) {
                    if (matches_with_pipe(then_clause->subjects[i_then]->name, if_clause->subjects[i_if]->name)) {
                        then_clause_subjects_match_if_clause_subjects = 1;
                    }
                }
            }
        }

        if (can_be_a_pointer(if_clause->objects)) {
            for (i = 0; can_be_a_pointer(if_clause->objects[i]) && can_be_a_pointer(if_clause->objects[i]->name); ++i) {
            }
            --i;
            if (i >= 0) {
                int i_if = i;

                if (strlen(then_clause->subjects[i_then]->name) > 1 && strlen(if_clause->objects[i_if]->name) > 1) {
                    if (matches_with_pipe(then_clause->subjects[i_then]->name, if_clause->objects[i_if]->name)) {
                        then_clause_subjects_match_if_clause_objects = 1;
                    }
                }
            }
        }
    }

    for (i = 0; can_be_a_pointer(then_clause->objects[i]) && can_be_a_pointer(then_clause->objects[i]->name); ++i) {
    }
    --i;
    if (i >= 0) {
        int i_then = i;

        if (can_be_a_pointer(if_clause->subjects)) {
            for (i = 0; can_be_a_pointer(if_clause->subjects[i]) && can_be_a_pointer(if_clause->subjects[i]->name); ++i) {
            }
            --i;
            if (i >= 0) {
                int i_if = i;

                if (strlen(then_clause->objects[i_then]->name) > 1 && strlen(if_clause->subjects[i_if]->name) > 1) {
                    if (matches_with_pipe(then_clause->objects[i_then]->name, if_clause->subjects[i_if]->name)) {
                        then_clause_objects_match_if_clause_subjects = 1;
                    }
                }
            }
        }

        if (can_be_a_pointer(if_clause->objects)) {
            for (i = 0; can_be_a_pointer(if_clause->objects[i]) && can_be_a_pointer(if_clause->objects[i]->name); ++i) {
            }
            --i;
            if (i >= 0) {
                int i_if = i;

                if (strlen(then_clause->objects[i_then]->name) > 1 && strlen(if_clause->objects[i_if]->name) > 1) {
                    if (matches_with_pipe(then_clause->objects[i_then]->name, if_clause->objects[i_if]->name)) {
                        then_clause_objects_match_if_clause_objects = 1;
                    }
                }
            }
        }
    }

    /// The then-clause is equivalent to the user input
    /// and if-clause to the output

    req->subjects     = 0;
    req->objects      = 0;

    printf("then_clause_has_variable_subjects: %s\n", then_clause_has_variable_subjects);
    printf("if_clause_has_variable_subjects: %s\n", if_clause_has_variable_subjects);

    if (then_clause_has_variable_subjects) {
        struct word*** thing_to_insert = filter_synonyms(synonym_set->subjects, level, FORWARD);

        if (if_clause_has_variable_subjects && 0 == req->subjects) {
            req->subjects = thing_to_insert;

            if (then_clause_objects_match_if_clause_objects) {
                struct word*** thing_to_insert = (strlen(objects) ? filter_synonyms(synonym_set->objects, level, direction) : filter_synonyms(synonym_set->extra, level, direction));
                req->objects = thing_to_insert;
            }
        }
        if (if_clause_has_variable_objects  && 0 == req->objects) {
            req->objects = thing_to_insert;

            if (then_clause_objects_match_if_clause_subjects) {
                struct word*** thing_to_insert = (strlen(objects) ? filter_synonyms(synonym_set->objects, level, direction) : filter_synonyms(synonym_set->extra, level, direction));
                req->subjects = thing_to_insert;
            }
        }
    }
    if (then_clause_has_variable_objects) {
        struct word*** thing_to_insert = (strlen(objects) ? filter_synonyms(synonym_set->objects, level, direction) : filter_synonyms(synonym_set->extra, level, direction));

        if (if_clause_has_variable_subjects && 0 == req->subjects) {
            req->subjects = thing_to_insert;

            if (then_clause_objects_match_if_clause_subjects) {
                struct word*** thing_to_insert = filter_synonyms(synonym_set->subjects, level, direction);
                req->objects = thing_to_insert;
            }
        }
        if (if_clause_has_variable_objects  && 0 == req->objects) {
            req->objects = thing_to_insert;

            if (then_clause_subjects_match_if_clause_subjects) {
                struct word*** thing_to_insert = filter_synonyms(synonym_set->subjects, level, direction);
                req->subjects = thing_to_insert;
            }
        }
    }

    if (then_clause_has_variable_objects && !if_clause_has_variable_objects) {
        if (0 == req->objects) {
            if (strcmp(then_clause_has_variable_objects, then_clause_has_variable_subjects)
             && strcmp(then_clause_has_variable_objects, if_clause_has_variable_subjects)) {

                int i;
                char temp[512];
                strcpy(temp, "");
                int size = 0;
                for (i = 0; can_be_a_pointer(if_clause->objects[i]) && can_be_a_pointer(if_clause->objects[i]->name); ++i) {
                    if (size + strlen(if_clause->objects[i]->name) < 512) {
                        if (i) strcat(temp, " ");
                        strcat(temp, if_clause->objects[i]->name);
                        size += strlen(if_clause->objects[i]->name) + 1;
                    }
                }

                struct word*** thing_to_insert = fake_synonyms(temp, 0);
                req->objects = thing_to_insert;
            }
        }
    }

    if (req->subjects == 0) {
        req->subjects = fake_synonyms("", 0);
    }
    if (req->objects  == 0) {
        req->objects  = fake_synonyms("", 0);
    }

    char* req_verbs   = join_words(if_clause->verbs);
    if (strlen(req_verbs) > 5 && (strstr(req_verbs, "0") || strstr(req_verbs, "1"))) {
        req_verbs[strlen(req_verbs)-5] = '\0';
        if (strstr(verbs, req_verbs)) {
            free(req_verbs);
            req_verbs = strdup(verbs);
        }
    }

    char* req_adverbs = join_words(if_clause->adverbs);
    char* req_extra   = join_words(if_clause->extra);
    req->verbs        = divide_words   (req_verbs);
    req->adverbs      = fake_synonyms(req_adverbs, 0);
    req->extra        = fake_synonyms(req_extra, 0);
    req->questionword = strdup         (if_clause->questionword);
    req->context      = strdup         (context);
    req->truth        = 1;

    int j;
    for (j = 0; req->verbs[j]; ++j) {
        if (req->verbs[j] && strlen(req->verbs[j]->name) >= 1) {
            char* general_verb = generalize_verb(req->verbs[j]->name);
            req->verbs[j] = set_word(general_verb);
            free(general_verb);
        }
    }

    free(req_verbs);
    free(req_adverbs);
    free(req_extra);

    return req;
}

struct fact** search_facts_deep(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak) {
    struct fact** linking_list = 0;
    struct fact**         list = 0;

    config_match_variables = 1; // match variables!

    {
        struct request* fact = calloc(sizeof(struct request), 1);
        fact->adverbs_any  = 0;

        fact->has_conditional_questionword = 1;

        fact->subjects     = filter_synonyms(synonym_set->subjects, 1, FORWARD);
        fact->objects      = filter_synonyms(synonym_set-> objects, 1, FORWARD);
        fact->adverbs      = filter_synonyms(synonym_set-> adverbs, 1, FORWARD);
        fact->extra        = filter_synonyms(synonym_set->   extra, 1, REVERSE);
        fact->verbs        =    divide_words(verbs);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;

        delete_in_first_if_second(fact->objects, subjects);
        delete_in_first_if_second(fact->subjects, extra);
        delete_in_first_if_in_second(fact->subjects, fact->objects);

        debugf(
            "Searching reasons of:\n"
            "    subjects (was: %s):\n", subjects);
        print_word_list_3rd_order(fact->subjects);
        debugf(
            "    objects (was: %s):\n" , objects);
        print_word_list_3rd_order(fact->objects);
        debugf(
            "    verbs:\n - %s\n"
            "    adverbs:\n",
            verbs
        );
        print_word_list_3rd_order(fact->adverbs);
        debugf(
            "    extra:\n");
        print_word_list_3rd_order(fact->extra);
        debugf(
            "    questionword:\n - %s\n",
            questionword);

        linking_list = filter_list_by_rules (
            search_in_net(fact, linking_list, ""),
            fact,
            weak
        );

        if (linking_list == TOOMUCH_P) {
            free_request(fact);
            return TOOMUCH_P;
        }

        free(fact->context);
        fact->context = strdup(context);

        int l;
        int loop_go_on = 1;
        for (l = 0; loop_go_on && (can_be_a_pointer(linking_list[l]) || INVALID_POINTER == linking_list[l]); ++l) {
            if (can_be_a_pointer(linking_list[l])) {

                struct fact** clauses = search_clauses(linking_list[l]->pk);
                if (can_be_a_pointer(clauses) && can_be_a_pointer(clauses[0])) {
                    printf("There is a sentence in the result list.\n");
                    printf("It has clauses.\n");

                    int c;
                    for (c = 0; can_be_a_pointer(clauses[c]); ++c) {
                        printf("Clause %d, question word %s\n", c, clauses[c]->questionword);

                        if (is_conditional_questionword(clauses[c]->questionword)) {
                            printf("There is a sentence with a conditional questionword.\n");

                            if (list) {
                                free(list);
                                list = 0;
                            }

                            struct request* req = 0;
                            int _level;
                            for (_level = 1; _level <= 3; ++_level) {
                                int _direction = (_level == 1 ? FORWARD : REVERSE);

                                if (req) {
                                    free(req);
                                    req = 0;
                                }

                                // do not match variables!
                                config_match_variables = 0;

                                req = negotiate_deep_search(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, "q_deep_search", clauses[c], linking_list[l], _level, _direction);
                                delete_in_first_if_in_second(req->subjects, req->objects);

                                debugf(
                                    "Negotiated request:\n"
                                    "    subjects\n");
                                print_word_list_3rd_order(req->subjects);
                                debugf(
                                    "    objects\n");
                                print_word_list_3rd_order(req->objects);
                                char* verbs = join_words(req->verbs);
                                debugf(
                                    "    verbs:\n - %s\n"
                                    "    adverbs:\n",
                                    verbs
                                );
                                if (verbs) free(verbs);
                                print_word_list_3rd_order(req->adverbs);
                                debugf(
                                    "    extra:\n");
                                print_word_list_3rd_order(req->extra);
                                debugf(
                                    "    questionword:\n - %s\n",
                                    questionword);

                                if (list) {
                                    free(list);
                                    list = 0;
                                }

                                list = filter_list_by_rules (
                                    search_in_net(req, list, ""),
                                    req,
                                    weak
                                );

                                if (can_be_a_pointer(list) && count_list((void**)list)) {
                                    break;
                                }
                            }

                            if (list != TOOMUCH_P) {

                                int l;
                                for (l = 0; can_be_a_pointer(list[l]) || INVALID_POINTER == list[l]; ++l) {
                                    if (can_be_a_pointer(list[l])) {
                                        // no variables
                                        if (can_be_a_pointer(list[l]->subjects) && can_be_a_pointer(list[l]->subjects[0]) && can_be_a_pointer(list[l]->subjects[0]->name) && is_a_variable(list[l]->subjects[0]->name)) {
                                            set_to_invalid_fact(&(list[l]));
                                            continue;
                                        }
                                        if (can_be_a_pointer(list[l]->objects) && can_be_a_pointer(list[l]->objects[0]) && can_be_a_pointer(list[l]->objects[0]->name) && is_a_variable(list[l]->objects[0]->name)) {
                                            set_to_invalid_fact(&(list[l]));
                                            continue;
                                        }
                                        // no thesaurus, ps- and fa-facts
                                        if (can_be_a_pointer(list[l]->verbs) && can_be_a_pointer(list[l]->verbs[0]) && can_be_a_pointer(list[l]->verbs[0]->name)) {
                                            if (is_a_fact_verb(list[l]->verbs[0]->name)) {
                                                if (strstr(list[l]->filename, "thes")) {
                                                    set_to_invalid_fact(&(list[l]));
                                                }
                                                else if (strstr(list[l]->filename, "ps-") && fact_matches_entity_by_entity(list[l]->objects, fact->subjects, EXACT, "")) {
                                                    set_to_invalid_fact(&(list[l]));
                                                }
                                                else if (strstr(list[l]->filename, "fa-") && fact_matches_entity_by_entity(list[l]->objects, fact->subjects, EXACT, "")) {
                                                    set_to_invalid_fact(&(list[l]));
                                                }
                                                else {
                                                    if (is_a_equal_verb(list[l]->verbs[0]->name)) {
                                                        free(list[l]->verbs);
                                                        list[l]->verbs = divide_words("equal");
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if (can_be_a_pointer(list[l])) {
                                        struct fact** clauses = search_clauses(list[l]->pk);
                                        if (can_be_a_pointer(clauses) && can_be_a_pointer(clauses[0])) {
                                            printf("pk %d, clauses!\n", list[l]->pk);
                                            int c;
                                            for (c = 0; can_be_a_pointer(clauses[c]); ++c) {
                                                printf("pk %d, clause %d, question word %s\n", list[l]->pk, c, clauses[c]->questionword);

                                                if (is_conditional_questionword(clauses[c]->questionword)) {
                                                    set_to_invalid_fact(&(list[l]));
                                                    break;
                                                }
                                            }
                                            for (c = 0; can_be_a_pointer(clauses[c]); ++c) {
                                                set_to_invalid_fact(&(clauses[c]));
                                            }
                                        }
                                        if (is_engine("disk")) {
                                            free(clauses);
                                        }
                                    }
                                }

                            }

                            if (req) {
                                free(req);
                                req = 0;
                            }

                            if (can_be_a_pointer(list) && count_list((void**)list)) {
                                loop_go_on = 0;
                                break;
                            }
                        }
                    }
                }

                if (is_engine("disk")) {
                    free(clauses);
                }
            }
        }

        free_request(fact);

        if (is_engine("ram")) {
            free(linking_list);
        }
        else {
            free_facts_deep(linking_list);
        }
    }

    return list;
}

struct fact** search_facts_thesaurus(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak) {
    struct fact** list = 0;

    config_match_variables = 0;

    {
        if (strstr(context, "where")) {
            verbs = "";
        }

        struct request* fact = calloc(sizeof(struct request), 1);
        fact->adverbs_any  = 0;
        fact->has_conditional_questionword = 0;
        fact->subjects     = filter_synonyms(synonym_set->subjects, 1, FORWARD);
        fact->objects      = filter_synonyms(synonym_set-> objects, 1, FORWARD);
        fact->adverbs      = filter_synonyms(synonym_set-> adverbs, 1,     FORWARD);
        fact->extra        = filter_synonyms(synonym_set->   extra, 1, REVERSE);
        fact->verbs        =    divide_words(verbs);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;

        delete_in_first_if_second(fact->objects, subjects);
        delete_in_first_if_second(fact->subjects, extra);

        debugf(
            "Searching thesaurus entries for:\n"
            "    subjects (was: %s):\n", subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "");
        print_word_list_3rd_order(fact->subjects);
        debugf(
            "    objects (was: %s):\n" , objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "");
        print_word_list_3rd_order(fact->objects);
        debugf(
            "    verbs:\n - %s\n"
            "    adverbs:\n",
            verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : ""
        );
        print_word_list_3rd_order(fact->adverbs);
        debugf(
            "    extra:\n");
        print_word_list_3rd_order(fact->extra);
        debugf(
            "    questionword:\n - %s\n",
            questionword);

        list = filter_list_by_rules (
            search_in_net(fact, list, ""),
            fact,
            weak
        );

        if (list == TOOMUCH_P) {
            free_request(fact);
            return TOOMUCH_P;
        }

        int l;
        for (l = 0; can_be_a_pointer(list[l]) || list[l] == INVALID_POINTER; ++l) {
            if (can_be_a_pointer(list[l])) {
                if (can_be_a_pointer(list[l]->verbs)) {
                    if (is_a_equal_verb(list[l]->verbs[0]->name)) {
                        free(list[l]->verbs);
                        list[l]->verbs = divide_words("equal");
                    }
                }
            }
        }


        free_request(fact);
    }

    return list;
}

const char* clean_str (const char* str) {
    return (str && str[0] && str[0] != '0' && str[0] != ' ' ? str : "");
}

char** divide_by_logical_operators(char* exp) {
    char** divided = calloc(sizeof(char*), max_synonym_count+1);
    int d = 0;
    divided[d] = calloc(sizeof(char), 4096);
    
    char** splitted_exp_by_and = divide_by('&', exp);
    int p;
    for (p = 0; splitted_exp_by_and[p]; ++p) {
        printf("-----");
        char** divided_old = divided;
        divided = calloc(sizeof(char*), max_synonym_count+1);
        d = 0;
        
        char** splitted_exp_by_or = divide_by('|', splitted_exp_by_and[p]);
        int q;
        for (q = 0; splitted_exp_by_or[q]; ++q) {
            int k;
            for (k = 0; divided_old[k]; ++k) {
                divided[d] = calloc(sizeof(char), 4096);
                if (strlen(divided_old[k]) >= 1 && strlen(splitted_exp_by_or[q]) >= 1) {
                    sprintf(divided[d], "%s & %s", divided_old[k], splitted_exp_by_or[q]);
                }
                else if (strlen(divided_old[k]) >= 1) {
                    sprintf(divided[d], "%s", divided_old[k]);
                }
                else {
                    sprintf(divided[d], "%s", splitted_exp_by_or[q]);
                }
                
                while (divided[d][0] && divided[d][0] == ' ') {
                    strcpy(divided[d], divided[d]+1);
                }
                while (divided[d][0] && strlen(divided[d]) > 0 && divided[d][strlen(divided[d])-1] == ' ') {
                    divided[d][strlen(divided[d])-1] = '\0';
                }
                
                printf("-> %s\n", divided[d]);
                ++d;
            }
        }
        for (q = 0; splitted_exp_by_or[q]; ++q) {
            free(splitted_exp_by_or[q]);
        }
        free(splitted_exp_by_or);
        
        for (q = 0; divided_old[q]; ++q) {
            free(divided_old[q]);
        }
        free(divided_old);
    }
    for (p = 0; splitted_exp_by_and[p]; ++p) {
        free(splitted_exp_by_and[p]);
    }
    free(splitted_exp_by_and);
    
    return divided;
}

struct fact** search_facts(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    struct fact** list = 0;

    universal_clear_cache();

    subjects     = clean_str(    subjects);
    objects      = clean_str(     objects);
    adverbs      = clean_str(     adverbs);
    extra        = clean_str(       extra);
    verbs        = clean_str(       verbs);
    questionword = clean_str(questionword);


    struct synonym_set* synonym_set = calloc(sizeof(struct synonym_set), 1);
    synonym_set->subject_object_switched = 0;
    synonym_set->position_subjects  = 0;
    synonym_set->position_objects   = 0;
    synonym_set->position_adverbs   = 0;
    synonym_set->position_extra     = 0;
    synonym_set->subjects           = prepare_synonyms();
    synonym_set-> objects           = prepare_synonyms();
    synonym_set-> adverbs           = prepare_synonyms();
    synonym_set->   extra           = prepare_synonyms();

/*    synonym_set->subjects           = add_synonyms(subjects, 0, synonym_set->subjects, &synonym_set->position_subjects, "subjects");
    synonym_set-> objects           = add_synonyms( objects, 0, synonym_set-> objects, &synonym_set-> position_objects, "objects");
    synonym_set-> adverbs           = add_synonyms( adverbs, 0, synonym_set-> adverbs, &synonym_set-> position_adverbs, "adverbs");
    synonym_set->   extra           = add_synonyms(   extra, 0, synonym_set->   extra, &synonym_set->   position_extra, "extra");*/

    synonym_set->subjects           = add_synonyms(0, divide_by_logical_operators(subjects), synonym_set->subjects, &synonym_set->position_subjects, "subjects");
    synonym_set-> objects           = add_synonyms(0, divide_by_logical_operators( objects), synonym_set-> objects, &synonym_set-> position_objects, "objects");
    synonym_set-> adverbs           = add_synonyms(0, divide_by_logical_operators( adverbs), synonym_set-> adverbs, &synonym_set-> position_adverbs, "adverbs");
    synonym_set->   extra           = add_synonyms(0, divide_by_logical_operators(   extra), synonym_set->   extra, &synonym_set->   position_extra, "extra");


    if (!can_be_a_pointer(list) || !count_list((void**)list)) {
        list = search_facts_by_weakness(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, context, EXACT);
    }

    if (!can_be_a_pointer(list) || !count_list((void**)list)) {
        list = search_facts_by_weakness(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, context, WEAK);
    }

    free_synonym_set(synonym_set);
    return list;

    /*void* temp;
    temp                  = synonym_set-> objects;
    synonym_set-> objects = synonym_set->subjects;
    synonym_set->subjects =                  temp;
    synonym_set->subject_object_switched = 1;

    if (!can_be_a_pointer(list) || !count_list((void**)list)) {
        list = search_facts_by_weakness(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, context, WEAK);
    }

    free_synonym_set(synonym_set);
    return list;
    */
}

struct fact** search_facts_by_weakness(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak) {
    struct fact** list = 0;

    printf("\n------------------------------------------------------------------------\n\n");
    printf("Do we need the 'double facts' search?\n");
    if (0 == strcmp(context, "double_facts")) {
        if (!can_be_a_pointer(list) || !count_list((void**)list)) {
            printf("We do.\n");

            printf("'double facts' search is allowed.\n");

            struct fact** _list = search_facts_double_facts(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, context, weak);
            if (can_be_a_pointer(_list)) {
                if (can_be_a_pointer(list)) {
                    free(list);
                }
                list = _list;
            }
        }
    }
    else {
        printf("No.\n");
    }

    printf("\n------------------------------------------------------------------------\n\n");
    printf("Do we need the 'not meant' search?\n");
    if (0 == strcmp(context, "not_meant")) {
        if (!can_be_a_pointer(list) || !count_list((void**)list)) {
            printf("We do.\n");

            printf("'not meant' search is allowed.\n");

            struct fact** _list = search_facts_wiki(subjects, CONTINUE);
            if (can_be_a_pointer(_list)) {
                if (can_be_a_pointer(list)) {
                    free(list);
                }
                list = _list;
            }
        }
    }
    else {
        printf("No.\n");
    }

    printf("\n------------------------------------------------------------------------\n\n");
    printf("Do we need the simple search?\n");
    if (!can_be_a_pointer(list) || !count_list((void**)list)) {
        printf("We do.\n");

        int level = 0;

        if (0 == strcmp(context, "nosynonyms")) {
            level = 1;
        }

        while (level <= 3 && ( !can_be_a_pointer(list) || !count_list((void**)list) )) {

            struct fact** _list = search_facts_simple(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, context, level, weak);
            if (_list == TOOMUCH_P) {
                return TOOMUCH_P;
            }

            if (can_be_a_pointer(_list)) {
                if (can_be_a_pointer(list)) {
                    free(list);
                }
                list = _list;
            }
            printf("Simple Search: %d (%d), level = %d\n", count_list((void**)list), can_be_a_pointer(list), level);

            ++level;
        }
        printf("Simple Search: %d (%d)\n", count_list((void**)list), can_be_a_pointer(list));
    }
    else {
        printf("No.\n");
    }

    printf("\n------------------------------------------------------------------------\n\n");
    printf("Do we need the do_general simple search?\n");
    if ((!can_be_a_pointer(list) || !count_list((void**)list)) && (verbs && verbs[0] && verbs[0] != '0' && verbs[0] != ' ' && strstr(verbs, "|do_general"))) {
        printf("We do (we have a do_general verb).\n");

        int level = 2;

        if (0 == strcmp(context, "nosynonyms")) {
            level = 1;
        }

        while (level <= 3 && ( !can_be_a_pointer(list) || !count_list((void**)list) )) {

            struct fact** _list = search_facts_simple(synonym_set, subjects, objects, "", adverbs, extra, questionword, context, level, weak);
            if (_list == TOOMUCH_P) {
                return TOOMUCH_P;
            }

            if (can_be_a_pointer(_list)) {
                if (can_be_a_pointer(list)) {
                    free(list);
                }
                list = _list;
            }

            ++level;
        }
    }
    else {
        printf("No (there's no do_general verb).\n");
    }

    printf("\n------------------------------------------------------------------------\n\n");
    printf("Do we need the deep search?\n");

    if ((!can_be_a_pointer(list) || !count_list((void**)list)) && (0 == synonym_set->subject_object_switched)) {
        printf("We do.\n");

        struct fact** _list = search_facts_deep(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, context, weak);
        if (_list == TOOMUCH_P) {
            return TOOMUCH_P;
        }
        if (can_be_a_pointer(_list)) {
            if (can_be_a_pointer(list)) {
                free(list);
            }
            list = _list;
        }
    }
    else {
        printf("No.\n");
    }

    printf("\n------------------------------------------------------------------------\n\n");
    printf("Do we need the wiki search?\n");

    if ((!strstr(context, "what") && !strstr(context, "who")) || strstr(context, "what_prep") || strlen(adverbs) >= 3) {
        printf("No, no what-context.\n");
    }
    else if (strstr(context, "nowiki")) {
        printf("No, no-wiki-context.\n");
    }
    // && !strstr(context, "how")
    /*else if ((strstr(context, "what") || strstr(context, "who")) && !(verbs && verbs[0] && verbs[0] != '0' && verbs[0] != ' ' && is_a_fact_verb(verbs))) {
        printf("No, no what-context.\n");
    }*/
    else if (!can_be_a_pointer(list) || !count_list((void**)list)) {
        printf("We do.\n");

        if (wiki_begin()) {
            printf("Wiki search is allowed.\n");

            struct fact** _list = search_facts_wiki(subjects, NEW);
            if (can_be_a_pointer(_list)) {
                if (can_be_a_pointer(list)) {
                    free(list);
                }
                list = _list;
            }
        }
        else {
            printf("Wiki search is NOT allowed.\n");
        }
    }
    else {
        printf("No.\n");
    }

    printf("\n------------------------------------------------------------------------\n\n");
    printf("Do we need the thesaurus search?\n");

    if ((!can_be_a_pointer(list) || !count_list((void**)list)) && (is_a_fact_verb(verbs) || strstr(context, "where"))) {
        printf("We do.\n");

        struct fact** _list = search_facts_thesaurus(synonym_set, subjects, objects, verbs, adverbs, extra, questionword, context, weak);
        if (_list == TOOMUCH_P) {
            return TOOMUCH_P;
        }
        if (can_be_a_pointer(_list)) {
            if (can_be_a_pointer(list)) {
                free(list);
            }
            list = _list;
        }
    }
    else {
        printf("No.\n");
    }

    return list;
}

struct fact** search_clauses(int rel) {
    if (is_engine("ram")) {
        return ram_search_clauses(rel);
    }
    else {
        return disk_search_clauses(rel);
    }
}

int free_words_deep(struct word** words) {
    if (!can_be_a_pointer(words)) {
        words = 0;
        return 1;
    }

    int c;
    for (c = 0; words[c]; ++c) {
        if (!can_be_a_pointer(words[c])) {
            continue;
        }
        if (can_be_a_pointer(words[c])) {
            if (can_be_a_pointer(words[c]->name))
                free(words[c]->name);
            words[c]->name = 0;

            free(words[c]);
        }
        words[c] = 0;
    }
    free(words);
    words = 0;
}

int free_words_weak(struct word** words) {
    if (!can_be_a_pointer(words)) {
        words = 0;
        return 1;
    }
    free(words);
    words = 0;
}

int free_facts_deep(struct fact** facts) {
    if (!can_be_a_pointer(facts)) {
        facts = 0;
        return 1;
    }

    int c;
    for (c = 0; facts[c]; ++c) {
        if (!can_be_a_pointer(facts[c])) {
            facts[c] = 0;
            continue;
        }

        free_words_weak(facts[c]->verbs);
        free_words_weak(facts[c]->subjects);
        free_words_weak(facts[c]->objects);
        free_words_weak(facts[c]->adverbs);
        free_words_weak(facts[c]->extra);

        free(facts[c]);
        facts[c] = 0;
    }
    free(facts);
    facts = 0;

    return 0;
}

char* join_words(struct word** words) {
    return join_words_by(" ", words);
}

char* join_words_by(const char* by, struct word** words) {
    int size = 0;
    int c;
    for (c = 0; words[c]; ++c) {
        size += 3+strlen(words[c]->name);
    }

    char* string = calloc(sizeof(char), size+1);
    for (c = 0; words[c]; ++c) {
        if (c) {
            strcat(string, by);
        }
        strcat(string, words[c]->name);
    }

    return string;
}

int append_on_dataset_record(int offset, int limit, char** record, struct fact** list) {
    int column_count = offset;

    if (!list)
        return column_count;

    int d;
    for (d = 0; list[d]; ++d) {
        if (list[d] != INVALID_POINTER) {
            record[offset+(d*6)+0] = join_words(list[d]->verbs);
            record[offset+(d*6)+1] = join_words(list[d]->subjects);
            record[offset+(d*6)+2] = join_words(list[d]->objects);
            record[offset+(d*6)+3] = join_words(list[d]->adverbs);
            record[offset+(d*6)+4] = strdup(list[d]->questionword ? list[d]->questionword : "");
            record[offset+(d*6)+5] = calloc(sizeof(char), 10);
            if (list[d]->truth > 0.2 && list[d]->truth < 0.8) {
                sprintf(record[offset+(d*6)+5], "0.5");
            }
            else {
                sprintf(record[offset+(d*6)+5], "%d.0", (int)(list[d]->truth));
            }

            column_count = offset+(d*6)+6;

            printf("appended clause: %s, %s, %s, %s, %s", record[offset+(d*6)+0], record[offset+(d*6)+1], record[offset+(d*6)+2], record[offset+(d*6)+3], record[offset+(d*6)+4]);

            if (offset+(d*6)+6*2 >= limit) {
                break;
            }
        }
    }

    if (is_engine("disk")) {
        free(list);
    }

    return column_count;
}

struct DATASET as_dataset(struct fact** list) {
    struct DATASET set;
    set.size = 0;
    set.column_count = 0;
    set.data = 0;
    set.err  = 0;

    if (list == TOOMUCH_P) {
        set.err = TOOMUCH;
        return set;
    }

    if (list == 0) {
        printf("Nothing given to transform.\n");
        return set;
    }

    int size;
    for (size = 0; list[size]; ++size) {
    }

    set.size = size;
    set.column_count = 9;
    set.data = calloc(sizeof(char*), size+2);

    int d;
    for (d = 0; list[d]; ++d) {
        if (list[d] != INVALID_POINTER) {
            //char** record = calloc(sizeof(char*), sizeof(struct fact)*100);
            char** record = calloc(sizeof(char*), 100);
            record[0] = calloc(sizeof(char), 11);
            sprintf(record[0], "%d", (int)(list[d]->pk));
            record[1] = join_words(list[d]->verbs);
            record[2] = join_words(list[d]->subjects);
            record[3] = join_words(list[d]->objects);
            record[4] = join_words(list[d]->adverbs);
            record[5] = strdup("50");
            record[6] = strdup(list[d]->filename ? list[d]->filename : "");
            record[7] = strdup(list[d]->line ? list[d]->line : "");
            record[8] = calloc(sizeof(char), 10);
            if (list[d]->truth > 0.2 && list[d]->truth < 0.8) {
                sprintf(record[8], "0.5");
            }
            else {
                sprintf(record[8], "%d.0", (int)(list[d]->truth));
            }

            int column_count = append_on_dataset_record(9, 100, record, search_clauses(list[d]->pk));
            if (column_count > set.column_count)
                set.column_count = column_count;

            set.data[d] = record;
        }
    }

    if (is_engine("ram")) {
        free(list);
    }
    else {
        free_facts_deep(list);
    }

    return set;
}

int fileid(const char* filename) {
    int result = - strlen(filename);
    int i;
    for (i = 0; i < strlen(filename); ++i) {
        result += filename[i];
    }
    return result;
}

char* small_identifier(const char* word) {
    if (word[0] == '_' || word[0] == '*' || word[0] == '%') {
        if (strlen(word) >= 3) {
            return small_identifier(word+1);
        }
        else {
            return strdup("__");
        }
    }
    if (word[0] == '=') {
        return strdup("__");
    }
    if ((word[0] == '_' && strlen(word) > 2) || strlen(word) < 2) {
        return strdup("__");
    }

    char* identifier = calloc(1, 5);
    identifier[2] = 0;

    identifier[0] =  word[0] >= 'a' && word[0] <= 'z'
                  ?  word[0]
                  :  '_'
                  ;

    identifier[1] =  word[1]
                  && word[1] >= 'a' && word[1] <= 'z'
                  ?  word[1]
                  :  '_'
                  ;

//    identifier[1] = 'a';

    return identifier;
}

int sql_universal_add_link (const char* link, int key_1, int key_2) {
    int r = 0;
    if (is_engine("ram")) {
        r = ram_add_link(link, key_1, key_2);
    }
    else {
        r = disk_add_link(link, key_1, key_2);
    }
    return r;
}

char* universal_del_record(char* key) {
    if (is_engine("ram")) {
        // does not exist
    }
    else {
        return disk_del_record(key);
    }
    return 0;
}

char* universal_get_source(char* key) {
    printf("universal_get_source: %s", key);
    if (is_engine("ram")) {
        // does not exist
    }
    else {
        return disk_get_source(key);
    }
    return 0;
}

int universal_re_index() {
    printf("...\n");
    if (is_engine("ram")) {
        // does not exist
    }
    else {
        return disk_re_index();
    }
    return 0;
}

