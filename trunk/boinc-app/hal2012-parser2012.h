/*
 * hal2012-parser2012.h
 *
 *  Created on: 23.05.2012
 *      Author: tobias
 */

#ifndef HAL2012_PARSER2012_H_
#define HAL2012_PARSER2012_H_

#include "hal2012-util2012.h"
#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-database2012.h"
#include "hal2012-serialization.h"

#define EXTERN_C extern "C"
EXTERN_C char* check_config(const char* name, const char* _default);

namespace grammar2012 {

class sentence;
class parser;

const string print_vector(const vector<sentence*>&);

enum sentence_mode {
	ONLY_LEARN, QUESTION, STATEMENT, UNKNOWN
};
class sentence {
private:
	string input;
	sentence_mode mode;
	vector<string> words_list;
	vector<tags*> tags_list;
	boost::shared_ptr<parsed_t> parsed;
	boost::shared_ptr<xml_fact> xfact;
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
	~sentence();
	const string to_str() const;
	const string to_grammar_input() const;
	void parse();

	const string get_input() const;
	sentence_mode get_mode() const;
	vector<string> get_words_list() const;
	vector<tags*> get_tags_list() const;
	boost::shared_ptr<parsed_t> get_parsed() const;
	boost::shared_ptr<xml_fact> get_fact() const;

private:
	void find_mode();
};

class parser: public freehal_base {
private:
	string input_raw;
	string input_clean;
	vector<string> input_simplified;
	vector<string> input_extended;
	vector<sentence*> sentences;

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

		ar & boost::serialization::make_nvp("tagger_ref", t);
		ar & boost::serialization::make_nvp("grammar_ref", g);
	}

public:
	parser();
	~parser();
	void parse(const string&);
	vector<sentence*> get_sentences() const;

	void set_tagger(tagger*);
	void set_grammar(grammar*);
	tagger* get_tagger() const;
	grammar* get_grammar() const;
};

}

#endif /* HAL2012_PARSER2012_H_ */
