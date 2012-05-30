/*
 * hal2012-util2012.cpp
 *
 *  Created on: 30.05.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"

namespace grammar2012 {

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
const string unique_pos_type(const string& type) {
	if (type == "vi" || type == "vt" || type == "ci")
		return "v";
	else if (algo::starts_with(type, "a") && type != "art")
		return "adj";
	else if (type == "n" || type == "f" || type == "m"
			|| algo::starts_with(type, "n,") || type == "pron" || type == "b")
		return "n";
	return type;
}

bool regex_ifind(boost::smatch& what, const string& str, const string& exp) {
	boost::regex regex(exp, boost::regex::perl | boost::regex::icase);
	if (safe_iregex(regex, exp))
		return boost::regex_search(str.begin(), str.end(), what, regex);
	else
		return false;
}
bool regex_find(boost::smatch& what, const string& str, const string& exp) {
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
	if (safe_iregex(regex, exp))
		str = boost::regex_replace(str, regex, repl,
				boost::match_default | boost::format_perl);
	else
		return;
}
void regex_replace(string& str, const string& exp, const string& repl) {
	boost::regex regex;
	if (safe_regex(regex, exp))
		str = boost::regex_replace(str, regex, repl,
				boost::match_default | boost::format_perl);
	else
		return;
}
void str_replace(string& str, const string& tofind, const string& repl) {
	boost::replace_all(str, tofind, repl);
}

bool safe_regex(boost::regex& regex, const string& exp) {
	try {
		regex.assign(exp, boost::regex::perl);
		return true;
	} catch (boost::regex_error& e) {
		cout << "regex error! \"" << e.what() << "\" (exp: \"" << exp << "\")" << endl;
	}
	return false;
}
bool safe_iregex(boost::regex& regex, const string& exp) {
	try {
		regex.assign(exp, boost::regex::perl | boost::regex::icase);
		return true;
	} catch (boost::regex_error& e) {
		cout << "regex error! \"" << e.what() << "\" (exp: \"" << exp << "\")" << endl;
	}
	return false;
}

}
