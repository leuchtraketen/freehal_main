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

#include "hal2009.h"

#ifndef HAL2009_SQL_UNIVERSAL
#define HAL2009_SQL_UNIVERSAL 1

#if defined(__cplusplus) /* GENERATED */

#endif /* GENERATED */

// functions begin
#if defined(__cplusplus) /* GENERATED */
char* sql_universal_del_record(struct RECORD* r);
char* sql_universal_get_source(struct RECORD* r);
int sql_universal_add_filename(const char* filename);
int sql_universal_add_record(struct RECORD* r, const char* relation_to);
int sql_universal_begin(const char* modes);
int sql_universal_delete_everything_from(const char* filename);
int sql_universal_end();
int sql_universal_re_index();
int sql_universal_set_filename(const char* filename);
struct DATASET sql_universal_get_records(struct RECORD* r);
#endif /* GENERATED */
// functions end
#endif /* HAL2009_SQL_UNIVERSAL */

