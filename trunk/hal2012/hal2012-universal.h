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

#include "hal2012.h"

BEGIN_EXTERN_C /* GENERATED */
#ifndef HAL2012_UNIVERSAL
#define HAL2012_UNIVERSAL 1

#define n(ch)       ((1-'a'+ch)<=  0 ? WRONG : ((1-'a'+ch) > 27  ? WRONG : (1-'a'+ch)))
#define c(nm)       ((nm-1+'a')< 'a' ? WRONG : ((nm-1+'a') > 'z' ? WRONG : (nm-1+'a')))
#define is_wrong(x) (x == 'z'+1)
#define WRONG       ('z'-'a'+2)

#define TOOMUCH      -7
#define TOOMUCH_P    (void*) -7

#define REVERSE 1
#define FORWARD 0

#define VERY_EXACT   4
#define EXACT        2
#define WEAK         3
#define USE_SUBJECTS 11
#define USE_OBJECTS  12
#define NEW          21
#define CONTINUE     22

#define DEBUG       1

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
    char* questionword;
    struct fact** clauses;
    char* filename;
    char* line;
    float truth;
    int pk;
    int rel;

    short only_logic;
};

struct request {
    struct word*** subjects;
    struct word*** objects;
    struct word** verbs;
    struct word*** adverbs;
    short adverbs_any;
    struct word*** extra;
    char* questionword;
    char* context;
    struct flist* clauses;
    char* filename;
    char* line;
    float truth;

    short only_logic;
    short has_conditional_questionword;
};

struct synonym {
    struct word** words;
    int level;
    short direction;
};

struct synonym_set {
    struct synonym** subjects;
    struct synonym** objects;
    struct synonym** adverbs;
    struct synonym** extra;

    int position_subjects;
    int position_objects;
    int position_adverbs;
    int position_extra;

    short subject_object_switched;
};

struct word {
    char* name;
    int length;
    struct flist* related_facts;
    struct word*** synonyms;
    int synonyms_level;
};

struct flist {
    long size;
    void** list;
    long allocated_until;
};

struct string_pair {
    char* subjects;
    char*  objects;
};

extern struct word** variable_replacements;
extern int variable_replacements_c;

// functions begin
char** divide_by(const char by, const char* str);
char** divide_by_logical_operators(const char* exp);
char** divide_string(const char* str);
char* from_number(int i);
char* join_words_by(const char* by, struct word** words);
char* join_words(struct word** words);
char* small_identifier(const char* word);
char** synonyms_to_expressions(struct synonym** synonyms, int a, int b);
char* universal_del_record(char* key);
char* universal_get_source(char* key);
char* use_thesaurus_synonyms(const char* key);
const char* clean_str (const char* str);
const char* get_catchf();
const char* get_thesaurus_synonyms(const char* key, const char** keys, struct string_pair** facts, int limit, int* position, int level, short reverse);
int append_on_dataset_record(int offset, int limit, char** record, struct fact** list);
int begin_catchf();
int catchf(const char* fmt, void* arg1);
int check_synonym(struct synonym* syn_a, struct synonym** synonyms, int a, int b);
int clear_catchf();
int copy_list(int a_1, int a_2, int b_1, int b_2, void** a, void** b, int(*except)(const void*));
int count_list(void** list);
int count_of_synonym(struct word*** words);
int delete_in_first_if_in_second(struct word*** delete_in, struct word*** find_in);
int delete_in_first_if_second(struct word*** list, const char* exp);
int _diff(int a, int b);
int divide_and_round_up(int a, int b);
int end_catchf();
int except_article(const void* _a);
int fact_matches_adverb_by_adverb(struct fact* fact, struct request* request, int weak);
int fact_matches_adverb_by_object(struct fact* fact, struct request* request, int weak);
int fact_matches_anything_by_extra(struct fact* fact, struct request* request);
int fact_matches_entity_by_entity(struct word** words, struct word*** request_words, int flags, const char* comment);
int fact_matches_object_by_adverb(struct fact* fact, struct request* request, int weak);
int fact_matches_object_by_object(struct fact* fact, struct request* request, int weak);
int fact_matches_object_by_subject(struct fact* fact, struct request* request, int weak);
int fact_matches_questionword_rules_of_q_from_where(struct fact* fact, struct request* request);
int fact_matches_questionword_rules_of_q_how(struct fact* fact, struct request* request);
int fact_matches_questionword_rules_of_q_what_exactly(struct fact* fact, struct request* request);
int fact_matches_questionword_rules_of_q_what_extra(struct fact* fact, struct request* request);
int fact_matches_questionword_rules_of_q_what_weakly(struct fact* fact, struct request* request);
int fact_matches_questionword_rules_of_q_where(struct fact* fact, struct request* request);
int fact_matches_questionword_rules_of_q_who(struct fact* fact, struct request* request);
int fact_matches_questionword_rules_of_search_reasons(struct fact* fact, struct request* request);
int fact_matches_questionword_rules(struct fact* fact, struct request* request);
int fact_matches_subject_by_object(struct fact* fact, struct request* request, int weak);
int fact_matches_subject_by_subject(struct fact* fact, struct request* request, int weak);
int fact_matches_truth(struct fact* fact, struct request* request);
int fact_matches_verb(struct fact* fact, struct request* request);
int fileid(const char* filename);
int free_expressions(char** exps);
int free_facts_deep(struct fact** facts);
int free_facts_synonyms(struct fact** facts_synonyms);
int free_request(struct request* fact);
int free_string_pair(struct string_pair** sp);
int free_synonym_set(struct synonym_set* synonym_set);
int free_synonym(struct synonym* syn);
int free_words_deep(struct word** words);
int free_words_weak(struct word** words);
int higher(int a, int b);
int is_bad(const char* s);
int is_good(const char* s);
int matches(const char* a, const char* b);
int matches_with_pipe(const char* a, const char* b);
int net_go_way(const char* way, int value, struct request* fact, struct fact** facts, int limit, int* position);
int net_supports_way(const char* way, int value);
int next_in_list(void** list);
int search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position);
int set_to_invalid_fact(struct fact** p);
int set_to_invalid_value(void** p);
int sql_universal_add_link (const char* link, int key_1, int key_2);
int to_number(const char* str);
int universal_add_filename(const char* filename);
int universal_begin(const char* modes);
int universal_clear_cache();
int universal_delete_everything_from(const char* filename);
int universal_end();
int universal_re_index();
int word_matches_word_array(struct word* word, struct word** words, int words_begin, int words_end, int flags);
long can_be_a_pointer(void* _p);
struct DATASET as_dataset(struct fact** list);
struct DATASET search_clauses_as_dataset(int rel);
struct DATASET search_facts_as_dataset(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context);
struct fact* add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should);
struct fact* add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword);
struct fact* filter_fact_by_rules(struct fact* fact, struct request* request, int weak);
struct fact** filter_list_by_rules(struct fact** list, struct request* request, int weak);
struct fact* replace_variables_in_fact(struct fact* fact);
struct fact** replace_variables_in_list(struct fact** list);
struct fact** search_clauses(int rel);
struct fact** search_facts_by_weakness(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak);
struct fact** search_facts(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context);
struct fact** search_facts_deep(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak);
struct fact** search_facts_double_facts(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak);
struct fact** search_facts_simple(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int level, int weak);
struct fact** search_facts_synonyms(const char* subjects, const char** subjects_array, const char* objects, const char** objects_array, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context);
struct fact** search_facts_thesaurus(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, int weak);
struct fact** search_in_net(struct request* fact, struct fact** list, const char* hints);
struct flist* look_at_list(struct flist* l);
struct request* negotiate_deep_search(struct synonym_set* synonym_set, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* context, const struct fact* if_clause, const struct fact* then_clause, int level, int direction);
struct synonym** add_synonyms_by_search(const char** exps, struct synonym** synonyms, int* position, const char* category);
struct synonym** add_synonyms_by_string(const char** exps, struct synonym** synonyms, int* position, const char* category);
struct synonym** add_synonyms(const char* exp, const char** _exps, struct synonym** synonyms, int* position, const char* category);
struct synonym* construct_synonym(const char* _exp, const char** _exps, struct word* word, struct word** words, int level, int direction);
struct synonym** prepare_synonyms();
struct synonym** put_search_synonyms(const char* subj, const char* obj, const char* verb, const char* adverbs, int use_what, struct fact** facts, struct synonym** synonyms, int* position, int level, short direction);
struct synonym** put_synonym_raw(struct synonym* syn, struct synonym** synonyms, int* position);
struct synonym** put_synonym(struct synonym* syn, struct synonym** synonyms, int* position);
struct word** divide_words(const char* str);
struct word*** fake_synonyms(const char* exp, const char** exp_array);
struct word*** filter_synonyms(struct synonym** synonyms, int level, short direction);
struct word* get_word(const char* name);
struct word** replace_variables_in_words(struct word** list);
struct word* replace_variables_in_word(struct word* word);
struct word* set_word(const char* name);
void flowchart_write_list_of_synonym(struct word*** words);
void print_word_list_3rd_order(struct word*** list);
// functions end
#endif /* HAL2012_UNIVERSAL */
END_EXTERN_C /* GENERATED */
