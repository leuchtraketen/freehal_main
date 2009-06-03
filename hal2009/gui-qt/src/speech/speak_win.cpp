#include "defs.h"
#include <boost/asio.hpp>

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

using namespace std;


#include <windows.h>
#include <boost/thread.hpp>


bool speechOk = false;
bool freehal::speech_activated = false;

void speech_init() {
}



void speak(string str) {
    ofstream o("mplayer.bat");
    o << "mplayer.exe \"http://tobias-schulz.info/freehal/online/now.au.pl?text=" + str + "\"" << endl;
    o.close();

    if (not freehal::speech_activated) {
        return;
    }
    
    boost::thread t_v(boost::bind(std::system, "mplayer.bat"));
}

