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

#ifndef JELIZA_NAMESPACE_H
#define JELIZA_NAMESPACE_H 1

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <sstream>

#include <list>

#include <time.h>
#include <vector>
#include <unistd.h>
#include <getopt.h>

#include <utility>


#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>



using namespace std;
using namespace std::rel_ops;
using namespace boost;

namespace jeliza {
    /*
     * Main Classes
     */
    class string;
    class db_sentence;
    class db_manager;
    class jeliza_impl;
    class jeliza_manager;
    template <typename T1, typename T2> class Pair;

    /*
     * Enums
     */
    enum Wordtype { NO_WORD_TYPE, WORDTYPE_UNKNOWN, VERB, NOUN, ADJ, PREP, PP, ART, QUESTIONWORD, KOMMA, SPO, UNIMPORTANT, LINKING };
    enum SentencePart { NO_CLAUSE, UNKNOWN_CLAUSE, SUBJ, OBJ, PRAE, QUESWORD, PROPERTIES, MAIN_CLAUSE, SUB_CLAUSE };
    enum Genus { NO_GENUS, MALE, FEMALE, THING };

    /*
     * Languages
     */
    enum Lang { ENGLISH, GERMAN };

    /*
     * Type definitions
     */
    typedef vector<jeliza::string> strings;
    typedef vector<jeliza::db_sentence> DB;
    typedef pair<jeliza::string, Wordtype> worddef;


    /*
     * declaration
     */
    class string {
    public:
        shared_ptr<std::string> m_sent;

        explicit string (std::string sent)
         : m_sent(new std::string(sent))
        {
        }
        string (const string& sent)
         : m_sent(new std::string(&(*sent.m_sent) ? *sent.m_sent : ""))
        {
        }
        explicit string (const char* sent)
         : m_sent(new std::string(sent))
        {
        }

        string& operator= (const string& sent);
        char& operator[] (int index);
        bool operator== (const string&) const;
        bool operator== (const std::string&) const;
        bool operator== (const char*) const;
        bool operator< (const string&) const;

        string ()
         : m_sent(new std::string(""))
        {
        }

        std::string get ();
        std::string get () const;
        std::string& ref ();
        std::string ref () const;
        string& set(const string&);
        string& set(const std::string&);
        string& set(const char*);
        unsigned int size() const;
        unsigned int length() const;
        int find(std::string s);
        int find(const char s);
        string substr(int x, int y);
        string toASCII();
        void lower();
        void upper();
        string get_lower() const;
        string get_upper() const;
        string replace(const string, const string);
        string replace(const string, const std::string);
        string replace(const std::string, const std::string);
        string replace_first_nocase(const string, const string);
        string replace_first(const string, const string);
        string replace_first(const string, const std::string);
        string replace_first(const std::string, const std::string);
        string replace_nocase(const string, const string);
        string replace_nocase_safe(const string, const string);
        bool contains (const string rep);
        bool contains (const char*);
        bool startswith (const string rep);
        bool startswith (const char*);
        bool endswith (const string rep);
        bool endswith (const char*);
        void strip();
        string get_strip() const;
        bool equal (const string& s2) const;
        bool equal (const std::string& s2) const;
        bool equal (const char* s2) const;
        std::string::iterator begin() const;
        std::string::iterator end() const;
    };

    class db_sentence {
    public:
        jeliza::string subject;
        jeliza::string verb;
        jeliza::string object;
        jeliza::string prefix;
        jeliza::string suffix;
        jeliza::string feeling;
        jeliza::string category;
        jeliza::string zusatz;
        jeliza::string quesword;
        unsigned int priority;
        jeliza::strings parts;
        jeliza::strings advs;
        shared_ptr<vector<db_sentence> > sub;

        db_sentence ()
         : subject(string("")), verb(string("")), object(string("")), prefix(string("")), suffix(string("")),
           feeling(string("normal")), category("normal"), zusatz(""), quesword(""), priority(50), parts(jeliza::strings()),
           advs(jeliza::strings()), sub(new vector<db_sentence>())
        {
        }

        bool operator== (const db_sentence& sent) const;
        db_sentence& operator= (const db_sentence& sent);
        db_sentence& set (const db_sentence& sent);
        db_sentence (const db_sentence& sent);

        void print(std::string, bool, bool);
        jeliza::string toXML();
        void toXMLPrint();
        jeliza::string printPart (string str, jeliza::string temp, std::string wert);
        void strip();
        vector<jeliza::string> genSentences(bool);
        vector<jeliza::string> genSentences_all(bool);
    };

    class db_manager {
    private:
        shared_ptr<jeliza::DB> m_db;

    public:
        jeliza::DB get();
        void new_from(std::string file);
        void save(std::string);
        std::string get_string();
        DB* parse (std::string file);
        void push(db_sentence);
    };

    class jeliza_impl {
    private:
        db_manager jeliza_db;

    public:

        jeliza_impl()
        : jeliza_db()
        {
        }

        ~jeliza_impl()
        {
        }

        void learn(jeliza::string);
        void learn(strings);
        void init();
        void init_databases();
        void start();
        void ask(jeliza::string frage);
        static unsigned int is_question(jeliza::string);
        void add_wortschatz_word(strings);
        void add_wortschatz_genus(strings);
        bool add_word(char);
        bool add_genus(char, std::string);
        db_manager& get_db_manager();
    };

    class jeliza_manager {
    public:
        jeliza_impl m_jel;

        jeliza_manager();
        ~jeliza_manager();

        jeliza_manager& operator<<(jeliza::strings);
        jeliza_manager& operator<<(std::string);
        void start();
        jeliza_impl& get();
        vector<jeliza::strings>* Wordtype_Database;
        void make_current();
        static void destroy_current();
        static jeliza_manager* get_current();
    };

    class spo_split {
    private:
        map<jeliza::string, jeliza::db_sentence> m_cache;

    public:
        vector<jeliza::strings>* Wordtype_Database;
        shared_ptr<vector<Pair<std::string, jeliza::string> > > Wordtype_equal;
        shared_ptr<vector<Pair<std::string, jeliza::string> > > Wordtype_ohne_prefix;
        shared_ptr<vector<Pair<std::string, jeliza::string> > > Wordtype_equal_low;
        shared_ptr<vector<Pair<std::string, jeliza::string> > > Wordtype_ohne_prefix_low;
        map<vector<Wordtype>, vector<pair<Wordtype, pair<SentencePart, SentencePart> > > > sentences_splitted;
        shared_ptr<map<std::string, std::string> > genus_index;
        shared_ptr<vector<std::string> > genuss;


        static spo_split& get();
        static spo_split instance;

        db_sentence split_spo (jeliza::string, SentencePart = NO_CLAUSE);
        shared_ptr<db_sentence> get_spo (jeliza::string);
        bool hat_gleichen_wortstamm(jeliza::string, jeliza::string, jeliza::string, jeliza::string);
        jeliza::Wordtype bestimmeWordtype(jeliza::string, const bool);
        jeliza::Wordtype bestimmeWordtypeWrapper(jeliza::string, const bool);
        jeliza::Genus bestimmeGenus(jeliza::string);
        jeliza::string ermittle_wortstamm(jeliza::string);
        jeliza::string entferne_prefixe(jeliza::string);
        void init_sentences_splitted();
    };

    /*
     * functions
     */

    strings findit(shared_ptr<vector<Pair<std::string, jeliza::string> > >*, std::string);
    jeliza::string ohne_muell (jeliza::string);
    bool is_similar(jeliza::string, jeliza::string);
    bool is_similar(string, std::string);

    void update_prozent(float);

    void msleep (unsigned long);

    void display_sentence(jeliza::string);

#include "jeliza-comm.h"

    void log(std::string);

    std::string download (std::string);
    void async_download (std::string);

    extern bool global_fertig;
    extern bool force_answer_check;
    extern jeliza::string force_answer;
    extern jeliza::string force_answer_frage;
    extern std::string force_answer_word;
    extern std::string force_answer_genus;

    void send_name();

    extern bool speech_activated;
    extern bool use_online_db;

    extern std::string proxy;
    extern unsigned int proxy_port;

    extern std::string next_word_type;
    extern std::string next_genus;

    extern bool fast_exit;
    Lang get_lang();
    jeliza::string get_lang_str();
    void abbauen();
}

jeliza::string& operator+ (jeliza::string, jeliza::string);
jeliza::string& operator+ (jeliza::string, std::string);
jeliza::string& operator+ (jeliza::string, const char*);
jeliza::string operator+ (string, jeliza::string);
jeliza::string& operator+ (const char*, jeliza::string);
jeliza::string& operator+= (jeliza::string&, const jeliza::string);
jeliza::string& operator+= (jeliza::string&, std::string);
jeliza::string& operator+= (jeliza::string&, const char*);

template<typename T>
vector<T>& operator<< (vector<T>& a1, const vector<T>& a2) {
    for (int x = 0; x < a2.size(); x++) {
        a1.push_back(a2[x]);
    }
    return a1;
}
template<typename T>
vector<T>& operator<< (vector<T>& a1, int*) {
    a1.erase(a1.begin(), a1.end());
    return a1;
}


jeliza::string search_in_wikipedia (jeliza::string);
jeliza::string search_in_wikipedia_with_newlines (jeliza::string);

jeliza::strings wikipedia (jeliza::string, int, bool, bool);
jeliza::strings wikipedia_article (jeliza::string, int, bool, bool);
jeliza::strings search_in_wikipedia_acticle (jeliza::string);

extern jeliza::strings wikipedia_words;

namespace util {
    void split (jeliza::string&, std::string, jeliza::strings&);
    void join (jeliza::string&, std::string, jeliza::strings&);
    void split_string (jeliza::string, jeliza::string, jeliza::strings&, bool);
    jeliza::string tausche (jeliza::string, jeliza::string, jeliza::string);
    jeliza::string umwandlung (jeliza::string);
}

class Logger {
public:

};

Logger& operator<< (Logger, std::string);
extern jeliza::string log_all;
extern ofstream clogger;
extern bool offline_mode;
extern bool www_surf_mode;
extern jeliza::string allWikiWords;

void speak(string);
void speech_init();

extern bool speechOk;

extern jeliza::strings listenLinkUrls;

jeliza::string s(const char*);

#endif
