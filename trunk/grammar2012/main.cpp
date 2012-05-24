/*
 * main.cpp
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"

void grammar2012::tagger::ask_user(const string word, grammar2012::tags& tags) {
}
EXTERN_C char* check_config(const char* name, const char* _default) {
	return strdup("1");
}

int main() {

	grammar2012::tagger* t = new grammar2012::tagger();
	t->set_verbose(true);
	t->get_pos("wie");
	t->read_pos_file("brain.pos");
	t->read_pos_file("memory.pos");
	t->read_regex_pos_file("regex.pos");
	t->get_pos("ich");
	t->get_pos("bist");
	t->get_pos("dbfdsdf");
	t->get_pos("jhvbhjbhb345333345");
	t->get_pos("Dgtdfgfcersdgfcredsfd");
	t->get_pos("dgtdfgfcersdgfcredsfd");
	t->get_pos("äöüäöüäöüEuphemäöüismusßßß");

	return 0;

	grammar2012::grammar* g = new grammar2012::grammar();
	g->read_grammar("grammar.txt");
	g->set_verbose(true);
	g->expand();
	//if (g->is_verbose())
	//	cout << g->to_str();

	//g->parse("d-article|der#d-noun|Hund#d-verb|ist#d-adjective|da");
	//g->parse("d-questionword|wie#d-adjective|alt#d-verb|bist#d-noun|du");
	grammar2012::parsed_type* parsed = g->parse(
			"d-questionword < wie > d-verb < geht > d-noun < es > "
					"d-title < Felix > d-linking < & > d-title < Tobias >");
	grammar2012::grammar::print_perl(parsed);

	ofstream o("y.dot");
	o << grammar2012::grammar::print_graph(parsed);
	o.close();
	system("dot -Tpng y.dot > y.png");
}

