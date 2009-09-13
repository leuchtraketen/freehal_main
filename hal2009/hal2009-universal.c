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
    return 0;
}

struct word*** add_synonyms_by_string(const char* exp, struct word*** synonyms, int* position) {
    if (is_bad(exp)) {
        return synonyms;
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
    
    debugf("Added synonym %s by string, position is now %p.\n", exp, *position);
    
    return synonyms;
}

int store_synonyms(const char* exp, struct word*** synonyms) {
    struct word* word = set_word(exp);
    word->synonyms = synonyms;
    debugf("Synonyms of phrase %s are now at %p.\n", exp, word->synonyms);
}

struct word*** get_stored_synonyms(const char* exp) {
    struct word* word = set_word(exp);
    debugf("Synonyms of phrase %s are at %p.\n", word ? exp : "(the word is illegal)", word ? word->synonyms : 0);
    if (word->synonyms && can_be_a_pointer(word->synonyms) && word->synonyms[0] && can_be_a_pointer(word->synonyms[0])) {
        debugf("Searching synonyms of phrase %s found (in cache): %p.\n", exp, word->synonyms);
        return word->synonyms;
    }
    return 0;
}

struct word*** add_synonyms_by_search(const char* subj, const char* obj, const char* verb, const char* adverbs, int use_what, struct word*** synonyms, int* position, int* allocated_until) {
    struct fact** facts = search_facts(subj, obj, verb, adverbs, "", "");
    
    // count the facts
    int size, f;
    for (size = 0, f = 0; facts[f]; ++f) {
        if (can_be_a_pointer(facts[f]) && can_be_a_pointer(facts[f]->objects) && facts[f]->objects[0]) {
            ++size;
        }
    }
    
    // allocate more memory
    *allocated_until += size;
    
    synonyms = realloc(synonyms, sizeof(struct word**)*(*allocated_until+1));
    
    //for (f = *allocated_until - size; f <= *allocated_until; ++f) {
        //synonyms[f] = 0;
    //}
    
    // add to synonyms matrix
    if (use_what == USE_SUBJECTS) {
        for (f = 0; facts[f]; ++f) {
            if (   can_be_a_pointer(facts[f])
                && can_be_a_pointer(facts[f]->subjects)
                && can_be_a_pointer(facts[f]->subjects[0])
            ) {
                struct word** temp = calloc(sizeof(facts[f]->subjects), 1);
                *temp = *(facts[f]->subjects);
                synonyms[*position] = temp;
                ++(*position);
                debugf("Added synonym %s by search, position is now %p.\n", facts[f]->subjects[0]->name ? facts[f]->subjects[0]->name : "(useless)", *position);
            }
        }
    }
    else if (use_what == USE_OBJECTS) {
        for (f = 0; facts[f]; ++f) {
            if (   can_be_a_pointer(facts[f])
                && can_be_a_pointer(facts[f]->objects)
                && can_be_a_pointer(facts[f]->objects[0])
            ) {
                struct word** temp = calloc(sizeof(facts[f]->objects), 1);
                *temp = *(facts[f]->objects);
                synonyms[*position] = temp;
                ++(*position);
                debugf("Added synonym %s by search, position is now %p.\n", facts[f]->objects[0]->name ? facts[f]->objects[0]->name : "(useless)", *position);
            }
        }
    }
    
    if (is_engine("ram")) {
        free(facts);
    }
    else {
        free_facts_deep(facts);
    }
    
    return synonyms;
}

static int in_search_synonyms = 0;
struct word*** search_synonyms(const char* exp) {
    debugf("Searching synonyms of phrase %s. | in_search_synonyms = %p\n", exp && exp[0] ? exp : "(useless)", in_search_synonyms);

    int allocated_until     = 3;
    struct word*** synonyms = calloc(sizeof(struct word**), allocated_until+1);
    int position            = 0;
    synonyms = add_synonyms_by_string(exp, synonyms, &position);
    
    if (is_good(exp) && !in_search_synonyms && !strstr(exp, "|")) {
        struct word*** stored_synonyms = get_stored_synonyms(exp);
        if (stored_synonyms)
            return stored_synonyms;

        in_search_synonyms = 1;
        
        synonyms = add_synonyms_by_search(exp, "", "bi|bin|bist|ist|sind|seid|sein|heisst|heisse|heissen", "", USE_OBJECTS,  synonyms, &position, &allocated_until);
        synonyms = add_synonyms_by_search("", exp, "bi|bin|bist|ist|sind|seid|sein|heisst|heisse|heissen", "", USE_SUBJECTS, synonyms, &position, &allocated_until);

        store_synonyms(exp, synonyms);
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
        if (str[p] == ' ') {
            e = p;
            if (e - a > 0) {
                char* name = calloc(e-a+2, 1);
                strncpy(name, str+a, e-a);
                struct word* word = set_word(name);
                
                ++count;
                list = realloc(list, sizeof(struct word*)*(count+1));
                list[count] = 0;
                list[count-1] = word;
                debugf("divided(1): %s\n", name);
                a = e+1;
            }
        }
    }
    
    e = p;
    if (e - a > 0) {
        char* name = calloc(e-a+2, 1);
        strncpy(name, str+a, e-a);
        struct word* word = set_word(name);
        
        ++count;
        list = realloc(list, sizeof(struct word*)*(count+1));
        list[count] = 0;
        list[count-1] = word;
        debugf("divided(2): %s\n", name);
        a = e;
    }
    
    debugf("name: %s (a = %p, e = %p), count = %p, list = %p\n", str && str[0] ? str : "(useless)", a, e, count, list);
    
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
    return (!s || !s[0] || s[0] == '0' || s[0] == ' ');
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

struct fact* add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    if (is_engine("ram")) {
        return ram_add_fact(subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
    }
    else {
        return disk_add_fact(subjects, objects, verbs, adverbs, extra, questionword, from, truth, verb_flag_want, verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should);
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

struct DATASET search_facts_as_dataset(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword) {
    return as_dataset(search_facts(subjects, objects, verbs, adverbs, extra, questionword));
}
struct DATASET search_clauses_as_dataset(int rel) {
    return as_dataset(search_clauses(rel));
}

int fact_matches_truth(struct fact* fact, struct request* request) {
    int does_match = fact->truth > 0.8;
    
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
        }
        if (strstr(c, "*")) {
            int i;
            int size = strlen(c);
            for (i = 0; i < size; ++i) {
                c[i] = c[i] == '*' ? (i >= size-2 ? '\0' : ' ') : c[i];
            }
        }
        
        return strstr(a, c);
    }
    
    return 0;
}

int fact_matches_entity_by_entity(struct word** words, struct word*** request_words, int flags) {
    if (!request_words[0]) {
        return -1;
    }
    //debugf("entity 0: %s\n", request_words[0]                                                                ? request_words[0]->name : "(null)");
    //debugf("entity 1: %s\n", request_words[0] && request_words[1]                                            ? request_words[1]->name : "(null)");
    //debugf("entity 2: %s\n", request_words[0] && request_words[1] && request_words[2]                        ? request_words[2]->name : "(null)");
    //debugf("entity 3: %s\n", request_words[0] && request_words[1] && request_words[2] && request_words[3]    ? request_words[3]->name : "(null)");
    
    int count_of_words;
    for (count_of_words = 0; words[count_of_words] && words[count_of_words]->name; ++count_of_words) {
    }

    int does_match = 0;
    
    int u;
    for (u = 0; request_words[u] && request_words[u][0]; ++u) {
        int does_match_with_this_synonym   = 0;
        int should_match_with_this_synonym = 0;
        
        int count_of_words_request;
        for (count_of_words_request = 0; request_words[u][count_of_words_request] && request_words[u][count_of_words_request]->name; ++count_of_words_request)
        { }
        
        int v;
        for (v = 0; request_words[u][v] && request_words[u][v]->name; ++v) {
            if (is_a_trivial_word(request_words[u][v]->name)) {
                continue;
            }
            
            int does_match_here = 0;
            
            if (   (count_of_words <= count_of_words_request+2 && flags == EXACT)
                || (                                              flags == WEAK)) {
                    
                int m;
                for (m = 0; words[m] && words[m]->name; ++m) {
                    if (matches(words[m]->name, request_words[u][v]->name)) {
                        debugf("does match:     %s and %s.\n", words[m]->name, request_words[u][v]->name);
                        does_match_here = 1;
                    }
                    else {
                        debugf("does not match: %s and %s.\n", words[m]->name, request_words[u][v]->name);
                    }
                }
            }
            
            does_match_with_this_synonym   += does_match_here;
            should_match_with_this_synonym += 1;
        }
        
        does_match = does_match || (should_match_with_this_synonym && does_match_with_this_synonym == should_match_with_this_synonym);
    }
    
    does_match = does_match || (!u);
    
    return does_match;
}

int fact_matches_subject_by_subject(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->subjects, EXACT);
    debugf("subject by subject: %p\n", does_match);
    if (does_match == -1)
        does_match = 1;
    return does_match;
}
int fact_matches_object_by_subject(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->subjects, EXACT);
    debugf("object by subject:  %p\n", does_match);
    if (does_match == -1)
        does_match = 0;
    return does_match;
}
int fact_matches_subject_by_object(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->objects, EXACT);
    debugf("subject by object:  %p\n", does_match);
    if (does_match == -1)
        does_match = 0;
    return does_match;
}
int fact_matches_object_by_object(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->objects, EXACT);
    debugf("object by object:   %p\n", does_match);
    if (does_match == -1)
        does_match = 1;
    return does_match;
}
int fact_matches_adverb_by_adverb(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->adverbs, request->adverbs, WEAK);
    if (does_match == -1)
        does_match = 1;
    debugf("adverb by adverb:   %p\n", does_match);
    return does_match;
}
int fact_matches_anything_by_extra(struct fact* fact, struct request* request) {
    int does_match = 
           fact_matches_entity_by_entity(fact->subjects, request->extra, WEAK)
        || fact_matches_entity_by_entity(fact->objects,  request->extra, WEAK)
        || fact_matches_entity_by_entity(fact->adverbs,  request->extra, WEAK)
        || fact_matches_entity_by_entity(fact->extra,    request->extra, WEAK)
    ;
    // TODO: -1, see above
    debugf("anything by extra:  %p\n", does_match);
    return does_match;
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
        
        does_match = does_match && does_match_here;
        
        for (p = 0; splitted_by_pipe[p]; ++p) {
            free(splitted_by_pipe[p]);
        }
        free(splitted_by_pipe);
    }
    
    return does_match;
}

struct fact* filter_fact_by_rules(struct fact* fact, struct request* request) {
    
    fact = 
    
            fact_matches_verb                 (fact, request)
         && ( fact_matches_subject_by_subject (fact, request) || fact_matches_object_by_subject (fact, request) )
         && ( fact_matches_object_by_object   (fact, request) || fact_matches_object_by_object  (fact, request) )
         && fact_matches_adverb_by_adverb     (fact, request)
         && fact_matches_anything_by_extra    (fact, request)
    
         ?  fact
         :  (struct fact*)-1
         ;


    return fact;
}

struct fact** filter_list_by_rules(struct fact** list, struct request* request) {
    int b, count_of_true_facts;
    for (b = 0, count_of_true_facts = 0; list[b]; ++b) {
        if (list[b] != -1) {
            list[b] = filter_fact_by_rules(list[b], request);
        }
        if (list[b] != -1) {
            if (fact_matches_truth(list[b], request)) {
                ++count_of_true_facts;
            }
        }
    }
    
    if (count_of_true_facts >= 1) {
        for (b = 0, count_of_true_facts = 0; list[b]; ++b) {
            if (list[b] != -1) {
                if (!fact_matches_truth(list[b], request)) {
                    list[b] = (struct fact*)-1;
                }
            }
        }
    }

    return list;
}

int search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position) {
    if (is_engine("ram")) {
        return ram_search_facts_for_words_in_net(words, facts, limit, position);
    }
    else {
        return disk_search_facts_for_words_in_net(words, facts, limit, position);
    }
}

struct fact** search_in_net(struct request* fact) {
    int limit = 20000;
    struct fact** facts = calloc(1, sizeof(struct fact*)*(limit+1));
    
    int position = 0;
    search_facts_for_words_in_net(fact->subjects, facts, limit, &position);
    search_facts_for_words_in_net(fact->objects, facts, limit, &position);
    search_facts_for_words_in_net(fact->adverbs, facts, limit, &position);
    search_facts_for_words_in_net(fact->extra, facts, limit, &position);
    
    return facts;
}

void print_word_list_3rd_order(struct word*** list) {
    int i;
    for (i = 0; can_be_a_pointer(list[i]); ++i) {
        debugf("(\n");
        int j;
        for (j = 0; can_be_a_pointer(list[i][j]); ++j) {
            debugf(" - %s\n", list[i][j]->name);
        }
        if (j == 0) {
            debugf("   array at %p: first = %p, second = %p\n", list[i], list[i][0], list[i][0]?list[i][1]:0);
        }
        debugf(")\n");
    }
    debugf("\n");
}

struct fact** search_facts(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword) {
    struct request* fact = calloc(sizeof(struct request), 1);
    fact->subjects     = search_synonyms(subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "");
    fact->objects      = search_synonyms(objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "");
    fact->verbs        =    divide_words(verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : "");
    fact->adverbs      = search_synonyms(adverbs  &&  adverbs[0] &&  adverbs[0] != '0' &&  adverbs[0] != ' ' ?  adverbs : "");
    fact->extra        = search_synonyms(extra    &&    extra[0] &&    extra[0] != '0' &&    extra[0] != ' ' ?    extra : "");
    fact->questionword = strdup(questionword);
    fact->truth        = 1;

    debugf(
        "Searching answers for:\n"
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

     // subjects && subjects[0] && subjects[0] != '0' && subjects[0] != ' ' ? subjects : "",
     // objects  &&  objects[0] &&  objects[0] != '0' &&  objects[0] != ' ' ?  objects : "",
     // verbs    &&    verbs[0] &&    verbs[0] != '0' &&    verbs[0] != ' ' ?    verbs : "",
     // adverbs  &&  adverbs[0] &&  adverbs[0] != '0' &&  adverbs[0] != ' ' ?  adverbs : "",
     // extra    &&    extra[0] &&    extra[0] != '0' &&    extra[0] != ' ' ?    extra : "",
     // questionword

    struct fact** list = filter_list_by_rules (
        search_in_net(fact),
        fact
    );

    /*  Do not free, because search_synonyms returns WORD->synonyms, which has be kept
    
    
        free(fact->verbs);
        free(fact->subjects);
        free(fact->objects);
        free(fact->adverbs);
        free(fact->extra);
    */
    free(fact->questionword);
    free(fact);
    
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
    
    char* string = calloc(sizeof(char), size);
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
            sprintf(record[offset+(d*6)+5], "%p.0", (int)(list[d]->truth));
            
            column_count = offset+(d*6)+6;
            
            if (offset+(d*6)+6*2 >= limit) {
                break;
            }
        }
    }
    
    return column_count;
}

struct DATASET as_dataset(struct fact** list) {
    struct DATASET set;
    set.size = 0;
    set.data = 0;
    
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
            char** record = calloc(sizeof(char*), sizeof(struct fact)*100);
            record[0] = calloc(sizeof(char), 10);
            sprintf(record[0], "%p", (int)(list[d]->pk));
            record[1] = join_words(list[d]->verbs);
            record[2] = join_words(list[d]->subjects);
            record[3] = join_words(list[d]->objects);
            record[4] = join_words(list[d]->adverbs);
            record[5] = strdup("50");
            record[6] = strdup(list[d]->from ? list[d]->from : "");
            record[7] = calloc(sizeof(char), 10);
            sprintf(record[7], "%p.0", (int)(list[d]->truth));
            
            int column_count = append_on_dataset_record(8, 100, record, search_clauses(list[d]->pk));
            if (column_count > set.column_count)
                set.column_count = column_count;
            
            set.data[d] = record;
            printf("transformed record no %d.\n", d);
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
         || 0 == strcmp(word, "in")
         || 0 == strcmp(word, "an")
    
         ?  1
         :  0
    );
}
