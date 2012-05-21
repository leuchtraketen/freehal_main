/*
 * grammar2012.h
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

#ifndef PARSER2012_H_
#define PARSER2012_H_

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find_format.hpp>
#include <boost/algorithm/string/split.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace algo = boost::algorithm;

namespace grammar2012 {
typedef unsigned int size_t;

class entity;
class grammar;

class entity {
private:

	grammar* grammar_p;
	string data;
	string symbol;
	string repl;
	vector<string> virt;
	string text;
	vector<entity*> embed;

	entity();
	void init(const string);

public:
	typedef boost::unordered_multimap<string, string> perlmap;

	entity(grammar*, const string);
	entity(grammar*, const string, vector<entity*>);
	void add(const string);

	perlmap* to_groups(perlmap*, vector<string>, string) const;
	const string print_graph(string*) const;
	const string print() const;
	const string print_long(string) const;
	const string to_str() const;
	const string to_key() const;
	const char type() const;

	void set_text(const string);

	const string get_data() const;
	const string get_symbol() const;
	const string get_repl() const;
	const vector<string> get_virt() const;
	const vector<string> get_marker() const;
	const vector<entity*> get_embed() const;

	static const string print_perl(entity::perlmap*, string, string);
};
std::size_t hash_value(entity const&);
typedef vector<entity*> entities;
typedef vector<entities*> parsed_type;

class grammar {
private:
	typedef boost::unordered_map<string, entity*> symbolmap_so;
	typedef boost::unordered_map<entity*, string> symbolmap_os;
	typedef boost::unordered_multimap<string, entities*> grammarmap;
	typedef boost::unordered_multimap<string, pair<entity*, entities*> > reducemap;
	typedef vector<string> reducekeys;
	grammarmap* gra;
	reducemap* red;
	reducekeys* red_keys_sorted;
	symbolmap_so* sym_so;
	symbolmap_os* sym_os;
	bool verbose;

	typedef boost::unordered_map<string, entities*> reducelist;

	entity* add_entity(entity*);
	entity* add_symbol(const string);
	entity* get_symbol(const string) const;
	const string all_get_key(const entities&);
	const string all_to_str(const entities&) const;

	void build_reducemap();
	bool expand_step(int*);
	vector<entities*>* expand_entry(entities*, int*, bool*);
	entities* parse_input(const string);
	vector<entities*>* reduce(entities*i);
	reducelist* reduce_step(entities*i);
	entities* replace_in_vector(const entities&, const entities&, entity*);

public:
	grammar();
	int read_grammar(const string);
	const string to_str() const;
	void expand();
	vector<entities*>* parse(const string);
	void set_verbose(bool);
	bool is_verbose();

	string o2s(entity*) const;
	entity* s2o(string) const;

	entity* modify_symbol(entity*, const vector<string>&);

	const string print_vector(const entities&);
	const string print_entity(entity*);

	static const string print_input(const string);
	static const string print_output(vector<entities*>*);
	static const string print_perl(vector<entities*>*);
	static const string print_graph(vector<entities*>*);
};

}

#endif /* PARSER2012_H_ */
