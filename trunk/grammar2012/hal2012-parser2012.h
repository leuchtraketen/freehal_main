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

namespace grammar2012 {
typedef unsigned int size_t;


class parser {
private:
	string input_raw;
	string input_clean;

	bool verbose;
	bool buffered;

	void clean_input(string&);
	void build_pair_sentences(string&, const string&, const string&);
	void build_pair_sentences(string&, const string&, const string&, const string&);

public:
	parser(const string&);
	~parser();

	void set_verbose(bool);
	bool is_verbose();
	void set_buffered(bool);
	bool is_buffered();
};

}

#endif /* HAL2012_PARSER2012_H_ */
