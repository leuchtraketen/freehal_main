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


#include "defs.h"
// Boost headers
#undef BOOST_ASIO_HAS_EPOLL
#undef BOOST_ASIO_HAS_KQUEUE
#undef BOOST_ASIO_HAS_DEV_POLL
#define BOOST_ASIO_DISABLE_EPOLL 1
#ifndef NO_ASIO_HERE
#include <boost/asio.hpp>
#endif
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#undef BOOST_ASIO_HAS_EPOLL
#undef BOOST_ASIO_HAS_KQUEUE
#undef BOOST_ASIO_HAS_DEV_POLL


using namespace std;


bool speechOk = false;
bool freehal::speech_activated = false;


void speech_init() {
}


void speak(string str) {
    ofstream o("./mplayer.sh");
    o << "#!/bin/bash" << endl;
    o << "mplayer \"http://tobias-schulz.info/freehal/online/now.au.pl?text=" + str + "\" &" << endl;
    o.close();

    if (not freehal::speech_activated) {
        return;
    }
    
    boost::thread t_v(boost::bind(std::system, "bash mplayer.sh"));
}

