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

#ifndef HAL2009_RAM_H
#define HAL2009_RAM_H 1

static struct list*** ram_net = 0;
static struct list*   ram_fact_by_key = 0;
static struct list*   ram_linking = 0;

struct linking_entity {
    int _1;
    int _2;
    char* type;
};

int ram_begin(const char* begin);
int ram_end();
struct word* ram_get_word(const char* name);
int insert_fact_by_list_into_net(struct word** list, struct fact* fact);
struct fact* ram_add_clause(int rel, const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should);
struct fact* ram_add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_learn, short has_conditional_questionword);
struct word* ram_set_word(const char* name);
int ram_search_facts_for_words_in_net(struct word*** words, struct fact** facts, int limit, int* position);
char* ram_get_thesaurus_synonyms(const char* key, struct string_pair** facts, int limit, int* position, int level, short reverse);
struct fact** ram_search_clauses(int rel);



#endif
