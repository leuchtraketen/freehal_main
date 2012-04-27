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

#ifndef HAL2009_UTIL
#define HAL2009_UTIL 1

#if defined(__cplusplus) /* GENERATED */

class params_answer {
  public:
    params_answer()
    : input(), planguage(), tlanguage(), base_dir(), start_type(SINGLE)
    {}

    string input;
    string planguage;
    string tlanguage;
    string base_dir;
    short start_type;
};
#endif /* GENERATED */

// functions begin
EXTERN_C char* check_config (const char* name, const char* _default);
EXTERN_C char* halgetline(FILE *handle);
EXTERN_C int cstat (char* c, struct stat* s);
EXTERN_C int halsleep(double seconds);
EXTERN_C int halusleep(double seconds);
EXTERN_C int network_lock(const char* c);
EXTERN_C int network_unlock(const char* c);
#if defined(__cplusplus) /* GENERATED */
std::vector<std::string>* simple_split (const string text, const string search);
string ascii(const string& not_ascii);
string& replace (string& in, const string& rep, const string& wit);
#endif /* GENERATED */
// functions end
#endif /* HAL2009_UTIL */

