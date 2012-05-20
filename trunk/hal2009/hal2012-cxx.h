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


// Boost headers
#undef BOOST_ASIO_HAS_EPOLL
#undef BOOST_ASIO_HAS_KQUEUE
#undef BOOST_ASIO_HAS_DEV_POLL
#define BOOST_ASIO_DISABLE_EPOLL 1
#ifndef NO_ASIO_HERE
#   ifdef WINDOWS
#       include <winsock2.h>
#   endif
#   include <boost/asio.hpp>
#endif
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#undef BOOST_ASIO_HAS_EPOLL
#undef BOOST_ASIO_HAS_KQUEUE
#undef BOOST_ASIO_HAS_DEV_POLL

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cstring>
#include <stdarg.h>
#include <pthread.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace boost;
using namespace algorithm;
using boost::asio::ip::tcp;

#define cxxhalfree( p )            { { free( p ); p = 0; --halmallocs; } }
#define cxxhalfreef( p, f )        { { free( p ); p = 0; --halmallocs; } }
#define cxxhalmalloc( s, f )       calloc( s, 1 ); { ++halmallocs; }
#define cxxzero_p( p, s )          { long i; for (i = 0; i < ( s ) - 1 ; ++i) { ( p ) [i] = 0; } }
#define line_size 4096


