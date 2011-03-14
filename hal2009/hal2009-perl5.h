#ifndef HAL2009_PERL5
#define HAL2009_PERL5 1

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
#include "hal2009.h"


string convert_to_perl5(string hals, int just_compile);
int convert_to_perl5_convert_file(string filename);
static inline void convert_to_perl5_structure (string& hals, int just_compile);
void execute_perl5(string filename);
void perl5_hal2009_send_signal(char* vfile, char* data);

bp::postream* child_stdin = 0;
bp::pistream* child_stdout = 0;


#endif

