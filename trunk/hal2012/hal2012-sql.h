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

#ifndef HAL2009_SQL
#define HAL2009_SQL 1

#if defined(__cplusplus) /* GENERATED */

#endif /* GENERATED */

// functions begin
EXTERN_C const char* engine();
EXTERN_C const char* is_engine(const char* m);
EXTERN_C int sql_begin(const char* modes);
EXTERN_C int sql_end();
#if defined(__cplusplus) /* GENERATED */
char* sql_del_record(struct RECORD* r);
char* sql_get_source(struct RECORD* r);
int sql_add_filename(const char* filename);
int sql_add_link (char* link, int key_1, int key_2);
int sql_add_record(struct RECORD* r);
int sql_delete_everything_from(const char* filename);
int sql_re_index();
int sql_set_quiet(int i);
struct DATASET sql_get_records(struct RECORD* r);
#endif /* GENERATED */
// functions end
#endif /* HAL2009_SQL */

