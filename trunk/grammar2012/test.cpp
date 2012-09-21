/*
 * main.cpp
 *
 *  Created on: 18.05.2012
 *      Author: tobias
 */

// Freehal
#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-parser2012.h"
#include "hal2012-phrase2012.h"
#include "hal2012-xml2012.h"
#include "hal2012-diskdb2012.h"
#include "hal2012-filterfacts2012.h"
#include "hal2012-predefined2012.h"
#include "hal2012-util2012.h"
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

int main() {

	bool buffered = false;

	g::tagger* _t = new g::tagger();
	g::grammar* _g = new g::grammar();
	g::parser* p = new g::parser();
	g::phraser* h = new g::phraser();
	g::database* d = new g::database(g::diskdb::new_instance(0));
	g::predefined* r = new g::predefined();

	_t->set_verbose(true);
	_t->set_buffered(buffered);
	_t->set_lang(language);
	_t->set_path(path);
	_t->read_pos_file("guessed.pos");
	_t->read_pos_file("brain.pos");
	_t->read_pos_file("memory.pos");
	_t->read_regex_pos_file("regex.pos");

	_g->set_lang(language);
	_g->set_path(path);
	_g->read_grammar("grammar.txt");
	_g->set_verbose(true);
	_g->set_buffered(buffered);
	_g->expand();

	p->set_lang(language);
	p->set_path(path);
	p->set_tagger(_t);
	p->set_grammar(_g);
	p->set_verbose(false);
	p->set_buffered(buffered);

	h->set_lang(language);
	h->set_path(path);
	h->set_tagger(_t);
	h->set_grammar(_g);
	h->set_verbose(true);
	h->set_buffered(buffered);

	g::filterlist::set_verbose(true);

	d->set_verbose(true);
	d->set_buffered(buffered);
	d->set_lang(language);
	d->set_path(path);
	d->set_tagger(_t);
	print_memory();
	// d->prepare("../hal2012/lang_de");

	r->set_lang(language);
	r->set_path(path);
	r->set_tagger(_t);
	r->set_grammar(_g);
	r->set_verbose(true);
	r->set_buffered(buffered);

	d->prepare(d->get_language_directory());

	d->think();
}

