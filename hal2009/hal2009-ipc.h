#ifndef HAL2009_IPC
#define HAL2009_IPC 1

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

#include <boost/algorithm/string.hpp>

using namespace std;

#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1
#define USE_CXX 1
#include "hal2009.h"


extern "C" {
	extern std::vector<std::pair<string, string> > signals_to_child;
	void hal2009_send_signal(char* vfile, char* data);
	void hal2009_send_signals();
}
extern void (*hal2009_send_signal_func)(string vfile, string data);

#endif

