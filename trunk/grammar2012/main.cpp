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
#include "hal2012-util2012.h"

namespace g = grammar2012;

void g::tagger::ask_user(const string word, g::tags* tags) {
}
EXTERN_C char* check_config(const char* name, const char* _default) {
	return strdup("1");
}

void print_memory() {
	system("ps aux | grep main | grep -v grep");
}

int main() {
	if (1) {
		g::tagger* _t = new g::tagger();
		_t->set_verbose(true);
		_t->read_pos_file("brain.pos");
		_t->read_pos_file("memory.pos");
		_t->read_regex_pos_file("regex.pos");

		g::grammar* _g = new g::grammar();
		_g->read_grammar("grammar.txt");
		_g->set_verbose(false);
		_g->expand();

		g::parser* p = new g::parser();
		p->set_lang("de");
		p->set_path(".");
		p->set_tagger(_t);
		p->set_grammar(_g);
		p->set_verbose(false);

		p->parse("In der Zeitung steht, dass es nur fuer die "
				"Schlecker-Toechter IhrPlatz und "
				"SchleckerXL eine Zukunft geben soll.");
		const vector<g::sentence*>& vs = p->get_sentences();
		foreach (g::sentence* s, vs) {
			//cout << g::grammar::print_xml(s->get_parsed());
			cout << *s->get_fact() << endl;
		}

		g::database<g::diskdb>* d = new g::database<g::diskdb>();
		d->set_verbose(true);
		d->set_lang("de");
		d->set_path(".");
		print_memory();
		d->prepare("../hal2012/lang_de");

		print_memory();
		vector<boost::shared_ptr<g::xml_fact> > facts;
		d->find_by_word(facts, "FreeHAL");
		foreach (boost::shared_ptr<g::xml_fact> fact, facts) {
			cout << fact->print_str() << endl;
		}
		print_memory();
	}

	if (0) {
		g::database<g::diskdb>* d = new g::database<g::diskdb>();
		d->set_verbose(true);
		d->set_lang("de");
		d->set_path(".");

		print_memory();
		vector<boost::shared_ptr<g::xml_fact> > facts;
		d->find_by_word(facts, "katze");
		foreach (boost::shared_ptr<g::xml_fact> fact, facts) {
			cout << fact->print_str() << endl;
		}
		print_memory();
	}

	if (0) {
		g::database<g::diskdb>* d = new g::database<g::diskdb>();
		d->set_verbose(true);
		d->set_lang("de");
		d->set_path(".");

		print_memory();
		d->prepare("freehal.xml");
		print_memory();
		d->prepare("../hal2012/lang_de");
		d->prepare("test.xml");
		print_memory();
		vector<boost::shared_ptr<g::xml_fact> > facts;
		d->find_by_word(facts, "katze");
		print_memory();

		{
			try {
				std::ofstream ofs("filename2.dat", ios::binary);
				boost::archive::xml_oarchive oa(ofs);
				oa << boost::serialization::make_nvp("database", d);
			} catch (boost::archive::archive_exception& ex) {
				cout << "Error! exception during serialization: " << ex.what()
						<< endl;
			}
		}
	}
	print_memory();

	if (0) {
		g::tagger* _t = new g::tagger();
		_t->set_verbose(true);
		_t->read_pos_file("brain.pos");
		_t->read_pos_file("memory.pos");
		_t->read_regex_pos_file("regex.pos");

		g::grammar* _g = new g::grammar();
		_g->read_grammar("grammar.txt");
		_g->set_verbose(true);
		_g->expand();

		g::parser* p = new g::parser();
		p->set_lang("de");
		p->set_path(".");
		p->set_tagger(_t);
		p->set_grammar(_g);
		p->set_verbose(true);

		{
			try {
				std::ofstream ofs("filename1.dat", ios::binary);
				boost::archive::xml_oarchive oa(ofs);
				oa << boost::serialization::make_nvp("parser", p);
			} catch (boost::archive::archive_exception& ex) {
				cout << "Error! exception during serialization: " << ex.what()
						<< endl;
			}
		}

		delete p;
	}

	if (1) {
		g::parser* p = new g::parser();
		{
			try {
				std::ifstream ifs("filename1.dat");
				if (ifs.is_open()) {
					boost::archive::xml_iarchive ia(ifs);
					ia >> boost::serialization::make_nvp("parser", p);
				}

			} catch (boost::archive::archive_exception& ex) {
				cout << "Error! exception during deserialization: " << ex.what()
						<< endl;
			}
		}

		p->parse("Wie alt bist du? wie gehts? ich heisse Winfried!");

		print_memory();
	}

	return (0);

	g::parser* p = new g::parser();
	p->parse("Wie alt bist du? wie gehts? ich heisse Winfried!");
	const vector<g::sentence*>& vs = p->get_sentences();
	foreach (g::sentence* s, vs) {
		ofstream o("current.dot");
		o << g::grammar::print_graph(s->get_parsed());
		o.close();
	}

#if defined(MINGW) || defined(WIN32)
	::system("dot.exe -Tpng current.dot > current.png");
#else
	::system("dot -Tpng current.dot > current.png");
#endif

	return 0;

	g::tagger* t = new g::tagger();
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

	g::grammar* g = new g::grammar();
	g->read_grammar("grammar.txt");
	g->set_verbose(true);
	g->expand();
	//if (g->is_verbose())
	//	cout << g->to_str();

	//g->parse("d-article|der#d-noun|Hund#d-verb|ist#d-adjective|da");
	//g->parse("d-questionword|wie#d-adjective|alt#d-verb|bist#d-noun|du");
	g::parsed_t* parsed = g->parse(
			"d-questionword < wie > d-verb < geht > d-noun < es > "
					"d-title < Felix > d-linking < & > d-title < Tobias >");
	g::grammar::print_perl(parsed);

	ofstream o("y.dot");
	o << g::grammar::print_graph(parsed);
	o.close();
	system("dot -Tpng y.dot > y.png");
}

