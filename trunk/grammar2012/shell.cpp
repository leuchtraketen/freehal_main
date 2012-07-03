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
#include "hal2012-util2012.h"
namespace g = grammar2012;

// GNU readline
#ifdef READLINE
#  include <stdio.h>
#  include <stdlib.h>
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

// Boost program_options
#include <boost/program_options.hpp>
namespace po = boost::program_options;

string language("de");
fs::path path(".");

void g::tagger::ask_user(const string word, g::tags* tags) {
	// TODO
}
EXTERN_C char* check_config(const char* name, const char* _default) {
	return strdup("1");
}

int print_memory() {
	return system("ps aux | grep main | grep -v grep");
}

string new_sentence(g::tagger* _t, g::grammar* _g, g::parser* p, g::phraser* h,
		g::database<g::diskdb>* d, g::sentence* s) {

	boost::shared_ptr<g::xml_fact> infact = s->get_fact();
	if (!infact)
		return "";
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
	string phrased;
	if (best) {
		cout << endl;
		cout << best->print_str() << endl;

		best->prepare_tags(h->get_tagger());
		phrased = h->phrase(best);
		cout << phrased << endl;
	}

	return phrased;
}

int init(g::tagger* _t, g::grammar* _g, g::parser* p, g::phraser* h,
		g::database<g::diskdb>* d, int verbose) {
	_t->set_verbose(true);
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
	_g->expand();

	p->set_lang(language);
	p->set_path(path);
	p->set_tagger(_t);
	p->set_grammar(_g);
	p->set_verbose(false);

	h->set_lang(language);
	h->set_path(path);
	h->set_tagger(_t);
	h->set_grammar(_g);
	h->set_verbose(true);

	g::filterlist::set_verbose(false);

	d->set_verbose(true);
	d->set_buffered(false);
	d->set_lang(language);
	d->set_path(path);
	d->set_tagger(_t);
	print_memory();
	// d->prepare("../hal2012/lang_de");
	d->prepare(d->get_language_directory());

	return 0;
}

string get_answer(g::tagger* _t, g::grammar* _g, g::parser* p, g::phraser* h,
		g::database<g::diskdb>* d, string input) {

	p->parse(input);
	const vector<g::sentence*>& vs = p->get_sentences();
	vector<string> output;
	foreach (g::sentence* s, vs) {
		output.push_back(new_sentence(_t, _g, p, h, d, s));
	}
	return algo::join(output, " ");
}

void get_graph(g::tagger* _t, g::grammar* _g, g::parser* p, g::phraser* h,
		g::database<g::diskdb>* d, const string& input,
		const string& output_prefix) {

	p->parse(input);
	const vector<g::sentence*>& vs = p->get_sentences();
	int k = 0;
	foreach (g::sentence* s, vs) {
		stringstream ss;
		ss << ++k;
		const string file_prefix = output_prefix + ss.str();

		ofstream o((file_prefix + ".dot").c_str());
		o << grammar2012::grammar::print_graph(s->get_parsed());
		o.close();

		const string dot_args = " -Tpng " + file_prefix + ".dot > "
				+ file_prefix + ".png";
#if defined(MINGW) || defined(WIN32)
		const string cmdline = "dot.exe " + dot_args;
#else
		const string cmdline = "dot " + dot_args;
#endif
		if (::system(cmdline.c_str())) {
			cout << "Error! cannot execute: " << cmdline << endl;
		}
	}
}

int shell(g::tagger* _t, g::grammar* _g, g::parser* p, g::phraser* h,
		g::database<g::diskdb>* d) {

	print_memory();

	stringstream history;

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

		const string output = get_answer(_t, _g, p, h, d, input);

		history << "You:     " << input << endl;
		history << "Freehal: " << output << endl;

		cout << endl << history.str();

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

int main(int ac, char* av[]) {
	try {
		po::options_description generic("Generic options");
		generic.add_options()("help,h", "help")("verbose,v",
				po::value<int>()->implicit_value(1),
				"enable verbosity (optionally specify level)");

		po::options_description lang("Language options");
		lang.add_options()("language,l",
				po::value<string>()->implicit_value("de"), "the language")("path,p",
				po::value<string>()->implicit_value("path"), "the path containing the lang_XY "
						"and cache_XY directories");

		po::options_description io("I/O options");
		io.add_options()("input,i", po::value<string>(), "the input sentences")(
				"input-file,j", po::value<string>(),
				"the file to read the input sentences from")("output-file,o",
				po::value<string>(), "the file to write the answer to")(
				"graph-file,g", po::value<string>(),
				"write the syntax graph to files with this prefix, "
						"followed by an index (first is 1), "
						"with the file extension .dot, and use "
						"graphviz to convert it to a PNG file");

		po::options_description cmdline_options;
		cmdline_options.add(generic).add(lang).add(io);

		po::variables_map vm;
		po::store(po::parse_command_line(ac, av, cmdline_options), vm);
		po::notify(vm);

		if (vm.count("help")) {
			cout << cmdline_options << "\n";
			return 1;
		}

		if (vm.count("path")) {
			path = vm["path"].as<string>();
		}
		if (vm.count("language")) {
			language = vm["language"].as<string>();
		}

		int verbose = 0;
		if (vm.count("verbose")) {
			verbose = vm["verbose"].as<int>();
			cout << "Verbosity level was set to " << verbose << ".\n";
		}

		g::tagger* _t = new g::tagger();
		g::grammar* _g = new g::grammar();
		g::parser* p = new g::parser();
		g::phraser* h = new g::phraser();
		g::database<g::diskdb>* d = new g::database<g::diskdb>();

		string input;
		if (vm.count("input") > 0) {
			input = vm["input"].as<string>();
		} else if (vm.count("input-file") > 0) {
			input = g::read_file(vm["input-file"].as<string>());
		}

		if (input.size() > 0) {
			cout << "Input: " << input << ".\n";
			init(_t, _g, p, h, d, verbose);

			if (vm.count("output-file") > 0) {
				fs::path outputfile = vm["output-file"].as<string>();
				const string output = get_answer(_t, _g, p, h, d, input);
				g::write_file(outputfile, output);
			}
			if (vm.count("graph-file") > 0) {
				string graphfile = vm["graph-file"].as<string>();
				get_graph(_t, _g, p, h, d, input, graphfile);
			}

			if (vm.count("output-file") == 0 && vm.count("graph-file") == 0) {
				cerr << "Error! input string "
						<< "but no answer or graph output file given." << endl;
			}

		} else if (input.size() == 0 && vm.count("output-file") > 0) {
			cerr << "Error! answer output file but no input string given."
					<< endl;
		} else if (input.size() == 0 && vm.count("graph-file") > 0) {
			cerr << "Error! graph output file but no input string given."
					<< endl;

		} else {
			init(_t, _g, p, h, d, verbose);
			shell(_t, _g, p, h, d);
		}

		return 0;
	} catch (exception& e) {
		cerr << "Error: " << e.what() << "\n";
		return 1;
	} catch (...) {
		cerr << "Error: exception of unknown type!\n";
		return 1;
	}
}

