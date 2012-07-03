/*
 * graph.cpp
 *
 *  Created on: 31.05.2012
 *      Author: tobias
 */

#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-parser2012.h"
#include "hal2012-util2012.h"

const string language("de");
const fs::path path(".");

void grammar2012::tagger::ask_user(const string word, grammar2012::tags* tags) {
}
EXTERN_C char* check_config(const char* name, const char* _default) {
	return strdup("1");
}

int main(int argc, char** argv) {

	grammar2012::tagger* _t = new grammar2012::tagger();
	_t->set_lang(language);
	_t->set_path(path);
	_t->set_verbose(true);
	_t->set_buffered(true);
	_t->read_pos_file("brain.pos");
	_t->read_pos_file("memory.pos");
	_t->read_regex_pos_file("regex.pos");

	grammar2012::grammar* _g = new grammar2012::grammar();
	_g->set_lang(language);
	_g->set_path(path);
	_g->read_grammar("grammar.txt");
	_g->set_verbose(true);
	_g->set_buffered(true);
	_g->expand();

	grammar2012::parser* p = new grammar2012::parser();
	p->set_lang(language);
	p->set_path(path);
	p->set_tagger(_t);
	p->set_grammar(_g);
	p->set_verbose(true);

	int k = 0;
	for (int c = 1; c < argc; ++c) {
		p->parse(argv[c]);
		const vector<grammar2012::sentence*>& vs = p->get_sentences();
		foreach (grammar2012::sentence* s, vs) {

			stringstream ss;
			ss << ++k;
			const string file_prefix = "current" + ss.str();

			ofstream o((file_prefix + ".dot").c_str());
			o << grammar2012::grammar::print_graph(s->get_parsed());
			o.close();

			const string dot_args = " -Tpng " + file_prefix + ".dot > "
					+ file_prefix + ".png";
#if defined(MINGW) || defined(WIN32)
			::system(("dot.exe "+dot_args).c_str());
#else
			::system(("dot " + dot_args).c_str());
#endif
		}
	}

	delete p;
	delete _g;
	delete _t;
}

