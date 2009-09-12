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


#define n(ch)       ((1-'a'+ch)<=  0 ? WRONG : ((1-'a'+ch) > 27  ? WRONG : (1-'a'+ch)))
#define c(nm)       ((nm-1+'a')< 'a' ? WRONG : ((nm-1+'a') > 'z' ? WRONG : (nm-1+'a')))
#define is_wrong(x) (x == 'z'+1)
#define WRONG       ('z'-'a'+2)

#define EXACT       2
#define WEAK        3

#define DEBUG       0

#if DEBUG
# define debugf      printf
#else
# define debugf      _debugf
#endif

struct fact;
struct word;

struct fact {
    struct word** subjects;
    struct word** objects;
    struct word** verbs;
    struct word** adverbs;
    struct word** extra;
    struct word* questionword;
    struct fact** clauses;
    char* from;
    float truth;
    int pk;
    int rel;
};

struct request {
    struct word*** subjects;
    struct word*** objects;
    struct word** verbs;
    struct word*** adverbs;
    struct word*** extra;
    char* questionword;
    struct list* clauses;
    char* from;
    float truth;
};

struct word {
    char* name;
    int length;
    struct list* related_facts;
    struct word*** synonyms;
};

struct list {
    long size;
    void** list;
    long allocated_until;
};

static struct list*** net = 0;
static struct list*   fact_by_key = 0;

int semtree_begin();
struct word* word_from_net(const char* name);
int can_be_a_pointer(void* p);
struct word*** search_synonyms(const char* exp);
struct word** divide_words(const char* str);
char** divide_by(const char by, const char* str);
struct list* look_at_list(struct list* l);
int insert_fact_by_list_into_net(struct word** list, struct fact* fact);
int to_number(const char* str);
char* from_number(int i);
struct fact* insert_clause_at(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth);
struct fact* insert_fact_into_net(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* from, float truth);
struct word* insert_word_into_net(const char* name);
struct DATASET answer_fact_in_net_as_dataset(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword);
struct DATASET search_clauses_as_dataset(int rel);
int fact_matches_truth(struct fact* fact, struct request* request);
int fact_matches_entity_by_entity(struct word** words, struct word*** request_words, int flags);
int fact_matches_subject_by_subject(struct fact* fact, struct request* request);
int fact_matches_object_by_subject(struct fact* fact, struct request* request);
int fact_matches_subject_by_object(struct fact* fact, struct request* request);
int fact_matches_object_by_object(struct fact* fact, struct request* request);
int fact_matches_adverb_by_adverb(struct fact* fact, struct request* request);
int fact_matches_anything_by_extra(struct fact* fact, struct request* request);
int fact_matches_verb(struct fact* fact, struct request* request);
struct fact* filter_fact_by_rules(struct fact* fact, struct request* request);
struct fact** filter_list_by_rules(struct fact** list, struct request* request);
int search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position);
struct fact** search_in_net(struct request* fact);
void print_word_list_3rd_order(struct word*** list);
struct fact** answer_fact_in_net(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword);
struct fact** search_clauses(int rel);
int free_words_deep(struct word** words);
int free_facts_deep(struct fact** facts);
char* join_words(struct word** words);
char* join_words_by(const char* by, struct word** words);
int append_on_dataset_record(int offset, int limit, char** record, struct fact** list);
struct DATASET as_dataset(struct fact** list);



int _debugf(const char* c, ...) {
}


