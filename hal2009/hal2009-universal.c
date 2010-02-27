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

#include "hal2009-universal.h"


int universal_begin() {
    int r = 0;
    if (is_engine("ram")) {
        r = ram_begin();
    }
    else {
        r = disk_begin();
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
    if (p > 1000000*1000*100) {
        return 1;
    }
    if (p > 10000) {
        return 1;
    }
    if (p == -1) {
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
        free((*p)->from);
        
        return disk_set_to_invalid_value(p);
    }
    return 0;
}
        
struct word*** add_synonyms_by_string(const char* exp, struct word*** synonyms, int* position) {
    if (is_bad(exp)) {
        return synonyms;
    }
    
    if (strlen(exp) > 1) {
        synonyms[*position] = calloc(sizeof(struct word*), strlen(exp)+10);
        
        char* _exp = calloc(1, strlen(exp)+10);
        strcpy(_exp, "_");
        strcat(_exp, exp);
        strcat(_exp, "_");
        
        struct word* _word = set_word(_exp);
        synonyms[*position][0] = _word;
        ++(*position);
    }
    
    struct word**  words    = divide_words(exp);
    synonyms[*position]     = calloc(sizeof(struct word*), strlen(exp)+10);
    int f, g;
    for (f = 0, g = 0; words && words[f]; ++f) {
        if (can_be_a_pointer(words[f])) {
            synonyms[*position][g] = words[f];
            ++g;
        }
    }
    ++(*position);
    free(words);
    
    if (exp[0] == '*' && strlen(exp) > 1) {
        struct word**  words    = divide_words(exp+1);
        synonyms[*position]     = calloc(sizeof(struct word*), strlen(exp)+10);
        int f, g;
        for (f = 0, g = 0; words && words[f]; ++f) {
            if (can_be_a_pointer(words[f])) {
                synonyms[*position][g] = words[f];
                ++g;
            }
        }
        ++(*position);
        free(words);
    }
    
    if (exp[0] == '_' && strlen(exp) >= 3) {
        char* cpy_of_exp = strdup(exp+1);
        char* offset_of_last_underscore = strstr(cpy_of_exp+strlen(cpy_of_exp)-2, "_");
        if (offset_of_last_underscore) {
            offset_of_last_underscore[0] = '\0';
            int c;
            for (c = 0; cpy_of_exp[c]; ++c) {
                cpy_of_exp[c] = (cpy_of_exp[c] == '_') ? ' ' : cpy_of_exp[c];
            }
            struct word**  words    = divide_words(cpy_of_exp);
            synonyms[*position]     = calloc(sizeof(struct word*), strlen(cpy_of_exp)+10);
            int f, g;
            for (f = 0, g = 0; words && words[f]; ++f) {
                if (can_be_a_pointer(words[f])) {
                    synonyms[*position][g] = words[f];
                    ++g;
                }
            }
            ++(*position);
            free(words);
        }
        free(cpy_of_exp);
    }
    
    debugf("-> Synonym '%s' by string, position is now %p.\n", exp, *position);
    
    return synonyms;
}

int store_synonyms(const char* exp, struct word*** synonyms, int level) {
    struct word* word = set_word(exp);
    word->synonyms = synonyms;
    word->synonyms_level = level;
    debugf("Synonyms of phrase %s are now at %p.\n", exp, word->synonyms);
}

struct word*** get_stored_synonyms(const char* exp, int level) {
    struct word* word = set_word(exp);
    debugf("Synonyms of phrase %s are at %p.\n", word ? exp : "(the word is illegal)", word ? word->synonyms : 0);
    if (word->synonyms_level == level && word->synonyms && can_be_a_pointer(word->synonyms) && word->synonyms[0] && can_be_a_pointer(word->synonyms[0])) {
        debugf("Searching synonyms of phrase %s found (in cache): %p.\n", exp, word->synonyms);
        return word->synonyms;
    }
    return 0;
}

struct word*** add_synonyms_by_search(const char* subj, const char* obj, const char* verb, const char* adverbs, int use_what, struct word*** synonyms, struct fact** facts, int* position, int* allocated_until) {
    if (facts == TOOMUCH) {
        return synonyms;
    }
    
    // count the facts
    int size, f;
    for (size = 0, f = 0; facts[f]; ++f) {
        if (can_be_a_pointer(facts[f]) && can_be_a_pointer(facts[f]->objects) && can_be_a_pointer(facts[f]->objects[0])) {
            ++size;
        }
        if (can_be_a_pointer(facts[f]) && can_be_a_pointer(facts[f]->subjects) && can_be_a_pointer(facts[f]->subjects[0])) {
            ++size;
        }
    }
    
    for (f = 0; facts[f]; ++f) {
        if (can_be_a_pointer(facts[f]) && can_be_a_pointer(facts[f]->adverbs) && can_be_a_pointer(facts[f]->adverbs[0]) && can_be_a_pointer(facts[f]->adverbs[0]->name)) {
            set_to_invalid_fact(&(facts[f]));
        }
        if (can_be_a_pointer(facts[f]) && facts[f]->truth < 0.5) {
            set_to_invalid_fact(&(facts[f]));
        }
    }
    
    // allocate more memory
    *allocated_until += size;
    
    synonyms = realloc(synonyms, sizeof(struct word**)*(*allocated_until+1));
    
    
    // add to synonyms matrix
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
                for (c = 0; can_be_a_pointer(facts[f]->subjects[c]); ++c) { }
                struct word** temp = calloc(sizeof(facts[f]->subjects), (c+2));
                temp[0] = 0;
                for (c = 0; can_be_a_pointer(facts[f]->subjects[c]) && can_be_a_pointer(facts[f]->subjects[c]->name) && is_good(facts[f]->subjects[c]->name); ++c) {
                    temp[c] = facts[f]->subjects[c];
                }
                temp[c] = 0;
                if (*position == 0 || strcmp(synonyms[*position - 1], temp)) {
                    synonyms[*position] = temp;
                    ++(*position);
                }
                else {
                    free(temp);
                    continue;
                }
                if (c && temp && temp[0]) {
                    debugf("-> Synonym '%s' by search, position is now %p.\n", temp[0]->name ? temp[0]->name : "(useless)", *position);
                    debugf("        ... %s\n", temp[0]->name && can_be_a_pointer(temp[1]) && can_be_a_pointer(temp[1]->name) ? temp[1]->name : "(useless)");
                }
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
                for (c = 0; can_be_a_pointer(facts[f]->objects[c]); ++c) { }
                struct word** temp = calloc(sizeof(facts[f]->objects), (c+2));
                temp[0] = 0;
                for (c = 0; can_be_a_pointer(facts[f]->objects[c]) && can_be_a_pointer(facts[f]->objects[c]->name) && is_good(facts[f]->objects[c]->name); ++c) {
                    temp[c] = facts[f]->objects[c];
                }
                temp[c] = 0;
                if (*position == 0 || strcmp(synonyms[*position - 1], temp)) {
                    synonyms[*position] = temp;
                    ++(*position);
                }
                else {
                    free(temp);
                    continue;
                }
                if (c && temp && temp[0]) {
                    debugf("-> Synonym '%s' by search, position is now %p.\n", temp[0]->name ? temp[0]->name : "(useless)", *position);
                    debugf("        ... %s\n", temp[0]->name && can_be_a_pointer(temp[1]) && can_be_a_pointer(temp[1]->name) ? temp[1]->name : "(useless)");
                }
            }
        }
    }
    
    return synonyms;
}

static int in_search_synonyms = 0;
struct word*** search_synonyms(const char* exp, int level) {
    debugf("Searching synonyms of phrase %s. | in_search_synonyms = %p\n", exp && exp[0] ? exp : "(useless)", in_search_synonyms);

    int allocated_until     = 3;
    struct word*** synonyms = calloc(sizeof(struct word**), allocated_until+1);
    int position            = 0;
    synonyms = add_synonyms_by_string(exp, synonyms, &position);
    
    if (is_good(exp) && can_be_a_synonym(exp) && !in_search_synonyms && !strstr(exp, "|") && level >= 0) {
        struct word*** stored_synonyms = get_stored_synonyms(exp, level);
        if (stored_synonyms)
            return stored_synonyms;

        in_search_synonyms = 1;
        
        int _min = position;
        
        struct fact** facts_synonyms = search_facts_synonyms(exp, "", "bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|=", "", "", "", "default");
        synonyms = add_synonyms_by_search(exp, "", "bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|=", "", USE_OBJECTS,  synonyms, facts_synonyms, &position, &allocated_until);
        
        int l;
        for (l = 0; l < level; ++l) {
            int u;
            int _max = position;
            for (u = _min; u < _max; ++u) {
                if (synonyms[u] && synonyms[u] != -1) {
                    char* req_synonym = join_words(synonyms[u]);
                    printf("Search for sub synonyms of: %s\n", req_synonym);
                    
                    struct fact** facts_synonyms =  search_facts_synonyms(req_synonym, "", "bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|=", "", "", "", "default");
                    int f;
                    for (f = 0; facts_synonyms[f]; ++f) {
                        if (can_be_a_pointer(facts_synonyms[f]) && can_be_a_pointer(facts_synonyms[f]->from) && strstr(facts_synonyms[f]->from, "ps-")) {
                            set_to_invalid_fact(&(facts_synonyms[f]));
                        }
                    }
                    synonyms = add_synonyms_by_search(req_synonym, "", "bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|=", "", USE_OBJECTS,  synonyms, facts_synonyms, &position, &allocated_until);
                    synonyms = add_synonyms_by_search("", req_synonym, "bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|=", "", USE_SUBJECTS,  synonyms, facts_synonyms, &position, &allocated_until);
                    if (is_engine("ram")) {
                        free(facts_synonyms);
                    }
                    else {
                        free_facts_deep(facts_synonyms);
                    }
                }
            }
        }

        synonyms = add_synonyms_by_search("", exp, "bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|=", "", USE_SUBJECTS, synonyms, facts_synonyms, &position, &allocated_until);

        if (is_engine("ram")) {
            free(facts_synonyms);
        }
        else {
            free_facts_deep(facts_synonyms);
        }
        
        store_synonyms(exp, synonyms, level);
        in_search_synonyms = 0;
    }
    
    synonyms[position] = 0;

    debugf("Synonyms of phrase %s found (at end of function): %p. | in_search_synonyms = %p\n", exp, synonyms, in_search_synonyms);
    return synonyms;
}

struct word** divide_words(const char* str) {
    struct word** list = calloc(sizeof(struct word*), 1);
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
                struct word* word = set_word(name);
                if (name) free(name);
                
                ++count;
                list = realloc(list, sizeof(struct word*)*(count+1));
                list[count] = 0;
                list[count-1] = word;
                //debugf("divided(1): %s\n", name);
                a = e+1;
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

struct list* look_at_list(struct list* l) {
    if (!l) {
        l = calloc(sizeof(struct list), 1);
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

const char* from_number(int i) {
    char str[101];
    snprintf(str, 100, "%d", i);
    return str;
}

int is_bad(const char* s) {
    return (!can_be_a_pointer(s) || !s || !s[0] || ((s[0] == ' ' || s[0] == '0' || s[0] == ')' || ((s[0] < 'a' || s[0] > 'Z') && s[0] != '_')) && strlen(s) < 1));
}
int is_good(const char* s) {
    return (!is_bad(s));
}

struct fact* add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    if (is_engine("ram")) {
        return ram_add_clause(rel, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
    }
    else {
        return disk_add_clause(rel, subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
    }
}

struct fact* add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic) {
    if (is_engine("ram")) {
        return ram_add_fact(subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, only_logic);
    }
    else {
        return disk_add_fact(subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should, only_logic);
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
                    printf("Is '%s' in '%s'? ", a, c);
                    q = q || strstr(a, c);
                    if (q) {
                        printf("yes.\n");
                    }
                    else {
                        printf("no.\n");
                    }
                    if (i >= size-2) {
                        printf("break;\n", a, c);
                        break;
                    }
                    else {
                        printf("going on...\n", a, c);
                        c += i+1;
                        size -= i;
                        i = -1;
                    }
                }
            }
            return q;
        }
        else {
            return strstr(a, c);
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
            return strstr(a, c);
        }
    }
    
    return 0;
}

/*
int fact_matches_entity_by_entity(struct word** words, struct word*** request_words, int flags) {
    if (!request_words[0]) {
        return -1;
    }
    
    int count_of_words = 0;
    int count_of_words_with_trivial = 0;
    int c;
    for (c = 0, count_of_words = 0; words[c] && words[c]->name; ++c) {
        if (is_good(words[c]->name) && strlen(words[c]->name) > 1) {
            if (!is_a_trivial_word(words[c]->name)) {
                ++count_of_words;
            }
            ++count_of_words_with_trivial;
        }
    }

    
    int does_match = 0;
    int request_words_all = 0;
    int u;
    for (u = 0; request_words[u] && (-1 == request_words[u] || request_words[u][0]); ++u) {
        if (!can_be_a_pointer(request_words[u])) {
            continue;
        }
        
        int does_match_with_this_synonym   = 0;
        int should_match_with_this_synonym = 0;
        
        int count_of_words_request = 0;
        int count_of_words_request_with_trivial = 0;
        int c;
        for (c = 0, count_of_words_request = 0; request_words[u][c] && (-1 == request_words[u][c] || request_words[u][c]->name); ++c) {
            if (!can_be_a_pointer(request_words[u][c])) {
                continue;
            }
            
            if (!is_a_trivial_word(request_words[u][c]->name)) {
                ++count_of_words_request;
                if (strstr(request_words[u][c]->name, "*")) {
                    ++count_of_words_request;
                }
            }
            ++count_of_words_request_with_trivial;
            if (strstr(request_words[u][c]->name, "*")) {
                ++count_of_words_request_with_trivial;
            }
        }
        request_words_all += count_of_words_request;
        
        int v;
        for (v = 0; request_words[u][v] && (-1 == request_words[u][v] || request_words[u][v]->name); ++v) {
            if (!can_be_a_pointer(request_words[u][v])) {
                continue;
            }
            
            if (is_a_trivial_word(request_words[u][v]->name)) {
                continue;
            }
            
            int does_match_here = 0;
            int should_match_here = 0;
            
            if (   (count_of_words <= count_of_words_request+2 && flags == EXACT)
                || (                                              flags == WEAK)) {
                    
                int m;
                for (m = 0; words[m] && words[m]->name; ++m) {
                    if (!is_a_trivial_word(words[m]->name)) {
                        if (matches(words[m]->name, request_words[u][v]->name)) {
                            debugf("does match:     %s and %s.\n", words[m]->name, request_words[u][v]->name);
                            ++does_match_here;
                        }
                        else {
                            debugf("does not match: %s and %s.\n", words[m]->name, request_words[u][v]->name);
                        }
                        ++should_match_here;
                    }
                }
            }
            
            does_match_with_this_synonym   += (does_match_here > 0); // && (does_match_here == should_match_here || does_match_here >= should_match_here + (count_of_words_request>=2?3:1) - (count_of_words_request>=2?3:count_of_words_request))) ? 1 : 0;
            should_match_with_this_synonym += 1;
        }
        
        does_match = does_match || (should_match_with_this_synonym && ((does_match_with_this_synonym == should_match_with_this_synonym && should_match_with_this_synonym < 3)||(does_match_with_this_synonym+1 >= should_match_with_this_synonym && should_match_with_this_synonym >= 3)) && (count_of_words_request_with_trivial + 1 > count_of_words_with_trivial || count_of_words_request >= count_of_words));
    }
    if (request_words_all == 0) {
        return -1;
    }
    
    return does_match;
}
*/

int word_matches_word_array(struct word* word, struct word** words, int flags) {
    // debugf("    does this word match?: %i - %s\n", word, word->name);

    if (word->name == strstr(word->name, "%")) {
        return 1;
    }
    
    int m;
    for (m = 0; words[m] && words[m]->name; ++m) {
        if (is_good(words[m]->name) && strlen(words[m]->name) >= 1) {
            if (!is_a_trivial_word(words[m]->name)) {
                
                if (matches(word->name, words[m]->name)) {
                    // debugf("      %s matches with     %s\n", word->name, words[m]->name);
                    return 1;
                }
                else {
                    // debugf("      %s not matches with %s\n", word->name, words[m]->name);
                }
            }
        }
    }
    
    return 0;
}

int _diff(int a, int b) {
    if (a > b) return a-b;
    if (b > a) return b-a;
    return 0;
}

int fact_matches_entity_by_entity(struct word** words, struct word*** request_words, int flags) {
    if (!request_words[0]) {
        return -1;
    }
    
    int given_words_all                = 0;
    int given_words_all_only_important = 0;
    int given_words_all_also_trivial   = 0;
    int m;
    for (m = 0; words[m] && words[m]->name; ++m) {
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
    
    int count_requests = 0;
    
    int u;
    int does_match = 0;
    for (u = 0; request_words[u] && (-1 == request_words[u] || request_words[u][0]); ++u) {
        if (can_be_a_pointer(request_words[u])) {
            
            int request_words_all_also_trivial   = 0;
            int request_words_all_only_important = 0;
            int request_words_all = 0;
            
            int c;
            for (c = 0; request_words[u][c] && (-1 == request_words[u][c] || request_words[u][c]->name); ++c) {
                if (can_be_a_pointer(request_words[u][c])) {
                    if (!is_a_trivial_word(request_words[u][c]->name)) {
                        ++request_words_all;
                    }
                    if (is_important_word(request_words[u][c]->name)) {
                        ++request_words_all_only_important;
                    }
                    ++request_words_all_also_trivial;
                }
            }
            
            count_requests += (request_words_all_also_trivial) ? 1 : 0;
            
            // debugf("  next synonym.\n");
            int   does_match_this_synonym = 0;
            int should_match_this_synonym = 0;
    
            for (c = 0; request_words[u][c] && (-1 == request_words[u][c] || request_words[u][c]->name); ++c) {
                if (can_be_a_pointer(request_words[u][c])) {
                    if (!is_a_trivial_word(request_words[u][c]->name)) {
                        if (strstr(request_words[u][c]->name, "time_")) {
                            should_match_this_synonym -= 10;
                        }
                        else {
                            does_match_this_synonym   += word_matches_word_array(request_words[u][c], words, flags);
                            should_match_this_synonym += 1;
                        }
                    }
                }
            }
            // debugf("  does match this synonym: %i\n", does_match_this_synonym >= should_match_this_synonym);
            
            does_match = does_match || does_match_this_synonym >= should_match_this_synonym;
            if (flags == WEAK) {
                does_match = does_match || does_match_this_synonym;
            }
            if (flags != WEAK) {
                
                does_match = does_match && (given_words_all_also_trivial < 2 || (_diff(given_words_all_also_trivial, request_words_all_also_trivial) < 2 && _diff(given_words_all_only_important, request_words_all_only_important) <= 0));
            }
            if (does_match) {
                break;
            }
        }
    }
    
    if (count_requests == 0) {
        debugf("  => %i\n\n", -1);
        return -1;
    }
    
    
    if (does_match)  {
        debugf("  => %i\n\n", does_match);
    }
    return does_match;
}

int fact_matches_subject_by_subject(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->subjects, weak?WEAK:EXACT);
    debugf("subject by subject: %d\n", does_match);
    if (does_match == -1)
        does_match = 1;
    return does_match;
}
int fact_matches_object_by_subject(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->subjects, weak?WEAK:EXACT);
    debugf("object by subject:  %d\n", does_match);
    if (does_match == -1)
        does_match = 0;
    return does_match;
}
int fact_matches_subject_by_object(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->objects, weak?WEAK:EXACT);
    debugf("subject by object:  %d\n", does_match);
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
    int does_match = fact_matches_entity_by_entity(fact->objects, request->objects, weak?WEAK:EXACT);
    debugf("object by object:   %d\n", does_match);
    if (does_match == -1)
        does_match = 1;
    return does_match;
}
int fact_matches_adverb_by_adverb(struct fact* fact, struct request* request, int weak) {
    int does_match = fact_matches_entity_by_entity(fact->adverbs, request->adverbs, weak?WEAK:EXACT);
    if (does_match == -1)
        does_match = 1;
    debugf("adverb by adverb:   %d\n", does_match);
    return does_match;
}
int fact_matches_anything_by_extra(struct fact* fact, struct request* request) {
    int does_match = 
        (     fact_matches_entity_by_entity(fact->subjects, request->extra, WEAK) < 0
           && fact_matches_entity_by_entity(fact->objects,  request->extra, WEAK) < 0
           && fact_matches_entity_by_entity(fact->adverbs,  request->extra, WEAK) < 0
           && fact_matches_entity_by_entity(fact->extra,    request->extra, WEAK) < 0 )
        ||
           fact_matches_entity_by_entity(fact->subjects, request->extra, WEAK) > 0
        || fact_matches_entity_by_entity(fact->objects,  request->extra, WEAK) > 0
        || fact_matches_entity_by_entity(fact->adverbs,  request->extra, WEAK) > 0
        || fact_matches_entity_by_entity(fact->extra,    request->extra, WEAK) > 0
    ;
    // TODO: -1, see above
    debugf("anything by extra:  %d\n", does_match);
    return does_match;
}

int fact_matches_questionword_rules_of_q_what_weakly(struct fact* fact, struct request* request) {
    int does_match = 1;
    
    debugf("q_what_weakly: %d\n", does_match);
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
    
    debugf("q_what_extra: %d\n", does_match);
    return does_match;
}

int fact_matches_questionword_rules_of_q_what_exactly(struct fact* fact, struct request* request) {
    int does_match = 0;
    
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
    
    debugf("q_what_exactly: %d\n", does_match);
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
            debugf("q_who: %s\n", fact->objects[i]->name);
            if (0 == strcmp(fact->objects[i]->name, "ein") || 0 == strcmp(fact->objects[i]->name, "eine")) {
                does_match = 0;
            }
        }
    }
    
    debugf("q_who: %d\n", does_match);
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
            printf("adverb: %s\n", fact->adverbs[i]->name);
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
    
    debugf("q_where: %d\n", does_match);
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
    
    debugf("q_how: %d\n", does_match);
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
    
    debugf("q_from_where: %d\n", does_match);
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
    
    debugf("search_reasons: %d\n", does_match);
    return does_match;
}

int fact_matches_questionword_rules(struct fact* fact, struct request* request) {
    
    if (can_be_a_pointer(request->context)) {
        if (0 == strcmp(request->context, "q_what_weakly"))
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
                if (0 == strcmp(fact->verbs[m]->name, splitted_by_pipe[p]) || fact->verbs[m]->name == strstr(fact->verbs[m]->name, splitted_by_pipe[p])) {
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

struct fact* filter_fact_by_rules(struct fact* fact, struct request* request) {
    
    if (!(
    
            fact_matches_verb                 (fact, request)
         && (
                (
                    fact_matches_subject_by_subject (fact, request, (0 == strcmp(request->context, "reasonof")))
                 && fact_matches_object_by_object   (fact, request, (0 == strcmp(request->context, "reasonof")))
                )
            ||  (
                    strcmp(request->context, "q_what_extra")
                 && fact_matches_object_by_subject (fact, request, (0 == strcmp(request->context, "reasonof")))
                 && fact_matches_subject_by_object (fact, request, (0 == strcmp(request->context, "reasonof")))
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
         && fact_matches_adverb_by_adverb     (fact, request, (0 == strcmp(request->context, "reasonof")))
         && fact_matches_anything_by_extra    (fact, request)
         //&& fact_matches_questionword_rules   (fact, request)
    
         )) {
        
        set_to_invalid_fact(&fact);
    }


    return fact;
}

struct fact** filter_list_by_rules(struct fact** list, struct request* request) {
    printf("Starting fact filter!\n");
    
    if (list == TOOMUCH) {
        printf("Do not filter, too much data.\n");
        printf("Not filtered.\n");
        return TOOMUCH;
    }
    
    int b, count_of_facts, count_of_true_facts, count_of_context_matching_facts;
    for (b = 0, count_of_facts = 0, count_of_true_facts = 0, count_of_context_matching_facts = 0; list[b]; ++b) {
    
        printf("Filtering fact %i.\n", b);
        
        if (list[b] != -1) {
            list[b] = filter_fact_by_rules(list[b], request);
        }
        if (list[b] != -1) {
            ++count_of_facts;
        }
        if (list[b] != -1) {
            if (fact_matches_truth(list[b], request)) {
                ++count_of_true_facts;
            }
        }
        if (list[b] != -1) {
            if (fact_matches_questionword_rules(list[b], request)) {
                ++count_of_context_matching_facts;
            }
        }
    }
    
    if (count_of_true_facts >= 1 && (count_of_facts - count_of_true_facts) > (count_of_true_facts/2) ) {
        for (b = 0; list[b]; ++b) {
            if (list[b] != -1) {
                if (!fact_matches_truth(list[b], request)) {
                    set_to_invalid_fact(&(list[b]));
                }
            }
        }
    }
    
    if (count_of_context_matching_facts >= 1) {
        for (b = 0; list[b]; ++b) {
            if (list[b] != -1) {
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

int search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position) {
    if (is_engine("ram")) {
        return ram_search_facts_for_words_in_net(words, facts, limit, position);
    }
    else {
        return disk_search_facts_for_words_in_net(words, facts, limit, position);
    }
}

struct fact** search_in_net(struct request* fact, struct fact** list) {
    int limit = 10000;
    if (strcmp("1", check_config("limit-amount-of-answers", "1"))) {
        limit = 1000000;
    }
    
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
                return TOOMUCH;
            }
            printf("Found double facts.\n");
        }
        return facts;
    }
    
    
    
    int succ_1 = search_facts_for_words_in_net(fact->subjects, facts, limit, &position);
    int succ_2 = search_facts_for_words_in_net(fact->objects,  facts, limit, &position);
    int succ_3 = search_facts_for_words_in_net(fact->adverbs,  facts, limit, &position);
    int succ_4 = search_facts_for_words_in_net(fact->extra,    facts, limit, &position);
    int succ_5 = 0;

    if (fact->verbs && fact->verbs[0] && fact->verbs[0]->name && 0 == strcmp(fact->verbs[0]->name, ">>>")) {
        struct fact*** verbs = calloc(sizeof(struct fact***), 2);
        verbs[0] = fact->verbs;
        verbs[1] = 0;
        
        succ_5 = search_facts_for_words_in_net(verbs, facts, limit, &position);
        
        free(verbs);
    }
    
    if (succ_1 == TOOMUCH || succ_2 == TOOMUCH || succ_3 == TOOMUCH || succ_4 == TOOMUCH || succ_5 == TOOMUCH) {
        printf("(%i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH) = 1\n", succ_1, succ_2, succ_3, succ_4, succ_5);
        free(facts);
        return TOOMUCH;
    }
    printf("(%i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH || %i == TOOMUCH) = 0\n", succ_1, succ_2, succ_3, succ_4, succ_5);
    
    return facts;
}

void print_word_list_3rd_order(struct word*** list) {
    int i;
    for (i = 0; can_be_a_pointer(list[i]); ++i) {
        debugf("(\n");
        int j;
        for (j = 0; can_be_a_pointer(list[i][j]); ++j) {
            debugf(" 1: %p\n", list[i][j]);
            if (can_be_a_pointer(list[i][j]->name)) {
                debugf(" 2: %p\n", list[i][j]->name);
                debugf(" - %s\n", list[i][j]->name);
            }
        }
        //if (j == 0) {
            //debugf("   array at %p: first = %p, second = %p\n", list[i], list[i][0], can_be_a_pointer(list[i][0])&&can_be_a_pointer(list[i][1])?list[i][1]:0);
        //}
        debugf(")\n");
    }
    debugf("\n");
}

int count_list(void** list) {
    int i, size;
    for (i = 0, size = 0; list[i] || -1 == list[i]; ++i) {
        if (can_be_a_pointer(list[i]) && list[i] != -1) {
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
        for (i = 0; !found_one && (-1 == list[i] || can_be_a_pointer(list[i])); ++i) {
            if (found_one) {
                break;
            }
            
            int j;
            for (j = 0; !found_one && (-1 == list[i] || can_be_a_pointer(list[i][j])); ++j) {
                if (found_one) {
                    break;
                }
                
                if (can_be_a_pointer(list[i][j]->name)) {
                    if (strstr(list[i][j]->name, exp) && strlen(list[i][j]->name)-3 <= strlen(exp)) {
                        printf("Deleting synonym: %s\n", list[i][j]->name);
                        int no_more_items = 1;
                        if (j) {
                            int k;
                            for (k = j+1; -1 == list[i] || can_be_a_pointer(list[i][k]); ++k) {
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
                            for (k = i+1; -1 == list[k] || can_be_a_pointer(list[k]); ++k) {
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

struct fact** search_facts_simple(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int level) {
    struct fact** list = 0;
    
    {
        struct request* fact = calloc(sizeof(struct request), 1);
        fact->subjects     = search_synonyms(subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "", level);
        fact->objects      = search_synonyms(objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "", level);
        fact->verbs        =    divide_words(verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : "");
        fact->adverbs      = search_synonyms(adverbs  &&  adverbs[0] &&  adverbs[0] != '0' &&  adverbs[0] != ' ' ?  adverbs : "", 0);
        fact->extra        = search_synonyms(extra    &&    extra[0] &&    extra[0] != '0' &&    extra[0] != ' ' ?    extra : "", level);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;
        
        delete_in_first_if_second(fact->objects, subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "");

        debugf(
            "Searching simple answers for:\n"
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
            search_in_net(fact, list),
            fact
        );
        
        if (list != TOOMUCH) {
            
            int l;
            for (l = 0; can_be_a_pointer(list[l]) || -1 == list[l]; ++l) {
                if (can_be_a_pointer(list[l])) {
                    if (can_be_a_pointer(list[l]->verbs) && can_be_a_pointer(list[l]->verbs[0]) && can_be_a_pointer(list[l]->verbs[0]->name)) {
                        if (strstr(list[l]->verbs[0]->name, "=") && strlen(verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : "")) {
                            if (strstr(list[l]->from, "thes")) {
                                set_to_invalid_fact(&(list[l]));
                            }
                            else {
                                free(list[l]->verbs);
                                list[l]->verbs = divide_words("equal");
                            }
                        }
                    }
                }
            }
            
        }

        if (is_engine("ram")) {
            free(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
        else {
            free_words_weak(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
    }
    
    return list;
}



struct fact** search_facts_double_facts(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    struct fact** list = 0;
    
    {
        struct request* fact = calloc(sizeof(struct request), 1);
        fact->subjects     = search_synonyms(subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "", 0);
        fact->objects      = search_synonyms(objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "", 0);
        fact->verbs        =    divide_words(verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : "");
        fact->adverbs      = search_synonyms(adverbs  &&  adverbs[0] &&  adverbs[0] != '0' &&  adverbs[0] != ' ' ?  adverbs : "", 0);
        fact->extra        = search_synonyms(extra    &&    extra[0] &&    extra[0] != '0' &&    extra[0] != ' ' ?    extra : "", 0);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;
        
        delete_in_first_if_second(fact->objects, subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "");

        debugf(
            "Searching double facts for:\n"
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
            search_in_net(fact, list),
            fact
        );

        if (is_engine("ram")) {
            free(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
        else {
            free_words_weak(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
    }
    
    return list;
}

struct fact** search_facts_synonyms(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    struct fact** list = 0;
    
    {
        struct request* fact = calloc(sizeof(struct request), 1);
        fact->subjects     = search_synonyms(subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "", 0);
        fact->objects      = search_synonyms(objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "", 0);
        fact->verbs        =    divide_words(verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : "");
        fact->adverbs      = search_synonyms(adverbs  &&  adverbs[0] &&  adverbs[0] != '0' &&  adverbs[0] != ' ' ?  adverbs : "", 0);
        fact->extra        = search_synonyms(extra    &&    extra[0] &&    extra[0] != '0' &&    extra[0] != ' ' ?    extra : "", 0);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;
        
        delete_in_first_if_second(fact->objects, subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "");
        
        debugf(
            "Searching synonym-answers for:\n"
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
            search_in_net(fact, list),
            fact
        );
        
        if (is_engine("ram")) {
            free(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
        else {
            free_words_weak(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
    }
    
    return list;
}

char* generalize_verb(char* verb) {
    char* verb_1_chk =        "|=|bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|is|am|are|";
    char* verb_1     = strdup( "=|bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|is|am|are");
    
    char* verb_chk   = calloc(strlen(verb)+3, 1);
    sprintf(verb_chk, "|%s", verb);
    if (strstr(verb_chk, "0")) {
        strstr(verb_chk, "0")[0] = '\0';
    }
    if (strstr(verb_chk, "1")) {
        strstr(verb_chk, "1")[0] = '\0';
    }
    printf("strstr: Is '%s' in '%s'?\n", verb_chk, verb_1_chk);
    if (strstr(verb_1_chk, verb_chk)) {
        printf("  yes.\n");
        free(verb_chk);
        free(verb);
        return (verb_1);
    }
    else {
        printf("  no.\n");
        free(verb_chk);
        free(verb_1);
        return (verb);
    }
}

struct request* negotiate_deep_search(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, const struct fact* if_clause, const struct fact* then_clause) {
    struct request* req = calloc(sizeof(struct request), 1);
    
    int then_clause_has_variable_subjects = 0;
    int then_clause_has_variable_objects  = 0;
    int if_clause_has_variable_subjects   = 0;
    int if_clause_has_variable_objects    = 0;

    int i;
    if (can_be_a_pointer(then_clause->subjects)) {
        for (i = 0; can_be_a_pointer(then_clause->subjects[i]) && can_be_a_pointer(then_clause->subjects[i]->name); ++i) {
            if (then_clause->subjects[i]->name[0] >= 'a' && then_clause->subjects[i]->name[0] <= 'g') {
                then_clause_has_variable_subjects = then_clause->subjects[i]->name[0];
            }
        }
    }
    
    if (can_be_a_pointer(then_clause->objects)) {
        for (i = 0; can_be_a_pointer(then_clause->objects[i]) && can_be_a_pointer(then_clause->objects[i]->name); ++i) {
            if (then_clause->objects[i]->name[0] >= 'a' && then_clause->objects[i]->name[0] <= 'g') {
                then_clause_has_variable_objects  = then_clause->objects[i]->name[0];
            }
        }
    }
    
    if (can_be_a_pointer(if_clause->subjects)) {
        for (i = 0; can_be_a_pointer(if_clause->subjects[i]) && can_be_a_pointer(if_clause->subjects[i]->name); ++i) {
            if (if_clause->subjects[i]->name[0] >= 'a' && if_clause->subjects[i]->name[0] <= 'g') {
                if_clause_has_variable_subjects   = if_clause->subjects[i]->name[0];
            }
        }
    }
    
    if (can_be_a_pointer(if_clause->objects)) {
        for (i = 0; can_be_a_pointer(if_clause->objects[i]) && can_be_a_pointer(if_clause->objects[i]->name); ++i) {
            if (if_clause->objects[i]->name[0] >= 'a' && if_clause->objects[i]->name[0] <= 'g') {
                if_clause_has_variable_objects    = if_clause->objects[i]->name[0];
            }
        }
    }
    
    /// The then-clause is equivalent to the user input
    /// and if-clause to the output
    
    req->subjects     = 0;
    req->objects      = 0;

    if (then_clause_has_variable_subjects) {
        struct word*** thing_to_insert = search_synonyms(subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "", 0);
        
        if (if_clause_has_variable_subjects && 0 == req->subjects) {
            req->subjects = thing_to_insert;
        }
        if (if_clause_has_variable_objects  && 0 == req->objects) {
            req->objects = thing_to_insert;
        }
    }
    if (then_clause_has_variable_objects) {
        struct word*** thing_to_insert = search_synonyms(objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "", 0);
        
        if (if_clause_has_variable_subjects && 0 == req->subjects) {
            req->subjects = thing_to_insert;
        }
        if (if_clause_has_variable_objects  && 0 == req->objects) {
            req->objects = thing_to_insert;
        }
    }
    
    if (req->subjects == 0) {
        req->subjects =  search_synonyms("", 0);
    }
    if (req->objects  == 0) {
        req->objects  =  search_synonyms("", 0);
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
    req->adverbs      = search_synonyms(req_adverbs, 0);
    req->extra        = search_synonyms(req_extra, 0);
    req->questionword = strdup         (if_clause->questionword);
    req->context      = strdup         (context);
    req->truth        = 1;

    int j;
    for (j = 0; req->verbs[j]; ++j) {
        if (strlen(req->verbs[j]) >= 1) {
            req->verbs[j] = set_word(generalize_verb(req->verbs[j]->name));
        }
    }
    
    free(req_verbs);
    free(req_adverbs);
    free(req_extra);
    
    return req;
}
    
struct fact** search_facts_deep(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    struct fact** linking_list = 0;
    struct fact**         list = 0;
    
    {
        struct request* fact = calloc(sizeof(struct request), 1);
        fact->subjects     = search_synonyms(subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "", 0);
        fact->objects      = search_synonyms(objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "", 0);
        fact->verbs        =    divide_words(verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : "");
        fact->adverbs      = search_synonyms(adverbs  &&  adverbs[0] &&  adverbs[0] != '0' &&  adverbs[0] != ' ' ?  adverbs : "", 0);
        fact->extra        = search_synonyms(extra    &&    extra[0] &&    extra[0] != '0' &&    extra[0] != ' ' ?    extra : "", 0);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup("search_reasons");
        fact->truth        = 1;
        
        delete_in_first_if_second(fact->objects, subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "");

        debugf(
            "Searching reasons of:\n"
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

        linking_list = filter_list_by_rules (
            search_in_net(fact, linking_list),
            fact
        );
        
        if (list == TOOMUCH) {
            return TOOMUCH;
        }
        
        free(fact->context);
        fact->context = strdup(context);
        
        int l;
        for (l = 0; can_be_a_pointer(linking_list[l]) || -1 == linking_list[l]; ++l) {
            if (can_be_a_pointer(linking_list[l])) {
                printf("There is a sentence in the result list.\n");
                
                struct fact** clauses = search_clauses(linking_list[l]->pk);
                if (can_be_a_pointer(clauses) && can_be_a_pointer(clauses[0])) {
                    printf("It has clauses.\n");
                    
                    int c;
                    for (c = 0; can_be_a_pointer(clauses[c]); ++c) {
                        printf("Clause %d, question word %s\n", c, clauses[c]->questionword);
                        
                        if (is_conditional_questionword(clauses[c]->questionword)) {
                            printf("There is a sentence with a conditional questionword.\n");
                            
                            struct request* req = negotiate_deep_search(subjects, objects, verbs, adverbs, extra, questionword, context, clauses[c], linking_list[l]);
                            
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

                            list = filter_list_by_rules (
                                search_in_net(req, list),
                                req
                            );
                            
                            if (is_engine("ram")) {
                                free(req);
                            }
                            else {
                                free(req);
                            }
                        }
                    }
                }

                if (is_engine("disk")) {
                    free(clauses);
                }
            }
        }

        if (is_engine("ram")) {
            free(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
        else {
            free_words_weak(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }

        if (is_engine("ram")) {
            free(linking_list);
        }
        else {
            free_facts_deep(linking_list);
        }
    }
    
    return list;
}
    
struct fact** search_facts_thesaurus(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    struct fact** list = 0;
    
    {
        struct request* fact = calloc(sizeof(struct request), 1);
        fact->subjects     = search_synonyms(subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "", 0);
        fact->objects      = search_synonyms(objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "", 0);
        fact->verbs        =    divide_words("=");
        fact->adverbs      = search_synonyms(adverbs  &&  adverbs[0] &&  adverbs[0] != '0' &&  adverbs[0] != ' ' ?  adverbs : "", 0);
        fact->extra        = search_synonyms(extra    &&    extra[0] &&    extra[0] != '0' &&    extra[0] != ' ' ?    extra : "", 0);
        fact->questionword =          strdup(questionword);
        fact->context      =          strdup(context);
        fact->truth        = 1;
        
        delete_in_first_if_second(fact->objects, subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "");

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
            search_in_net(fact, list),
            fact
        );
        
        if (list == TOOMUCH) {
            return TOOMUCH;
        }
        
        int l;
        for (l = 0; can_be_a_pointer(list[l]) || list[l] == -1; ++l) {
            if (can_be_a_pointer(list[l])) {
                if (can_be_a_pointer(list[l]->verbs)) {
                    free(list[l]->verbs);
                    list[l]->verbs = divide_words("equal");
                }
            }
        }

        if (is_engine("ram")) {
            free(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
        else {
            free_words_weak(fact->verbs);
            free(fact->questionword);
            free(fact->context);
            free(fact);
        }
    }
    
    return list;
}
    
struct fact** search_facts(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context) {
    struct fact** list = 0;
    
    printf("Do we need the 'double facts' search?\n");
    if (0 == strcmp(context, "double_facts")) {
        if (!can_be_a_pointer(list) || !count_list(list)) {
            printf("We do.\n");
            
            printf("'double facts' search is allowed.\n");
                
            struct fact** _list = search_facts_double_facts(subjects, objects, verbs, adverbs, extra, questionword, context);
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
    
    printf("Do we need the 'not meant' search?\n");
    if (0 == strcmp(context, "not_meant")) {
        if (!can_be_a_pointer(list) || !count_list(list)) {
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
    
    printf("Do we need the simple search?\n");
    if (!can_be_a_pointer(list) || !count_list(list)) {
        printf("We do.\n");
        
        int level = 1;
        
        if (verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ') {
            level = 0;
        }
        if (0 == strcmp(context, "nosynonyms")) {
            level = -1;
        }
        
        while (level < 2 && ( !can_be_a_pointer(list) || !count_list(list) )) {
            
            struct fact** _list = search_facts_simple(subjects, objects, verbs, adverbs, extra, questionword, context, level);
            if (_list == TOOMUCH) {
                return TOOMUCH;
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
        printf("No.\n");
    }
    
    printf("Do we need the deep search?\n");
    
    if (!can_be_a_pointer(list) || !count_list(list)) {
        printf("We do.\n");
        
        struct fact** _list = search_facts_deep(subjects, objects, verbs, adverbs, extra, questionword, context);
        if (_list == TOOMUCH) {
            return TOOMUCH;
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
    
    printf("Do we need the wiki search?\n");
    
    if ((!strstr(context, "what") && !strstr(context, "how") && !strstr(context, "who")) || strstr(context, "what_prep") || strlen(adverbs) >= 3) {
        printf("No, no what-context.\n");
    }
    else if (!can_be_a_pointer(list) || !count_list(list)) {
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
    
    printf("Do we need the thesaurus search?\n");
    
    if ((!can_be_a_pointer(list) || !count_list(list)) && (verbs && verbs[0] && verbs[0] != '0' && verbs[0] != ' ' && strstr(verbs, "="))) {
        printf("We do.\n");
        
        struct fact** _list = search_facts_thesaurus(subjects, objects, verbs, adverbs, extra, questionword, context);
        if (_list == TOOMUCH) {
            return TOOMUCH;
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
        if (list[d] != -1) {
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
    
    if (list == TOOMUCH) {
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
    set.column_count = 8;
    set.data = calloc(sizeof(char*), size+2);
    
    int d;
    for (d = 0; list[d]; ++d) {
        if (list[d] != -1) {
            //char** record = calloc(sizeof(char*), sizeof(struct fact)*100);
            char** record = calloc(sizeof(char*), 100);
            record[0] = calloc(sizeof(char), 10);
            sprintf(record[0], "%d", (int)(list[d]->pk));
            record[1] = join_words(list[d]->verbs);
            record[2] = join_words(list[d]->subjects);
            record[3] = join_words(list[d]->objects);
            record[4] = join_words(list[d]->adverbs);
            record[5] = strdup("50");
            record[6] = strdup(list[d]->from ? list[d]->from : "");
            record[7] = calloc(sizeof(char), 10);
            if (list[d]->truth > 0.2 && list[d]->truth < 0.8) {
                sprintf(record[7], "0.5");
            }
            else {
                sprintf(record[7], "%d.0", (int)(list[d]->truth));
            }
            
            int column_count = append_on_dataset_record(8, 100, record, search_clauses(list[d]->pk));
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

int is_a_trivial_word(const char* word) {
    return (
        can_be_a_pointer(word) && (
            0 == strcmp(word, "the")
         || 0 == strcmp(word, "a")
         || 0 == strcmp(word, "an")
    
         || 0 == strcmp(word, "der")
         || 0 == strcmp(word, "die")
         || 0 == strcmp(word, "das")
         || 0 == strcmp(word, "des")
         || 0 == strcmp(word, "den")
         || 0 == strcmp(word, "dem")
         || 0 == strcmp(word, "ein")
         || 0 == strcmp(word, "eine")
         || 0 == strcmp(word, "einer")
         || 0 == strcmp(word, "einem")
         || 0 == strcmp(word, "einen")
         || 0 == strcmp(word, "eines")
         || 0 == strcmp(word, "nicht")
    
         || 0 == strcmp(word, "sehr")
         || 0 == strcmp(word, "viel")
         || 0 == strcmp(word, "viele")
         || 0 == strcmp(word, "weder")
         || 0 == strcmp(word, "noch")
         || 0 == strcmp(word, "schon")
         || 0 == strcmp(word, "bereits")
         || 0 == strcmp(word, "nur")
         || 0 == strcmp(word, "zu")
         || 0 == strcmp(word, "mit")
         || 0 == strcmp(word, "ohne")
         || 0 == strcmp(word, "in")
        )
         ?  1
         :  0
    );
}

int is_conditional_questionword(const char* word) {
    return (
        can_be_a_pointer(word) && (
            0 == strcmp(word, "if")
         || 0 == strcmp(word, "when")
    
         || 0 == strcmp(word, "wenn")
         || 0 == strcmp(word, "falls")
         || 0 == strcmp(word, "sobald")
        )
         ?  1
         :  0
    );
}

int can_be_a_synonym(const char* word) {
    return (
            !is_a_trivial_word(word)
         && strcmp(word, "es")
         && strcmp(word, "*")
         && strcmp(word, "in")
         && strcmp(word, "im")
         && strcmp(word, "an")
         && strcmp(word, "am")
         && strcmp(word, "von")
         && strcmp(word, "vom")
         && strcmp(word, "auf")
         && strcmp(word, "so")
         && strcmp(word, "genau")
         && strcmp(word, "genauso")
         && !strstr(word, "wie")
    
         && strcmp(word, "ich")
         && strcmp(word, "du")
         && strcmp(word, "_ich_")
         && strcmp(word, "_du_")
         // && ( (strlen(word) > 3 && !strstr(word, "_")) || strlen(word) > 6 )
         
         ?  1
         :  0
    );
}

int is_important_word(const char* word) {
    return (
            !is_a_trivial_word(word)
         && strcmp(word, "es")
         && strcmp(word, "*")
         && strcmp(word, "in")
         && strcmp(word, "im")
         && strcmp(word, "an")
         && strcmp(word, "am")
         && strcmp(word, "von")
         && strcmp(word, "vom")
         && strcmp(word, "auf")
         && strcmp(word, "so")
         && strcmp(word, "genau")
         && strcmp(word, "genauso")
         && strcmp(word, "wie")
    
         && !strstr(word, "$")
         && !strstr(word, "ein")
         && strcmp(word, "der")
         && strcmp(word, "die")
         && strcmp(word, "das")
         
         ?  1
         :  0
    );
}

const char* small_identifier(const char* word) {
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

