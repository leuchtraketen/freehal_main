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

#ifndef HAL2009_TALK
#define HAL2009_TALK 1

#if defined(__cplusplus) /* GENERATED */

#endif /* GENERATED */

// functions begin
EXTERN_C char* generalize_verb(char* _verb);
#if defined(__cplusplus) /* GENERATED */
char* fact_read_from_source (const char* source);
char* hal2012_make_csv(struct DATASET* set);
const string filter_empty_str(const string& s);
const string& hal2012_get_text_language();
int fact_delete_from_source (const char* source);
int fact_replace_in_source (const char* source, const char* _replacement);
int filter_empty_int(const string& _s);
int hal2012_add_filename(const char* filename);
int hal2012_add_link (char* link, int key_1, int key_2);
int hal2012_add_xml_file (const string& filename);
int hal2012_compile(const string& file, const string& planguage);
int hal2012_delete_everything_from(const char* filename);
int hal2012_execute_code(const string& code, const string& planguage);
int hal2012_execute_file(const string& file, const string& planguage);
int hal2012_set_text_language(const string& s);
pthread_t hal2012_answer(const string& _input, const string& planguage, const string& tlanguage, const string& base_dir, int join, short start_type);
struct DATASET hal2012_get_csv(const char* csv_request);
void* hal2012_answer_thread(void* parameters);
void hal2012_clean();
void hal2012_init(const string& planguage, const string& tlanguage, const string& base_dir);
#endif /* GENERATED */
// functions end
#endif /* HAL2009_TALK */

