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

#ifndef HAL2009_STARTUP
#define HAL2009_STARTUP 1

#if defined(__cplusplus) /* GENERATED */

vector<string>* params_argv(int argc, char** argv);
vector<string>* params_cin();

class instance {
  public:
    instance()
    : planguage(), tlanguage(), base_dir(), sql_engine(), server_port(5174)
    {}

    string planguage;
    string tlanguage;
    string base_dir;
    string sql_engine;
    int server_port;
};

class debugging_options {
  public:
    debugging_options()
    : g_gdb(false), g_valgrind(false), g_valgrind_tool()
    {}

    bool g_gdb;
    bool g_valgrind;
    string g_valgrind_tool;
};

extern instance* current_instance;
#endif /* GENERATED */

// functions begin
#if defined(__cplusplus) /* GENERATED */
vector<string>* params_argv(int argc, char** argv);
vector<string>* params_cin();
#endif /* GENERATED */
// functions end
#endif /* HAL2009_STARTUP */

