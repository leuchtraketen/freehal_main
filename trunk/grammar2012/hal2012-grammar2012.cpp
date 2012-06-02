/*
 * hal2012-grammar2012.cpp
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-grammar2012.h"

namespace grammar2012 {

const string grammar::print_vector(const entities& v) {
	stringstream ss;
	ss << "[";
	for (entities::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (i != v.begin())
			ss << ", ";
		ss << (*i)->print();
	}
	ss << "]";
	return ss.str();
}
const string grammar::print_entity(entity* i) {
	return "{" + i->print() + "}";
}

entity::entity() :
		data(), symbol(), repl(), virt(), text(), embed() {
}
entity::entity(grammar* _grammar, const string text) :
		grammar_p(_grammar), data(), symbol(), repl(), virt(), text(), embed() {
	init(text);
}
entity::entity(grammar* _grammar, const string text, entities _embed) :
		grammar_p(_grammar), data(), symbol(), repl(), virt(), text(), embed(
				_embed.begin(), _embed.end()) {
	init(text);
}
void entity::add(const string text) {
	init(text);
}
void entity::init(const string text) {
	vector<string> parts;
	algo::split(parts, text, algo::is_any_of("/"));
	vector<string>::iterator it;
	for (it = parts.begin(); it != parts.end(); it++) {
		if (*it == "" || *it == "null") {
			// ignore
		} else if (symbol.size() == 0 && repl.size() == 0 && data.size() == 0) {
			if (algo::starts_with(*it, "s-")) {
				symbol = *it;
			} else if (algo::starts_with(*it, "r-")) {
				repl = *it;
			} else if (algo::starts_with(*it, "d-")) {
				data = *it;
			} else {
				virt.push_back(*it);
			}
		} else {
			if (algo::starts_with(*it, "r-")) {
				// ignore
			} else {
				virt.push_back(*it);
			}
		}
	}
}
entity::perlmap* entity::to_groups(perlmap* pm = 0, vector<string> v_keys =
		vector<string>(), string keyprefix = "v-clause-1") const {

	for (vector<string>::const_iterator it = virt.begin(); it != virt.end();
			++it) {
		if (algo::starts_with(*it, "v-clause")) {
			keyprefix = *it;
		} else if (algo::starts_with(*it, "v-subject")) {
			v_keys.push_back("subjects");
		} else if (algo::starts_with(*it, "v-object")) {
			v_keys.push_back("objects");
		} else if (algo::starts_with(*it, "v-verb")
				&& !algo::starts_with(*it, "v-verbprefix")) {
			v_keys.push_back("verbs");
		} else if (algo::starts_with(*it, "v-verbprefix")) {
			v_keys.push_back("verbprefixes");
		} else if (algo::starts_with(*it, "v-adverb")
				|| algo::starts_with(*it, "v-longadverb")) {
			v_keys.push_back("adverbs");
		} else if (algo::starts_with(*it, "v-questionword")) {
			v_keys.push_back("questionwords");
		} else if (algo::starts_with(*it, "v-extra-after")) {
			v_keys.push_back("extra");
		} else if (algo::starts_with(*it, "v-extra-before")) {
			v_keys.push_back("before-adverbs");
		}
	}

	if (pm == 0) {
		pm = new perlmap();
	}
	if (text.size() > 0) {
		string last = "";
		for (vector<string>::const_iterator v_key_it = v_keys.begin();
				v_key_it != v_keys.end(); ++v_key_it) {
			if (*v_key_it != last)
				pm->insert(
						perlmap::value_type(keyprefix + "/" + *v_key_it, text));
			last = *v_key_it;
		}

	} else {
		if (embed.size() > 0) {
			for (entities::const_iterator it = embed.begin(); it != embed.end();
					++it) {
				(*it)->to_groups(pm, v_keys);
			}
		}
	}

	return pm;
}
const string entity::print_graph(string* _key = 0) const {
	static int u = 1;
	stringstream sskey;
	if (virt.size() > 0)
		sskey << virt.at(0);
	else
		sskey << to_key();
	sskey << " (" << u++ << ")";
	string key = sskey.str();
	if (_key != 0)
		*_key = key;

	stringstream ss;
	ss << "\"" << key << "\" [shape=record,regular=1];" << endl;

	if (text.size() > 0) {
		stringstream sstext;
		sstext << text << " (" << u++ << ")";
		ss << "\"" << sstext.str()
				<< "\" [shape=record,fontsize=14,style=filled,fillcolor=yellow,regular=1];"
				<< endl;
		ss << "\"" << key << "\" -> \"" << sstext.str()
				<< "\" [dir=none,weight=10];" << endl;
	} else {
		if (embed.size() > 0) {
			for (entities::const_iterator it = embed.begin(); it != embed.end();
					++it) {
				string key_it;
				ss << (*it)->print_graph(&key_it);

				ss << "\"" << key << "\" -> \"" << key_it
						<< "\" [dir=none,weight=10];" << endl;
			}
		}
	}

	return ss.str();
}
const string entity::print_perl(entity::perlmap* pm, string v_key = "",
		string keyprefix = "v-clause-1") {
	stringstream ss;
	if (v_key.size() == 0) {
		if (keyprefix == "v-clause-1") {
			ss << "$parsed = ";
		}

		const string subjects = print_perl(pm, "subjects", keyprefix);
		const string objects = print_perl(pm, "objects", keyprefix);
		const string verbs = print_perl(pm, "verbs", keyprefix);
		const string adverbs = print_perl(pm, "before-adverbs", keyprefix)
				+ print_perl(pm, "adverbs", keyprefix);
		const string extra = print_perl(pm, "extra", keyprefix);
		const string questionword = print_perl(pm, "questionwords", keyprefix);

		if (keyprefix == "v-clause-1" || subjects.size() || objects.size()
				|| verbs.size() || adverbs.size() || extra.size()
				|| questionword.size()) {
			ss << "{" << endl;
			ss << "'subjects' => [ " << subjects << " ],";
			ss << "'objects' => [ " << objects << " ],";
			ss << "'verbs' => [ " << verbs << " ],";
			ss << "'adverbs' => [ " << adverbs << " ],";
			ss << "'extra' => [ " << extra << " ],";
			ss << "'questionword' => [ " << questionword << " ],";
			if (keyprefix == "v-clause-1") {
				ss << endl;
				ss << "'clauses' => [";
				ss << print_perl(pm, "", "v-clause-2");
				ss << "],";
			}
			ss << "}";

			if (keyprefix == "v-clause-1") {
				ss << ";";
			} else {
				ss << "," << endl;
			}
		}
	} else {
		if (v_key == "verbs" && pm->count(keyprefix + "/verbs") == 1
				&& pm->count(keyprefix + "/verbprefixes") > 0) {
			// fix verbprefixes
			ss << "[ '" << pm->find(keyprefix + "/verbprefixes")->second
					<< pm->find(keyprefix + "/verbs")->second << "', 0 ], ";
		} else {
			// normal
			pair<perlmap::iterator, perlmap::iterator> iters = pm->equal_range(
					keyprefix + "/" + v_key);
			for (perlmap::iterator iter = iters.first; iter != iters.second;
					++iter) {
				ss << "[ '" << iter->second << "', 0 ], ";
			}
		}
		if (v_key == "adverbs" && pm->count(keyprefix + "/verbs") != 1
				&& pm->count(keyprefix + "/verbprefixes") > 0) {
			// fix verbprefixes
			ss << "[ '" << pm->find(keyprefix + "/verbprefixes")->second
					<< "', 0 ], ";
		}
	}

	return ss.str();
}
const string entity::print_long(string left = "") const {
	string str = to_key();
	if (virt.size()) {
		vector<string>::const_iterator it;
		for (it = virt.begin(); it != virt.end(); ++it) {
			str += "/";
			str += *it;
		}
	}

	stringstream ss;
	ss << left << str;
	if (embed.size()) {
		ss << ":" << endl;
		for (entities::const_iterator it = embed.begin(); it != embed.end();
				++it) {
			ss << (*it)->print_long(left + "  ");
		}
	} else {
		ss << ": '" << text << "'" << endl;
	}
	return ss.str();
}
const string entity::print() const {
	string str = to_key();
	if (virt.size()) {
		vector<string>::const_iterator it;
		for (it = virt.begin(); it != virt.end(); ++it) {
			str += "/";
			str += *it;
		}
	}
	if (text.size() > 0) {
		str += ": '" + text + "'";
	}
	if (embed.size()) {
		str += " < ";
		entities::const_iterator it;
		for (it = embed.begin(); it != embed.end(); ++it) {
			if (it != embed.begin())
				str += ", ";
			str += (*it)->print();
		}
		str += " > ";
	}
	return str;
}
const string entity::to_str() const {
	string str = to_key();
	if (virt.size()) {
		vector<string>::const_iterator it;
		for (it = virt.begin(); it != virt.end(); ++it) {
			str += "/";
			str += *it;
		}
	}
	if (embed.size()) {
		str += " < ";
		entities::const_iterator it;
		for (it = embed.begin(); it != embed.end(); ++it) {
			if (it != embed.begin())
				str += ", ";
			str += (*it)->to_str();
		}
		str += " > ";
	}
	return str;
}
const string entity::to_key() const {
	string str =
			(data.size() == 0) ?
					(symbol.size() == 0 ?
							(repl.size() == 0 ? "null" : repl) : symbol) :
					data;
	return str;
}
const char entity::type() const {
	return (data.size() == 0) ?
			(symbol.size() == 0 ?
					(repl.size() == 0 ? (virt.size() == 0 ? 0 : 'v') : 'r') :
					's') :
			'd';
}
std::size_t hash_value(grammar2012::entity const& o) {
	std::size_t seed = 0;
	boost::hash_combine(seed, o.get_data());
	boost::hash_combine(seed, o.get_repl());
	boost::hash_combine(seed, o.get_symbol());
	boost::hash_combine(seed, o.get_virt());
	boost::hash_combine(seed, o.get_embed());
	return seed;
}
void entity::set_text(const string text) {
	this->text = text;
}
const string entity::get_data() const {
	return data;
}
const string entity::get_symbol() const {
	return symbol;
}
const string entity::get_repl() const {
	return repl;
}
const vector<string> entity::get_virt() const {
	return virt;
}
const vector<entity*> entity::get_embed() const {
	return embed;
}
const vector<string> entity::get_marker() const {
	if (virt.size() > 0) {
		return virt;
	} else {
		vector<string> v;
		v.push_back(to_str());
		return v;
	}
}

grammar::grammar() :
		gra(new grammarmap()), red(new reducemap()), red_keys_sorted(
				new reducekeys()), sym_so(new symbolmap_so()), sym_os(
				new symbolmap_os()), verbose(true), buffered(false) {
}

int grammar::read_grammar(const string filename) {
	ifstream i;
	i.open(filename.c_str());

	if (!i) {
		cout << "Error! Could not open grammar file: " << filename << endl;
		return 1;
	}

	string line;
	entity* key_obj;
	while (std::getline(i, line)) {
		if (algo::ends_with(line, ":")) {
			algo::trim_right_if(line, boost::is_any_of(":"));
			key_obj = add_symbol(line);
		}
		if (algo::starts_with(line, "  ")) {
			algo::trim_left_if(line, boost::is_any_of(" "));

			entities* value_i = new entities();
			vector<string> parts;
			algo::split(parts, line, algo::is_any_of(" "));
			vector<string>::iterator it;
			for (it = parts.begin(); it != parts.end(); it++) {
				entity* val_obj = add_symbol(*it);
				if (val_obj->type() != 0) {
					value_i->push_back(val_obj);
				}
			}

			gra->insert(grammarmap::value_type(key_obj->to_key(), value_i));
		}
	}

	build_reducemap();

	return 0;
}

string grammar::o2s(entity* o) const {
	if (o == NULL) {
		return "null";
	}
	symbolmap_os::iterator found = sym_os->find(o);
	if (found != sym_os->end()) {
		return found->second;
	}
	return 0;
}
entity* grammar::s2o(string s) const {
	symbolmap_so::iterator found = sym_so->find(s);
	if (found != sym_so->end()) {
		return found->second;
	}
	return 0;
}

entity* grammar::add_symbol(const string str) {
	entity* key = s2o(str);
	if (key != NULL) {
		return key;
	}

	key = new entity(this, str);
	if (key->to_str() != str) {
		cout << "Error! key->to_str() != line: " << key->to_str() << " != "
				<< str << endl;
	}
	add_entity(key);
	return key;
}

entity* grammar::add_entity(entity* key) {
	sym_os->insert(symbolmap_os::value_type(key, key->to_str()));
	sym_so->insert(symbolmap_so::value_type(key->to_str(), key));
	return key;
}

entity* grammar::modify_symbol(entity* obj, const vector<string>& _str) {
	string str;
	vector<string>::const_iterator it;
	for (it = _str.begin(); it != _str.end(); ++it) {
		str += "/" + *it;
	}

	entity* modified = new entity(*obj);
	modified->add(str);

	return add_entity(modified);
}

const string grammar::to_str() const {
	string grammar_str;
	grammarmap::iterator it;
	for (it = gra->begin(); it != gra->end(); ++it) {
		stringstream ss;

		ss << s2o(it->first)->to_str();
		ss << " = ";

		entities* value = it->second;
		entities::iterator val;
		for (val = value->begin(); val != value->end(); ++val) {
			if (val != value->begin()) {
				ss << ", ";
			}
			ss << (*val)->to_str();
		}
		if (it->second->size() == 0) {
			ss << "EMPTY";
		}

		ss << endl;
		grammar_str += ss.str();
	}
	return grammar_str;
}

int insert_back(entities::iterator begin, entities::iterator end,
		entities* into, entity* previous, grammar* grammar) {

	/// cout << "get_virt: size=" << previous->get_virt().size() << endl;
	if (previous->get_marker().size() > 0) {
		entities::iterator it;
		for (it = begin; it != end; ++it) {
			entity* e = grammar->modify_symbol(*it, previous->get_marker());
			into->push_back(e);
		}
	} else {
		std::copy(begin, end, std::back_inserter(*into));
	}
	return 0;
}

const string grammar::all_get_key(const entities& vec) {
	if (vec.size() == 0)
		return "";
	stringstream str;
	str << " ";
	entities::const_iterator it;
	for (it = vec.begin(); it != vec.end(); ++it) {
		str << (*it)->to_key() << " ";
	}
	return str.str();
}

const string grammar::all_to_str(const entities& vec) const {
	if (vec.size() == 0)
		return "";
	stringstream str;
	str << " ";
	entities::const_iterator it;
	for (it = vec.begin(); it != vec.end(); ++it) {
		str << (*it)->to_str() << " ";
	}
	return str.str();
}

bool string_compare_by_length(const string &left, const string &right) {
	if (left.size() > right.size())
		return true;
	else
		return false;
}
void grammar::build_reducemap() {
	if (red != 0) {
		delete red;
		red = 0;
	}

	red = new reducemap();
	boost::unordered_set<string> red_keys;

	grammarmap::iterator it;
	for (it = gra->begin(); it != gra->end(); ++it) {
		if (s2o(it->first)->get_repl().size() == 0) {
			const string keys = all_get_key(*it->second);
			if (keys.size() > 0) {
				red->insert(
						reducemap::value_type(keys,
								pair<entity*, entities*>(s2o(it->first),
										it->second)));
				red_keys.insert(keys);
			}
		}
	}

	red_keys_sorted = new reducekeys(red_keys.begin(), red_keys.end());
	std::sort(red_keys_sorted->begin(), red_keys_sorted->end(),
			string_compare_by_length);
}

vector<entities*>* grammar::expand_entry(entities* oldvalue, int* expanded,
		bool* complete) {

	vector<entities*>* newvalues = new vector<entities*>();
	newvalues->push_back(new entities());

	entities::iterator oldvalue_iter;
	for (oldvalue_iter = oldvalue->begin(); oldvalue_iter != oldvalue->end();
			++oldvalue_iter) {

		entity* to_be_replaced_obj = *oldvalue_iter;

		string key_of_to_be_replaced = to_be_replaced_obj->to_key();
		int count = gra->count(key_of_to_be_replaced);

		if (to_be_replaced_obj->type() == 'r') {

			if (count == 0) {
				cout << "Error! Count of symbol is 0: "
						<< to_be_replaced_obj->to_str() << endl;
			} else if (count == 1) {
				entities* replacement = gra->find(key_of_to_be_replaced)->second;
				for (size_t j = 0; j < newvalues->size(); ++j) {
					insert_back(replacement->begin(), replacement->end(),
							newvalues->at(j), to_be_replaced_obj, this);
				}
			} else { // count >= 2
				if (expanded)
					++(*expanded);
				if (newvalues->size() == 1) {
					// make count minus 1 copies
					int g;
					for (g = 1; g < count; ++g) {
						newvalues->push_back(new entities(*newvalues->at(0)));
					}

					// for each copy, add the replacement at the end
					pair<grammarmap::iterator, grammarmap::iterator> it_pair =
							gra->equal_range(key_of_to_be_replaced);
					grammarmap::iterator replacement;
					g = 0;
					for (replacement = it_pair.first;
							replacement != it_pair.second; ++replacement) {

						insert_back(replacement->second->begin(),
								replacement->second->end(), newvalues->at(g++),
								to_be_replaced_obj, this);
					}
				} else {
					// we need one more run of this function to handle this
					// just copy the rest
					(*complete) = false;
					for (size_t j = 0; j < newvalues->size(); ++j) {
						newvalues->at(j)->push_back(to_be_replaced_obj);
					}
				}
			}
		} else {
			for (size_t j = 0; j < newvalues->size(); ++j) {
				newvalues->at(j)->push_back(to_be_replaced_obj);
			}
		}
	}
	return newvalues;
}

bool grammar::expand_step(int* expanded) {
	bool complete = true;
	grammarmap* newgrammar = new grammarmap();

	grammarmap::iterator it;
	for (it = gra->begin(); it != gra->end(); ++it) {

		entities* oldvalue = it->second;
		vector<entities*>* newvalues = expand_entry(oldvalue, expanded,
				&complete);

		for (size_t j = 0; j < newvalues->size(); ++j) {
			newgrammar->insert(
					grammarmap::value_type(it->first, newvalues->at(j)));
		}
	}

	delete gra;
	gra = newgrammar;

	return complete;
}

void grammar::expand() {
	bool complete = false;
	int run = 0;
	while (!complete) {
		if (is_verbose())
			cout << "Expanding grammar: Step " << ++run;
		int expanded = 0;

		complete = expand_step(&expanded);

		if (is_verbose())
			cout << " (" << expanded << ")" << endl;
	}

	build_reducemap();
}

entities* grammar::parse_input(const string words_str) {
	entities* words_i = new entities();

	// marker
	{
		entity* obj = add_entity(new entity(this, "d-^"));
		obj->set_text("");
		words_i->push_back(obj);
	}

	// split by # or >
	vector<string> words;
	algo::split(words, words_str, algo::is_any_of("#>"));
	vector<string>::iterator word;
	for (word = words.begin(); word != words.end(); ++word) {
		// ignore invalid words
		if (word->size() == 0 || *word == "null") {
			continue;
		}

		// split by | or <
		vector<string> parts;
		algo::split(parts, *word, algo::is_any_of("|<"));
		if (parts.size() != 2) {
			cout << "Error! no or too many pipe characters in: \"" << *word
					<< "\" from: \"" << words_str << "\"" << endl;
			continue;
		}

		// construct objects
		string part_of_speech = parts.at(0);
		string text = parts.at(1);
		boost::trim(part_of_speech);
		boost::trim(text);
		entity* obj = add_entity(new entity(this, part_of_speech));
		obj->set_text(text);
		words_i->push_back(obj);
	}

	// marker
	{
		entity* obj = add_entity(new entity(this, "d-$"));
		obj->set_text("");
		words_i->push_back(obj);
	}

	return words_i;
}
entities* grammar::replace_in_vector(const entities& vec, const entities& find,
		entity* replacement) {
	if (find.size() == 0)
		return new entities(vec.begin(), vec.end());

	entities* new_vector = new entities();
	for (size_t i = 0; i < vec.size(); ++i) {
		if (vec.at(i)->to_key() == find.at(0)->to_key()) {
			bool found = true;
			size_t f, j;
			for (f = 1, j = i + 1; j < vec.size() && f < find.size();
					++f, ++j) {
				if (vec.at(j)->to_key() != find.at(f)->to_key()) {
					found = false;
					break;
				}
			}
			if (found) {
				entities found_vec;
				for (f = 0, j = i; j < vec.size() && f < find.size();
						++f, ++j) {
					entity* e = modify_symbol(vec.at(j),
							find.at(f)->get_marker());

					found_vec.push_back(e); ///////////////////////////
				}

				entity* e = new entity(this, replacement->to_str(), found_vec);
				add_entity(e);
				new_vector->push_back(e);
				i = j - 1;
			} else {
				new_vector->push_back(vec.at(i));
			}
		} else {
			new_vector->push_back(vec.at(i));
		}
	}

	if (is_verbose())
		cout << "      replace " << print_vector(find) << " with "
				<< print_entity(replacement) << endl << "      in "
				<< print_vector(vec) << endl << "      is "
				<< print_vector(*new_vector) << endl;

	return new_vector;
}
grammar::reducelist* grammar::reduce_step(entities* old_words_i) {
	const string old_impression = all_get_key(*old_words_i);
	reducelist_by_complexity*new_words_complexity_map =
			new reducelist_by_complexity();
	string in_this_step_reduce_to = "";

	// for each key
	reducekeys::iterator iter;
	for (iter = red_keys_sorted->begin(); iter != red_keys_sorted->end();
			++iter) {
		///cout << *iter << endl;

		// does it match?
		if (old_impression.find(*iter) != string::npos) {
			if (is_verbose())
				cout << "    found: '" << *iter << "' in '" << old_impression
						<< "'" << endl;
			pair<reducemap::iterator, reducemap::iterator> it_pair =
					red->equal_range(*iter);

			// for each rule we found
			for (reducemap::iterator rule_iter = it_pair.first;
					rule_iter != it_pair.second; ++rule_iter) {

				const string& complexity_key = print_entity(
						rule_iter->second.first);
				if (in_this_step_reduce_to.size() > 0
						&& complexity_key != in_this_step_reduce_to) {
					cout
							<< "    wrong target entity, in this step we'll reduce to "
							<< in_this_step_reduce_to << endl;
					continue;
				}

				if (is_verbose())
					cout << "    rule: " << *iter << " --> "
							<< print_entity(rule_iter->second.first) << " ; "
							<< print_vector(*rule_iter->second.second) << endl;

				int complexity = rule_iter->second.second->size();
				int best_complexity = 0;
				if (new_words_complexity_map->find(complexity_key)
						!= new_words_complexity_map->end()) {
					best_complexity = new_words_complexity_map->find(
							complexity_key)->second.first;
				}
				if (complexity >= best_complexity) {
					if (complexity > best_complexity) {
						new_words_complexity_map->erase(complexity_key);
						new_words_complexity_map->insert(
								reducelist_by_complexity::value_type(
										complexity_key,
										pair<int, reducelist>(complexity,
												reducelist())));
						in_this_step_reduce_to = complexity_key;

						if (is_verbose())
							cout
									<< "      best complexity. deleting worse data. ("
									<< complexity << " > " << best_complexity
									<< ")" << endl;
					} else {
						if (is_verbose())
							cout << "      equal complexity. (" << complexity
									<< " = " << best_complexity << ")" << endl;

					}

					entities* new_words_i = replace_in_vector(*old_words_i,
							*rule_iter->second.second, rule_iter->second.first);

					new_words_complexity_map->find(complexity_key)->second.second.insert(
							reducelist::value_type(all_to_str(*new_words_i),
									new_words_i));

				} else {
					if (is_verbose())
						cout << "      too low complexity. (" << complexity
								<< " < " << best_complexity << ")" << endl;

				}
				//new_words_list->insert(
				//		reducelist::value_type(all_to_str(*new_words_i),
				//			new_words_i));
			}
		}
	}

	reducelist* new_words_list = new reducelist();
	for (reducelist_by_complexity::iterator iter =
			new_words_complexity_map->begin();
			iter != new_words_complexity_map->end(); ++iter) {
		for (reducelist::iterator i = iter->second.second.begin();
				i != iter->second.second.end(); ++i) {
			new_words_list->insert(*i);
		}
	}
	delete new_words_complexity_map;
	return new_words_list;
}

vector<entities*>* grammar::reduce(entities* old_words_i) {
	//if (is_verbose())
	cout << "reduce: " << print_vector(*old_words_i) << endl << endl;

	vector<entities*>* final = new vector<entities*>();
	reducelist* words_list = new reducelist();
	words_list->insert(
			reducelist::value_type(all_to_str(*old_words_i), old_words_i));
	int step = 0;
	while (step++ <= 50) {
		//	if (is_verbose())
		cout << "reduce step " << step << "... " << words_list->size()
				<< " possibilities..." << endl << endl;
		if (words_list->size() == 0)
			break;

		reducelist* new_words_list = new reducelist();
		for (reducelist::iterator iter = words_list->begin();
				iter != words_list->end(); ++iter) {
			if (is_verbose())
				cout << "  with " << print_vector(*iter->second) << endl;
			reducelist* temp = reduce_step(iter->second);

			for (reducelist::iterator temp_iter = temp->begin();
					temp_iter != temp->end(); ++temp_iter) {

				new_words_list->insert(*temp_iter);
			}
			delete temp;
			temp = 0;
			if (is_verbose())
				cout << endl;
		}
		delete words_list;
		words_list = new_words_list;

		for (reducelist::iterator iter = words_list->begin();
				iter != words_list->end(); ++iter) {
			if (iter->second->size() == 1
					&& iter->second->at(0)->to_key() == "s-all") {

				if (is_verbose())
					cout << " !!! done: " << print_vector(*iter->second)
							<< endl;
				final->push_back(iter->second);
			}
		}
	}

	return final;
}
const string grammar::print_input(const string words_str) {
	stringstream ss;

	// split by #
	vector<string> words;
	algo::split(words, words_str, algo::is_any_of("#>"));
	vector<string>::iterator word;
	for (word = words.begin(); word != words.end(); ++word) {
		// ignore invalid words
		if (word->size() == 0 || *word == "null") {
			continue;
		}

		// split by |
		vector<string> parts;
		algo::split(parts, *word, algo::is_any_of("|<"));
		if (parts.size() != 2) {
			cout << "  - Error! no or too many pipe characters in:" << endl
					<< "    " << *word << endl;
			continue;
		}

		string part_of_speech = parts.at(0);
		string text = parts.at(1);
		boost::trim(part_of_speech);
		boost::trim(text);
		ss << "  - " << part_of_speech << ": '" << text << "'" << endl;
	}

	return ss.str();
}

const string grammar::print_output(vector<entities*>* output_list) {
	stringstream ss;

	for (vector<entities*>::iterator iter_list = output_list->begin();
			iter_list != output_list->end(); ++iter_list) {
		entities* output = *iter_list;
		for (entities::iterator iter = output->begin(); iter != output->end();
				++iter) {
			ss << (*iter)->print_long();

			/*entity::perlmap* pm = (*iter)->to_groups();
			 for (entity::perlmap::iterator it = pm->begin(); it != pm->end();
			 ++it) {
			 ss << it->first << " => " << it->second << endl;
			 }*/

			ss << endl << endl;
			entity::perlmap* pm = (*iter)->to_groups();
			ss << entity::print_perl(pm);
			ss << endl << endl;
		}
	}

	return ss.str();
}
const string grammar::print_perl(vector<entities*>* output_list) {
	stringstream ss;

	for (vector<entities*>::iterator iter_list = output_list->begin();
			iter_list != output_list->end(); ++iter_list) {
		entities* output = *iter_list;
		for (entities::iterator iter = output->begin(); iter != output->end();
				++iter) {
			entity::perlmap* pm = (*iter)->to_groups();
			ss << entity::print_perl(pm);
		}
	}

	return ss.str();
}
const string grammar::print_graph(vector<entities*>* output_list) {
	stringstream ss;
	ss << "digraph parsed {" << endl;

	for (vector<entities*>::iterator iter_list = output_list->begin();
			iter_list != output_list->end(); ++iter_list) {
		entities* output = *iter_list;
		for (entities::iterator iter = output->begin(); iter != output->end();
				++iter) {
			ss << (*iter)->print_graph();
		}
		break;
	}

	ss << "}" << endl;
	return ss.str();
}
vector<entities*>* grammar::parse(const string words_str) {
	cout << "========================================" << endl
			<< "============  Grammar 2012  ============" << endl
			<< "========================================" << endl << endl
			<< "input: " << endl << endl << grammar::print_input(words_str)
			<< endl;

	entities* words_i = parse_input(words_str);
	vector<entities*>* reduced = reduce(words_i);
	reduced->empty();

	cout << "output:" << endl << endl << grammar::print_output(reduced) << endl;

	return reduced;
}

void grammar::set_verbose(bool v) {
	verbose = v;
}
bool grammar::is_verbose() {
	return verbose;
}

void grammar::set_buffered(bool v) {
	buffered = v;
}
bool grammar::is_buffered() {
	return buffered;
}

}
