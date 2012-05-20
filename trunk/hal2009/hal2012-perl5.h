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

#ifndef HAL2009_PERL5
#define HAL2009_PERL5 1

#if defined(__cplusplus) /* GENERATED */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include <boost/process.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
namespace bp = ::boost::process;

#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1
#define USE_CXX 1

#include "hal2012-ipc.h"

extern bp::postream* child_stdin;
extern bp::pistream* child_stdout;

void* hal2012_handle_signal(void* _p); // individual definition
#endif /* GENERATED */

// functions begin
#if defined(__cplusplus) /* GENERATED */
int file_read(const string& c, string& code);
int file_size(const string& c);
int file_write(const string& c, const string code);
int perl5_convert_file(string filename);
static inline void perl5_convert_structure (string& hals, int just_compile);
string perl5_convert_code(string halcode, int just_compile);
void perl5_execute(string filename);
void perl5_hal2012_send_signal(string vfile, string data);
#endif /* GENERATED */
// functions end
#endif /* HAL2009_PERL5 */

