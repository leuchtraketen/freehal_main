/*
 * hal2012-parser2012.cpp
 *
 *  Created on: 23.05.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-parser2012.h"

namespace grammar2012 {

parser::parser(const string& txt) :
		input_raw(txt), input_clean(txt), verbose(true), buffered(false) {
	if (is_verbose())
		cout << "parser2012: raw input: " << input_raw << endl;
	clean_input(input_clean);
	if (is_verbose())
		cout << "parser2012: clean input: " << input_clean << endl;
}
parser::~parser() {

}

void parser::clean_input(string& str) {
	regex_replace(str, "[?]+", "?");
	regex_replace(str, "([?])", "\\1.");
	regex_replace(str, "[.!~]\\s*?[.!~]", "~");
	regex_replace(str, "[.!~]\\s*?[.!~]", "~");
	regex_replace(str, "[.](\\s|$)", " STOP ");
	regex_replace(str, "([0-9]) STOP ", "\\1.");
	regex_replace(str, " STOP \\((.*)\\)", " ($0) STOP . ");

	regex_replace(str, "[=]{1,3}[>]", " reasonof ");
	regex_replace(str, "[-]{1,3}[>]", " reasonof ");
	regex_replace(str, "\\(reasonof\\)", " reasonof ");
	regex_replace(str, "\\(reason of\\)", " reasonof ");
	regex_replace(str, "\\(reason\\)", " reasonof ");
	regex_replace(str, "\\(r\\)", " reasonof ");

	regex_replace(str, "\\s+", " ");
	algo::trim(str);

	boost::smatch m;
	if (algo::contains(str, " reasonof ")) {
		build_pair_sentences(str, " reasonof ", "reasonof");
	} else if (algo::contains(str, "learn:")) {
		str_replace(str, "learn:", "");
		build_pair_sentences(str, ",", "=");
	} else if (regex_ifind(m, str, "properties:(.*?):")) {
		regex_replace(str, "properties:(.*?):", "");
		build_pair_sentences(str, ",", "ist", m[1]);
	} else if (regex_find(str, "[=].*?[=]")) {
		build_pair_sentences(str, "=", "=");
	}
}

void parser::build_pair_sentences(string& str, const string& divideby,
		const string& joinby) {
	algo::trim(str);
	str_replace(str, divideby, "@");
	vector<string> parts;
	algo::split(parts, str, algo::is_any_of("@"));
	stringstream ss;
	foreach (string i, parts) {
		foreach (string j, parts) {
			if (i != j) {
				str_replace(i, "STOP", "");
				str_replace(j, "STOP", "");
				str_replace(i, "\"", "");
				str_replace(j, "\"", "");
				algo::trim(i);
				algo::trim(j);
				regex_replace(i, "\\s+", "_");
				regex_replace(j, "\\s+", "_");
				ss << "_" << i << "_ " << joinby << " _" << j << "_ STOP ";
			}
		}
	}
	str = ss.str();
}

void parser::build_pair_sentences(string& str, const string& divideby,
		const string& joinby, const string& word_1) {
	algo::trim(str);
	str_replace(str, divideby, "@");
	vector<string> parts;
	algo::split(parts, str, algo::is_any_of("@"));
	stringstream ss;
	foreach (string j, parts) {
		if (word_1 != j) {
			str_replace(j, "STOP", "");
			str_replace(j, "\"", "");
			algo::trim(j);
			ss << word_1 << " " << joinby << " " << j << " STOP ";
		}
	}

	str = ss.str();
}

void parser::set_verbose(bool v) {
	verbose = v;
}
bool parser::is_verbose() {
	return verbose;
}

void parser::set_buffered(bool v) {
	buffered = v;
}
bool parser::is_buffered() {
	return buffered;
}

}

