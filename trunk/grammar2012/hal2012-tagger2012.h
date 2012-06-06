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

#include "hal2012-serialization.h"

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
typedef pair<string, string> tags;

bool is_empty(const tags*);
const string print_tags(const tags*);
const string print_vector(const vector<tags*>&);

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

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::make_nvp("type_map", type);
		ar & boost::serialization::make_nvp("genus_map", genus);
		ar & boost::serialization::make_nvp("regex_type_map", regex_type);
		ar & boost::serialization::make_nvp("regex_genus_map", regex_genus);;
		ar & BOOST_SERIALIZATION_NVP(verbose);
		ar & BOOST_SERIALIZATION_NVP(buffered);
	}

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

	static bool is_name(const string&);
	static bool is_job(const string&);
	static const string unique_pos_type(const string&);
	static const string to_grammar_pos(tags*, const string&);

	static string __builtin_entity_ends;
	static string __builtin_male_names;
	static string __builtin_female_names;
	static string __builtin_pos_types[];
	static vector<string> builtin_entity_ends;
	static vector<string> builtin_male_names;
	static vector<string> builtin_female_names;
	static vector<string> custom_names;
};

}

#endif /* HAL2012_PARSER2012_H_ */
