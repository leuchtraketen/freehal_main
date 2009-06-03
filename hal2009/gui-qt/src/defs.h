#ifndef JELIZA_DEFS_H
#define JELIZA_DEFS_H 1

/*
 * This file is part of FreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
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

#ifdef HAVE_LONG_LONG
#undef HAVE_LONG_LONG
#endif

#define NO_PYTHON 1

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <sstream>

#include <string>
#include <list>

#include <time.h>
#include <vector>
#include <unistd.h>
#include <getopt.h>

#include <algorithm>

#include <sys/time.h>

#include <cstdlib>

#ifdef linux
//#include <process.h>
#define CLEAR_CMD "clear"
#else
#ifdef __linux__
//#include <process.h>
#define CLEAR_CMD "clear"
#else
//#include <conio.h>
#define CLEAR_CMD "cls"s
#endif
#endif

#ifndef JELIZA_DEC_VERSION
#define JELIZA_DEC_VERSION 1
#endif

#ifndef JELIZA_FULL_VERSION
#define JELIZA_FULL_VERSION string("74")
#endif

#ifndef KI_NAME
#define KI_NAME string("FreeHAL 2009 rev.") + JELIZA_FULL_VERSION
#endif

#include <utility>
using namespace std::rel_ops;

#include <unistd.h>

#include "freehal-namespace.h"

#endif
