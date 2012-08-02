/*
 * hal2012-grammar2012.h
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

#ifndef HAL2012_GRAMMAR2012_H_
#define HAL2012_GRAMMAR2012_H_

#include "hal2012-serialization.h"
#include "hal2012-util2012.h"

using namespace std;

namespace algo = boost::algorithm;

namespace grammar2012 {

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
	int order;

	entity();
	void init(const string);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::make_nvp("grammar_ref", grammar_p);
		ar & BOOST_SERIALIZATION_NVP(data);
		ar & BOOST_SERIALIZATION_NVP(symbol);
		ar & boost::serialization::make_nvp("replacesymbol", repl);
		ar & boost::serialization::make_nvp("virtual", virt);
		ar & BOOST_SERIALIZATION_NVP(text);
		ar & boost::serialization::make_nvp("embedded", embed);
		ar & BOOST_SERIALIZATION_NVP(order);
	}

public:
	typedef boost::unordered_multimap<string, string> perlmap;

	entity(grammar*, const string);
	entity(grammar*, const string, vector<entity*>);
	void add(const string);

	perlmap* to_groups(perlmap*, vector<string>, string) const;
	const string print_graph(string*) const;
	const string to_xml(string*, string*, int) const;
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
	int get_order() const;

	static const string print_perl(entity::perlmap*, string, string);
};
std::size_t hash_value(entity const&);
typedef vector<entity*> entities;
typedef vector<entities*> parsed_t;

class grammar: public freehal_base {
private:
	typedef boost::unordered_map<string, entity*> symbolmap_so;
	typedef boost::unordered_map<entity*, string> symbolmap_os;
	typedef boost::unordered_multimap<string, entities*> grammarmap;
	typedef boost::unordered_multimap<string, pair<entity*, entities*> > reducemap;
	typedef vector<string> reducekeys;
	grammarmap* gra;
	vector<reducemap> red;
	vector<reducekeys> red_keys_sorted;
	symbolmap_so* sym_so;
	symbolmap_os* sym_os;

	typedef boost::unordered_map<string, entities*> reducelist;
	typedef boost::unordered_map<string, pair<int, reducelist> > reducelist_by_complexity;

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

	friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const {
		ar & boost::serialization::make_nvp("grammar_map", gra);
		ar & boost::serialization::make_nvp("symbol_to_object", sym_so);
		ar & boost::serialization::make_nvp("object_to_symbol", sym_os);
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version) {
		ar & boost::serialization::make_nvp("grammar_map", gra);
		ar & boost::serialization::make_nvp("symbol_to_object", sym_so);
		ar & boost::serialization::make_nvp("object_to_symbol", sym_os);
		build_reducemap();
	}

    BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
	grammar();
	int read_grammar(const fs::path);
	const string to_str() const;
	void expand();
	vector<entities*>* parse(const string);

	string o2s(entity*) const;
	entity* s2o(string) const;

	entity* modify_symbol(entity*, const vector<string>&);

	const string print_vector(const entities&);
	const string print_entity(entity*);

	static const string print_input(const string);
	static const string print_output(vector<entities*>*);
	static const string print_perl(vector<entities*>*);
	static const string print_graph(vector<entities*>*);
	static const string print_xml(vector<entities*>*);
};

}

#endif /* PARSER2012_H_ */
