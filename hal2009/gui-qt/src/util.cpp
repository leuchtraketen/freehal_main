#ifndef JELIZA_util
#define JELIZA_util 1

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

freehal::string s(const char* cha) {
    return freehal::string(cha);
}

namespace util {

void split (freehal::string& text2, string separators, freehal::strings& words) {
    words << NULL;

	int n = text2.ref().length();
	const string text(text2.ref());
	int start, stop;

	if (text.find_first_of(separators) == 0) {
	    words.push_back(freehal::string(""));
	}

	start = text.find_first_not_of(separators);
	while ((start >= 0) && (start < n)) {
		stop = text.find_first_of(separators, start);
		if ((stop < 0) || (stop > n)) stop = n;
		words.push_back(freehal::string(text.substr(start, stop - start)));
		start = text.find_first_not_of(separators, stop+1);
	}
}

void join (freehal::string& text2, string separators, freehal::strings& words) {
	text2.set("");
	for (unsigned int x = 0; x < words.size(); x++) {
	    if (x == 0) {
	        text2 += words[x];
	    }
	    else {
	        text2 += separators;
	        text2 += words[x];
	    }
	}
}

void split_string (freehal::string text, freehal::string separators, freehal::strings& words, bool includeEmpties) {
	freehal::strings ret;
	text.set(" " + text + " ");
	separators.strip();
	text.replace("\r", "\n");
	text.replace(" " + separators + " ", separators);
	text.replace(separators, "\r");
	separators.set("\r");

	split(text, "\r", ret);

	for (unsigned int x = 0; x < ret.size(); x++) {
	    ret[x].strip();
//	    cout << "\t" << ret[x].ref();
	}
//	cout << endl;

	words << NULL << ret;
}

freehal::string tausche (freehal::string str, freehal::string s1, freehal::string s2) {
    str = str.replace_nocase_safe(s(", "), s(","));
    str = str.replace_nocase_safe(s(" ,"), s(","));
    str = str.replace_nocase_safe(s(","), s(" , "));
	str = " " + str + " ";
	s1 = " " + s1 + " ";
	s2 = " " + s2 + " ";
	str = str.replace_nocase_safe(s1, s("6352652630,5,64,4636"));
	str = str.replace_nocase_safe(s2, s1);
	str = str.replace_nocase_safe(s("6352652630,5,64,4636"), s2);
    str = str.replace_nocase_safe(s(" ,"), s(","));

	return str.substr(1, str.size() - 1);
}


int max (int a, int b) {
    return ((a>b ? a : b));
}

int min (int a, int b) {
    return ((a<b ? a : b));
}

}

freehal::Lang freehal::get_lang() {
	ifstream i("temp/language.tmp");
	if (!i) {
		ofstream o("temp/language.tmp");
		o << "de";
		o.close();
		return freehal::GERMAN;
	}
	freehal::string l;
	getline(i, l.ref());
	l.strip();
	if (l == s("en")) {
		return freehal::ENGLISH;
	}
	return freehal::GERMAN;
}

freehal::string freehal::get_lang_str() {
	ifstream i("temp/language.tmp");
	if (!i) {
		ofstream o("temp/language.tmp");
		o << "de";
		o.close();
		return s("de");
	}
	freehal::string l;
	getline(i, l.ref());
	return l;
}

#endif

