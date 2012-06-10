/*
 * This file is part of FreeHAL 2012.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Tobias Schulz and contributors.
 * http://www.freehal.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "hal2012-xml2012.h"
#include "hal2012-database2012.h"

namespace grammar2012 {

xml_obj::xml_obj(xml_obj_mode _mode) :
		mode(_mode), content(), text(""), name("") {
	//cout << "construct: " << "xml_obj" << endl;
}
xml_obj::xml_obj() :
		mode(LIST), content(), text(""), name("") {
	//cout << "construct: " << "xml_obj" << endl;
}
xml_obj::~xml_obj() {
	//cout << "destruct:  " << "xml_obj" << endl;
	if (mode == LIST) {
		int k;
		for (k = 0; k < content.size(); ++k) {
			delete (content[k]);
		}
		content.clear();
	}
}

xml_obj_mode xml_obj::get_mode() {
	return mode;
}

xml_obj* operator <<(xml_obj* o, xml_obj& i) {
	o->content.push_back(&i);
	return o;
}
xml_obj* operator <<(xml_obj* o, vector<xml_obj*> i) {
	int k;
	for (k = 0; k < i.size(); ++k) {
		o->content.push_back(i[k]);
	}
	return o;
}
xml_obj* operator >>(xml_obj* i, vector<xml_obj*> o) {
	int k;
	for (k = 0; k < i->content.size(); ++k) {
		o.push_back(i->content[k]);
	}
	return i;
}
void xml_obj::set_name(string name) {
	this->name = name;
}
string xml_obj::get_name() {
	return (name);
}

void trim(std::string& s, const char* t = " \t\n\r\f\v") {
	s.erase(0, s.find_first_not_of(t));
	s.erase(s.find_last_not_of(t) + 1);
}
void xml_obj::set_text(string str) {
	text = str;
	trim(text);
}
string xml_obj::print_xml() const {
	return print_xml(0, 0) + "\n";
}
string xml_obj::print_xml(int level, int secondlevel) const {
	if (mode == TEXT) {
		return (text);
	}
	if (mode == LIST) {
		if (name == "clause") {
			++level;
			secondlevel = 0;
		}

		string str;
		int r;
		str += "<" + name + ">" + (secondlevel == 0 ? "\n" : "");
		int k;
		for (k = 0; k < content.size(); ++k) {
			if (secondlevel == 0)
				for (r = 0; r < level + 1; ++r)
					str += "  ";
			str += content[k]->print_xml(level, secondlevel + 1)
					+ (secondlevel == 0 ? "\n" : "");
		}
		if (secondlevel == 0)
			for (r = 0; r < level; ++r)
				str += "  ";
		str += "</" + name + ">";
		return (str);
	}
	return (string());
}
string xml_obj::print_str(string tag_name) const {
	std::vector<xml_obj*> _content = part(tag_name);

	string str;
	int k;
	for (k = 0; k < _content.size(); ++k) {
		if (k > 0)
			str += " ";
		str += _content[k]->print_str();
	}
	return (str);
}
string xml_obj::print_str() const {
	if (mode == TEXT) {
		return "\"" + text + "\"";
	}
	if (mode == LIST) {
		string delem;
		if (name == "and" || name == "or") {
			delem = " " + name;
		} else {
			delem = ",";
		}

		stringstream ss;
		if (name == "text" && content.size() == 1) {
			ss << name << ":";
			ss << content[0]->print_str();
		} else {
			ss << "'" << name << "':{";

			if (content.size() == 1) {
				ss << content[0]->print_str();
			} else if (content.size() > 1) {
				int k;
				for (k = 0; k < content.size(); ++k) {
					if (k > 0)
						ss << delem;
					ss << " ";
					ss << content[k]->print_str();
				}
				ss << " ";
			}
			ss << "}";
		}
		return ss.str();
	}
	return "";
}

int xml_obj::get_words(vector<string>& words) const {
	if (mode == TEXT) {
		vector<string> _words;
		// !(algo::is_digit() || algo::is_alpha())
		algo::split(_words, text, !algo::is_alpha(), algo::token_compress_on);
		if (_words.size() > 0)
			copy(_words.begin(), _words.end(), back_inserter(words));
		return 0;
	}
	if (mode == LIST) {
		int k;
		foreach (xml_obj* embedded, content) {
			embedded->get_words(words);
		}
	}
	return 0;
}

std::size_t hash_value(const xml_obj& o) {
	std::size_t seed = 0;
	boost::hash_combine(seed, o.print_xml());
	return seed;
}
std::size_t hash_value(const xml_fact& o) {
	std::size_t seed = 0;
	boost::hash_combine(seed, o.print_xml());
	return seed;
}
bool operator==(const xml_obj& o1, const xml_obj& o2) {
	return o1.print_xml() == o2.print_xml();
}
bool operator==(const xml_fact& o1, const xml_fact& o2) {
	if (!o1.filename.empty() > 0 && !o2.filename.empty() > 0)
		return o1.filename == o2.filename && o1.line == o2.line;
	return o1.print_xml() == o2.print_xml();
}

xml_fact::xml_fact() :
		xml_obj(), line(), filename() {
	name = "fact";
	//cout << "construct: " << "fact" << endl;
}
xml_fact::~xml_fact() {
	if (content.size() > 0) {
		int k;
		for (k = 0; k < content.size(); ++k) {
			delete (content[k]);
		}
		content.clear();
	}
	//cout << "destruct:  " << "fact" << endl;
	/*int k;
	 if (content) {
	 for (k = 0; k < content.size(); ++k) {
	 delete (content[k]);
	 }
	 delete (content);
	 }*/
}

std::vector<xml_obj*> xml_obj::part(string tag_name) const {
	std::vector<xml_obj*> _content;
	int k;
	for (k = 0; k < content.size(); ++k) {
		if (content[k]->get_name() == tag_name) {
			_content.push_back(content[k]);
		}
	}
	return _content;
}

string halxml_readfile(const fs::path& infile) {
	fs::ifstream indata;
	indata.open(infile);
	if (!indata) {
		return string();
	}
	std::string str;
	indata.seekg(0, std::ios::end);
	str.reserve(indata.tellg());
	indata.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(indata)),
			std::istreambuf_iterator<char>());
	indata.close();
	return str;
}

int halxml_ordertags(string& indata, string& predata) {
	int k = 0;
	char num = 0;
	char _num = 0;
	int is_space = 0;
	int indata_size = indata.size();
	while (k < indata_size) {
		_num = num;
		num = indata[k++];
		if (num == '\n' || num == '\r' || num == '\t' || num == ' ') {
			if (!is_space && _num != '>')
				predata += " ";
			is_space = 1;
			continue;
		}
		is_space = 0;
		//cout << num << endl;

		if (num == '>') {
			predata += "\n";
		} else if (_num == '<') {
			if (num == '/') {
				predata += ">\n";
			} else {
				predata += "<\n";
				predata.append(1, num);
			}
		} else if (num == '<') {
			if (_num != '>') {
				predata += "\n";
			}
		} else {
			predata.append(1, num);
		}
	}
	return 0;
}

template<typename Tree>
Tree* halxml_readtree(Tree* tree, const string& tag_name, vector<string>& lines,
		int& i) {
	tree->set_name(tag_name);

	while (i < lines.size()) {
		if (lines[i] == ">") {
			++i;
			if (lines[i] == tag_name) {
				break;
			}
		} else if (lines[i] == "<") {
			++i;
			string _tag_name = lines[i];
			++i;
			{
				xml_obj* subtree = new xml_obj(LIST);
				halxml_readtree(subtree, _tag_name, lines, i);
				if (subtree->content.size() > 0) {
					tree << *subtree;
				} else {
					delete subtree;
				}
			}
		} else if (string(lines[i]).size() > 0) {
			const string& text = lines[i];
			// check for default values
			if (text != "00000" && !algo::starts_with(text, "0.50")) {
				xml_obj* t = new xml_obj(TEXT);
				t->set_text(text);
				if (tag_name == "text") {
					tree << *t;
				} else {
					xml_obj* text_obj = new xml_obj(LIST);
					text_obj->set_name("text");
					text_obj << *t;
					tree << *text_obj;
				}
			}
		}
		++i;
	}
	return (tree);
}

xml_fact* halxml_readxml_fact(vector<string>& lines, int& i) {
	xml_fact* fact = new xml_fact();
	halxml_readtree(fact, "fact", lines, i);
	return (fact);
}

xml_fact* record_to_xml_fact(void* r, int level = 0) {
	xml_fact* fact = new xml_fact();

	xml_obj* tree = 0;
	xml_obj* t = 0;

	tree = new xml_obj(LIST);
	tree->set_name("subject");
	t = new xml_obj(TEXT);
	//t->set_text(r->subjects);
	tree << *t;
	fact << *tree;

	tree = new xml_obj(LIST);
	tree->set_name("object");
	t = new xml_obj(TEXT);
	//t->set_text(r->objects);
	tree << *t;
	fact << *tree;

	tree = new xml_obj(LIST);
	tree->set_name("verb");
	t = new xml_obj(TEXT);
	//t->set_text(r->verb);
	tree << *t;
	fact << *tree;

	tree = new xml_obj(LIST);
	tree->set_name("adverbs");
	t = new xml_obj(TEXT);
	//t->set_text(r->adverbs);
	tree << *t;
	fact << *tree;

	tree = new xml_obj(LIST);
	tree->set_name("extra");
	t = new xml_obj(TEXT);
	//t->set_text(r->extra);
	tree << *t;
	fact << *tree;

	tree = new xml_obj(LIST);
	tree->set_name("questionword");
	t = new xml_obj(TEXT);
	//t->set_text(r->questionword);
	tree << *t;
	fact << *tree;

	tree = new xml_obj(LIST);
	tree->set_name("truth");
	t = new xml_obj(TEXT);
	//char truth_str[10];
	//snprintf(truth_str, 9, "%f", r->truth);
	//t->set_text(truth_str);
	tree << *t;
	fact << *tree;

	tree = new xml_obj(LIST);
	tree->set_name("flags");
	t = new xml_obj(TEXT);
	//char flags_str[10];
	//snprintf(flags_str, 9, "%d%d%d%d%d", r->verb_flag_want, r->verb_flag_must, r->verb_flag_can, r->verb_flag_may, r->verb_flag_should);
	//t->set_text(flags_str);
	tree << *t;
	fact << *tree;

	/*
	 if (level == 0) {
	 int n;
	 int broken = 0;
	 for (n = 0; n < r->num_clauses && n+1 < MAX_CLAUSES && r->clauses && r->clauses[n]; ++n) {
	 struct RECORD* clause = (struct RECORD*)(r->clauses[n]);

	 if (( (!clause->subjects||0==strcmp(clause->subjects, "")) && (!clause->objects||0==strcmp(clause->objects, "")) && (!clause->verb||0==strcmp(clause->verb, "")) && !(clause->verb_flag_want || clause->verb_flag_must || clause->verb_flag_can || clause->verb_flag_may || clause->verb_flag_should)) || (clause->questionword && clause->questionword[0] == ')')) {
	 break;
	 }

	 xml_fact* clause_xml_fact = record_to_xml_fact(clause, level+1);
	 clause_xml_fact->set_name("clause");
	 xml_fact << *clause_xml_fact;
	 }
	 }
	 */

	//xml_fact->filename = r->filename;
	//xml_fact->line = r->line;
	return fact;
}


/*
 int add_xml_fact(xml_fact* xfact) {
 int only_logic = 0;
 int has_conditional_questionword = 0;

 string subject = xfact->print_str("subject");
 string object = xfact->print_str("object");
 string verb = xfact->print_str("verb");
 string adverbs = xfact->print_str("adverbs");
 string extra = xfact->print_str("extra");
 string questionword = xfact->print_str("questionword");
 string filename = xfact->filename;
 string line = xfact->line;
 string truth_str = xfact->print_str("truth");

 float truth;
 sscanf(truth_str.c_str(), "%fl", &truth);
 stringstream sst;
 sst << truth_str;
 sst >> truth;

 string verb_flags = xfact->print_str("flags");
 int verb_flag_want = 0, verb_flag_must = 0, verb_flag_can = 0,
 verb_flag_may = 0, verb_flag_should = 0;
 if (verb_flags.size() == 5)
 sscanf(verb_flags.c_str(), "%d%d%d%d%d", &verb_flag_want,
 &verb_flag_must, &verb_flag_can, &verb_flag_may,
 &verb_flag_should);

 struct xml_fact* xml_fact = add_xml_fact(subject.c_str(), object.c_str(), verb.c_str(),
 adverbs.c_str(), extra.c_str(), questionword.c_str(),
 filename.c_str(), line.c_str(), truth, verb_flag_want,
 verb_flag_must, verb_flag_can, verb_flag_may, verb_flag_should,
 only_logic, has_conditional_questionword);

 if (xml_fact && xml_fact->pk) {
 FILE* input_key = fopen("_input_key", "w+b");
 if (input_key) {
 fprintf(input_key, "%d", xml_fact->pk);
 fclose(input_key);
 }
 }

 if (xml_fact && xml_fact != INVALID_POINTER && xml_fact->pk) {
 std::vector<xml_obj*> _content = xfact->part("clause");
 string str;
 int k;
 for (k = 0; k < _content.size(); ++k) {
 xml_fact* xclause = (xml_fact*) _content[k];

 string clause_subject = xclause->print_str("subject");
 string clause_object = xclause->print_str("object");
 string clause_verb = xclause->print_str("verb");
 string clause_adverbs = xclause->print_str("adverbs");
 string clause_extra = xclause->print_str("extra");
 string clause_questionword = xclause->print_str("questionword");
 string clause_truth_str = xclause->print_str("truth");
 float clause_truth;
 sscanf(clause_truth_str.c_str(), "%fl", &clause_truth);
 string clause_verb_flags = xclause->print_str("flags");
 int clause_verb_flag_want = 0, clause_verb_flag_must = 0,
 clause_verb_flag_can = 0, clause_verb_flag_may = 0,
 clause_verb_flag_should = 0;
 if (clause_verb_flags.size() == 5)
 sscanf(clause_verb_flags.c_str(), "%d%d%d%d%d",
 &clause_verb_flag_want, &clause_verb_flag_must,
 &clause_verb_flag_can, &clause_verb_flag_may,
 &clause_verb_flag_should);

 struct xml_fact* clause = add_clause(xml_fact->pk, clause_subject.c_str(),
 clause_object.c_str(), clause_verb.c_str(),
 clause_adverbs.c_str(), clause_extra.c_str(),
 clause_questionword.c_str(), filename.c_str(), line.c_str(),
 clause_truth, clause_verb_flag_want, clause_verb_flag_must,
 clause_verb_flag_can, clause_verb_flag_may,
 clause_verb_flag_should);

 if (clause && clause != INVALID_POINTER) {
 free(clause);
 }
 }

 if (is_engine("disk")) {
 free(xml_fact);
 }
 }

 return 0;
 }
 */

}
