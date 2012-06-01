/*
 * hal2012-parser2012.cpp
 *
 *  Created on: 23.05.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-parser2012.h"

namespace grammar2012 {

sentence::sentence(parser* _p, const string& _input) :
		input(_input), mode(UNKNOWN), words_list(), tags_list(), p(_p) {

	{
		string tmp(input);
		regex_replace(tmp, "[.!?]", "");
		algo::trim(tmp);
		algo::split(words_list, tmp, algo::is_space());
	}
	{
		foreach (string word, words_list) {
			tags_list.push_back(p->get_tagger()->get_pos(word));
		}
	}

	find_mode();
}
void sentence::find_mode() {
	string message;
	if (words_list.size() == 0) {
		mode = UNKNOWN;
	} else if (algo::contains(input, "?")) {
		message = "sentence contains a question mark";
		mode = QUESTION;
		return;
	} else if (!algo::contains(input, "_no-question_")) {
		if (tags_list.size() >= 1) {
			if (tags_list[0]->first == "v") {
				message = "the first word is a verb";
				mode = QUESTION;
				return;
			}
		}
		foreach (tags* tags, tags_list) {
			if (tags->first == "questionword") {
				message = "sentence contains a questionword";
				mode = QUESTION;
				return;
			}
		}
	}
	mode = STATEMENT;
}

const string sentence::to_grammar_input() const {
	if (words_list.size() != tags_list.size()) {
		cout << "Error! words_list (" << words_list.size()
				<< ") and tags_list (" << tags_list.size()
				<< ") must have the same size.";
		return "";
	}

	stringstream ss;
	int i = 0;
	foreach (string word, words_list) {
		tags* tag = tags_list[i];

		ss << tagger::to_grammar_pos(tag, word) << "<" << word << ">";

		++i;
	}
	return ss.str();
}
const string sentence::to_str() const {
	stringstream ss;
	ss << "{";
	bool need_komma = false;
	if (words_list.size() > 0) {
		if (need_komma)
			ss << ",";
		ss << " words=" << print_vector(words_list);
		need_komma = true;
	}
	if (tags_list.size() > 0) {
		if (need_komma)
			ss << ",";
		ss << " tags=" << print_vector(tags_list);
		need_komma = true;
	}
	if (input.size() > 0) {
		if (need_komma)
			ss << ",";
		ss << " input=" << quote << input << quote;
		need_komma = true;
	}
	if (mode != UNKNOWN) {
		if (need_komma)
			ss << ",";
		ss << " mode="
				<< (mode == ONLY_LEARN ? "ONLY_LEARN" :
					mode == STATEMENT ? "STATEMENT" :
					mode == QUESTION ? "QUESTION" : "UNKNOWN");
		need_komma = true;
	}
	ss << " }";
	return ss.str();
}

void sentence::parse() {
	parsed = p->get_grammar()->parse(this->to_grammar_input());
}

const string sentence::get_input() const {
	return input;
}
Mode sentence::get_mode() const {
	return mode;
}
vector<string> sentence::get_words_list() const {
	return words_list;
}
vector<tags*> sentence::get_tags_list() const {
	return tags_list;
}
parsed_type* sentence::get_parsed() const {
	return parsed;
}

parser::parser() :
		input_raw(), input_clean(), input_simplified(), input_extended(), sentences(), verbose(
				true), buffered(false), lang(), path() {
}
parser::~parser() {
	for (size_t i = 0; i < sentences.size(); ++i) {
		if (sentences[i]) {
			delete (sentences[i]);
			sentences[i] = 0;
		}
	}
}

void parser::parse(const string& txt) {
	if (lang.size() == 0) {
		cout << "Error! parser2012: language is undefined." << endl;
		return;
	} else if (path.size() == 0) {
		cout << "Error! parser2012: path is undefined." << endl;
		return;
	} else if (t == 0) {
		cout << "Error! parser2012: tagger is undefined." << endl;
		return;
	}

	{
		input_raw.assign(txt);
		if (is_verbose())
			cout << "parser2012: raw input: " << quote << input_raw << quote
					<< endl;
	}

	{
		input_clean.assign(input_raw);
		clean_input(input_clean);
		if (is_verbose())
			cout << "parser2012: clean input: " << quote << input_clean << quote
					<< endl;
	}

	{
		algo::split(input_simplified, input_clean, algo::is_any_of("@"));
		for (size_t i = 0; i < input_simplified.size(); ++i) {
			simplify_input(input_simplified[i]);
		}
		if (is_verbose())
			cout << "parser2012: simplified input: "
					<< print_vector(input_simplified) << endl;
	}

	{
		std::copy(input_simplified.begin(), input_simplified.end(),
				back_inserter(input_extended));
		for (size_t i = 0; i < input_extended.size(); ++i) {
			extend_input(input_extended[i]);
		}
		if (is_verbose())
			cout << "parser2012: extended input: "
					<< print_vector(input_extended) << endl;
	}

	{
		for (size_t i = 0; i < input_extended.size(); ++i) {
			sentences.push_back(new sentence(this, input_extended[i]));
		}
		if (is_verbose())
			cout << "parser2012: ready to parse: " << print_vector(sentences)
					<< endl;
	}

	{
		foreach (sentence* s, sentences) {
			if (is_verbose())
				cout << "parser2012: parse: " << s->to_str() << endl;
			s->parse();
		}
	}

}
vector<sentence*> parser::get_sentences() const {
	return sentences;
}

void parser::set_lang(const string& _lang) {
	lang = _lang;
}
void parser::set_path(const string& _path) {
	path = _path;
}
void parser::set_tagger(tagger* _t) {
	t = _t;
}
void parser::set_grammar(grammar* _g) {
	g = _g;
}
const string parser::get_lang() const {
	return lang;
}
const string parser::get_path() const {
	return path;
}
tagger* parser::get_tagger() const {
	return t;
}
grammar* parser::get_grammar() const {
	return g;
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

	regex_replace(str, "\\s+", " ");
	algo::trim(str);

	regex_replace(str, "@", "AT");
	regex_replace(str, "\\s*STOP\\s*", "@");
	algo::trim_if(str, algo::is_any_of("@"));
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

void parser::simplify_input(string& str) {

	REGEX_DEBUG = true;
	boost::smatch m;

	bool is_question = regex_find(str, "[?]");

	regex_ireplace(str, "9637", "\\$\\$");
	regex_ireplace(str, "9489", "\\$\\$");
	regex_ireplace(str, "[?][=][>]", " questionnext ");
	regex_ireplace(str, "[!][=][>]", " factnext ");
	regex_ireplace(str, "[=][>]", " questionnext ");
	regex_ireplace(str, "\\s*[?]", " ?");
	regex_ireplace(str, "^[und]<ws>[,]<ws>", "");

	if (lang == "en") {
		if (str.size() > 7) {
			regex_ireplace(str, "(^|\\s)no(\\s|$)", "\\1not a\\2");
		}
		regex_ireplace(str, "(^|\\s)an\\s(...)", "\\1a \\2");
		regex_ireplace(str,
				"(^|\\s)(are|is|be|was|were|am)\\s(.+?)\\sable\\sto\\s",
				"\\1can \\3 ");
		regex_ireplace(str, "(^|\\s)(are|is|be|was|were|am)\\sable\\sto\\s",
				"\\1 can ");
		regex_ireplace(str,
				"(^|\\s)(are|is|be|was|were|am)\\s(.+?)\\sunable\\sto\\s",
				"\\1not can \\3 ");
		regex_ireplace(str, "(^|\\s)(are|is|be|was|were|am)\\sunable\\sto\\s",
				"\\1 not can ");
	}
	regex_ireplace(str, "[,]", " , ");
	regex_ireplace(str, "\\s+", " ");
	regex_ireplace(str, "<ws>[,]<ws>(und|oder|or|and)", " \\1");
	regex_ireplace(str, "^na<ws>[,]<ws>", "");
	regex_ireplace(str, "^naja<ws>[,]<ws>", "");
	regex_ireplace(str, "^und[,]<ws>", "");
	regex_ireplace(str, "^na[,]<ws>", "");
	if (str.size() < 8) {
		regex_ireplace(str, "^und\\s+", "");
		regex_ireplace(str, "^ok\\s+", "");
		regex_ireplace(str, "^gut\\s+", "");
	}
	regex_ireplace(str, "^nein\\s+", "");
	if (str.size() > 5) {
		regex_ireplace(str, "^ja\\s+", "");
	}
	regex_ireplace(str, "^oder\\s+", "");
	regex_ireplace(str, "^na\\s+", " ");
	regex_ireplace(str, "^naja\\s+", " ");
	regex_ireplace(str, "^h[m]+?\\s+", " ");
	regex_ireplace(str, "^(wie|was)<ws>?[,]<ws>?", " ");
	regex_ireplace(str, "\\s\\s", " ");
	regex_ireplace(str, "^[,]", " ");
	regex_ireplace(str, "\\s\\s", " ");

	regex_ireplace(str, "^du\\s([a-zA-Z0-9]+?.?.?)$", "\\1");

	regex_ireplace(str, "\\(true\\)", "_(true)_");
	regex_ireplace(str, "\\(maybe\\)", "_(maybe)_");
	regex_ireplace(str, "\\(false\\)", "_(false)_");
	regex_ireplace(str, "\\(logic\\)", "_(logic)_");

	regex_ireplace(str,
			"(^|\\s)es\\s(.*?\\s)zu\\s(.?ein)em\\s(.*?)\\s((?:kommt)|(?:kommen))",
			"\\1\\2 \\3 \\4 ist");
	regex_ireplace(str,
			"(^|\\s)es\\s(.*?\\s)zu\\s(.?ein)em\\s(.*?)\\s((?:gekommen ist)|(?:gekommen war)|(?:kam))",
			"\\1\\2 \\3 \\4 war");
	regex_ireplace(str,
			"(^|\\s)es\\s(.*?\\s)zu\\s(.?ein)er\\s(.*?)\\s((?:kommt)|(?:kommen))",
			"\\1\\2 \\3 \\4 ist");
	regex_ireplace(str,
			"(^|\\s)es\\s(.*?\\s)zu\\s(.?ein)er\\s(.*?)\\s((?:gekommen ist)|(?:gekommen war)|(?:kam))",
			"\\1\\2 \\3e \\4 war");

	regex_ireplace(str, "jeden Tag ", "_jeden_tag_ ");

	regex_replace(str, "Sie([^a-zA-Z])", "du\\1");
	regex_replace(str, "Ihnen([^a-zA-Z])", "dir\\1");
	regex_replace(str, "ihnen([^a-zA-Z])", "dir\\1");

	regex_replace(str, "eine Menge", "viele");
	regex_replace(str, "(\\d)\\.(\\d)", "\\1\\2");

	if (regex_find(m, str, "^(ist|war|sind|waren)\\s")) {
		string verb = m[1];
		regex_ireplace(str, "^(ist|war|sind|waren)\\s", verb + " ");
	}
	regex_replace(str,
			"^(ist|war|sind|waren)\\s([a-z]+?)\\s(ein.?.?)\\s([A-Z][a-z]+?)(\\s?[?]?)$",
			"\\1 _\\2_ \\3 \\4 \\5");

	regex_ireplace(str, "^(.*?)du mir deinen Namen.*?$", "Wie ist dein Name? ");
	regex_ireplace(str, "^(.*?)du mir.*?deinen Namen.*?$",
			"Wie ist dein Name? ");

	if (regex_find(str, "\\(bad\\)")) {
		regex_ireplace(str, "\\s*?\\(bad\\)", "");
		algo::trim(str);
		regex_ireplace(str, "\\s+ ", "_");
		str = "_" + str + "_ = _(bad)_";
	}
	if (regex_find(str, "\\(good\\)")) {
		regex_ireplace(str, "\\s*?\\(good\\)", "");
		algo::trim(str);
		regex_ireplace(str, "\\s+ ", "_");
		str = "_" + str + "_ = _(good)_";
	}

	regex_ireplace(str,
			"sowohl\\s(.+?)als auch([a-z0-9\\s]*?[A-Z][a-z]+?)(\\s|$)",
			"\\1 \\$\\$aswellas\\$\\$ \\2\\3");
	regex_ireplace(str, "sowohl\\s(.+?)als auch(.*)",
			"\\1 \\$\\$aswellas\\$\\$ \\2");
	regex_ireplace(str, "weder\\s(.+?)noch([a-z0-9\\s]*?[A-Z][a-z]+?)(\\s|$)",
			"nicht \\1 \\$\\$aswellas\\$\\$ \\2\\3");
	regex_ireplace(str, "weder\\s(.+?)noch(.*)",
			"nicht \\1 \\$\\$aswellas\\$\\$ \\2");

	regex_ireplace(str,
			"(Anfang|Ende) (januar|jaenner|februar|maerz|april|mai|juni|juli|august|september|oktober|november|dezember)",
			"in dem _\\2_");

	regex_ireplace(str, "jaenner", "januar");
	string months[] = { "januar", "jaenner", "februar", "maerz", "april", "mai",
			"juni", "juli", "august", "september", "oktober", "november",
			"dezember" };
	int month_num = 1;
	const string& year = "2012";
	foreach (string month, months) {
		stringstream ss;
		ss << month_num;
		const string& _month_num = ss.str();
		regex_ireplace(str, "(seit|bis) " + month + " ([0-9]+)",
				"\\1 01." + _month_num + ".\\2s");
		regex_ireplace(str, "(seit|bis) " + month,
				"\\1 01." + _month_num + "." + year);
		++month_num;
	}

	regex_ireplace(str, "(^)([A-Z][a-z]+?en) (ist) ", "\\1_\\2_ \\3 ");

	regex_ireplace(str, "(^|\\s)genauso (.*?) wie ",
			"\\1genauso \\2 wie{{{adj}}} ");
	regex_ireplace(str, "(^|\\s)so (.*?) wie ", "\\1so \\2 wie{{{adj}}} ");

	regex_ireplace(str, "was ist ", "was ist ");
	regex_replace(str, "was ist ([A-Z][a-z]+?)([\\.?!+,;-\\s]*?)$",
			"was ist _\\1_\\2");

	regex_ireplace(str, "wie war ", "wie ist ");
	regex_ireplace(str, "\\s+kein", " nicht ein");
	regex_ireplace(str, "(^|\\s)?und sonst(\\s|\\$)", " wie geht es dir ");
	regex_ireplace(str, "(^|\\s)?bevor\\s", " , bevor ");
	regex_ireplace(str, "(^|\\s)?kurz \\, bevor\\s", " , kurz bevor ");
	regex_ireplace(str, "^ \\,", " ");
	regex_ireplace(str, "^\\,", " ");
	regex_ireplace(str, " mehr als ", " mehr als{{{adj}}} ");
	regex_ireplace(str, " lust zu ", " lust , zu ");
	regex_ireplace(str, " weisst du was (.*) ist ", " was ist \\1 ");
	if (str.size() < 14) {
		regex_ireplace(str, " weisst du ", " weisst du , ");
	}
	regex_ireplace(str, " weniger als ", " wenigerals ");
	regex_ireplace(str, " bis zu ", " biszu ");
	regex_ireplace(str, " bis in ", " bisin ");
	regex_ireplace(str, " bis auf ", " bisauf ");
	regex_ireplace(str, "^bis zu ", " biszu ");
	regex_ireplace(str, "^bis in ", " bisin ");
	regex_ireplace(str, "^bis auf ", " bisauf ");
	regex_ireplace(str, "^kein(.*)", "ein\\1 nicht");
	regex_ireplace(str, "wozu braucht man ", "was ist ");
	regex_ireplace(str, "(brauch)(st|e|en)(.*?)zu\\s(haben)", "\\1\\2\\3 \\4");

	regex_replace(str, "(^|\\s)X(\\s|$)", "\\1\\$a\\$\\2");
	regex_replace(str, "(^|\\s)Y(\\s|$)", "\\1\\$b\\$\\2");
	regex_replace(str, "(^|\\s)Z(\\s|$)", "\\1\\$c\\$\\2");

	regex_ireplace(str, "([0-9]|\\$)\\s*?(mal)\\s*?([0-9]|\\$)", "\\1*\\3");

	regex_ireplace(str, "(^|\\s)([+-])ein(\\s|$)", "\\1\\2 1\\3");
	string numbers[] = { "eins", "zwei", "drei", "vier", "fuenf", "sechs",
			"sieben", "acht", "neun", "zehn", "elf", "zwoelf", "dreizehn",
			"vierzehn", "fuenfzehn", "sechzehn", "siebzehn", "achtzehn",
			"neunzehn", "zwanzig" };

	int i = 0;
	foreach (string number, numbers) {
		stringstream ss;
		ss << i;
		const string& _i = ss.str();

		regex_ireplace(str, "(^|\\s)" + number + "(\\s|$)",
				"\\1 " + _i + "\\3");
		regex_ireplace(str, "(^|\\s)[+]" + number + "(\\s|$)",
				"\\1+" + _i + "\\3");
		regex_ireplace(str, "(^|\\s)[-]" + number + "(\\s|$)",
				"\\1-" + _i + "\\3");
		++i;
	}

	regex_ireplace(str, "(tag|datum|monat|woche|jahr) war\\s", "\\1 ist ");
	regex_ireplace(str, "welche uhrzeit\\s", "wie uhr ");
	regex_ireplace(str, "\\suhr\\shaben\\swir\\s", " uhr ist es ");
	regex_ireplace(str, "\\suhr\\shaben\\swir[?]", " uhr ist es?");
	regex_ireplace(str, "aneinander ", "aneinander");
	regex_ireplace(str, "\\shaben\\swir\\sheute", " haben wir ");
	regex_ireplace(str, "\\sist\\sheute", " ist ");
	regex_ireplace(str, "(\\s|^)(ist|bist)\\s(.*?)\\sheute\\s", "\\1\\2 \\3 ");
	regex_ireplace(str, "welchen\\stag\\shaben\\swir\\s",
			"welch Datum haben wir ");
	regex_ireplace(str, "welcher\\stag\\sist\\s", "welch Datum haben wir ");

	regex_ireplace(str, " hab ", " habe ");

	if (!regex_find(str, "(heiss|name)") && lang == "de") {
		regex_ireplace(str, " FreeHAL(.?.?.?.?)$", " \\1");
	}

	str = " " + str + " ";
	regex_ireplace(str, "\\snoch\\s(nie|nicht)([\\s!.,?]+)", " noch-\\1\\2");
	regex_ireplace(str, "\\snoch([\\s!.,?]+)", "\\1");
	regex_ireplace(str, "\\snoch[-](nie|nicht)([\\s!.,?]+)", " noch \\1\\2");
	regex_ireplace(str, "(^|[\\s!.,?]+)(so)\\setwas([\\s!.,?]+)",
			"\\1_\\2_etwas_\\3");
	regex_ireplace(str, "(^|[\\s!.,?]+)(so)was([\\s!.,?]+)",
			"\\1_\\2_etwas_\\3");

	ifstream remove_words_file;
	remove_words_file.open(
			(path + "/lang_" + lang + "/remove-words.csv").c_str());
	if (i) {
		string line;
		vector<string> remove_words_file_lines;
		while (std::getline(remove_words_file, line)) {
			remove_words_file_lines.push_back(line);
		}
		foreach (string remove_word, remove_words_file_lines) {
			algo::trim(remove_word);
			bool at_beginning = algo::starts_with(remove_word, "^");
			bool at_end = algo::ends_with(remove_word, "$");
			if (at_beginning) {
				regex_ireplace(str,
						"^([\\s!.,?]+)" + remove_word + "([\\s!.,?]+)",
						"\\1\\2");
			} else if (at_end) {
				regex_ireplace(str,
						"([\\s!.,?]+)" + remove_word + "([\\s!.,?]+)$",
						"\\1\\2");
			} else {
				regex_ireplace(str,
						"([\\s!.,?]+)" + remove_word + "([\\s!.,?]+)",
						"\\1\\2");
			}
			regex_ireplace(str, "\\s+", " ");
		}
	}
	regex_ireplace(str, "do you know ", "");
	algo::trim(str);
	regex_ireplace(str, "^you know (wh)", "\\1");

	regex_ireplace(str, "\\sreally", "");

	regex_ireplace(str, "world wide web", "_world_wide_web_");
	regex_ireplace(str, "Hersteller von", "Hersteller fuer");

	regex_ireplace(str, "mein name", "_mein_name_");
	regex_ireplace(str, "dein name", "_dein_name_");

	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?januar(\\s|$)", "\\1\\2.01.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?jaenner(\\s|$)",
			"\\1\\2.01.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?februar(\\s|$)",
			"\\1\\2.02.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?maerz(\\s|$)", "\\1\\2.03.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?april(\\s|$)", "\\1\\2.04.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?mai(\\s|$)", "\\1\\2.05.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?juni(\\s|$)", "\\1\\2.06.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?juli(\\s|$)", "\\1\\2.07.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?august(\\s|$)", "\\1\\2.08.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?september(\\s|$)",
			"\\1\\2.09.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?oktober(\\s|$)",
			"\\1\\2.10.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?november(\\s|$)",
			"\\1\\2.11.\\4");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.\\s*?dezember(\\s|$)",
			"\\1\\2.12.\\4");

	regex_ireplace(str, "(^|\\s)(\\d)\\.(\\d+?)\\.(\\d+?)(\\s|$)",
			"\\1 0\\2.\\3.\\4\\5");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.(\\d)\\.(\\d+?)(\\s|$)",
			"\\1\\2.0\\3.\\4\\5");
	regex_ireplace(str, "(^|\\s)(\\d+?)\\.(\\d+?)\\.(\\d\\d)(\\s|$)",
			"\\1\\2.\\3.19\\4\\5");
	regex_replace(str, "(^|\\s)(\\d+?)\\.(\\d+?)\\.(\\d+?)(\\s|$)",
			"\\2.\\3.\\4\\5");
	regex_replace(str, "(^|\\s)(\\d+?)\\.(\\d+?)\\.?(\\s|$)",
			"\\2.\\3.0000\\5");

	algo::trim(str);

	regex_ireplace(str, "(^|\\s)sein\\s([A-Z])", "\\1sein{{{art}}} \\2");

	regex_ireplace(str, "\\s([mds])eines\\s(.*?[a-z])s(\\s|$)",
			" von \\1einem \\2\\3");
	regex_ireplace(str, "\\s([mds])eines\\s", " von \\1einem ");
	regex_ireplace(str, "\\s([mds])eines\\s", " von \\1einem ");
	regex_ireplace(str, "\\s([mds])einer\\s", " von \\1einer ");
	regex_ireplace(str, "\\s(aus|von|in|an)\\svon\\s([mds])eine([rs])\\s",
			" \\1 \\2eine\\3 ");

	regex_replace(str, "(\\s)([A-Z][a-z]*?)\\sder\\s([A-Z])",
			"\\1\\2 von der _\\3_");
	regex_replace(str,
			"(\\s)([A-Z][a-z]*?)\\sdes\\s((?:[a-z]+?\\s)*[A-Z][a-z]*?)s([^a-zA-Z])",
			"\\1\\2 von dem _\\3_\\4");
	regex_replace(str, "(\\s)([A-Z][a-z]*?)\\sdes\\s([A-Z])",
			"\\1\\2 von dem _\\3_");

	regex_replace(str, "(\\s)([A-Z][a-z]*?)\\sjeder\\s([A-Z])",
			"\\1\\2 von jeder _\\3_");
	regex_replace(str,
			"(\\s)([A-Z][a-z]*?)\\sjedes\\s([A-Z][a-z]*?)s([^a-zA-Z])",
			"\\1\\2 von jedem _\\3_\\4");
	regex_replace(str, "(\\s)([A-Z][a-z]*?)\\sjedes\\s([A-Z])",
			"\\1\\2 von jedem _\\3_");

	regex_replace(str,
			"(\\s)([A-Z][a-z]*?)\\s(mehrer|viel|wenig|einig)er\\s([A-Z])",
			"\\1\\2 von \\3en \\4");

	if (regex_find(str, "(^|\\s)(.+?)\\s(den|dem|der|des)\\s([A-Z])")) {
		if (regex_find(m, lc(str),
				"(^|\\s)(laut|nach)\\s(den|dem|der|des)\\s")) {
			string preposition = m[2];
			regex_ireplace(str, "(^|\\s)(laut|nach)\\s(den|dem|der|des)\\s",
					"\\1_" + preposition + "_ \\3 ");
		}
	}

	regex_ireplace(str, "\\s?ist dasselbe wie\\s?", " = ");
	regex_ireplace(str, "\\s?ist dasgleiche wie\\s?", " = ");
	regex_ireplace(str, "\\s?ist das selbe wie\\s?", " = ");
	regex_ireplace(str, "\\s?ist das gleiche wie\\s?", " = ");

	regex_ireplace(str, "\\s?is the same as\\s?", " = ");
	regex_ireplace(str, "\\s?is same as\\s?", " = ");

	regex_replace(str, "befindet sich", "liegt");
	regex_replace(str, "sich (.*?)befindet", "\\1liegt");
	regex_ireplace(str, "teil von ein.?.?\\s", "teil von ");

	if (is_question) {
		if (regex_find(m, str,
				"^(.*?) hat (?:der|die|das)\\s([^A-Z]*?[A-Z][^\\s]+?)\\s(.*?)$")) {
			const string& subject = m[1];
			string prop = m[2];
			string value = m[3];

			to_name(prop);
			to_name(value);

			str = subject + " is-property " + prop + " _:_ " + value;
		}

		/*if (regex_find(m, str, "^(.*?) ist ([a-z]+?)\\s*?$")) {
		 const string& subject = m[1];
		 const string& prop = "";
		 string value = m[2];

		 //////////////////////////////////////////////////////////////////
		 // TODO
		 //set new flag contains_verb to contains_part_of_speech with var value, var lang, var path
		 //if (flag contains_verb) {
		 //	value = ""
		 //}

		 if (value.size() > 0) {
		 trim(value);
		 regex_replace(value, "\\s+", " ");

		 set new array possible_props to find_property_by_value with var value, var lang, var path
		 foreach (new var _prop in array possible_props) {
		 print "prossible property foreach (value: " concat var _prop concat " = " concat var value concat new line
		 prop = var _prop
		 }

		 if (var prop) {
		 prop = to_name with var prop
		 value = to_name with var value

		 string expr = var prop concat " _:_ " concat var value
		 str = "var subject is-property var expr"
		 }
		 }
		 }*/
	}

	else {
		if (regex_find(m, str, "^(.*?) hat (.*?)((?:[?].*?)?)$")) {
			const string& subject = m[2];
			const string& prop = m[1];

			str = subject + " is-property " + prop + " ?";
		}
	}

	if (!algo::contains(str, "?") && !algo::contains(str, ",")) {
		if (regex_ifind(m, str,
				"^(.+?)\\s+?((?:gehoert zu)|(?:liegt in)|(?:\\s?ist in)|(?:\\s?ist .*?teil von))\\s+?(.+?)$")) {
			string a = m[1];
			string b = m[3];
			string adverbs = "";

			remove_adverbs(a, adverbs);
			remove_adverbs(b, adverbs);

			to_name(a);
			to_name(b);

			str = a + " is-part " + b + " " + adverbs;
		}
		if (regex_ifind(m, str, "^(.+?)\\s+?(gehoert)\\s+?(.+?)$")) {
			string a = m[1];
			string b = m[3];
			string adverbs = "";

			remove_adverbs(a, adverbs);
			remove_adverbs(b, adverbs);

			to_name(a);
			to_name(b);

			str = a + " is-own " + b + " " + adverbs;
		}
		if (regex_ifind(m, str, "^(.+?)\\s+?(beinhaltet)\\s+?(.+?)$")) {
			string a = m[3];
			string b = m[1];
			string adverbs = "";

			remove_adverbs(a, adverbs);
			remove_adverbs(b, adverbs);

			to_name(a);
			to_name(b);

			str = a + " is-part " + b + " " + adverbs;
		}
		if (regex_ifind(str, "^(.+?)\\s+?(besitzt)\\s+?(.+?)$")) {
			string a = m[3];
			string b = m[1];
			string adverbs = "";

			remove_adverbs(a, adverbs);
			remove_adverbs(b, adverbs);

			to_name(a);
			to_name(b);

			str = a + " is-own " + b + " " + adverbs;
		}
		/*		if (regex_find(str, "^(.+?)\\s+?(ist|sind|[=])\\s+?(.+?)$") and flag is_question is false) {
		 string a = m[1];
		 string b = m[3];
		 string adverbs = "";
		 remove_adverbs(a, adverbs);
		 remove_adverbs(b, adverbs);

		 //////////////////////////////////////////////////////////////////
		 // TODO
		 set new flag contains_verb to contains_part_of_speech with var b, var lang, var path
		 if (flag contains_verb is false) {
		 a = to_name with var a
		 b = to_name with var b

		 if (regex_find(a not, "\\(a\\)") and regex_find(b, "\\(a\\)") and regex_find(input not, "[=]")) {
		 str = var a concat " is-a " concat var b concat " " concat var adverbs
		 }
		 else {
		 str = var a concat " = " concat var b concat " " concat var adverbs
		 }
		 }}*/

		/*if (!regex_find(str, "ein|eine|der|die|das")) {
		 bool contains_verb = contains_part_of_speech(str, lang, path);
		 if (!contains_verb) {
		 regex_ireplace(str, "^(.+?)\\s+?(ist|sind)\\s+?(.+?)\\s?$", "\\1 = \\3");
		 }
		 }*/

		regex_ireplace(str, "^(ein|eine)\\s", " ");
	}

	/*    set new var __input to var input
	 while var __input matches /(ein.?.?)\s(<[A-Z]><[a-z]>+?)(\s|$)/ do
	 set new var word to lc with $1
	 set new array _collective_nouns to global array collective_nouns
	 push into array _collective_nouns, var word
	 print "collective noun: " concat var word concat new line

	 do regex with var __input: /(ein.?.?)\s(var word)/ -> "" :global:i
	 done

	 */

	regex_ireplace(str, "(^|\\s)du hast (.*?) aus ", "\\1du bekommst \\2 aus ");
	regex_ireplace(str, "(^|\\s)woher hast du ", "\\1woher bekommst du ");
	regex_ireplace(str, "(^|\\s)woher hast du ", "\\1woher bekommst du ");

	regex_ireplace(str, " wehnig ", " wenig ");
	regex_ireplace(str, "niss(\\s|$)", "nis\\1");
	regex_ireplace(str, "^wovon ", "was ");
	regex_ireplace(str, " wovon ", " was ");
	regex_ireplace(str, "^von was ", "was ");
	regex_ireplace(str, " von was ", " was ");

	regex_ireplace(str, "^([A-Z][a-z]+?en) ist ", "_\\1_ ist ");

	regex_ireplace(str, " wirst du genannt", " heisst du ");
	regex_ireplace(str, " wird (.*?) genannt", " \\1 ist ");

	regex_ireplace(str, "^das\\s([a-z]+?)\\s(ich|du)", "\\2 \\1 das ");

	regex_ireplace(str, " ein jeder ", " _jeder_ ");
	regex_ireplace(str, " sinn des lebens", " _sinn_des_lebens_ ");

	regex_ireplace(str, " du jetzt ", " du ");
	regex_ireplace(str, " ich jetzt ", " ich ");

	regex_ireplace(str, " befindet sich ", " liegt ");
	regex_ireplace(str, " befinden sich ", " liegen ");
	regex_ireplace(str, " befinde mich ", " liege ");
	regex_ireplace(str, " befindest dich ", " liegst ");
	regex_ireplace(str, " sich befindet ", " liegt ");
	regex_ireplace(str, " sich befinden ", " liegen ");
	regex_ireplace(str, " mich befinde ", " liege ");
	regex_ireplace(str, " dich befindest ", " liegst ");

	regex_ireplace(str, "(^|\\s)da([r]?)(durch|auf|fuer|an) ", " \\3 _das_ ");

	regex_ireplace(str, "([0-9])([a-z]|[A-Z])", "\\1 \\2");
	regex_ireplace(str, "([a-z]|[A-Z])([0-9])", "\\1 \\2");
	regex_ireplace(str, "([0-9])\\.([a-zA-Z])", "\\1 \\2");
	regex_ireplace(str, "([a-z]|[A-Z])\\.([0-9])", "\\1 \\2");

	regex_ireplace(str, "\\smacht man mit\\s", " ist ");
	regex_ireplace(str, "\\sist mit\\s", " ist-mit ");

	regex_ireplace(str, "Was fuer (.*?) kennst du.*", "was ist \\1 ?");

	regex_ireplace(str, "dem Begriff der ", "der ");
	regex_ireplace(str, "den Begriff der ", "die ");
	regex_ireplace(str, "der Begriff der ", "die ");
	regex_ireplace(str, "dem Begriff des ", "dem ");
	regex_ireplace(str, "den Begriff des ", "das ");
	regex_ireplace(str, "der Begriff des ", "das ");

	regex_ireplace(str,
			"([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)\\s*?[,]\\s*?([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)\\s*?(und|oder)\\s*?([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)",
			"\\1 \\3 \\2 \\3 \\4");
	regex_ireplace(str,
			"([-_a-zA-Z]+?)\\s*?[,]\\s*?([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)\\s*?(und|oder)\\s*?([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)",
			"\\1 \\3 \\2 \\3 \\4");
	regex_ireplace(str,
			"([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)\\s*?[,]\\s*?([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)\\s*?(und|oder)\\s*?([-_a-zA-Z]+?)",
			"\\1 \\3 \\2 \\3 \\4");
	regex_ireplace(str,
			"([-_a-zA-Z]+?)\\s*?[,]\\s*?([-_a-zA-Z]+?)\\s*?(und|oder)\\s*?([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)",
			"\\1 \\3 \\2 \\3 \\4");
	regex_ireplace(str,
			"([-_a-zA-Z]+?)\\s*?[,]\\s*?([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)\\s*?(und|oder)\\s*?([-_a-zA-Z]+?)",
			"\\1 \\3 \\2 \\3 \\4");
	regex_ireplace(str,
			"([-_a-zA-Z]+?\\s+?[-_a-zA-Z]+?)\\s*?[,]\\s*?([-_a-zA-Z]+?)\\s*?(und|oder)\\s*?([-_a-zA-Z]+?)",
			"\\1 \\3 \\2 \\3 \\4");
	regex_ireplace(str,
			"([-_a-zA-Z]+?)\\s*?[,]\\s*?([-_a-zA-Z]+?)\\s*?(und|oder)\\s*?([-_a-zA-Z]+?)",
			"\\1 \\3 \\2 \\3 \\4");

	regex_ireplace(str, "^was\\sgeht<ws>[?]", "wie geht es dir?");
	regex_ireplace(str, "^was\\sgeht$", "wie geht es dir?");
	regex_ireplace(str, "^was\\sgeht\\sab<ws>[?]", "wie geht es dir?");
	regex_ireplace(str, "^was\\sgeht\\sab$", "wie geht es dir?");

	regex_ireplace(str, "^wie\\slang\\s", "wie ");
	regex_ireplace(str, "^wie\\slange\\s", "wie ");

	regex_ireplace(str, "Ihnen", "dir");
	regex_ireplace(str, "\\sdenn<ws>?[?]", " ?");
	regex_ireplace(str, "\\sdenn[?]", " ?");
	regex_ireplace(str, "\\sdann<ws>?[?]", " ?");
	regex_ireplace(str, "\\sdann[?]", " ?");
	regex_ireplace(str, "St\\.", "St");
	regex_ireplace(str, "bitte (sag|erzaehl)", "\\1");
	regex_ireplace(str, "Kannst du mir sagen[,]+", "");
	regex_ireplace(str, "Kannst du mir sagen", "");
	regex_ireplace(str, "sage mir ", "was ist ");
	regex_ireplace(str, "sag was ", "was ist ");
	regex_ireplace(str, "sag etwas ", "was ist ");
	regex_ireplace(str, "sag ", "was ist ");

	regex_ireplace(str, "(ich glaube) ([a-zA-Z])", "\\1 , \\2");
	regex_ireplace(str, "(ich denke) ([a-zA-Z])", "\\1 , \\2");
	regex_ireplace(str, "stelle mir eine frage", "was ist ");
	regex_ireplace(str, "stell mir eine frage", "was ist ");
	regex_ireplace(str, "stelle eine frage", "was ist ");
	regex_ireplace(str, "stell eine frage", "was ist ");
	regex_ireplace(str, "Was kannst du mir ueber (.*?) sagen", "was ist \\1");
	regex_ireplace(str, "Was weisst du ueber (.*?)$", "was ist \\1");
	regex_ireplace(str, "Was kannst du mir ueber (.*?) erzaehlen",
			"was ist \\1");
	regex_ireplace(str, "Was kannst du ueber (.*?) sagen", "was ist \\1");
	regex_ireplace(str, "Was weisst du alles", "was ist");

	regex_ireplace(str, "frag mich was", "was ist");
	regex_ireplace(str, "frag mich etwas", "was ist");
	regex_ireplace(str, "frag mich<ws>?[,]", "");
	regex_ireplace(str, "was ist ([dsmk]?ein)([a-zA-Z]+?)\\s",
			"was ist \\1\\2 ");
	regex_ireplace(str, "was denkst du ueber ", "was ist ");
	regex_ireplace(str, "wie denkst du ueber ", "was ist ");
	regex_ireplace(str, "was haeltst du von ", "was ist ");
	regex_ireplace(str, "was haelst du von ", "was ist ");
	regex_ireplace(str, "erzaehl mir was(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehl mir etwas(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehle mir was(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehle mir etwas(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehl mir bitte was(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehl mir bitte etwas(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehle mir bitte was(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehle mir bitte etwas(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzael mir was(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzael mir etwas(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaele mir was(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaele mir etwas(?:([\\s!?.]?.*?$)|$)",
			"was ist \\1 ?");
	regex_ireplace(str, "erzaehl was(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzaehl etwas(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzaehle was(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzaehle etwas(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzael was(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzael etwas(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzaele was(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "erzaele etwas(?:([\\s!?.]?.*?$)|$)", "was ist \\1 ?");
	regex_ireplace(str, "Erzaehlst du .*", "was ist?");
	regex_ireplace(str, "was ist\\s+?ueber ", "was ist ");

	if (regex_ifind(m, str, "(was heisst) ([a-z])")) {
		string temp = ucfirst(m[2]);
		regex_ireplace(str, "(was heisst) ([a-z])", "\\1 " + temp);
	}

	regex_ireplace(str, "(was\\sheisst\\s)", "was heisst ");
	regex_replace(str,
			"(was\\sheisst\\s)([A-Z][a-z]+?\\s[a-z]+?)((?:\\s.?.?.?.?.?)|$)",
			"\\1 _\\2_\\3");

	regex_ireplace(str,
			"(ich weiss nicht)\\s+?(?:(global var questionwords_reg_ex) (.*?))($|[.,])",
			"_\\2_ _no-question_ \\3 , weiss ich nicht\\4");
	regex_ireplace(str,
			"(du weisst nicht)\\s+?(?:(global var questionwords_reg_ex) (.*?))($|[.,])",
			"_\\2_ _no-question_ \\3 , weisst du nicht\\4");
	regex_ireplace(str, "(weiss ich)\\s+?(global var questionwords_reg_ex)",
			"\\1 , \\2");
	regex_ireplace(str, "(weisst du)\\s+?(global var questionwords_reg_ex)",
			"\\1 , \\2");

	if (!algo::contains(str, "_no-question_")
			&& regex_ifind(str,
					"^\\s*?(?:wie|wer|was|wo|wann|warum|wieso|weshalb|welcher|welchem|welches|welche|who|how|where|when|if|what)\\s")
			&& (!algo::ends_with(str, "?") || regex_find(str, "[?].*?[?]"))) {
		regex_replace(str, "\\s*[?]", "");
		algo::trim(str);
		str += " ?";
	}

	regex_ireplace(str,
			"(was (?:machst|tust) du).*?(heute|jetzt|momentan|gerade|grade).*?$",
			"\\1 ?");
	regex_ireplace(str, "(das weiss ich,)$", "\\1 ?");
	regex_ireplace(str, "(das weiss ich)$", "\\1 ?");
	regex_ireplace(str, "sag mir ", "");
	regex_ireplace(str, "sag mir[,]", "");
	regex_ireplace(str, "^<ws>?ob\\s", "");
	regex_ireplace(str, "can you remmember that ", "");
	regex_ireplace(str, "do you know whether ", "");
	regex_ireplace(str, "you know whether ", "");
	regex_ireplace(str, "von wo ", "woher ");
	regex_ireplace(str, "(^|\\s)was fuer eine\\s", " welche ");
	regex_ireplace(str, "(^|\\s)was fuer einen\\s", " welchen ");
	regex_ireplace(str, "(^|\\s)was fuer einem\\s", " welchem ");
	regex_ireplace(str, "(^|\\s)was fuer ein\\s", " welches ");
	regex_ireplace(str, "(^|\\s)was fuer\\s", " welch ");
	regex_ireplace(str, "was (.+?) fuer eine\\s(.+)", "welche \\2 \\1");
	regex_ireplace(str, "was (.+?) fuer einen\\s(.+)", "welchen \\2 \\1");
	regex_ireplace(str, "was (.+?) fuer einem\\s(.+)", "welchem \\2 \\1");
	regex_ireplace(str, "was (.+?) fuer ein\\s(.+)", "welches \\2 \\1");
	regex_ireplace(str, "was (.+?) fuer\\s(.+)", "welch \\2 \\1");
	regex_ireplace(str, "can you tell me whether\\s", "");
	regex_ireplace(str, "can you tell me (who|how|where|when|if|what)", "\\1 ");
	regex_ireplace(str, "can you tell me\\s", "what is ");

	regex_ireplace(str, "^sobald\\s*?(.*),\\s*?(.*)$", "\\1, wenn \\2");
	regex_ireplace(str, "^(.*),\\s*?sobald\\s*?(.*)$", "\\2, wenn \\1");
	regex_ireplace(str, "^wenn\\s*?(.*),\\s*?(.*)$", "\\2, wenn \\1");

	if (not regex_find(str, " aus.?.?.?.?$")) {
		regex_ireplace(str, "kennst du ein(e|en) ", "what-nowiki ist ");
		regex_ireplace(str, "kennst du ", "was ist ");
	}

	regex_ireplace(str, "wie heisst du mit ", "du ");

	regex_ireplace(str, "was macht ", "was ");

	regex_ireplace(str, "kannst du (.*?isch)($|(?:.?.?.?.?$))",
			"kannst du \\1 sprechen \\2");
	regex_ireplace(str, "kann ich (.*?isch)($|(?:.?.?.?.?$))",
			"kann ich \\1 sprechen \\2");

	regex_ireplace(str, "(^|\\s)?wie wird ", "\\1 wie ist ");
	regex_ireplace(str, "Wie ist das Wetter heute", "Wie ist das Wetter ");
	regex_ireplace(str, "dir heute", "dir ");
	if (str.size() > 10) {
		regex_ireplace(str, " ja ", " ");
	}

	regex_ireplace(str, "es ist<ws>?$", "ist es ");
	regex_ireplace(str, "es ist<ws>?[?]<ws>?$", "ist es ?");

	// cout << "parser2012: step 3: " << str << endl;

	if (regex_find(m, str, "\\s(|d|m|k)ein\\s([a-z]+?en)(\\s|$)")) {
		const string& noun = ucfirst(m[2]);
		regex_replace(str, "\\s(|d|m|k)ein\\s([a-z]+?en)(\\s|$)",
				" \\1ein " + noun + "\\3");
	}

	if (regex_find(str, "(([?])|global var questionwords_reg_ex)")) {
		regex_ireplace(str,
				" (seinen|ihren|seiner|ihrer|seines|ihres|seine|ihre) ",
				" das ");
	}

	regex_ireplace(str, "Weisst du etwas ueber ", "was ist ");
	regex_ireplace(str, "was weisst du ueber ", "was ist ");
	regex_ireplace(str, "heise", "heisse");
	regex_ireplace(str, "heist", "heisst");
	regex_ireplace(str, " has to ", " must ");
	regex_ireplace(str, " have to ", " must ");
	regex_ireplace(str, " had to ", " must ");
	regex_ireplace(str, " is able to ", " can ");
	regex_ireplace(str, " am able to ", " can ");
	regex_ireplace(str, "m able to ", " can ");
	regex_ireplace(str, " are able to ", " can ");
	regex_ireplace(str, " were able to ", " can ");
	regex_ireplace(str, " was able to ", " can ");
	regex_ireplace(str, " has been able to ", " can ");
	regex_ireplace(str, " have been able to ", " can ");
	regex_ireplace(str, "don['`']t", "do not");
	regex_ireplace(str, "can['`']t", "can not");
	regex_ireplace(str, "cannot", "can not");
	regex_ireplace(str, "hasn['`']t", "has not");
	regex_ireplace(str, "havn['`']t", "have not");
	regex_ireplace(str, "didn['`']t", "did not");
	regex_ireplace(str, "mustn['`']t", "must not");
	regex_ireplace(str, "n['`']t", " not");
	regex_ireplace(str, "gehts(.?.?.?.?)$", "geht es dir ?");
	regex_ireplace(str, "gehts", "geht es");
	regex_ireplace(str, "geht['`']s", "geht es");
	regex_ireplace(str, "^(.?.?)gibt es ", "\\1was ist ");
	regex_ireplace(str, "^(.?.?)gibt es", "\\1was ist");
	regex_ireplace(str, "was ist neues", "was gibt es neues");

	regex_ireplace(str, "geht es so[?\\s]", "geht es\\1");
	regex_ireplace(str, "wie geht es [?]", "wie geht es dir ?");
	regex_ireplace(str, "wie geht es\\s*?$", "wie geht es dir ?");
	regex_ireplace(str, "wie geht es<ws>?$", "wie geht es dir ?");

	// cout << "parser2012: step 4: " << str << endl;

	for (int i = 1; i < 20; ++i) {
		regex_ireplace(str,
				"([a-zA-Z0-9_]+)<ws>[,]<ws>([a-zA-Z0-9_]+)\\s+(und|oder|or|and)<ws>",
				"\\1 \\3 \\2 \\3 ");
		regex_ireplace(str,
				"<ws>[,]<ws>([a-zA-Z0-9_]+\\s+[a-zA-Z0-9_]+)\\s+(und|oder|or|and)<ws>",
				" \\2 \\1 \\2 ");
	}
	regex_ireplace(str, "heisse\\sich", "ist _mein_name_");
	regex_ireplace(str, "ich\\sheisse", "_mein_name_ ist");
	regex_ireplace(str, "wie heisst\\sdu", "wer bist du");
	regex_ireplace(str, "wie heisse\\s", "wer bin ");
	regex_ireplace(str, "heisst\\sdu", "ist _dein_name_");
	regex_ireplace(str, "du\\sheisst", "_dein_name_ ist");

	regex_replace(str, "(ist) ([A-Z][a-z]+?) (ein)", "\\1 _\\2_ \\3");

	regex_ireplace(str, ", die du kennst\\s", " ");
	regex_ireplace(str, "die du kennst\\s", " ");

	regex_ireplace(str, "(^|\\s)es leben", "\\1leben");

	regex_ireplace(str,
			" (\\d+?)\\s*?(gb|gigabyte|mb|megabyte|kb|kilobyte|byte)\\s*?(ram|arbeitsspeicher|festplatte|speicher)",
			" _\\1_\\2_\\3_ ");

	regex_ireplace(str, " brauchst du ", " du brauchst ");
	regex_ireplace(str, " brauche ich ", " ich brauche ");

	regex_ireplace(str, "nenne mir (.*)", "zaehle \\1 auf");
	regex_ireplace(str, "nenne (.*)", "zaehle \\1 auf");

	regex_ireplace(str, "Zaehle mir alle (.*)en auf.*", "ENUMALL ein \\1e");
	regex_ireplace(str, "Zaehle mir die (.*)en auf.*", "ENUMALL eine \\1e");
	regex_ireplace(str, "Zaehle mir den (.*)en auf.*", "ENUMALL ein \\1e");
	regex_ireplace(str, "Zaehle mir das (.*)en auf.*", "ENUMALL ein \\1e");
	regex_ireplace(str, "Zaehle alle (.*)en auf.*", "ENUMALL ein \\1e");
	regex_ireplace(str, "Zaehle die (.*)en auf.*", "ENUMALL eine \\1e");
	regex_ireplace(str, "Zaehle den (.*)en auf.*", "ENUMALL ein \\1e");
	regex_ireplace(str, "Zaehle das (.*)en auf.*", "ENUMALL ein \\1e");

	regex_ireplace(str, "Zaehle mir alle (.*)n auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle mir die (.*)n auf.*", "ENUMALL eine \\1");
	regex_ireplace(str, "Zaehle mir den (.*)n auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle mir das (.*)n auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle alle (.*)n auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle die (.*)n auf.*", "ENUMALL eine \\1");
	regex_ireplace(str, "Zaehle den (.*)n auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle das (.*)n auf.*", "ENUMALL ein \\1");

	regex_ireplace(str, "Zaehle mir alle (.*)s auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle mir die (.*)s auf.*", "ENUMALL eine \\1");
	regex_ireplace(str, "Zaehle mir den (.*)s auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle mir das (.*)s auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle alle (.*)s auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle die (.*)s auf.*", "ENUMALL eine \\1");
	regex_ireplace(str, "Zaehle den (.*)s auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle das (.*)s auf.*", "ENUMALL ein \\1");

	regex_ireplace(str, "Zaehle mir alle (.*[rmndtp])e auf.*",
			"ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle alle (.*[rmndtp]) auf.*", "ENUMALL ein \\1");

	regex_ireplace(str, "Zaehle mir alle (.*) auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle mir die (.*) auf.*", "ENUMALL eine \\1");
	regex_ireplace(str, "Zaehle mir den (.*) auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle mir das (.*) auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle alle (.*) auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle die (.*) auf.*", "ENUMALL eine \\1");
	regex_ireplace(str, "Zaehle den (.*) auf.*", "ENUMALL ein \\1");
	regex_ireplace(str, "Zaehle das (.*) auf.*", "ENUMALL ein \\1");

	regex_ireplace(str, "Zaehle (.*) auf.*", "ENUMALL \\1");
	regex_ireplace(str, "ENUMALL mir (.*?) auf.*", "ENUMALL \\1");
	regex_ireplace(str, "ENUMALL mir (.*)", "ENUMALL \\1");

	// cout << "parser2012: step 4.1: " << str << endl;

	regex_ireplace(str, "http[:/]+", "http_");

	regex_ireplace(str,
			"(^|\\s)(eigentlich|wirklich|doch|nun|wenigstens|schliesslich|denn)(\\s|$)",
			"\\1\\3");

	regex_ireplace(str, " das verlangen ", " das _verlangen_ ");

	regex_ireplace(str, "wie viel uhr", "wie uhr");
	regex_ireplace(str, "wie viel[a-zA-Z]*\\s", "wie ");
	regex_ireplace(str, "wieviel[a-zA-Z]*\\s", "wie ");
	regex_ireplace(str, "wie spaet", "wie uhr");
	regex_ireplace(str, "wie frueh", "wie uhr");
	regex_ireplace(str, "wie uhr ", "wie _$$time$$_ ");

	regex_ireplace(str, "[=]", " = ");
	regex_replace(str, "opposite", " opposite ");
	regex_ireplace(str, "wofuer steht ", "was ist ");
	regex_ireplace(str, " schon mal ", " ");
	regex_ireplace(str, "hast du schon mal von (.*?) gehoert", "was ist \\1");
	regex_ireplace(str, "hast du schon von (.*?) gehoert", "was ist \\1");
	regex_ireplace(str, "hast du von (.*?) gehoert", "was ist \\1");
	regex_ireplace(str, "hast du mal von (.*?) gehoert", "was ist \\1");
	regex_ireplace(str, "hast du schon mal was von (.*?) gehoert",
			"was ist \\1");
	regex_ireplace(str, "hast du schon was von (.*?) gehoert", "was ist \\1");
	regex_ireplace(str, "hast du was von (.*?) gehoert", "was ist \\1");
	regex_ireplace(str, "hast du was mal von (.*?) gehoert", "was ist \\1");
	regex_ireplace(str, "^(...*?) hast du ", "du hast \\1 ");
	regex_ireplace(str, "^(...*?) habe ich ", "ich habe \\1 ");

	// cout << "parser2012: step 4.2: " << str << endl;

	if (str.size() > 24 && regex_find(str, "(question|fact)next")) {
		regex_ireplace(str, "^weisst du<ws>[,]*<ws>", "");
		regex_ireplace(str, "^weisst du", "");
	}
	regex_ireplace(str, "^weisst du<ws>[,]*<ws>", "");
	regex_ireplace(str, "^weisst du", "");

	str = " " + str + " ";
	regex_ireplace(str, "\\snoch\\s(nie|nicht)([\\s!.,?]+)", " noch-\\1\\2");
	regex_ireplace(str, "\\snoch([\\s!.,?]+)", "\\1");
	regex_ireplace(str, "\\snoch[-](nie|nicht)([\\s!.,?]+)", " noch \\1\\2");
	regex_ireplace(str, "(^|[\\s!.,?]+)(so)\\setwas([\\s!.,?]+)",
			"\\1_\\2_etwas_\\3");
	regex_ireplace(str, "(^|[\\s!.,?]+)(so)was([\\s!.,?]+)",
			"\\1_\\2_etwas_\\3");

	if (str.size() > 24 && algo::contains(str, "?")) {
		regex_ireplace(str, "\\sgerne([\\s!.,?]+)", "\\1");
	}

	regex_ireplace(str,
			"\\s(kein|keine|keinen|keiner|keinem|nicht)\\skein(|e|en|er|em)\\s",
			" kein\\2 ");
	regex_ireplace(str, "\\s(kein|keine|keinen|keiner|keinem|nicht)\\snicht\\s",
			" \\1 ");
	regex_ireplace(str, "(^|\\s)?k(ein|eine|einen|einer|einem)\\s",
			"\\1nicht \\2 ");
	regex_ireplace(str, "\\sim\\s", " in dem ");
	regex_ireplace(str, "\\sbeim\\s", " bei dem ");
	if (algo::contains(lang, "de")) {
		regex_ireplace(str, "\\sam\\s([a-zA-Z]*?)ten($|\\s|[,])",
				" am_\\1ten{{{adj}}} \\2 ");
		regex_ireplace(str, "\\sam\\s", " an dem ");
		regex_ireplace(str, "\\sins\\s", " in das ");
		regex_ireplace(str, "^im\\s", " in dem ");
		regex_ireplace(str, "^am\\s", " an dem ");
		regex_ireplace(str, "^ins\\s", " in das ");
	}

	if (regex_find(m, str,
			"\\szu[mr]\\s+([a-zA-Z_]+)\\s+([a-zA-Z_]+)(<ws>?[,.?!]*?<ws>?)$")) {
		const string& _match = m[2];
		if (algo::ends_with(_match, "t")) {

			regex_ireplace(str,
					"\\szu([mt])\\s+([a-zA-Z_]+)\\s+([a-zA-Z_]+)(<ws>?[,.?!]*?<ws>?)$",
					" zu\\1_\\l\\2_\\l\\3 \\4");
		}
	}

	regex_ireplace(str, "\\szu([mr])\\s+([a-zA-Z_]+)\\s+([A-Z_][a-zA-Z_]+)",
			" zu\\1_\\l\\2_\\l\\3 ");
	regex_ireplace(str, "\\szu([mr])\\s+([a-zA-Z_]+)", " zu\\1_\\l\\2 ");
	regex_ireplace(str, "^zu([mr])\\s+([a-zA-Z_]+)", " zu\\1_\\l\\2 ");
	regex_ireplace(str, "[,]\\s+[,]", ",");
	regex_ireplace(str, "^wozu\\s", "wie ");
	regex_ireplace(str, "\\swozu\\s", " wie ");
	regex_ireplace(str, "\\soppo\\s", " opposite ");
	regex_ireplace(str, "\\s.?.?opposite.?.?\\s", " opposite ");

	// cout << "parser2012: step 4.3: " << str << endl;

	regex_ireplace(str, "(^|\\s)(ich|du|das) (weiss) ",
			"\\1\\2 \\3\\{\\{\\{v\\}\\}\\} ");
	regex_ireplace(str, " (weiss) (ich|du|das) ",
			" \\1\\{\\{\\{v\\}\\}\\} \\2 ");

	if (regex_find(str, " opposite ")) {
		algo::trim(str);
		regex_replace(str, "\\s+", "_");

		str_replace(str, "_opposite_", "@");
		vector<string> opposites;
		algo::split(opposites, str, algo::is_any_of("@"));
		str = "_" + opposites[0] + "_ opposite _" + opposites[1] + "_";
	}

	regex_replace(str, "\\szu\\s([a-z]+?)\\s([a-z]+?)(.?.?.?)$",
			" \\2 , _to_ \\1 \\3");

	{
		str_replace(str, "KOMMA", ",");
		vector<string> clauses;
		algo::split(clauses, str, algo::is_any_of(","));
		foreach (string clause, clauses) {
			if (regex_find(m, clause, "\\szu\\s([a-z]+?en)\\s")) {
				cout << "found 'zu'." << endl;
				string zu_verb = m[1];

				vector<string> words;
				algo::split(words, clause, algo::is_space());

				bool found_other_verb = 0;
				bool found_zu_verb = 0;
				foreach (string word, words) {
					if (word.size() > 0) {
						if (word != zu_verb) {
							tags* tags = t->get_pos(word);
							if (tags->first == "linking" && found_zu_verb) {
								break;
							}
							if (tags->first == "v") {
								found_other_verb = true;
								cout << "other verb: " << word << endl;
							}
						} else {
							found_zu_verb = true;
						}
					}
				}
				cout << "found an other verb: "
						<< (found_other_verb ? "true" : "false") << endl;

				if (found_other_verb) {
					regex_replace(str, "\\szu\\s([a-z]+?en)\\s",
							" , _to_ \\1 ");
				} else {
					regex_replace(str, "\\szu\\s([a-z]+?en)\\s", " _to_ \\1 ");
				}
			}
		}
	}

	if (regex_find(str, "ist es.*?\\szu\\s")) {
		regex_ireplace(str, " ist es ", " ist ");
	}
	if (regex_find(str,
			"ist\\ses(\\s[A-Za-z]+?)?(\\s[A-Za-z]+?)?\\s(das|der|die)\\s")) {
		regex_ireplace(str, " ist es ", " ist ");
	}

	algo::trim_left_if(str, algo::is_space() || algo::is_any_of(","));

	// cout << "parser2012: step 4.4: " << str << endl;

/////////////////////////////////////////////////////
// Here was the chapter about replacing user defined strings

	/*
	 static
	 if (is an empty global array replace_array) {
	 string cache_file_name = '_cache_replace'

	 if (exists: var cache_file_name, end test) {
	 string cache_str = handle foreach (file name var cache_file_name, read
	 foreach (new var line from var cache_input is rw) {
	 push into global array replace_array, var line
	 }
	 }
	 else {

	 string csv_output = '>>>^0^0^0^0^just_verb^0^0^0^0^0^0'
	 do hal2012_send_signal with "database_request", var csv_output
	 string csv_str = hal2012_fetch_signal with "database_request"
	 set new array csv_input_lines to split with /\\r?\\n/, var csv_input

	 string cache_output = handle foreach (file name var cache_file_name, write
	 foreach (new var line in array csv_input_lines) {
	 do regex with var a: /[+]/ -> "\\\\+" :global:i
	 push into global array replace_array, var line
	 print into var cache_output data var line concat new line
	 }
	 do close with var cache_output
	 do trigger_check_files without arguments
	 }

	 push into global array replace_array, ""
	 }
	 set new array _replace_array to global array replace_array
	 foreach (new var line in array _replace_array) {
	 set new array result to an empty array
	 set new array rawresult to split using /\\^/, var line
	 if (var line) {
	 string a = from array rawresult element [ 2 ]
	 string b = from array rawresult element [ 3 ]
	 if (var a and var b and var a not matches var b) {
	 regex_ireplace(str, "(^|\\s)var a(\\s|$)", "\\1var b\\2");

	 #print "#var a# --> #var b#"
	 #print new line
	 #print var input
	 #print new line
	 }
	 }
	 }*/

// cout << "parser2012: step 4.5: " << str << endl;
	regex_ireplace(str, "kind of ", "kind_of_");
	regex_ireplace(str, " mal n ", " einen ");
	regex_ireplace(str, " mal nen ", " einen ");
	regex_ireplace(str, " n ", " einen ");
	regex_ireplace(str, " nen ", " einen ");
	regex_ireplace(str, " mal [']n ", " einen ");
	regex_ireplace(str, " mal [']nen ", " einen ");
	regex_ireplace(str, " [']n ", " einen ");
	regex_ireplace(str, " [']nen ", " einen ");
	regex_ireplace(str, " mal [`]n ", " einen ");
	regex_ireplace(str, " mal [`]nen ", " einen ");
	regex_ireplace(str, " [`]n ", " einen ");
	regex_ireplace(str, " [`]nen ", " einen ");

	regex_ireplace(str, " .... username .... ", " \\$\\$username\\$\\$ ");
	regex_ireplace(str, " ..... username ..... ", " \\$\\$username\\$\\$ ");
	regex_ireplace(str, " .... unknownproperty .... ",
			" \\$\\$unknownproperty\\$\\$ ");
	regex_ireplace(str, " ..... unknownproperty ..... ",
			" \\$\\$unknownproperty\\$\\$ ");

	if (regex_ifind(str, "ist\\s(\\d+?)")) {
		regex_ireplace(str, "(^|\\s)(\\d+?) ", "\\1_\\2_ ");
	}

	regex_ireplace(str, "(^|\\s)tobias schulz", "\\1_tobias_schulz_");

	if (algo::contains(lang, "de")) {
		regex_ireplace(str, "(^|\\s)im jahre (\\d\\d\\d\\d) ", "\\1\\2 ");
		regex_ireplace(str, "(^|\\s)im jahr (\\d\\d\\d\\d) ", "\\1\\2 ");
		if (regex_find(str, "\\d\\d\\d\\d")) {
			if (!regex_find(str, "\\svon\\s(\\d\\d\\d\\d)\\s")
					and !regex_ifind(str, "ist\\s(\\d\\d\\d\\d)")) {
				regex_ireplace(str, "(^|\\s)(\\d\\d\\d\\d) ",
						"\\1in_jahre_\\2 ");
			}
			if (!regex_find(str, "(\\d\\d\\d\\d)......")) {
				regex_ireplace(str, "(^|\\s)(\\d\\d\\d\\d) ", "\\1_\\2_ ");
			}
		}
	}

	// cout << "parser2012: step 5: " << str << endl;

	if (regex_find(str, "[?]")) {
		regex_ireplace(str, "(^|\\s)?(nicht|not)(\\s)", "\\1");
	}

	regex_ireplace(str,
			"(herr|frau|mr|mrs|miss|doktor|dr|firma)\\.? (\\S\\S\\S+?)($|\\s)",
			"_\\1_\\2_ \\3");

	regex_ireplace(str, "sth\\.", "something");
	regex_ireplace(str, "sth\\s", "something ");
	regex_ireplace(str, "do you know (what|who|where|how|when|which|whose)",
			"\\1");
	regex_ireplace(str, "do you know something about ", "what is ");
	regex_ireplace(str, " do you do", " are you");
	algo::trim(str);
	regex_ireplace(str, "what<ws>up\\s($|[?])", "how are you?");
	regex_ireplace(str, "what[']s<ws>up\\s($|[?])", "how are you?");
	regex_ireplace(str, "whats<ws>up\\s($|[?])", "how are you?");
	regex_ireplace(str, "how are you doing", "how are you");

	regex_ireplace(str, "what\\'s ", "what is ");
	regex_ireplace(str, "whats ", "what is ");
	regex_ireplace(str, "whos ", "what is ");
	regex_ireplace(str, "who\\'s ", "what is ");
	regex_ireplace(str, "whore ", "what is ");
	regex_ireplace(str, "who\\'re ", "what is ");
	regex_ireplace(str, "who are you.*", "what is your name?");

	regex_ireplace(str, "was ist mit (.*?) los", "was ist \\1");
	regex_ireplace(str, "was ist ueber (.*?)", "was ist \\1");
	regex_ireplace(str, "was ist los mit (.*?)", "was ist \\1");

	regex_ireplace(str, "^(.*?) muss man ", "Man muss \\1 ");

	// cout << "parser2012: step 6: " << str << endl;

	regex_ireplace(str, "^weisst du denn noch ", "weisst du ");
	regex_ireplace(str, "^weisst du denn ", "weisst du ");
	regex_ireplace(str, "^weisst du noch ", "weisst du ");
	regex_ireplace(str, "^weisst du (w[^\\s]*?)\\s([^?!.,]*)", "\\2 \\1");
	regex_ireplace(str, "^weisst du ", "");

	regex_ireplace(str, "wie vie[a-zA-Z]+\\s", "wie ");
	regex_ireplace(str, "^hm\\, ", " ");
	regex_ireplace(str, "^hm \\, ", " ");
	regex_ireplace(str, "\\shm\\, ", " ");
	regex_ireplace(str, "\\shm \\, ", " ");

	{
		str_replace(str, "KOMMA", ",");
		vector<string> clauses;
		algo::split(clauses, str, algo::is_any_of(","));
		int clause_no = 0;
		foreach (string clause, clauses) {
			++clause_no;

			if (clause_no > 1
					&& regex_find(m, clause,
							"^\\s*?(der|die|das|den|dem|dessen)\\s([a-z]+?)\\s")) {
				cout << "maybe found a relative clause." << endl;
				const string& rel_verb = m[2];
				vector<string> words;
				algo::split(words, clause, algo::is_space());

				tags* tags = t->get_pos(rel_verb);
				if (tags->first == "v" || words.size() < 3) {
					cout << "not found an relative clause verb: " << rel_verb
							<< endl;
					regex_replace(str,
							"\\s*?(der|die|das|den|dem|dessen)\\s(" + rel_verb
									+ ")\\s", " _\\1_ \\2 ");
				} else {
					cout << "found an relative clause verb: " << rel_verb
							<< endl;
					regex_replace(str,
							"\\s*?(der|die|das|den|dem|dessen)\\s(" + rel_verb
									+ ")\\s",
							" \\1\\{\\{\\{questionword\\}\\}\\} \\2 ");
				}
			}
		}
	}

	put_underscore_names_into_builtin_name(str);

	// cout << "parser2012: step 7: " << str << endl;

	to_unixtime(str);

	const string& mark = "\"";

	regex_ireplace(str,
			"\\s" + mark + "\\s?([A-Za-z0-9_" + mark + "]+?)\\s?" + mark + "",
			" " + mark + "\\1" + mark + "");

	regex_ireplace(str, "in dem jahr ([\\d]+)",
			"in dem " + mark + "jahre \\1" + mark + "");
	regex_ireplace(str, "in dem jahre ([\\d]+)",
			"in dem " + mark + "jahre \\1" + mark + "");

	int e = 50;
	while (e >= 0) {
		regex_ireplace(str,
				"" + mark + "([^\\s" + mark + "]+?)\\s([^" + mark + "]*?)"
						+ mark + "", "" + mark + "\\1_\\2" + mark + "");
		--e;
	}
	regex_ireplace(str, "" + mark + "", "_");

	// cout << "parser2012: step 8: " << str << endl;

	algo::trim(str);
	regex_ireplace(str, "questionnext", "q=>");
	regex_ireplace(str, "factnext", "f=>");
	regex_ireplace(str, "[?]<ws>[=]<ws>[>]", "?=>");
	regex_ireplace(str, "\\s+[?][=][>]", ", ?=>");
	regex_ireplace(str, "[!]<ws>[=]<ws>[>]", "!=>");
	regex_ireplace(str, "\\s+[!][=][>]", ", !=>");
	regex_ireplace(str, "[f]<ws>[=]<ws>[>]", "f=>");
	regex_ireplace(str, "\\s+[f][=][>]", ", f=>");
	regex_ireplace(str, "[q]<ws>[=]<ws>[>]", "q=>");
	regex_ireplace(str, "\\s+[q][=][>]", ", q=>");
	regex_ireplace(str, "[=]\\s+[>]", "=>");
	regex_ireplace(str, "\\s+[=][>]", ", =>");
	regex_ireplace(str, "[,]+", " , ");
	regex_ireplace(str, "\\s+", " ");
	regex_ireplace(str, "[_]+", "_");

	if (regex_find(str, "[?]")) {
		str = lcfirst(str);
	}

	// cout << "parser2012: step 9: " << str << endl;

	{
		ifstream male_hist_file;
		male_hist_file.open((path + "/lang_" + lang + "/male.history").c_str());
		if (i) {
			string line;
			string last_male_substantive;
			while (std::getline(male_hist_file, line)) {
				if (line.size() > 1)
					last_male_substantive = line;
			}
			if (last_male_substantive.size() > 0) {
				replace_he(str, last_male_substantive);
			}
		}
	}
	{
		ifstream female_hist_file;
		female_hist_file.open(
				(path + "/lang_" + lang + "/male.history").c_str());
		if (i) {
			string line;
			string last_female_substantive;
			while (std::getline(female_hist_file, line)) {
				if (line.size() > 1)
					last_female_substantive = line;
			}
			if (last_female_substantive.size() > 0) {
				replace_she(str, last_female_substantive);
			}
		}
	}

	REGEX_DEBUG = false;
}

void parser::extend_input(string& str) {
	// TODO!!!
}

void parser::to_name(string& text) {
	bool article_undef = regex_ifind(text, "(^|\\s)(ein|eine)\\s");
	regex_ireplace(text, "(^|\\s)(ein|eine)\\s", "");
	bool article_def = regex_ifind(text, "(^|\\s)(der|die|das)\\s");
	regex_ireplace(text, "(^|\\s)(der|die|das)\\s", "");

	algo::trim_if(text, algo::is_space() || algo::is_any_of("_"));
	regex_replace(text, "\\s+", "_");

	if (article_undef) {
		text = "(a) " + text;
	} else if (article_def) {
		text = "(the) " + text;
	}

	regex_replace(text, "_(und|and)_", "_ & _");
	regex_replace(text, "_(oder|or)_", "_ | _");
}
void parser::remove_adverbs(string& text, string& adverbs) {
	boost::smatch m;
	if (regex_find(m, text, "^(.*?)\\s(von.*?)$")) {
		text = m[1];
		if (adverbs.size() > 0)
			adverbs += " ";
		adverbs += m[2];
	}
}
void parser::put_underscore_names_into_builtin_name(string&) {
	// TODO
	// learn names
}
void parser::to_unixtime(string&) {
	// TODO
}
void parser::replace_he(string& text, const string& replacement) {
	regex_ireplace(text, "(^|\\s)(er|ihn)(\\s|$)", "\\1" + replacement + "\\3");
	regex_ireplace(text, "(^|\\s)(he|him)(\\s|$)", "\\1" + replacement + "\\3");
}
void parser::replace_she(string& text, const string& replacement) {
	regex_ireplace(text, "(^|\\s)(sie)(\\s|$)", "\\1" + replacement + "\\3");
	regex_ireplace(text, "(^|\\s)(she)(\\s|$)", "\\1" + replacement + "\\3");
}

void parser::set_verbose(bool v) {
	verbose = v;
}
bool parser::is_verbose() const {
	return verbose;
}

void parser::set_buffered(bool v) {
	buffered = v;
}
bool parser::is_buffered() const {
	return buffered;
}

const string print_vector(const vector<sentence*>& v) {
	stringstream ss;
	ss << "[";
	for (vector<sentence*>::const_iterator i = v.begin(); i != v.end(); ++i) {
		if (i != v.begin())
			ss << ", ";
		ss << (*i)->to_str();
	}
	ss << "]";
	return ss.str();
}

}

