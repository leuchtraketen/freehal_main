#ifndef JELIZA_SENTENCE
#define JELIZA_SENTENCE 1

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


string freehal::string::get() {
    return *m_sent;
}

string freehal::string::get() const {
    return *m_sent;
}

string& freehal::string::ref() {
    return *m_sent;
}

string freehal::string::ref() const {
    return *m_sent;
}

freehal::string& freehal::string::set(const std::string& str) {
    m_sent.reset(new std::string(str.c_str()));
    return (*this);
}
freehal::string& freehal::string::set(const freehal::string& str) {
    m_sent.reset(new std::string(str.get().c_str()));
    return (*this);
}
freehal::string& freehal::string::set(const char* str) {
    m_sent.reset(new std::string(str));
    return (*this);
}

unsigned int freehal::string::size() const {
    return (m_sent->size());
}

unsigned int freehal::string::length() const {
    return (size());
}

int freehal::string::find(std::string s) {
    return (ref().find(s));
}
int freehal::string::find(const char s) {
    return (ref().find(s));
}
freehal::string freehal::string::substr(int x, int y) {
    freehal::string ret = *this;
    ret.ref() = ret.ref().substr(x, y);
    return ret;
}

char& freehal::string::operator[] (int index) {
    if (index < 0) {
        index = ref().size() - index;
    }
    return (ref()[index]);
}

void freehal::string::lower() {
    if (&(*m_sent)) {
        if (size() == 0) {
            return;
        }
    }
    for (std::string::size_type x = 0; x < ref().size(); x++) {
		ref()[x] = tolower(ref()[x]);
	}
}
void freehal::string::upper() {
    if (&(*m_sent)) {
        if (size() == 0) {
            return;
        }
    }
    for (std::string::size_type x = 0; x < ref().size(); x++) {
		ref()[x] = toupper(ref()[x]);
	}
}

freehal::string freehal::string::get_lower() const {
    freehal::string ret = *this;
    ret.lower();
    return (ret);
}
freehal::string freehal::string::get_upper() const {
    freehal::string ret = *this;
    ret.upper();
    return (ret);
}

freehal::string freehal::string::get_strip() const {
    freehal::string ret = *this;
    ret.strip();
    return (ret);
}

freehal::string freehal::string::replace(const freehal::string rep, const freehal::string wit) {
    freehal::string tmp(*this);
	int pos;
	while (true) {
		pos = tmp.ref().find(rep.get());
		if (pos == -1) {
			break;
		} else {
			tmp.ref().erase(pos, rep.get().size());
			tmp.ref().insert(pos, wit.get());
		}
	}
	return (tmp);
}
freehal::string freehal::string::replace(const freehal::string rep, const std::string wit) {
    return (replace (rep, freehal::string(wit)));
}
freehal::string freehal::string::replace(const std::string rep, const std::string wit) {
    return (replace (freehal::string(rep), freehal::string(wit)));
}
freehal::string freehal::string::replace_nocase(const freehal::string rep, const freehal::string wit) {
    freehal::string tmp(*this);
	int pos;
	while (true) {
	    freehal::string low;
        (low.set(tmp.get())).lower();
		pos = low.find(rep.get());
		if (pos == -1) {
			break;
		} else {
			tmp.ref().erase(pos, rep.get().size());
			tmp.ref().insert(pos, wit.get());
		}
	}
	return (tmp);
}
freehal::string freehal::string::replace_nocase_safe(const freehal::string rep, const freehal::string wit) {
    freehal::string tmp(*this);
	tmp = tmp.replace_nocase(rep, freehal::string("indhwszterbvdtshsbcuebdfjsiahbsdsagsvbxybwlcombxgfsszsv"));
	tmp = tmp.replace_nocase(freehal::string("indhwszterbvdtshsbcuebdfjsiahbsdsagsvbxybwlcombxgfsszsv"), wit);
	return (tmp);
}
freehal::string freehal::string::replace_first(const freehal::string rep, const freehal::string wit) {
    freehal::string tmp(*this);
	int pos;
    pos = tmp.ref().find(rep.get());
    if (pos != -1) {
        tmp.ref().erase(pos, rep.get().size());
        tmp.ref().insert(pos, wit.get());
	}
	return (tmp);
}
freehal::string freehal::string::replace_first_nocase(const freehal::string rep, const freehal::string wit) {
    freehal::string tmp(*this);
	int pos;
    freehal::string low;
    (low.set(tmp.get())).lower();
    freehal::string low_rep;
    (low_rep.set(rep)).lower();
    pos = low.find(low_rep.get());
    if (pos != -1) {
        tmp.ref().erase(pos, rep.get().size());
        tmp.ref().insert(pos, wit.get());
	}
	return (tmp);
}
freehal::string freehal::string::replace_first(const freehal::string rep, const std::string wit) {
    return (replace_first (rep, freehal::string(wit)));
}
freehal::string freehal::string::replace_first(const std::string rep, const std::string wit) {
    return (replace_first (freehal::string(rep), freehal::string(wit)));
}
bool freehal::string::contains (const freehal::string rep) {
    int pos = ref().find(rep.get());
    if (pos == -1) {
		return 0;
	}
	return 1;
}
bool freehal::string::contains (const char* rep) {
    int pos = ref().find(std::string(rep));
    if (pos == -1) {
		return 0;
	}
	return 1;
}

bool freehal::string::startswith (const freehal::string rep) {
    int pos = ref().find(rep.get());
    if (pos == 0) {
		return true;
	}
	return false;
}
bool freehal::string::startswith (const char* rep) {
    int pos = ref().find(std::string(rep));
    if (pos == 0) {
		return true;
	}
	return false;
}
bool freehal::string::endswith (const freehal::string rep) {
    int pos = ref().find(rep.get()) + rep.size();
    if (pos >= ref().size() - 1) {
		return true;
	}
	return false;
}
bool freehal::string::endswith (const char* rep) {
    return endswith(freehal::string(rep));
}
void freehal::string::strip() {
/*    for (int y = 0; y < 2; y++) {
        if (m_sent->length()) {
            while (m_sent->length() > 0) {
                if ((*m_sent)[0] != ' ' && (*m_sent)[0] != '\n' && (*m_sent)[0] != '\r') {
                    break;
                }
                m_sent->erase(0, 1);
            }


            reverse(m_sent->begin(), m_sent->end());
        }
    }*/

    if (m_sent->length()) {
        while (m_sent->length() > 0) {
            if ((*m_sent)[0] != ' ' && (*m_sent)[0] != '\n' && (*m_sent)[0] != '\r') {
                break;
            }
            m_sent->erase(0, 1);
        }

        int len = m_sent->length() - 1;
        while (len > -1) {
            if ((*m_sent)[len] != ' ' && (*m_sent)[len] != '\n' && (*m_sent)[len] != '\r') {
                break;
            }
            m_sent->erase(len, 1);
            --len;
        }
    }
}

freehal::string freehal::string::toASCII() {
    freehal::string ret = *this;
    freehal::string& all = ret;

    all = all.replace("Ã¼", "ue");
	all = all.replace("Ã", "ss");
	all = all.replace("Ã¤", "ae");
	all = all.replace("ü", "ue");
	all = all.replace("ß", "ss");
	all = all.replace("ä", "ae");
	all = all.replace("ö", "oe");
	all = all.replace("Ü", "Ue");
	all = all.replace("Ä", "Ae");
	all = all.replace("Ö", "Oe");
	all = all.replace("Ã¢ÂÂ", "\"");
	all = all.replace("Ã¢ÂÂ", "\"");
	all = all.replace("&lt;/br&gt;", " ");

	freehal::string ascii(" \n\r!\"#$%&'()*+,-./0123456789:;<=>?    @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_    `abcdefghijklmnopqrstuvwxyz{|}~");

	string allAscii;
	for (int x = 0; x < all.size(); x++) {
		char array[2];
		array[0] = all[x];
		array[1] = '\0';
		string y(array);
		if (ascii.contains(y.ref().c_str())) {
			allAscii += y;
		}
	}
    ret.set(allAscii);

	return (ret);
}

freehal::string& operator+ (freehal::string s1, freehal::string s2) {
    (*s1.m_sent) += s2.ref();
    return s1;
}
freehal::string& operator+ (freehal::string s1, std::string s2) {
    (*s1.m_sent) += s2;
    return s1;
}
freehal::string& operator+ (freehal::string s1, const char* s2) {
    (*s1.m_sent) += s2;
    return s1;
}
freehal::string operator+ (string s2, freehal::string s1) {
    freehal::string temp(s2);
    temp += s1;
    return temp;
}
freehal::string& operator+ (const char* s2, freehal::string s1) {
    (*s1.m_sent) = std::string(s2) + (*s1.m_sent);
    return s1;
}


freehal::string& operator+= (freehal::string& s1, const freehal::string s2) {
    (*s1.m_sent) += (*s2.m_sent);
    return s1;
}
freehal::string& operator+= (freehal::string& s1, std::string s2) {
    (*s1.m_sent) += s2;
    return s1;
}
freehal::string& operator+= (freehal::string& s1, const char* s2) {
    (*s1.m_sent) += s2;
    return s1;
}

/*freehal::string& freehal::string::operator= (string sent) {
    set (sent);
    return (*this);
}*/
/*freehal::string& freehal::string::operator= (const char* sent) {
    set (string(sent));
    return (*this);
}*/
freehal::string& freehal::string::operator= (const freehal::string& sent) {
    set (sent);
    return (*this);
}

bool freehal::string::operator== (const freehal::string& s2) const {
    return (equal(s2));
}
bool freehal::string::operator== (const std::string& s2) const {
    return (equal(s2));
}
bool freehal::string::operator== (const char* s2) const {
    return (equal(s2));
}

bool freehal::string::equal (const freehal::string& s2) const {
    return ((*m_sent) == s2.ref());
}
bool freehal::string::equal (const std::string& s2) const {
    return ((*m_sent) == s2);
}
bool freehal::string::equal (const char* s2) const {
    return ((*m_sent) == std::string(s2));
}

bool freehal::string::operator< (const string& s) const {
    return (size() < s.size());
}


/*freehal::strings& operator<< (freehal::strings& a1, const freehal::strings& a2) {
    for (int x = 0; x < a2.size(); x++) {
        a1.push_back(a2[x]);
    }
    return a1;
}
freehal::strings& operator<< (freehal::strings& a1, int*) {
    a1.erase(a1.begin(), a1.end());
    return a1;
}*/

string::iterator freehal::string::begin() const {
    return m_sent->begin();
}
string::iterator freehal::string::end() const {
    return m_sent->end();
}




#endif
