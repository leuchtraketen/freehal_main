/*
 * hal2012-util2012.cpp
 *
 *  Created on: 30.05.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"

namespace grammar2012 {

const char quote = '"';
bool REGEX_DEBUG = false;

template<typename T>
const string print_vector(const vector<T>& v) {
	stringstream ss;
	ss << "[";
	for (typename vector<T>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (i != v.begin())
			ss << ", ";
		ss << *i;
	}
	ss << "]";
	return ss.str();
}
template<typename T>
const string print_doublevector(const vector<vector<T> >& v) {
	stringstream ss;
	ss << "[";
	for (typename vector<vector<T> >::const_iterator i = v.begin();
			i != v.end(); ++i) {
		if (i != v.begin())
			ss << ", ";
		ss << print_vector(*i);
	}
	ss << "]";
	return ss.str();
}
const string print_vector(const vector<string>& v) {
	stringstream ss;
	ss << "[";
	for (vector<string>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (i != v.begin())
			ss << ", ";
		ss << '"' << *i << '"';
	}
	ss << "]";
	return ss.str();
}

bool is_invalid_char(char c) {
	return !(c >= 0 && c < 128);
}
void to_ascii(string& str) {
	str.erase(remove_if(str.begin(), str.end(), is_invalid_char), str.end());
}
const string lc(const string& _data) {
	string data(_data);
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}
const string uc(const string& _data) {
	string data(_data);
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
	return data;
}
const string lcfirst(const string& _data) {
	string data(_data);
	data[0] = ::tolower(data[0]);
	return data;
}
const string ucfirst(const string& _data) {
	string data(_data);
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	data[0] = ::toupper(data[0]);
	return data;
}
bool is_lower(const string& _data) {
	return lc(_data) == _data;
}

bool regex_ifind(boost::smatch& what, const string& str, const string& exp) {
	what = boost::smatch();
	boost::regex regex(exp, boost::regex::perl | boost::regex::icase);
	if (safe_iregex(regex, exp))
		return boost::regex_search(str.begin(), str.end(), what, regex);
	else
		return false;
}
bool regex_find(boost::smatch& what, const string& str, const string& exp) {
	what = boost::smatch();
	boost::regex regex(exp, boost::regex::perl);
	if (safe_regex(regex, exp))
		return boost::regex_search(str.begin(), str.end(), what, regex);
	else
		return false;
}
bool regex_ifind(const string& str, const string& exp) {
	boost::regex regex;
	if (safe_iregex(regex, exp))
		return boost::regex_search(str.begin(), str.end(), regex);
	else
		return false;
}
bool regex_find(const string& str, const string& exp) {
	boost::regex regex;
	if (safe_regex(regex, exp))
		return boost::regex_search(str.begin(), str.end(), regex);
	else
		return false;
}
void regex_ireplace(string& str, const string& exp, const string& repl) {
	boost::regex regex;
	if (safe_iregex(regex, exp)) {
		const string copy = str;
		str = boost::regex_replace(str, regex, repl,
				boost::match_default | boost::format_perl);
		if (REGEX_DEBUG && copy != str)
			cout << "  regex replace: " << quote << copy << quote << " -> "
					<< quote << str << quote << " (" << quote << exp << quote
					<< " -> " << quote << repl << quote << ")"
					<< " [ignore case]" << endl;
	}
}
void regex_replace(string& str, const string& exp, const string& repl) {
	boost::regex regex;
	if (safe_regex(regex, exp)) {
		const string copy = str;
		str = boost::regex_replace(str, regex, repl,
				boost::match_default | boost::format_perl);
		if (REGEX_DEBUG && copy != str)
			cout << "  regex replace: " << quote << copy << quote << " -> "
					<< quote << str << quote << " (" << quote << exp << quote
					<< " -> " << quote << repl << quote << ")" << endl;
	}
}
void str_replace(string& str, const string& tofind, const string& repl) {
	boost::replace_all(str, tofind, repl);
}

bool safe_regex(boost::regex& regex, const string& exp) {
	try {
		regex.assign(exp, boost::regex::perl);
		return true;
	} catch (boost::regex_error& e) {
		cout << "regex error! \"" << e.what() << "\" (exp: \"" << exp << "\")"
				<< endl;
	}
	return false;
}
bool safe_iregex(boost::regex& regex, const string& exp) {
	try {
		regex.assign(exp, boost::regex::perl | boost::regex::icase);
		return true;
	} catch (boost::regex_error& e) {
		cout << "regex error! \"" << e.what() << "\" (exp: \"" << exp << "\")"
				<< endl;
	}
	return false;
}

int split_lines(vector<string>& lines, const string& prestr) {
	string _str;
	stringstream predata;
	predata << prestr;
	int i = 0;
	while (getline(predata, _str)) {
		lines.push_back(_str);
		++i;
	}
	return i;
}

freehal_base::freehal_base() :
		verbose(true), buffered(false), lang(), path() {
}
freehal_base::~freehal_base() {
}

void freehal_base::set_verbose(bool v) {
	verbose = v;
}
bool freehal_base::is_verbose() const {
	return verbose;
}
void freehal_base::set_buffered(bool v) {
	buffered = v;
}
bool freehal_base::is_buffered() const {
	return buffered;
}
void freehal_base::set_lang(const string& _lang) {
	lang = _lang;
}
void freehal_base::set_path(const fs::path& _path) {
	path = _path;
}
const string freehal_base::get_lang() const {
	return lang;
}
const fs::path freehal_base::get_path() const {
	return path;
}
const fs::path freehal_base::get_language_directory() const {
	if (path.empty())
		return "";
	else
		return path / ("lang_" + lang);
}
bool freehal_base::is_configured() const {
	if (lang.empty()) {
		cout << "Error! parser2012: language is undefined." << endl;
		return false;
	} else if (path.empty()) {
		cout << "Error! parser2012: path is undefined." << endl;
		return false;
	} else {
		return true;
	}
}

}
