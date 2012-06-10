/*
 * hal2012-util2012.h
 *
 *  Created on: 30.05.2012
 *      Author: tobias
 */

#ifndef HAL2012_UTIL2012_H_
#define HAL2012_UTIL2012_H_

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
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace algo = boost::algorithm;

#define foreach         BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

namespace grammar2012 {

extern const char quote;
extern bool REGEX_DEBUG;

template<typename T>
const string print_vector(const vector<T>& v);
template<typename T>
const string print_doublevector(const vector<vector<T> >& v);
const string print_vector(const vector<string>& v);

bool is_invalid_char(char c);
void to_ascii(string& str);
const string lc(const string& _data);
const string uc(const string& _data);
const string lcfirst(const string& _data);
const string ucfirst(const string& _data);
bool is_lower(const string& _data);
bool regex_find(boost::smatch& what, const string& str, const string& exp);
bool regex_ifind(boost::smatch& what, const string& str, const string& exp);
bool regex_find(const string& str, const string& exp);
bool regex_ifind(const string& str, const string& exp);
void regex_replace(string& str, const string& exp, const string& repl);
void regex_ireplace(string& str, const string& exp, const string& repl);
void str_replace(string& str, const string& exp, const string& repl);
bool safe_regex(boost::regex&, const string&);
bool safe_iregex(boost::regex&, const string&);
int split_lines(vector<string>&, const string&);

}

#endif /* HAL2012_UTIL2012_H_ */
