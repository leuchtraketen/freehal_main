/*
 * main.cpp
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-parser2012.h"
#include "hal2012-util2012.h"

void grammar2012::tagger::ask_user(const string word, grammar2012::tags* tags) {
}
EXTERN_C char* check_config(const char* name, const char* _default) {
	return strdup("1");
}

int main() {

	grammar2012::tagger* _t = new grammar2012::tagger();
	_t->set_verbose(true);
	_t->read_pos_file("brain.pos");
	_t->read_pos_file("memory.pos");
	_t->read_regex_pos_file("regex.pos");

	grammar2012::grammar* _g = new grammar2012::grammar();
	_g->read_grammar("grammar.txt");
	_g->set_verbose(true);
	_g->expand();

	grammar2012::parser* p = new grammar2012::parser();
	p->set_lang("de");
	p->set_path(".");
	p->set_tagger(_t);
	p->set_grammar(_g);
	p->set_verbose(true);
	p->parse("Wie alt bist du? wie gehts? ich heisse Winfried!");
	const vector<grammar2012::sentence*>& vs = p->get_sentences();
	foreach (grammar2012::sentence* s, vs) {
		ofstream o("current.dot");
		o << grammar2012::grammar::print_graph(s->get_parsed());
		o.close();
	}

#if defined(MINGW) || defined(WIN32)
	::system("dot.exe -Tpng current.dot > current.png");
#else
	::system("dot -Tpng current.dot > current.png");
#endif

	return 0;

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

