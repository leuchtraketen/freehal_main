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

#include "hal2009-semtree.h"


int semtree_begin() {
    static int called = 0;
    if (called) {
        return 1;
    }
    
    // initialize semantic net
    net = calloc(sizeof(void*)*(4+'z'-'a'), 1);
    int i;
    for (i = n('a'); i <= n('z'); ++i) {
        net[i] = calloc(sizeof(void*)*(4+'z'-'a'), 1);
        
        int k;
        for (k = n('a'); k <= n('z'); ++k) {
            net[i][k] = calloc(sizeof(struct list), 1);
            net[i][k]->size = 0;
            net[i][k]->list = 0;
        }
        
        k = WRONG;
        net[i][k] = calloc(sizeof(struct list), 1);
        net[i][k]->size = 0;
        net[i][k]->list = 0;
    }
    i = WRONG;
    net[i] = calloc(sizeof(void*)*(4+'z'-'a'), 1);
    int k;
    for (k = n('a'); k <= n('z'); ++k) {
        net[i][k] = calloc(sizeof(struct list), 1);
        net[i][k]->size = 0;
        net[i][k]->list = 0;
    }
    k = WRONG;
    net[i][k] = calloc(sizeof(struct list), 1);
    net[i][k]->size = 0;
    net[i][k]->list = 0;
    
    // initialize fact_by_key array
    fact_by_key                  = calloc(sizeof(struct list), 1);
    fact_by_key->size            = 0;
    fact_by_key->allocated_until = 0;
    fact_by_key->list            = calloc(sizeof(struct fact*), fact_by_key->allocated_until+1);
    
    
    // done
    called = 1;
    
    return 0;
}

struct word* word_from_net(const char* name) {
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
    
    struct word** list = (struct word**)(net[i][k]->list);
    
    if (0 == list) {
        debugf("illegal list while searching %s.\n", name);
        return 0;
    }
    
    int g;
    for (g = 0; g < net[i][k]->size; ++g) {
        if (length == list[g]->length && 0 == strcmp(list[g]->name, name)) {
            debugf("found: %s = %d.\n", name, list[g]);
            return list[g];
        }
    }
    
    return 0;
}

int can_be_a_pointer(void* _p) {
    int p = (int)_p;
    if (p < 0) p = -p;
    if (p > 10000 || p < -10000) {
        return 1;
    }
    return 0;
}

static int in_search_synonyms = 0;
struct word*** search_synonyms(const char* exp) {
    struct word**  words    = divide_words(exp);
    struct word*** synonyms;
    int allocated_until     = 1;
    synonyms                = calloc(sizeof(struct word**), allocated_until+1);
    synonyms[0]             = calloc(sizeof(struct word*), strlen(exp)+10);
    int position, f;
    for (position = 0, f = 0; words[f]; ++f) {
        synonyms[0][position] = words[f];
        ++position;
    }
    free(words);
    
    if (!in_search_synonyms && !strstr(exp, "|")) {
        struct word* whole_word = insert_word_into_net(exp);
        if (whole_word && whole_word->name) {
            debugf("Synonyms of phrase %s are at %d.\n", exp, whole_word->synonyms);
            if (whole_word->synonyms && can_be_a_pointer(whole_word->synonyms) && whole_word->synonyms[0] && can_be_a_pointer(whole_word->synonyms[0])) {
                return whole_word->synonyms;
            }
            
            in_search_synonyms = 1;
            
            struct fact** facts_with_objects  = answer_fact_in_net(exp, "", "bi|bin|bist|ist|sind|seid|sein|heisst|heisse|heissen", "", "", "");
            struct fact** facts_with_subjects = answer_fact_in_net("", exp, "bi|bin|bist|ist|sind|seid|sein|heisst|heisse|heissen", "", "", "");
            
            int size;
            for (size = 0, f = 0; facts_with_objects[f]; ++f) {
                if (can_be_a_pointer(facts_with_objects[f]) && can_be_a_pointer(facts_with_objects[f]->objects) && facts_with_objects[f]->objects[0]) {
                    ++size;
                }
            }
            for (f = 0; facts_with_subjects[f]; ++f) {
                if (can_be_a_pointer(facts_with_subjects[f]) && can_be_a_pointer(facts_with_subjects[f]->subjects) && facts_with_subjects[f]->subjects[0]) {
                    ++size;
                }
            }
            allocated_until += size;
            synonyms = realloc(synonyms, sizeof(struct word**)*(allocated_until+1));
            for (f = allocated_until - size; f <= allocated_until; ++f) {
                synonyms[f] = 0;
            }
            
            for (position = 1, f = 0; facts_with_objects[f]; ++f) {
                if (   can_be_a_pointer(facts_with_objects[f])
                    && can_be_a_pointer(facts_with_objects[f]->objects)
                    && can_be_a_pointer(facts_with_objects[f]->objects[0])
                    && can_be_a_pointer(facts_with_objects[f]->objects[1])
                ) {
                    synonyms[position] = facts_with_objects[f]->objects;
                    ++position;
                }
            }
            for (f = 0; facts_with_subjects[f]; ++f) {
                if (   can_be_a_pointer(facts_with_subjects[f])
                    && can_be_a_pointer(facts_with_subjects[f]->subjects)
                    && can_be_a_pointer(facts_with_subjects[f]->subjects[0])
                    && can_be_a_pointer(facts_with_subjects[f]->subjects[1])
                ) {
                    
                    synonyms[position] = facts_with_subjects[f]->subjects;
                    ++position;
                }
            }
            
            whole_word->synonyms = synonyms;
            debugf("Synonyms of phrase %s are now at %d.\n", exp, whole_word->synonyms);
    
            in_search_synonyms = 0;
        }
    }
    
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
                struct word* word = insert_word_into_net(name);
                
                ++count;
                list = realloc(list, sizeof(struct word*)*(count+1));
                list[count] = 0;
                list[count-1] = word;
                debugf("divided: %s\n", name);
                a = e+1;
            }
        }
    }
    
    e = p;
    if (e - a > 0) {
        char* name = calloc(e-a+2, 1);
        strncpy(name, str+a, e-a);
        struct word* word = insert_word_into_net(name);
        
        ++count;
        list = realloc(list, sizeof(struct word*)*(count+1));
        list[count] = 0;
        list[count-1] = word;
        a = e;
    }
    
    if (list[0] == 0) {
        debugf("Useless name: %s (a = %d, e = %d)\n", str, a, e);
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

int insert_fact_by_list_into_net(struct word** list, struct fact* fact) {
    int n;
    for (n = 0; n < 100 && list[n]; ++n) {
        list[n]->related_facts = look_at_list(list[n]->related_facts);

        if (list[n]->related_facts->size == 0) {
            list[n]->related_facts->list = calloc(sizeof(struct fact*), 11);
            list[n]->related_facts->allocated_until = 10;
        }
        else if (list[n]->related_facts->size >= list[n]->related_facts->allocated_until) {
            list[n]->related_facts->allocated_until += 10;
            list[n]->related_facts->list = realloc(list[n]->related_facts->list, sizeof(struct word*)*(list[n]->related_facts->allocated_until+1));
        }
        
        list[n]->related_facts->list[list[n]->related_facts->size] = fact;
        ++(list[n]->related_facts->size);

        debugf("added fact %d to word %s (%d)\n", fact, list[n]->name, list[n]);
    }
    
    
    return 0;
}

int insert_fact_by_key(struct fact* fact) {
    if (fact_by_key->size >= fact_by_key->allocated_until) {
        fact_by_key->allocated_until += 20;
        fact_by_key->list = realloc(fact_by_key->list, sizeof(struct fact*)*(fact_by_key->allocated_until+1));
    }
    
    fact->pk  = fact_by_key->size;
    fact->rel = 0;
    fact_by_key->list[fact_by_key->size] = fact;
    
    ++fact_by_key->size;
}

int insert_fact_at_key(int rel, struct fact* fact) {
    if (!((struct fact*)((struct fact*)fact_by_key->list[rel]))->clauses) {
        ((struct fact*)fact_by_key->list[rel])->clauses = calloc(1, sizeof(struct fact*)*21);
    }
    int pos;
    for (pos = 0; ((struct fact*)fact_by_key->list[rel])->clauses[pos] && pos < 20; ++pos)
    { }
    
    fact->rel = rel;
    ((struct fact*)fact_by_key->list[rel])->clauses[pos] = fact;
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
    snprintf(str, 100, "%d", i);
    return str;
}

int is_bad(const char* s) {
    return (!s || !s[0] || s[0] == '0' || s[0] == ' ');
}

struct fact* insert_clause_at(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth) {
    if ((is_bad(subjects) && is_bad(objects)) || is_bad(verbs)) {
        return 0;
    }
    
    struct fact* fact  = calloc(sizeof(struct fact), 1);
    fact->subjects     = divide_words(subjects);
    fact->objects      = divide_words(objects);
    fact->verbs        = divide_words(verbs);
    fact->adverbs      = divide_words(adverbs);
    fact->extra        = divide_words(extra);
    fact->questionword = strdup(questionword);
    fact->from         = strdup(from);
    fact->truth        = truth;
    
    insert_fact_by_key(fact);
    insert_fact_at_key(rel, fact);
    
    printf("Inserted a fact at rel=%d.\n", rel);
    
    return fact;
}

struct fact* insert_fact_into_net(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth) {
    if ((is_bad(subjects) && is_bad(objects)) || is_bad(verbs)) {
        return 0;
    }

    struct fact* fact  = calloc(sizeof(struct fact), 1);
    fact->subjects     = divide_words(subjects);
    fact->objects      = divide_words(objects);
    fact->verbs        = divide_words(verbs);
    fact->adverbs      = divide_words(adverbs);
    fact->extra        = divide_words(extra);
    fact->questionword = strdup(questionword);
    fact->from         = strdup(from);
    fact->truth        = truth;
    
    insert_fact_by_list_into_net(fact->subjects, fact);
    insert_fact_by_list_into_net(fact->objects, fact);
    insert_fact_by_list_into_net(fact->adverbs, fact);
    insert_fact_by_key          (fact);
    
    return fact;
}

struct word* insert_word_into_net(const char* name) {
    if (!name || 0 == strlen(name)) {
        return 0;
    }
    struct word* already_there = word_from_net(name);
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
    
    if (0 == net[i][k]->list) {
        debugf("empty list wile inserting %s.\n", name);
    }
    else {
        0 && debugf("not empty list wile inserting %s: %d, %d entries, last entry = %s\n", name, net[i][k]->list, net[i][k]->size, ((struct word**)(net[i][k]->list))[net[i][k]->size-1]->name);
    }
    
    if (net[i][k]->size == 0) {
        net[i][k]->list = calloc(sizeof(struct word*), 11);
        net[i][k]->allocated_until = 10;
    }
    else if (net[i][k]->size >= net[i][k]->allocated_until) {
        net[i][k]->allocated_until += 10;
        net[i][k]->list = realloc(net[i][k]->list, sizeof(struct word*)*(net[i][k]->allocated_until+1));
    }
    
    net[i][k]->list[net[i][k]->size] = calloc(1, sizeof(struct word));
    ((struct word**)(net[i][k]->list))[net[i][k]->size]->name   = strdup(name);
    ((struct word**)(net[i][k]->list))[net[i][k]->size]->length = strlen(name);
    0 && debugf("inserted: %s = %d, %d.\n", 
            ((struct word**)(net[i][k]->list))[net[i][k]->size]->name,
            net[i][k]->list[net[i][k]->size],
            net[i][k]->size);
    ++(net[i][k]->size);
    return net[i][k]->list[net[i][k]->size - 1];
}

struct DATASET answer_fact_in_net_as_dataset(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword) {
    return as_dataset(answer_fact_in_net(subjects, objects, verbs, adverbs, extra, questionword));
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
        int does_match_with_this_synonym = 1;
        
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
            
            does_match_with_this_synonym = does_match_with_this_synonym && does_match_here;
        }
        
        does_match = does_match || does_match_with_this_synonym;
    }
    
    does_match = does_match || (!u);
    
    return does_match;
}

int fact_matches_subject_by_subject(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->subjects, EXACT);
    if (does_match == -1)
        does_match = 1;
    does_match && debugf("subject by subject: %d\n", does_match);
    return does_match;
}
int fact_matches_object_by_subject(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->subjects, EXACT);
    if (does_match == -1)
        does_match = 0;
    does_match && debugf("object by subject:  %d\n", does_match);
    return does_match;
}
int fact_matches_subject_by_object(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->subjects, request->objects, EXACT);
    if (does_match == -1)
        does_match = 0;
    debugf("subject by object:  %d\n", does_match);
    return does_match;
}
int fact_matches_object_by_object(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->objects, request->objects, EXACT);
    if (does_match == -1)
        does_match = 1;
    debugf("object by object:   %d\n", does_match);
    return does_match;
}
int fact_matches_adverb_by_adverb(struct fact* fact, struct request* request) {
    int does_match = fact_matches_entity_by_entity(fact->adverbs, request->adverbs, WEAK);
    if (does_match == -1)
        does_match = 1;
    debugf("adverb by adverb:   %d\n", does_match);
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
    debugf("anything by extra:  %d\n", does_match);
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
                if (0 == strcmp(fact->verbs[m]->name, splitted_by_pipe[p])) {
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
    int n, m, q;
    debugf("Searching facts for words (at %d). position is %d, limit is %d.\n", words, *position, limit);
    for (n = 0; *position < limit && can_be_a_pointer(words[n]); ++n) {
        int is_new = 1;
        for (q = 0; *position < limit && words[q] && q+1 < n; ++q) {
            if (words[n] == words[q])
                is_new = 0;
        }
        debugf("synonym no %d: %d, %d, %s\n",
            n,
            can_be_a_pointer(words[n]),
            can_be_a_pointer(words[n]) ? words[n][0] : 0,
            can_be_a_pointer(words[n]) && can_be_a_pointer(words[n][0]) ? words[n][0]->name : "(null)"
        );
        if (!is_new) {
            debugf("not new.\n");
            continue;
        }
        
        for (m = 0; *position < limit && can_be_a_pointer(words[n][m]) && words[n][m]->name && words[n][m]->name[0]; ++m) {
            if (0 == words[n][m]->related_facts) {
                continue;
            }
            if (is_a_trivial_word(words[n][m]->name)) {
                continue;
            }
            
            int size = words[n][m]->related_facts->size;
            debugf("word: %s, size of related facts array: %d.\n", words[n][m]->name, size);
            int z;
            for (z = 0; *position < limit && z < size; ++z) {
                int add_the_fact = 1;
                int e;
                for (e = 0; e < *position; ++e) {
                    if (facts[e] == words[n][m]->related_facts->list[z]) {
                        add_the_fact = 0;
                    }
                }
                if (add_the_fact) {
                    facts[*position] = words[n][m]->related_facts->list[z];
                    debugf("Added fact no %d at %d.\n", *position, facts[*position]);
                    ++(*position);
                }
            }
        }
    }
    
    return 0;
}

struct fact** search_in_net(struct request* fact) {
    int limit = 20000;
    struct fact** facts = calloc(1, sizeof(struct fact*)*(limit+1));
    
    int position = 0;
    search_facts_for_words_in_net(fact->subjects, facts, limit, &position);
    search_facts_for_words_in_net(fact->objects, facts, limit, &position);
    struct word** temp[2];
    temp[1] = 0;
    temp[0] = fact->verbs;
    search_facts_for_words_in_net(temp, facts, limit, &position);
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
            debugf("%d: %d %d", list[i], list[i][0], list[i][0]?list[i][1]:0);
        }
        debugf(")\n");
    }
    debugf("\n");
}

struct fact** answer_fact_in_net(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword) {
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
        "    subjects:\n");
    print_word_list_3rd_order(fact->subjects);
    debugf(
        "    objects:\n");
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
    if (rel <= fact_by_key->size) {
        printf("Valid fact no: %d.\n", rel);
        if (fact_by_key->list[rel]) {
            printf("Valid fact: %d.\n", rel);
            
            if (((struct fact*)(fact_by_key->list[rel]))->clauses) {
                printf("There are clauses for %d.\n", rel);
                
                struct fact** clauses = calloc(1, sizeof(struct fact*)*21);
                
                int i;
                for (i = 0; i < 20; ++i) {
                    printf("clause: %d.\n", ((struct fact*)(fact_by_key->list[rel]))->clauses[i]);
                    clauses[i] = ((struct fact*)(fact_by_key->list[rel]))->clauses[i];
                }
                
                return clauses;
            }
            else {
                printf("There are no clauses for %d.\n", rel);
            }
        }
        else {
            printf("Invalid fact: %d.\n", rel);
        }
    }
    else {
        printf("Invalid fact no: %d (there are %d facts).\n", rel, fact_by_key->size);
    }
    return 0;
}

int free_words_deep(struct word** words) {
    int c;
    for (c = 0; words[c]; ++c) {
        if (words[c]->name) free(words[c]->name);
    }
    free(words);
}

int free_facts_deep(struct fact** facts) {
    int c;
    for (c = 0; facts[c]; ++c) {
        free_words_deep(facts[c]->verbs);
        free_words_deep(facts[c]->subjects);
        free_words_deep(facts[c]->objects);
        free_words_deep(facts[c]->adverbs);
        free_words_deep(facts[c]->extra);
        
        free(facts[c]);
    }
    free(facts);
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
            sprintf(record[offset+(d*6)+5], "%d.0", (int)(list[d]->truth));
            
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
            sprintf(record[0], "%d", (int)(list[d]->pk));
            record[1] = join_words(list[d]->verbs);
            record[2] = join_words(list[d]->subjects);
            record[3] = join_words(list[d]->objects);
            record[4] = join_words(list[d]->adverbs);
            record[5] = strdup("50");
            record[6] = strdup(list[d]->from ? list[d]->from : "");
            record[7] = calloc(sizeof(char), 10);
            sprintf(record[7], "%d.0", (int)(list[d]->truth));
            
            int column_count = append_on_dataset_record(8, 100, record, search_clauses(list[d]->pk));
            if (column_count > set.column_count)
                set.column_count = column_count;
            
            set.data[d] = record;
        }
    }
    
    free(list);
    
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
    
         ?  1
         :  0
    );
}
