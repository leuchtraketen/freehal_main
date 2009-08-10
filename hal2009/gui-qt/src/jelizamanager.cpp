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

shared_ptr<jeliza::jeliza_manager> global_jeliza(new jeliza::jeliza_manager());

/*
 * Die << und >> Operatoren
 */
jeliza::jeliza_manager& jeliza::jeliza_manager::operator<< (std::string fra) {
    m_jel.ask(jeliza::string(fra));

    return *this;
}
jeliza::jeliza_manager& jeliza::jeliza_manager::operator<< (jeliza::strings ans) {
    m_jel.init();
    m_jel.learn(ans);
    m_jel.init();

    return *this;
}

jeliza::jeliza_impl& jeliza::jeliza_manager::get() {
    return m_jel;
}

void jeliza::jeliza_manager::start () {
    m_jel.start();
}

bool database_present = true;

void jeliza::abbauen () {
    if (database_present) {
        cout << "Writing data to word database, language " << jeliza::get_lang_str().ref() << endl;
        jeliza::string temp;
        jeliza::string temp2;
        for (unsigned int x = 0; x < jeliza::spo_split::get().Wortart_Database->size(); x++) {
            jeliza::string str;
            (*jeliza::spo_split::get().Wortart_Database)[x][2] = jeliza::string(".");
            util::join(str, ";", (*jeliza::spo_split::get().Wortart_Database)[x]);
            temp += str;
            temp += "\n";
        }
        for (unsigned int x = 0; x < jeliza::spo_split::get().genuss->size(); x++) {
        	jeliza::string str((*jeliza::spo_split::get().genuss)[x]);
        	str = (*jeliza::spo_split::get().genus_index)[str.ref()] + ";" + str;
        	temp2 += str;
        	temp2 += "\n";
        }

        cout << "Writing data NOW to word database, language " << jeliza::get_lang_str().ref() << endl;

        ofstream o(("lang_" + jeliza::get_lang_str() + "/wsh/dic.wsh").ref().c_str());
        o << temp.ref() << endl;
        o.close();
        ofstream o2(("lang_" + jeliza::get_lang_str() + "/wsh/genus.wsh").ref().c_str());
        o2 << temp2.ref() << endl;
        o2.close();
        
//        std::exit(0);
        database_present = false;
    }
}
jeliza::jeliza_manager::~jeliza_manager () {
}

jeliza::jeliza_manager::jeliza_manager () {
}

void jeliza::jeliza_manager::make_current() {
    global_jeliza.reset(this);
}

void jeliza::jeliza_manager::destroy_current() {
}

jeliza::jeliza_manager* jeliza::jeliza_manager::get_current() {
    return &(*global_jeliza);
}


#endif

