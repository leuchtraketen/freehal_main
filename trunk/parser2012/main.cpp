/*
 * main.cpp
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

#include "parser2012.h"

int main() {

	grammar2012::grammar* g = new grammar2012::grammar();
	g->read_grammar("grammar.txt");
	g->set_verbose(false);
	g->expand();
	if (g->is_verbose())
		cout << g->to_str();

	g->parse("d-article|der#d-noun|Hund#d-verb|ist#d-adjective|da");
	g->parse("d-questionword|wie#d-adjective|alt#d-verb|bist#d-noun|du");
}

