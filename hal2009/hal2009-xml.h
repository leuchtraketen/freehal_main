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

#include "hal2009.h"

#ifndef HAL2009_XML
#define HAL2009_XML 1

#if defined(__cplusplus) /* GENERATED */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;

#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1
#define USE_CXX 1

extern "C" {
#include "hal2009-universal.h"
}

class XML_Object;

enum Mode { LIST, TEXT, ANY };
class XML_Object
{
  public:
    XML_Object(Mode);
    XML_Object();
    virtual ~XML_Object();
    void init();

// universal
  public:
    void set_name(string);
    string get_name();
    string print_xml();
    string print_xml(int, int);
    string print_str();
    string print_str(string);
    void free();
  protected:
    string name;

// list
  public:
    std::vector<XML_Object*> part(string);
    friend XML_Object* operator >>(XML_Object*, vector<XML_Object*>);
    friend XML_Object* operator <<(XML_Object*, vector<XML_Object*>);
    friend XML_Object* operator <<(XML_Object*, XML_Object&);
  protected:
    std::vector<XML_Object*>* content;

// text
  public:
    void set_text(string);
  private:
    string text;

// mode
  public:
    Mode get_mode();
  private:
    Mode mode;
};

class XML_Fact : public XML_Object
{
  public:
    XML_Fact();
    ~XML_Fact();

    string line;
    string filename;

  private:
    string context;
    string pkey;

    int prio;
    int num_clauses;
    short type;
    int hash_clauses;
    int only_logic;
    double truth;

    short everything_q;
};

extern "C" {
    char* record_to_xml(struct RECORD* r);
    int add_record(struct RECORD* r);
    int add_xml_fact(XML_Fact* fact);
    int add_xml_fact_file(const char* filename);
}
#endif /* GENERATED */

// functions begin
#if defined(__cplusplus) /* GENERATED */
int halxml_ordertags(string& indata, string& predata);
string halxml_readfile(string infile);
vector<XML_Fact*> halxml_readfacts(string& prestr, int(*use)(void*, int, time_t*, const char**, int), time_t* start_ref, const char** filename_ref);
XML_Fact* halxml_readfact(vector<string>& lines, int& i);
XML_Fact* record_to_fact(struct RECORD* r, int level);
XML_Object* halxml_readtree(string tag_name, vector<string>& lines, int& i);
XML_Object* operator >>(XML_Object* i, vector<XML_Object*> o);
XML_Object* operator <<(XML_Object* o, vector<XML_Object*> i);
XML_Object* operator <<(XML_Object* o, XML_Object& i);
#endif /* GENERATED */
// functions end
#endif /* HAL2009_XML */

