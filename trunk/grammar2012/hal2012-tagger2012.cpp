/*
 * hal2012-tagger2012.cpp
 *
 *  Created on: 23.05.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-tagger2012.h"

namespace grammar2012 {

bool is_empty(const tags* tags) {
	return tags->first.size() == 0 && tags->second.size() == 0;
}
const string print_tags(const tags* tags) {
	return "type=" + tags->first
			+ (tags->second.size() == 0 ? "" : ",genus=" + tags->second);
}
const string print_vector(const vector<tags*>& v) {
	stringstream ss;
	ss << "[";
	for (vector<tags*>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (i != v.begin())
			ss << ", ";
		ss << "(" << print_tags(*i) << ")";
	}
	ss << "]";
	return ss.str();
}

tagger::tagger() :
		type(new tagmap()), genus(new tagmap()), regex_type(new taglist()), regex_genus(
				new tagmap()), verbose(true), buffered(false) {

	algo::split(builtin_entity_ends, __builtin_entity_ends,
			algo::is_any_of(";"));
	algo::split(builtin_male_names, __builtin_male_names, algo::is_any_of(";"));
	algo::split(builtin_female_names, __builtin_female_names,
			algo::is_any_of(";"));

	{
		vector<string> splitted;
		for (int i = 0; __builtin_pos_types[i].size() > 0; ++i) {
			const string& builtin_pos = __builtin_pos_types[i];

			algo::split(splitted, builtin_pos,
					algo::is_any_of(":") || algo::is_space(),
					algo::token_compress_on);
			if (splitted.size() == 2) {
				const string& word = splitted[0];
				const string& pos = unique_pos_type(splitted[1]);
				type->insert(tagmap::value_type(word, pos));
			} else {
				cout << "Error! invalid builtin part of speech: " << quote
						<< builtin_pos << quote << endl;
			}
		}
	}
}
tagger::~tagger() {
	delete type;
	delete genus;
	delete regex_type;
	delete regex_genus;
}

void tagger::set_verbose(bool v) {
	verbose = v;
}
bool tagger::is_verbose() {
	return verbose;
}

void tagger::set_buffered(bool v) {
	buffered = v;
}
bool tagger::is_buffered() {
	return buffered;
}

int tagger::read_pos_file(const string filename) {
	ifstream i;
	i.open(filename.c_str());

	if (!i) {
		cout << "Error! Could not open part of speech file: " << filename
				<< endl;
		return 1;
	}

	if (is_verbose())
		cout << "read part of speech file: " << filename << endl;

	string line;
	vector<string> lines;
	while (std::getline(i, line)) {
		lines.push_back(line);
	}

	//string line;
	string word;
	int n = 0;
	//while (std::getline(i, line)) {
	for (vector<string>::iterator iter = lines.begin(); iter != lines.end();
			++iter) {
		line = *iter;

		algo::trim_right_if(line, boost::is_space());
		if (algo::ends_with(line, ":")) {
			algo::trim_if(line, boost::is_any_of(":") || boost::is_space());
			word = line;
		}
		if (algo::starts_with(line, " ")) {
			algo::trim_if(line, boost::is_any_of(":,;") || boost::is_space());
			if (algo::starts_with(line, "type")) {
				line = line.substr(4);
				algo::trim_left_if(line,
						boost::is_any_of(":") || boost::is_space());
				line = unique_pos_type(line);
				type->insert(tagmap::value_type(word, line));
			} else if (algo::starts_with(line, "genus")) {
				line = line.substr(5);
				algo::trim_left_if(line,
						boost::is_any_of(":") || boost::is_space());
				genus->insert(tagmap::value_type(word, line));
			}
		}

		if (is_verbose() && ++n % 10000 == 0) {
			cout << "\r  " << n << " lines...          " << flush;
			if (is_buffered())
				cout << endl;
		}
	}
	if (is_verbose()) {
		cout << "\r  " << n << " lines.          " << endl << flush;
	}

	return 0;
}

int tagger::read_regex_pos_file(const string filename) {
	ifstream i;
	i.open(filename.c_str());

	if (!i) {
		cout << "Error! Could not open regex part of speech file: " << filename
				<< endl;
		return 1;
	}

	if (is_verbose())
		cout << "read regex part of speech file: " << filename << endl;

	string line;
	string word;
	int n = 0;
	while (std::getline(i, line)) {
		algo::trim_right_if(line, boost::is_space());
		if (algo::ends_with(line, ":")) {
			algo::trim_if(line, boost::is_any_of(":") || boost::is_space());
			word = line;
		}
		if (algo::starts_with(line, " ")) {
			algo::trim_if(line, boost::is_any_of(":,;") || boost::is_space());
			if (algo::starts_with(line, "type")) {
				line = line.substr(4);
				algo::trim_left_if(line,
						boost::is_any_of(":") || boost::is_space());
				line = unique_pos_type(line);
				regex_type->push_back(pair<string, string>(word, line));
			} else if (algo::starts_with(line, "genus")) {
				line = line.substr(5);
				algo::trim_left_if(line,
						boost::is_any_of(":") || boost::is_space());
				regex_genus->insert(tagmap::value_type(word, line));
			}
		}

		if (is_verbose() && ++n % 10000 == 0) {
			cout << "\r  " << n << " lines...          " << flush;
			if (is_buffered())
				cout << endl;
		}
	}
	if (is_verbose()) {
		cout << "\r  " << n << " lines.          " << endl << flush;
	}

	return 0;
}

void tagger::impl_get_pos(const string word, tags* tags) {
	if (type->count(word)) {
		tags->first = type->find(word)->second;
		if (tags->first == "n" && genus->count(word)) {
			tags->second = genus->find(word)->second;
		}
		if (is_verbose())
			cout << "  found: " << print_tags(tags) << endl;
	}
}
void tagger::impl_regex_get_pos(const string word, tags* tags) {
	for (taglist::iterator iter = regex_type->begin();
			iter != regex_type->end(); ++iter) {
		const string& exp = iter->first;
		const string& value = iter->second;

		boost::smatch result;
		if (regex_ifind(result, word, exp)) {
			tags->first = value;
			if (tags->first == "n" && regex_genus->count(exp)) {
				tags->second = regex_genus->find(exp)->second;
			}
			if (is_verbose())
				cout << "  found: " << print_tags(tags) << endl
						<< "  by regex: '" << exp << "'" << endl;
			return;
		}

	}
}

tags* tagger::get_pos(const string _word) {
	string word = _word;
	to_ascii(word);
	tags* tags = new grammar2012::tags("", "");
	if (word.size() == 0) {
		return tags;
	}
	if (is_verbose())
		cout << "get part of speech: " << word << endl;

	if (is_empty(tags))
		impl_get_pos(word, tags);
	if (is_empty(tags))
		impl_get_pos(ucfirst(word), tags);
	if (is_empty(tags))
		impl_get_pos(lc(word), tags);

	if (is_empty(tags)) {
		boost::smatch result;
		if (word == "," || word == ";") {
			tags->first = "komma";
			if (is_verbose())
				cout << "  builtin: " << print_tags(tags) << endl;
		}
		if (regex_ifind(result, word, "[{]{3}(.*?)[}]{3}")) {
			tags->first = result[1];
			if (is_verbose())
				cout << "  predefined: " << print_tags(tags) << endl;
		}
	}
	if (is_empty(tags))
		if (is_name(word)) {
			tags->first = "n";
			if (is_verbose())
				cout << "  is name: " << print_tags(tags) << endl;
		}
	if (is_empty(tags))
		impl_regex_get_pos(word, tags);
	if (is_empty(tags)) {
		boost::smatch result;
		if (regex_find(result, word, "[ABCDEFGHIJKLMNOPQRSTUVWXYZ]")) {
			tags->first = "n";
			if (is_verbose())
				cout << "  found: " << print_tags(tags) << endl
						<< "  (upper case)" << endl;
		}
	}

	if (is_empty(tags))
		guess(word, tags);
	if (is_empty(tags))
		ask_user(word, tags);

	if (is_empty(tags)) {
		if (is_verbose())
			cout << "  not found." << endl;
	}
	return tags;
}

void tagger::impl_guess(const string word, tags* tags) {

	//return; /////////////////////////////////

	int word_size = word.size();
	bool word_is_lower = is_lower(word);
	if (is_verbose())
		cout << "  guess: " << word << endl;

	boost::unordered_map<string, int> ends;
	if (word_size > 1)
		ends.insert(pair<string, int>(word.substr(word_size - 1), 2));
	if (word_size > 2)
		ends.insert(pair<string, int>(word.substr(word_size - 2), 6));
	if (word_size > 3)
		ends.insert(pair<string, int>(word.substr(word_size - 3), 12));
	if (word_size > 4)
		ends.insert(pair<string, int>(word.substr(word_size - 4), 8));
	if (word_size > 5)
		ends.insert(pair<string, int>(word.substr(word_size - 5), 10));
	if (word_size > 6)
		ends.insert(pair<string, int>(word.substr(word_size - 6), 12));
	boost::unordered_map<string, int> begins;
	if (word_size > 1)
		begins.insert(pair<string, int>(word.substr(0, 1), 1));
	if (word_size > 2)
		begins.insert(pair<string, int>(word.substr(0, 2), 2));
	if (word_size > 3)
		begins.insert(pair<string, int>(word.substr(0, 3), 3));

	boost::unordered_map<string, int> pos_to_rating;
	boost::unordered_map<string, int> pos_to_count;

	for (tagmap::iterator iter = type->begin(); iter != type->end(); ++iter) {
		const string& val_word = iter->first;
		const string& val_type = iter->second;
		if (val_type != "n" && val_type != "adj" && val_type != "v")
			continue;
		bool val_word_is_lower = is_lower(val_word);

		if ((word_is_lower && !val_word_is_lower)
				|| (!word_is_lower && val_word_is_lower))
			continue;

		for (boost::unordered_map<string, int>::iterator end = ends.begin();
				end != ends.end(); ++end) {
			if (algo::ends_with(val_word, end->first)) {
				int& rating = pos_to_rating[val_type];
				rating += end->second;
				int& count = pos_to_count[val_type];
				++count;
			}
		}
		for (boost::unordered_map<string, int>::iterator begin = begins.begin();
				begin != begins.end(); ++begin) {
			if (algo::starts_with(val_word, begin->first)) {
				int& rating = pos_to_rating[val_type];
				rating += begin->second;
				int& count = pos_to_count[val_type];
				++count;
			}
		}
	}

	string best_pos_type;
	int best_score = 0;

	for (boost::unordered_map<string, int>::iterator iter =
			pos_to_rating.begin(); iter != pos_to_rating.end(); ++iter) {
		const string& pos_type = iter->first;
		int score = iter->second;
		int count = pos_to_count.find(iter->first)->second;

		score *= score;
		score = 100 * score / count;

		if (score > best_score || best_score == 0) {
			best_score = score;
			best_pos_type = pos_type;
		}

		if (is_verbose())
			cout << "  -> part of speech: '" << pos_type << "', rating: "
					<< score << endl;
	}

	tags->first = best_pos_type;
	if (is_verbose())
		cout << "  guessed: " << print_tags(tags) << endl;
}

void tagger::guess(const string word, tags* tags) {
	const string _config_value = check_config("tagger", "1");
	if (boost::contains(_config_value, "1")) {
		impl_guess(word, tags);
	} else {
		if (is_verbose())
			cout << "  (can't guess, deactivated in config)" << endl;
	}
}

bool tagger::is_name(const string& _name) {
	const string name = lc(_name);

	foreach (string& s, tagger::builtin_entity_ends) {
		if (s == name)
			return false;
	}

	foreach (string& s, tagger::builtin_male_names) {
		if (s == name)
			return true;
	}

	foreach (string& s, tagger::builtin_female_names) {
		if (s == name)
			return true;
	}

	foreach (string& s, tagger::custom_names) {
		if (s == name)
			return true;
	}

	if (is_job(name))
		return true;

	return false;
}

bool tagger::is_job(const string& _name) {
	return regex_find(_name,
			"(gebrueder)|(^bundes)|(minister)|(meister$)|(ger$)");
}

const string tagger::unique_pos_type(const string& type) {
	if (type == "komma")
		return "komma";
	else if (type == "vi" || type == "vt" || type == "ci")
		return "v";
	else if (algo::starts_with(type, "a") && type != "art")
		return "adj";
	else if (type == "n" || type == "f" || type == "m"
			|| algo::starts_with(type, "n,") || type == "pron" || type == "b")
		return "n";
	else if (type == "fw" || algo::starts_with(type, "ques"))
		return "questionword";
	return type;
}
const string tagger::to_grammar_pos(tags* tags, const string& word) {
	const string& type = tags->first;

	if (type == "komma")
		return "d-komma";
	else if (type == "v")
		return "d-verb";
	else if (type == "art")
		return "d-article";
	else if (type == "adj")
		return "d-adjective";
	else if (type == "prep")
		return "d-preposition";
	else if (type == "questionword")
		return "d-questionword";
	else if (type == "linking")
		return "d-linking";
	else if (type == "n") {
		if (tagger::is_name(word))
			return "d-title";
		else
			return "d-noun";
	}
	return "q";
}

}

