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

#include "hal2012.h"
#include "hal2012-universal-cxx.h"
BEGIN_EXTERN_C
#include "hal2012-universal.h"
END_EXTERN_C
#include "hal2012-util.h"
#include "hal2012-talk.h"

EXTERN_C int is_a_variable(const char* _word) {
    string word = _word;
    return (
        (
            word == ("$a$")
         || word == ("$b$")
         || word == ("$c$")
         || word == ("$d$")
         || word == ("$e$")
         || word == ("$f$")
         || word == ("$g$")
         || word == ("$h$")
        )
         ?  1
         :  0
    );
}

EXTERN_C int is_a_equal_verb(const char* _word) {
    string word = _word;
    return (
        (
            0 == word.find("=")
         || 0 == word.find("is-a")
        )
         ?  1
         :  0
    );
}

EXTERN_C int is_a_fact_verb(const char* _word) {
    string word = _word;
    return (
        (
            0 == word.find("=")
         || 0 == word.find("is-part")
         || 0 == word.find("is-own")
         || 0 == word.find("is-a")
        )
         ?  1
         :  0
    );
}

EXTERN_C int is_a_trivial_word(const char* _word) {
    string word = _word;
    const string lang = hal2012_get_text_language();

    return (
        (
            word == ("(a)")
         || word == ("_(a)_")
         || word == ("(the)")
         || word == ("_(the)_")

         || ( lang == "en" && (
            word == ("the")
         || word == ("a")
         || word == ("an")
         ))

         || ( lang == "de" && (
            word == ("der")
         || word == ("die")
         || word == ("das")
         || word == ("des")
         || word == ("den")
         || word == ("dem")
         || word == ("ein")
         || word == ("eine")
         || word == ("einer")
         || word == ("einem")
         || word == ("einen")
         || word == ("eines")
         || word == ("nicht")
         ))

         || word == ("sehr")
         || word == ("viel")
         || word == ("viele")
         || word == ("weder")
         || word == ("noch")
         || word == ("schon")
         || word == ("bereits")
         || word == ("nur")
         || word == ("zu")
         || word == ("mit")
         || word == ("ohne")
         || word == ("in")
         || word == ("von")
        )
         ?  1
         :  0
    );
}

EXTERN_C int is_conditional_questionword(const char* _word) {
    string word = _word;
    return (
        (
            word == ("if")
         || word == ("when")

         || word == ("wenn")
         || word == ("falls")
         || word == ("sobald")
        )
         ?  1
         :  0
    );
}

EXTERN_C int can_be_a_main_synonym(const char* _word) {
    string word = _word;
    return (
            word != ("ich")
         && word != ("du")
         && word != ("_ich_")
         && word != ("_du_")

         ?  1
         :  0
    );
}
EXTERN_C int can_be_a_synonym(const char* _word) {
    string word = _word;
    return (
            !is_a_trivial_word(word.c_str())
         && word != ("es")
         && word != ("*")
         && word != ("in")
         && word != ("aus")
         && word != ("auf")
         && word != ("ab")
         && word != ("sich")
         && word != ("im")
         && word != ("an")
         && word != ("am")
         && word != ("von")
         && word != ("vom")
         && word != ("auf")
         && word != ("so")
         && word != ("genau")
         && word != ("genauso")
         && word != ("$$aswellas$$")
         && word != ("$$notaswellas$$")
         && string::npos == word.find("wie")

//         && word != ("ich")
//         && word != ("du")
//         && word != ("_ich_")
//         && word != ("_du_")
         // && ( (strlen(word) > 3 && string::npos == word.find("_")) || strlen(word) > 6 )

         ?  1
         :  0
    );
}

EXTERN_C int is_important_word(const char* _word) {
    string word = _word;
    return ((
      (     !is_a_trivial_word(word.c_str())
         && word != ("es")
         && word != ("*")
         && word != ("in")
         && word != ("im")
         && word != ("an")
         && word != ("am")
         && word != ("von")
         && word != ("vom")
         && word != ("auf")
         && word != ("so")
         && word != ("genau")
         && word != ("genauso")
         && word != ("$$aswellas$$")
         && word != ("$$notaswellas$$")
         && word != ("wie")
         && string::npos == word.find("viel")

         && word != ("(a)")
         && word != ("(the)")

         && string::npos == word.find("$")
         && string::npos == word.find("ein")
         && word != ("der")
         && word != ("die")
         && word != ("das")
      )
      || (
            word == "ich"
         || word == "du"
         || word == "_ich_"
         || word == "_du_"
      ))

         ?  1
         :  0
    );
}


