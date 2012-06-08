/*
 * hal2012-parser2012.h
 *
 *  Created on: 23.05.2012
 *      Author: tobias
 */

#ifndef HAL2012_PARSER2012_H_
#define HAL2012_PARSER2012_H_

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

#include "hal2012-tagger2012.h"
#include "hal2012-grammar2012.h"

namespace grammar2012 {
typedef unsigned int size_t;

class sentence;
class parser;

const string print_vector(const vector<sentence*>&);

enum Mode {
	ONLY_LEARN, QUESTION, STATEMENT, UNKNOWN
};
class sentence {
private:
	string input;
	Mode mode;
	vector<string> words_list;
	vector<tags*> tags_list;
	grammar2012::parsed_type* parsed;
	parser* p;

	sentence();

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_NVP(input);
		ar & BOOST_SERIALIZATION_NVP(mode);
		ar & BOOST_SERIALIZATION_NVP(words_list);
		ar & BOOST_SERIALIZATION_NVP(tags_list);
		ar & BOOST_SERIALIZATION_NVP(parsed);
		ar & boost::serialization::make_nvp("parser_ref", p);
	}

public:
	sentence(parser*, const string&);
	const string to_str() const;
	const string to_grammar_input() const;
	void parse();

	const string get_input() const;
	Mode get_mode() const;
	vector<string> get_words_list() const;
	vector<tags*> get_tags_list() const;
	parsed_type* get_parsed() const;

private:
	void find_mode();
};

class parser {
private:
	string input_raw;
	string input_clean;
	vector<string> input_simplified;
	vector<string> input_extended;
	vector<sentence*> sentences;

	bool verbose;
	bool buffered;
	string lang;
	string path;
	tagger* t;
	grammar* g;

	void clean_input(string&);
	void simplify_input(string&);
	void extend_input(string&);
	void build_pair_sentences(string&, const string&, const string&);
	void build_pair_sentences(string&, const string&, const string&,
			const string&);

	void to_name(string&);
	void remove_adverbs(string&, string&);
	void put_underscore_names_into_builtin_name(string&);
	void to_unixtime(string&);
	void replace_he(string&, const string&);
	void replace_she(string&, const string&);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_NVP(input_raw);
		ar & BOOST_SERIALIZATION_NVP(input_clean);
		ar & BOOST_SERIALIZATION_NVP(input_simplified);
		ar & BOOST_SERIALIZATION_NVP(input_extended);
		ar & BOOST_SERIALIZATION_NVP(sentences);

		ar & BOOST_SERIALIZATION_NVP(verbose);
		ar & BOOST_SERIALIZATION_NVP(buffered);
		ar & BOOST_SERIALIZATION_NVP(lang);
		ar & BOOST_SERIALIZATION_NVP(path);
		ar & boost::serialization::make_nvp("tagger_ref", t);
		ar & boost::serialization::make_nvp("grammar_ref", g);
	}

public:
	parser();
	~parser();
	void parse(const string&);
	vector<sentence*> get_sentences() const;

	void set_lang(const string&);
	void set_path(const string&);
	void set_tagger(tagger*);
	void set_grammar(grammar*);
	const string get_lang() const;
	const string get_path() const;
	tagger* get_tagger() const;
	grammar* get_grammar() const;

	void set_verbose(bool);
	bool is_verbose() const;
	void set_buffered(bool);
	bool is_buffered() const;
};

}

#endif /* HAL2012_PARSER2012_H_ */
