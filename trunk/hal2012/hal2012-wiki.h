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

#ifndef HAL2012_WIKI
#define HAL2012_WIKI 1

#if defined(__cplusplus) /* GENERATED */

#include "hal2012-wiki-dummy.h"
#endif /* GENERATED */

// functions begin
EXTERN_C int wiki_begin();
EXTERN_C struct fact** search_facts_wiki(const char* _entity, short todo);
#if defined(__cplusplus) /* GENERATED */
const string define_general_verb(const string& sentence, const string& entity);
int is_no_wiki_word(const string& word);
string delete_articles(const string& _s);
string delete_bad_chars(const string& _s, short with_star, short with_underscore);
string remove_between(const string& orig, char start, char stop);
string replace_spaces(const string& _s);
string transform_sentence(const string& _sentence, const string& entity);
string upper (const string& _s);
struct fact** search_facts_wiki_page(const string& __url, const string& entity_upper);
#endif /* GENERATED */
// functions end
#endif /* HAL2012_WIKI */

