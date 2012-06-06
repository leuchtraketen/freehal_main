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
	for (typename vector<string>::const_iterator i = v.begin(); i != v.end();
			++i) {
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

}
