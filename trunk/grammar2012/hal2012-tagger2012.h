/*
 * hal2012-tagger2012.h
 *
 *  Created on: 23.05.2012
 *      Author: tobias
 */

#ifndef HAL2012_TAGGER2012_H_
#define HAL2012_TAGGER2012_H_

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/regex.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace algo = boost::algorithm;

#define EXTERN_C extern "C"
EXTERN_C char* check_config(const char* name, const char* _default);

namespace grammar2012 {
typedef unsigned int size_t;
typedef pair<string, string> tags;

bool is_invalid_char(char);
void to_ascii(string&);
bool is_empty(const tags*);
bool regex(boost::smatch&, const string&, const string&);
bool regex_case(boost::smatch&, const string&, const string&);
const string print_tags(const tags*);

class tagger;

class tagger {
private:
	typedef boost::unordered_map<string, string> tagmap;
	typedef vector<pair<string, string> > taglist;
	tagmap* type;
	tagmap* genus;
	taglist* regex_type;
	tagmap* regex_genus;
	bool verbose;
	bool buffered;

	void impl_get_pos(const string, tags*);
	void impl_regex_get_pos(const string, tags*);
	void impl_guess(const string, tags*);
	void guess(const string, tags*);
	void ask_user(const string, tags*);

public:
	tagger();
	~tagger();
	int read_pos_file(const string);
	int read_regex_pos_file(const string);
	tags* get_pos(const string);
	void set_verbose(bool);
	bool is_verbose();
	void set_buffered(bool);
	bool is_buffered();
};

}

#endif /* HAL2012_PARSER2012_H_ */
