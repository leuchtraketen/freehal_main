/*
 * main.cpp
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-parser2012.h"
#include "hal2012-xml2012.h"
#include "hal2012-diskdb2012.h"
#include "hal2012-filterfacts2012.h"
#include "hal2012-util2012.h"

#ifdef READLINE
#  include <stdio.h>
#  include <stdlib.h>
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

namespace g = grammar2012;

const string language("de");
const fs::path path(".");

void g::tagger::ask_user(const string word, g::tags* tags) {
	// TODO
}
EXTERN_C char* check_config(const char* name, const char* _default) {
	return strdup("1");
}

int print_memory() {
	return system("ps aux | grep main | grep -v grep");
}

int new_sentence(g::sentence* s, g::database<g::diskdb>* d) {
	boost::shared_ptr<g::xml_fact> infact = s->get_fact();
	if (!infact)
		return 1;
	cout << *infact << endl;

	vector<boost::shared_ptr<g::xml_fact> > facts;
	d->find_by_fact(facts, infact);

	g::ranking<boost::shared_ptr<g::xml_fact>, double> rank;
	int i = 0;
	foreach (boost::shared_ptr<g::xml_fact> fact2, facts) {
		cout << "match fact " << ++i << "\r" << flush;
		double match = infact / fact2;
		rank.insert(fact2, match);

		/*if (algo::contains(fact2->print_str(), "kuenstliche")
		 && algo::contains(fact2->print_str(), "intelligenz")
		 && algo::contains(fact2->print_str(),
		 "lernfaehige")) {
		 cout << fact2->print_str() << endl;
		 exit(0);
		 }*/
	}
	cout << endl;
	for (int i = 0; i < rank.size(); ++i) {
		cout << rank.rank(i) << ": " << rank[i]->print_str() << endl;
	}

        boost::shared_ptr<g::xml_fact> best = rank.best_one();

	return 0;
}

int shell() {
	g::tagger* _t = new g::tagger();
	_t->set_verbose(true);
	_t->set_lang(language);
	_t->set_path(path);
	_t->read_pos_file("guessed.pos");
	_t->read_pos_file("brain.pos");
	_t->read_pos_file("memory.pos");
	_t->read_regex_pos_file("regex.pos");

	g::grammar* _g = new g::grammar();
	_g->set_lang(language);
	_g->set_path(path);
	_g->read_grammar("grammar.txt");
	_g->set_verbose(false);
	_g->expand();

	g::parser* p = new g::parser();
	p->set_lang(language);
	p->set_path(path);
	p->set_tagger(_t);
	p->set_grammar(_g);
	p->set_verbose(false);

	g::filterlist::set_verbose(false);

	g::database<g::diskdb>* d = new g::database<g::diskdb>();
	d->set_verbose(true);
	d->set_buffered(false);
	d->set_lang(language);
	d->set_path(path);
	d->set_tagger(_t);
	print_memory();
	d->prepare("../hal2012/lang_de");

	print_memory();

	static string prompt = "\nfreehal:" + language + "> ";
#ifdef READLINE
	char *buf;
	while ((buf = readline(prompt.c_str()))!=NULL) {
		string input(buf);
#else
	string input;
	cout << prompt;
	while (getline(cin, input)) {
#endif

		if (input.size() == 0 || input == "q" || input == "quit"
				|| input == "exit")
			break;

		p->parse(input);
		const vector<g::sentence*>& vs = p->get_sentences();
		foreach (g::sentence* s, vs) {
			new_sentence(s, d);
		}

#ifdef READLINE
		if (buf[0]!=0)
		add_history(buf);
	}
	free(buf);
#else
		cout << prompt;
	}
#endif
	cout << endl;

	print_memory();

	return 0;
}

int main() {
	shell();
}

