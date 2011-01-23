
#ifndef HAL2009_XML
#define HAL2009_XML 1


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

using namespace std;

#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1
#define USE_CXX 1
#include "hal2009.h"

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

string halxml_readfile(string infile);
int halxml_ordertags(string& indata, string& predata);
XML_Object* halxml_readtree(string tag_name, vector<string>& lines, int& i);
XML_Fact* halxml_readfact(vector<string>& lines, int& i);
vector<XML_Fact*> halxml_readfacts(string& prestr);

XML_Fact* record_to_fact(struct RECORD* r, int level);
extern "C" {
    char* record_to_xml_by_xml(struct RECORD* r);
    int add_record(struct RECORD* r);
    int add_xml_fact(XML_Fact* fact);
}

#endif /* HAL2009_XML */

