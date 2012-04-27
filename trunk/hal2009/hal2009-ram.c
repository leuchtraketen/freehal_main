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

#include "hal2009-universal.h"
#include "hal2009-ram.h"
#include "hal2009-util.h"

int ram_begin(const char* modes) {
    static int called = 0;
    if (called) {
        return 1;
    }

    // initialize semantic ram_net
    ram_net = calloc(sizeof(void*)*(4+'z'-'a'), 1);
    int i;
    for (i = n('a'); i <= n('z'); ++i) {
        ram_net[i] = calloc(sizeof(void*)*(4+'z'-'a'), 1);

        int k;
        for (k = n('a'); k <= n('z'); ++k) {
            ram_net[i][k] = calloc(sizeof(struct flist), 1);
            ram_net[i][k]->size = 0;
            ram_net[i][k]->list = 0;
        }
        for (k = n('0'); k <= n('9'); ++k) {
            ram_net[i][k] = calloc(sizeof(struct flist), 1);
            ram_net[i][k]->size = 0;
            ram_net[i][k]->list = 0;
        }

        k = WRONG;
        ram_net[i][k] = calloc(sizeof(struct flist), 1);
        ram_net[i][k]->size = 0;
        ram_net[i][k]->list = 0;
    }
    for (i = n('0'); i <= n('9'); ++i) {
        ram_net[i] = calloc(sizeof(void*)*(4+'z'-'a'), 1);

        int k;
        for (k = n('a'); k <= n('z'); ++k) {
            ram_net[i][k] = calloc(sizeof(struct flist), 1);
            ram_net[i][k]->size = 0;
            ram_net[i][k]->list = 0;
        }
        for (k = n('0'); k <= n('9'); ++k) {
            ram_net[i][k] = calloc(sizeof(struct flist), 1);
            ram_net[i][k]->size = 0;
            ram_net[i][k]->list = 0;
        }

        k = WRONG;
        ram_net[i][k] = calloc(sizeof(struct flist), 1);
        ram_net[i][k]->size = 0;
        ram_net[i][k]->list = 0;
    }
    i = WRONG;
    ram_net[i] = calloc(sizeof(void*)*(4+'z'-'a'), 1);
    int k;
    for (k = n('a'); k <= n('z'); ++k) {
        ram_net[i][k] = calloc(sizeof(struct flist), 1);
        ram_net[i][k]->size = 0;
        ram_net[i][k]->list = 0;
    }
    for (k = n('0'); k <= n('9'); ++k) {
        ram_net[i][k] = calloc(sizeof(struct flist), 1);
        ram_net[i][k]->size = 0;
        ram_net[i][k]->list = 0;
    }
    k = WRONG;
    ram_net[i][k] = calloc(sizeof(struct flist), 1);
    ram_net[i][k]->size = 0;
    ram_net[i][k]->list = 0;

    // initialize ram_fact_by_key array
    ram_fact_by_key                  = calloc(sizeof(struct flist), 1);
    ram_fact_by_key->size            = 0;
    ram_fact_by_key->allocated_until = 0;
    ram_fact_by_key->list            = calloc(sizeof(struct fact*), ram_fact_by_key->allocated_until+1);


    // initialize ram_linking array
    ram_linking                  = calloc(sizeof(struct flist), 1);
    ram_linking->size            = 0;
    ram_linking->allocated_until = 10;
    ram_linking->list            = calloc(sizeof(struct fact*), ram_linking->allocated_until+1);

    // done
    called = 1;

    return 0;
}

int ram_end() {
    return 0;
}

struct word* ram_get_word(const char* name) {
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

    struct word** list = (struct word**)(ram_net[i][k]->list);

    if (0 == list) {
        // debugf("illegal list while searching %s.\n", name);
        return 0;
    }

    int g;
    for (g = 0; g < ram_net[i][k]->size; ++g) {
        if (length == list[g]->length && 0 == strcmp(list[g]->name, name)) {
            //debugf("found: %s = %d.\n", name, list[g]);
            return list[g];
        }
    }

    return 0;
}

int insert_fact_by_list_into_ram_net(struct word** list, struct fact* fact) {
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
    }


    return 0;
}

int insert_ram_fact_by_key(struct fact* fact) {
    if (ram_fact_by_key->size >= ram_fact_by_key->allocated_until) {
        ram_fact_by_key->allocated_until += 20;
        ram_fact_by_key->list = realloc(ram_fact_by_key->list, sizeof(struct fact*)*(ram_fact_by_key->allocated_until+1));
    }

    fact->pk  = ram_fact_by_key->size;
    fact->rel = 0;
    ram_fact_by_key->list[ram_fact_by_key->size] = fact;

    ++ram_fact_by_key->size;
}

int insert_fact_at_key(int rel, struct fact* fact) {
    if (!((struct fact*)((struct fact*)ram_fact_by_key->list[rel]))->clauses) {
        ((struct fact*)ram_fact_by_key->list[rel])->clauses = calloc(1, sizeof(struct fact*)*21);
    }
    int pos;
    for (pos = 0; ((struct fact*)ram_fact_by_key->list[rel])->clauses[pos] && pos < 20; ++pos)
    { }

    fact->rel = rel;
    ((struct fact*)ram_fact_by_key->list[rel])->clauses[pos] = fact;
}


struct fact* ram_add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should) {
    if ((is_bad(subjects) && is_bad(objects)) && is_bad(verbs)) {
        return 0;
    }

    struct fact* fact  = calloc(sizeof(struct fact), 1);
    fact->subjects     = divide_words(subjects);
    fact->objects      = divide_words(objects);
    fact->verbs        = divide_words(verbs);
    fact->adverbs      = divide_words(adverbs);
    fact->extra        = divide_words(extra);
    fact->questionword = strdup(questionword);
    fact->filename     = strdup(filename);
    fact->line         = strdup(line);
    fact->truth        = truth;

    insert_ram_fact_by_key(fact);
    insert_fact_at_key(rel, fact);

//    printf("Inserted a fact at rel=%d.\n", rel);

    return fact;
}

struct fact* ram_add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword) {
    if ((is_bad(subjects) && is_bad(objects)) && is_bad(verbs)) {
        return 0;
    }

    struct fact* fact  = calloc(sizeof(struct fact), 1);
    fact->subjects     = divide_words(subjects);
    fact->objects      = divide_words(objects);
    fact->verbs        = divide_words(verbs);
    fact->adverbs      = divide_words(adverbs);
    fact->extra        = divide_words(extra);
    fact->questionword = strdup(questionword);
    fact->filename     = strdup(filename);
    fact->line         = strdup(line);
    fact->truth        = truth;
    fact->only_logic   = only_logic;

    insert_fact_by_list_into_ram_net(fact->subjects, fact);
    insert_fact_by_list_into_ram_net(fact->objects, fact);
    insert_fact_by_list_into_ram_net(fact->adverbs, fact);
    insert_ram_fact_by_key          (fact);

    return fact;
}

struct word* ram_set_word(const char* name) {
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

    if (0 == ram_net[i][k]->list) {
        // debugf("empty list wile inserting %s.\n", name);
    }
    else {
        // debugf("not empty list wile inserting %s: %d, %d entries, last entry = %s\n", name, ram_net[i][k]->list, ram_net[i][k]->size, ((struct word**)(ram_net[i][k]->list))[ram_net[i][k]->size-1]->name);
    }

    if (ram_net[i][k]->size == 0) {
        ram_net[i][k]->list = calloc(sizeof(struct word*), 11);
        ram_net[i][k]->allocated_until = 10;
    }
    else if (ram_net[i][k]->size >= ram_net[i][k]->allocated_until) {
        ram_net[i][k]->allocated_until += 10;
        ram_net[i][k]->list = realloc(ram_net[i][k]->list, sizeof(struct word*)*(ram_net[i][k]->allocated_until+1));
    }

    ram_net[i][k]->list[ram_net[i][k]->size] = calloc(1, sizeof(struct word));
    ((struct word**)(ram_net[i][k]->list))[ram_net[i][k]->size]->name   = strdup(name);
    ((struct word**)(ram_net[i][k]->list))[ram_net[i][k]->size]->length = strlen(name);
    0 && debugf("inserted: %s = %li, %li.\n",
            ((struct word**)(ram_net[i][k]->list))[ram_net[i][k]->size]->name,
            (long)ram_net[i][k]->list[ram_net[i][k]->size],
            ram_net[i][k]->size);
    ++(ram_net[i][k]->size);
    return ram_net[i][k]->list[ram_net[i][k]->size - 1];
}


int ram_search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position) {
    int n, m, q;
    debugf("Searching facts for words (at %li). position is %d, limit is %d.\n", (long)words, *position, limit);
    for (n = 0; *position < limit && can_be_a_pointer(words[n]); ++n) {
        int is_new = 1;
        for (q = 0; *position < limit && words[q] && q+1 < n; ++q) {
            if (words[n] == words[q])
                is_new = 0;
        }
        debugf("synonym no %d: %li, %li, %s\n",
            n,
            (long)can_be_a_pointer(words[n]),
            (long)(can_be_a_pointer(words[n]) ? words[n][0] : 0),
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
                    //debugf("Added fact no %d at %d.\n", *position, facts[*position]);
                    ++(*position);
                }
            }
        }
    }

    return 0;
}

struct fact** related_facts_of_word(struct word* key, struct fact** facts, int limit, int* position) {
    if (!facts) {
        facts = calloc(1, sizeof(struct fact*)*(limit+1));
    }

    int size = key->related_facts->size;
    debugf("word: %s, size of related facts array: %d.\n", key->name, size);
    int z;
    for (z = 0; *position < limit && z < size; ++z) {
        int add_the_fact = 1;
        int e;
        for (e = 0; e < *position; ++e) {
            if (facts[e] == key->related_facts->list[z]) {
                add_the_fact = 0;
            }
        }
        if (add_the_fact) {
            facts[*position] = key->related_facts->list[z];
            ++(*position);
        }
    }

    return facts;
}

struct fact** related_facts_of_str(const char* key, struct fact** facts, int limit, int* position) {
    return related_facts_of_word(set_word(key), facts, limit, position);
}

const char* ram_get_thesaurus_synonyms(const char* key, const char** keys, struct string_pair** facts, int limit, int* position, int level, short reverse) {
    printf("ram_get_thesaurus_synonyms: %s %li\n", key, (long int)keys);
    if (!keys && (!key || !key[0]))
        return (char*)1;

    int free_keys = 0;
    if (key && !keys) {
        keys = calloc(sizeof(char*), 2);
        keys[0] = strdup(key);
        key = 0;
        free_keys = 1;
    }

    int i;

    // first level
    int position_before_level_1 = *position;
    if (level >= 1) {
        int _position = 0;
        int k;
        for (k = 0; keys[k]; ++k) {
            struct fact** unfiltered = related_facts_of_str(keys[k], 0, limit, &_position);
            for (i = 0; unfiltered[i]; ++i) {
                if (can_be_a_pointer(unfiltered[i])) {
                    char* joined_subjects = join_words(unfiltered[i]->subjects);
                    char* joined_objects = join_words(unfiltered[i]->objects);
                    char* joined_verbs = join_words(unfiltered[i]->verbs);
                    if (strstr(joined_verbs, "=") && 0 == strcmp(joined_objects, key) && !strstr(unfiltered[i]->filename, "fa-")) {
                        struct string_pair* pair  = calloc(sizeof(struct string_pair), 1);
                        pair->subjects     = strdup(joined_subjects);
                        pair->objects      = strdup(joined_objects);
                        facts[*position] = pair;
                        ++(*position);
                    }
                    free(joined_subjects);
                    free(joined_objects);
                    free(joined_verbs);
                }
            }
            free(unfiltered);
        }
    }

    // second level
    int position_before_level_2 = *position;
    if (level >= 2) {
        int _position = 0;
        int k;
        for (k = position_before_level_1; k < position_before_level_2; ++k) {
            int _position_last = _position;
            struct fact** unfiltered = related_facts_of_str(facts[k]->subjects, unfiltered, limit, &_position);
            char* level_1_joined_subjects = facts[k]->subjects;
            printf("thesaurus-synonyms (ram): %s\n", level_1_joined_subjects);

            for (i = _position_last; unfiltered[i]; ++i) {
                if (can_be_a_pointer(unfiltered[i])) {
                    char* joined_subjects = join_words(unfiltered[i]->subjects);
                    char* joined_objects = join_words(unfiltered[i]->objects);
                    char* joined_verbs = join_words(unfiltered[i]->verbs);

                    if ((strstr(joined_verbs, "=") || strstr(joined_verbs, "ist")) && strstr(joined_objects, level_1_joined_subjects) && !strstr(unfiltered[i]->filename, "fa-")) {
                        struct string_pair* pair  = calloc(sizeof(struct string_pair), 1);
                        pair->subjects     = strdup(joined_subjects);
                        pair->objects      = strdup(joined_objects);
                        facts[*position] = pair;
                        ++(*position);
                    }
                    free(joined_subjects);
                    free(joined_objects);
                    free(joined_verbs);
                }
            }
            free(unfiltered);
        }
    }

    if (free_keys) free(keys);
}

struct fact** ram_search_clauses(int rel) {
    if (rel <= ram_fact_by_key->size) {
        printf("Valid fact no: %d.\n", rel);
        if (ram_fact_by_key->list[rel]) {
            printf("Valid fact: %d.\n", rel);

            if (((struct fact*)(ram_fact_by_key->list[rel]))->clauses) {
                printf("There are clauses for %d.\n", rel);

                struct fact** clauses = calloc(1, sizeof(struct fact*)*21);

                int i;
                for (i = 0; i < 20; ++i) {
                    printf("clause: %li.\n", (long) ((struct fact*)(ram_fact_by_key->list[rel]))->clauses[i]);
                    clauses[i] = ((struct fact*)(ram_fact_by_key->list[rel]))->clauses[i];
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
        printf("Invalid fact no: %d (there are %li facts).\n", rel, ram_fact_by_key->size);
    }
    return 0;
}

int ram_set_to_invalid_value(void** p) {
    if (!p) return 1;
    *p = INVALID_POINTER;
    return 0;
}

int ram_add_link (const char* link, int key_1, int key_2) {
    if (ram_linking->size >= ram_linking->allocated_until) {
        ram_linking->allocated_until += 20;
        ram_linking->list = realloc(ram_linking->list, sizeof(struct fact*)*(ram_linking->allocated_until+1));
    }

    struct linking_entity* entity = calloc(sizeof(struct linking_entity), 1);
    entity->_1 = key_1;
    entity->_2 = key_2;
    entity->type = strdup(link);
    ram_linking->list[ram_linking->size] = entity;

    ++ram_linking->size;
    return 0;
}


