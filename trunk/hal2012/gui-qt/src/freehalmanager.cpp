#ifndef JELIZA_MANAGER
#define JELIZA_MANAGER 1

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

using namespace std;

shared_ptr<freehal::freehal_manager> global_freehal(new freehal::freehal_manager());

/*
 * Die << und >> Operatoren
 */
freehal::freehal_manager& freehal::freehal_manager::operator<< (std::string fra) {
    m_jel.ask(freehal::string(fra));

    return *this;
}
freehal::freehal_manager& freehal::freehal_manager::operator<< (freehal::strings ans) {
    m_jel.init();
    m_jel.learn(ans);
    m_jel.init();

    return *this;
}

freehal::freehal_impl& freehal::freehal_manager::get() {
    return m_jel;
}

void freehal::freehal_manager::start () {
    m_jel.start();
}

bool database_present = true;

void freehal::abbauen () {
    if (database_present) {
        cout << "Writing data to word database, language " << freehal::get_lang_str().ref() << endl;
        freehal::string temp;
        freehal::string temp2;
        for (unsigned int x = 0; x < freehal::spo_split::get().Wortart_Database->size(); x++) {
            freehal::string str;
            (*freehal::spo_split::get().Wortart_Database)[x][2] = freehal::string(".");
            util::join(str, ";", (*freehal::spo_split::get().Wortart_Database)[x]);
            temp += str;
            temp += "\n";
        }
        for (unsigned int x = 0; x < freehal::spo_split::get().genuss->size(); x++) {
        	freehal::string str((*freehal::spo_split::get().genuss)[x]);
        	str = (*freehal::spo_split::get().genus_index)[str.ref()] + ";" + str;
        	temp2 += str;
        	temp2 += "\n";
        }

        cout << "Writing data NOW to word database, language " << freehal::get_lang_str().ref() << endl;

        ofstream o(("lang_" + freehal::get_lang_str() + "/wsh/dic.wsh").ref().c_str());
        o << temp.ref() << endl;
        o.close();
        ofstream o2(("lang_" + freehal::get_lang_str() + "/wsh/genus.wsh").ref().c_str());
        o2 << temp2.ref() << endl;
        o2.close();
        
//        std::exit(0);
        database_present = false;
    }
}
freehal::freehal_manager::~freehal_manager () {
}

freehal::freehal_manager::freehal_manager () {
}

void freehal::freehal_manager::make_current() {
    global_freehal.reset(this);
}

void freehal::freehal_manager::destroy_current() {
}

freehal::freehal_manager* freehal::freehal_manager::get_current() {
    return &(*global_freehal);
}


#endif

