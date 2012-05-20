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

namespace freehal {
    /*
     * Main Classes
     */
    class string;
    class db_sentence;
    class db_manager;
    class freehal_impl;
    class freehal_manager;
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
    typedef vector<freehal::string> strings;
    typedef vector<freehal::db_sentence> DB;
    typedef pair<freehal::string, Wordtype> worddef;


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
        freehal::string subject;
        freehal::string verb;
        freehal::string object;
        freehal::string prefix;
        freehal::string suffix;
        freehal::string feeling;
        freehal::string category;
        freehal::string zusatz;
        freehal::string quesword;
        unsigned int priority;
        freehal::strings parts;
        freehal::strings advs;
        shared_ptr<vector<db_sentence> > sub;

        db_sentence ()
         : subject(string("")), verb(string("")), object(string("")), prefix(string("")), suffix(string("")),
           feeling(string("normal")), category("normal"), zusatz(""), quesword(""), priority(50), parts(freehal::strings()),
           advs(freehal::strings()), sub(new vector<db_sentence>())
        {
        }

        bool operator== (const db_sentence& sent) const;
        db_sentence& operator= (const db_sentence& sent);
        db_sentence& set (const db_sentence& sent);
        db_sentence (const db_sentence& sent);

        void print(std::string, bool, bool);
        freehal::string toXML();
        void toXMLPrint();
        freehal::string printPart (string str, freehal::string temp, std::string wert);
        void strip();
        vector<freehal::string> genSentences(bool);
        vector<freehal::string> genSentences_all(bool);
    };

    class db_manager {
    private:
        shared_ptr<freehal::DB> m_db;

    public:
        freehal::DB get();
        void new_from(std::string file);
        void save(std::string);
        std::string get_string();
        DB* parse (std::string file);
        void push(db_sentence);
    };

    class freehal_impl {
    private:
        db_manager freehal_db;

    public:

        freehal_impl()
        : freehal_db()
        {
        }

        ~freehal_impl()
        {
        }

        void learn(freehal::string);
        void learn(strings);
        void init();
        void init_databases();
        void start();
        void ask(freehal::string frage);
        static unsigned int is_question(freehal::string);
        void add_wortschatz_word(strings);
        void add_wortschatz_genus(strings);
        bool add_word(char);
        bool add_genus(char, std::string);
        db_manager& get_db_manager();
    };

    class freehal_manager {
    public:
        freehal_impl m_jel;

        freehal_manager();
        ~freehal_manager();

        freehal_manager& operator<<(freehal::strings);
        freehal_manager& operator<<(std::string);
        void start();
        freehal_impl& get();
        vector<freehal::strings>* Wordtype_Database;
        void make_current();
        static void destroy_current();
        static freehal_manager* get_current();
    };

    class spo_split {
    private:
        map<freehal::string, freehal::db_sentence> m_cache;

    public:
        vector<freehal::strings>* Wordtype_Database;
        shared_ptr<vector<Pair<std::string, freehal::string> > > Wordtype_equal;
        shared_ptr<vector<Pair<std::string, freehal::string> > > Wordtype_ohne_prefix;
        shared_ptr<vector<Pair<std::string, freehal::string> > > Wordtype_equal_low;
        shared_ptr<vector<Pair<std::string, freehal::string> > > Wordtype_ohne_prefix_low;
        map<vector<Wordtype>, vector<pair<Wordtype, pair<SentencePart, SentencePart> > > > sentences_splitted;
        shared_ptr<map<std::string, std::string> > genus_index;
        shared_ptr<vector<std::string> > genuss;


        static spo_split& get();
        static spo_split instance;

        db_sentence split_spo (freehal::string, SentencePart = NO_CLAUSE);
        shared_ptr<db_sentence> get_spo (freehal::string);
        bool hat_gleichen_wortstamm(freehal::string, freehal::string, freehal::string, freehal::string);
        freehal::Wordtype bestimmeWordtype(freehal::string, const bool);
        freehal::Wordtype bestimmeWordtypeWrapper(freehal::string, const bool);
        freehal::Genus bestimmeGenus(freehal::string);
        freehal::string ermittle_wortstamm(freehal::string);
        freehal::string entferne_prefixe(freehal::string);
        void init_sentences_splitted();
    };

    /*
     * functions
     */

    strings findit(shared_ptr<vector<Pair<std::string, freehal::string> > >*, std::string);
    freehal::string ohne_muell (freehal::string);
    bool is_similar(freehal::string, freehal::string);
    bool is_similar(string, std::string);

    void update_prozent(float);

    void msleep (unsigned long);

    void display_sentence(freehal::string);

#include "freehal-comm.h"

    void log(std::string);

    std::string download (std::string);
    void async_download (std::string);

    extern bool global_fertig;
    extern bool force_answer_check;
    extern freehal::string force_answer;
    extern freehal::string force_answer_frage;
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
    freehal::string get_lang_str();
    void abbauen();
}

freehal::string& operator+ (freehal::string, freehal::string);
freehal::string& operator+ (freehal::string, std::string);
freehal::string& operator+ (freehal::string, const char*);
freehal::string operator+ (string, freehal::string);
freehal::string& operator+ (const char*, freehal::string);
freehal::string& operator+= (freehal::string&, const freehal::string);
freehal::string& operator+= (freehal::string&, std::string);
freehal::string& operator+= (freehal::string&, const char*);

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


freehal::string search_in_wikipedia (freehal::string);
freehal::string search_in_wikipedia_with_newlines (freehal::string);

freehal::strings wikipedia (freehal::string, int, bool, bool);
freehal::strings wikipedia_article (freehal::string, int, bool, bool);
freehal::strings search_in_wikipedia_acticle (freehal::string);

extern freehal::strings wikipedia_words;

namespace util {
    void split (freehal::string&, std::string, freehal::strings&);
    void join (freehal::string&, std::string, freehal::strings&);
    void split_string (freehal::string, freehal::string, freehal::strings&, bool);
    freehal::string tausche (freehal::string, freehal::string, freehal::string);
    freehal::string umwandlung (freehal::string);
}

class Logger {
public:

};

Logger& operator<< (Logger, std::string);
extern freehal::string log_all;
extern ofstream clogger;
extern bool offline_mode;
extern bool www_surf_mode;
extern freehal::string allWikiWords;

void speak(string);
void speech_init();

extern bool speechOk;

extern freehal::strings listenLinkUrls;

freehal::string s(const char*);

#endif
