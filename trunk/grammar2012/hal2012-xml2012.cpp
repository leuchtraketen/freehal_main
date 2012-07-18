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
		mode(_mode), content(), text(""), name(""), is_cached_words(false), is_cached_tags(
				false) {
}
xml_obj::xml_obj() :
		mode(LIST), content(), text(""), name(""), is_cached_words(false), is_cached_tags(
				false) {
}
xml_obj::~xml_obj() {
	if (mode == LIST) {
		content.clear();
	}
}

boost::shared_ptr<xml_obj> xml_obj::from_text(const string& _text) {
	boost::shared_ptr<xml_obj> obj(new xml_obj(TEXT));
	obj->set_name("");
	obj->set_text(_text);
	return obj;
}

xml_obj_mode xml_obj::get_mode() const {
	return mode;
}

boost::shared_ptr<xml_obj>& operator <<(boost::shared_ptr<xml_obj>& o,
		boost::shared_ptr<xml_obj> i) {
	o->content.push_back(i);
	o->reset_cache();
	return o;
}
boost::shared_ptr<xml_obj>& operator <<(boost::shared_ptr<xml_obj>& o,
		vector<boost::shared_ptr<xml_obj> >& i) {
	foreach (boost::shared_ptr<xml_obj> elem, i) {
		o->content.push_back(elem);
		o->reset_cache();
	}
	return o;
}
const boost::shared_ptr<xml_obj>& operator >>(
		const boost::shared_ptr<xml_obj>& i,
		vector<boost::shared_ptr<xml_obj> >& o) {

	foreach (boost::shared_ptr<xml_obj> elem, i->content) {
		o.push_back(elem);
	}
	return i;
}
xml_obj* operator <<(xml_obj* o, boost::shared_ptr<xml_obj> i) {
	o->content.push_back(i);
	o->reset_cache();
	return o;
}
xml_obj* operator <<(xml_obj* o, vector<boost::shared_ptr<xml_obj> >& i) {
	foreach (boost::shared_ptr<xml_obj> elem, i) {
		o->content.push_back(elem);
		o->reset_cache();
	}
	return o;
}
const xml_obj* operator >>(const xml_obj* i,
		vector<boost::shared_ptr<xml_obj> >& o) {
	int k;
	for (k = 0; k < i->content.size(); ++k) {
		o.push_back(i->content[k]);
	}
	return i;
}
std::ostream& operator<<(std::ostream& stream, const xml_fact& xfact) {
	stream << xfact.print_xml();
	return stream;
}
std::ostream& operator<<(std::ostream& stream, const xml_obj& xobj) {
	stream << xobj.print_xml();
	return stream;
}
std::ostream& operator<<(std::ostream& stream,
		const boost::shared_ptr<xml_fact>& xfact) {
	stream << xfact->print_str();
	return stream;
}
std::ostream& operator<<(std::ostream& stream,
		const boost::shared_ptr<xml_obj>& xobj) {
	stream << xobj->print_str();
	return stream;
}

const std::vector<boost::shared_ptr<xml_obj> >& xml_obj::get_embedded() const {
	return content;
}
std::vector<boost::shared_ptr<xml_obj> >& xml_obj::get_embedded() {
	return content;
}

void xml_obj::set_name(const string& name) {
	this->name = name;
}
string xml_obj::get_name() const {
	return (name);
}

void xml_obj::set_text(const string& str) {
	text = str;
	algo::trim_if(text, algo::is_any_of(" \t\n\r\f\v"));
}
void xml_obj::set_text(const word& w) {
	text = w.get_word();
	cache_words.clear();
	cache_words.push_back(w);
	is_cached_words = true;
	if (w.has_tags())
		is_cached_tags = true;
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
	std::vector<boost::shared_ptr<xml_obj> > _content;
	part(_content, tag_name);

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
		} else if (name == "synonyms") {
			ss << name << ": \"";
			int k;
			for (k = 0; k < content.size(); ++k) {
				if (k > 0)
					ss << "|";
				ss << content[k]->print_text();
			}
			ss << "\"";
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

string xml_obj::print_text() const {
	if (mode == TEXT) {
		return text;
	}
	if (mode == LIST) {
		string delem;
		if (name == "and" || name == "or") {
			delem = " " + name + " ";
		} else {
			delem = " ";
		}

		stringstream ss;
		if (name == "text" && content.size() == 1) {
			ss << content[0]->print_text();
		} else {

			if (content.size() == 1) {
				ss << content[0]->print_text();
			} else if (content.size() > 1) {
				int k;
				for (k = 0; k < content.size(); ++k) {
					if (k > 0)
						ss << delem;
					ss << content[k]->print_text();
				}
			}
		}
		return ss.str();
	}
	return "";
}

int xml_obj::prepare_words() {
	if (is_cached_words)
		return 1;
	cache_words.clear();

	//cout << "prepare_words: " << this->print_str() << endl;

	if (mode == TEXT) {
		vector<string> _words;
		// !(algo::is_digit() || algo::is_alpha())
		algo::split(_words, text,
				!(algo::is_alpha() || algo::is_digit()
						|| algo::is_any_of("}{][=-)(_")),
				algo::token_compress_on);
		foreach (string& text, _words) {
			if (text.size() > 0 && text != "1")
				cache_words.push_back(word(text));
		}
	}
	if (mode == LIST) {
		if (name != "truth") {
			foreach (boost::shared_ptr<xml_obj> embedded, content) {
				embedded->prepare_words();
				embedded->get_words(cache_words);
			}
		}
	}
	is_cached_words = true;

	return 0;
}

int xml_obj::prepare_tags(tagger* _t = 0) {
	if (is_cached_tags)
		return 1;
	prepare_words();

	//cout << "prepare_tags: " << this->print_str() << endl;

	tagger* t = _t;
	if (_t == 0) {
		cout << "Error! prepare_words: no tagger given..." << endl;
		t = new tagger();
	}
	foreach (word& w, cache_words) {
		if (!w.has_tags())
			w.set_tags(t->get_pos(w.get_word()));
	}
	if (mode == LIST) {
		foreach (boost::shared_ptr<xml_obj> embedded, content) {
			embedded->prepare_tags(_t);
		}
	}
	if (_t == 0) {
		delete t;
	} else {
		is_cached_tags = true;
	}

	return 0;
}

int xml_obj::reset_cache() {
	is_cached_words = false;
	is_cached_tags = false;
	return 0;
}

const vector<word>& xml_obj::get_words() const {
	return cache_words;
}

const vector<word>& xml_obj::get_words() {
	if (!is_cached_words)
		this->prepare_words();
	return cache_words;
}

int xml_obj::get_words(vector<word>& words) const {
	words.insert(words.end(), cache_words.begin(), cache_words.end());
	return 0;
}

int xml_obj::get_words(vector<word>& words) {
	if (!is_cached_words)
		this->prepare_words();
	words.insert(words.end(), cache_words.begin(), cache_words.end());
	return 0;
}

size_t xml_obj::size() const {
	return content.size();
}

void xml_obj::trim() {
	if (mode == LIST) {
		boost::shared_ptr<xml_obj> subject(new xml_obj(LIST));
		boost::shared_ptr<xml_obj> object(new xml_obj(LIST));
		boost::shared_ptr<xml_obj> adverbs(new xml_obj(LIST));
		boost::shared_ptr<xml_obj> verb(new xml_obj(LIST));
		vector<boost::shared_ptr<xml_obj> > other;

		foreach (boost::shared_ptr<xml_obj> embedded, content) {
			boost::shared_ptr<xml_obj> unique;
			if (embedded->name == "subject")
				unique = subject;
			else if (embedded->name == "object")
				unique = object;
			else if (embedded->name == "adverbs")
				unique = adverbs;
			else if (embedded->name == "verb")
				unique = verb;
			embedded->trim();

			if (unique) {
				unique->set_name(embedded->name);
				unique << embedded->content;
			} else {
				other.push_back(embedded);
			}
		}

		content.clear();
		if (subject->size() > 0)
			this << subject;
		if (object->size() > 0)
			this << object;
		if (adverbs->size() > 0)
			this << adverbs;
		if (verb->size() > 0)
			this << verb;
		this << other;
	}
}

int xml_obj::toggle(tagger* t) {
	if (t == 0)
		return 1;
	bool delete_cache = false;

	t->read_verbs_file("toggle.csv");

	if (mode == LIST && content.size() > 0) {
		if (name == "synonyms") {
			// foreach (boost::shared_ptr < xml_obj > &subobj, content) {
			string word = content[0]->print_text();
			if (!t->toggle(word)) {
				boost::shared_ptr<xml_obj> text_obj(new xml_obj(LIST));
				text_obj->set_name("text");
				boost::shared_ptr<xml_obj> t(new xml_obj(TEXT));
				t->set_text(word);
				text_obj << t;
				content[0] = text_obj;
				delete_cache = true;
				// content.insert(content.begin(), text_obj); break;
			}
		} else {
			foreach (boost::shared_ptr < xml_obj > &subobj, content) {
				if (subobj->toggle(t) == 0) {
					delete_cache = true;
				}
			}
		}
	}

	if (delete_cache) {
		reset_cache();
		prepare_tags(t);
	}

	return delete_cache ? 0 : 1;
}

std::size_t hash_value(const xml_obj& o) {
	std::size_t seed = 0;
	boost::hash_combine(seed, o.print_text());
	/*if (o.get_mode() == TEXT) {
	 boost::hash_combine(seed, o.print_text());
	 } else {
	 const vector<boost::shared_ptr<xml_obj> >& vec = o.get_embedded();
	 foreach(const boost::shared_ptr<xml_obj>& sub, vec) {
	 hash_value(*sub);
	 }
	 }*/
	//boost::hash_combine(seed, o.print_xml());
	return seed;
}
std::size_t hash_value(const xml_fact& o) {
	return hash_value((const xml_obj&) o);
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
		xml_obj(LIST), line(), filename() {
	name = "fact";
}
xml_fact::~xml_fact() {
	if (content.size() > 0) {
		content.clear();
	}
}

int xml_obj::part(std::vector<boost::shared_ptr<xml_obj> >& _content,
		const string& tag_name) const {
	int j;
	foreach (boost::shared_ptr<xml_obj> o, content) {
		if (o->get_name() == tag_name) {
			_content.push_back(o);
			++j;
		}
	}
	return j;
}

boost::shared_ptr<xml_obj> xml_obj::part(const string& tag_name) const {

	boost::shared_ptr<xml_obj> subtree;
	int count = 0;
	foreach (boost::shared_ptr<xml_obj> o, content) {
		if (o->get_name() == tag_name) {
			++count;
			subtree = o;
		}
	}
	if (count == 1) {
		return subtree;
	} else {
		subtree.reset(new xml_obj(LIST));
		subtree->set_name(tag_name);
		foreach (boost::shared_ptr<xml_obj> o, content) {
			if (o->get_name() == tag_name) {
				subtree << o->content;
			}
		}
		return subtree;
	}
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
				boost::shared_ptr<xml_obj> subtree(new xml_obj(LIST));
				halxml_readtree(subtree.get(), _tag_name, lines, i);
				if (subtree->content.size() > 0) {
					tree << subtree;
				}
			}
		} else if (string(lines[i]).size() > 0) {
			const string& text = lines[i];
			// check for default values
			if (text != "00000" && !algo::starts_with(text, "0.50")) {
				boost::shared_ptr<xml_obj> t(new xml_obj(TEXT));
				t->set_text(
						algo::starts_with(text, "0.0") ? "0" :
						algo::starts_with(text, "1.0") ? "1" : text);
				if (tag_name == "text") {
					tree << t;
				} else {
					boost::shared_ptr<xml_obj> text_obj(new xml_obj(LIST));
					text_obj->set_name("text");
					text_obj << t;
					tree << text_obj;
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

word::word() :
		w(""), tag(0) {
}
word::word(const string _w) :
		w(_w), tag(0) {
}
word::word(const word& _w) :
		w(_w.w), tag(_w.tag) {
}
word::word(const string _w, tags* _tags) :
		w(_w), tag(_tags) {
}
void word::set_word(const string& _w) {
	w = _w;
}
void word::set_tags(tags* _tag) {
	tag = _tag;
}
const string& word::get_word() const {
	return w;
}
tags* word::get_tags() const {
	return tag;
}
bool word::has_tags() const {
	return tag != 0 ? true : false;
}
size_t word::size() const {
	return w.size();
}
bool word::operator ==(const string& _w) const {
	return w == _w;
}
bool word::operator !=(const string& _w) const {
	return w != _w;
}
bool word::operator ==(const word& _w) const {
	return w == _w.w;
}
bool word::operator !=(const word& _w) const {
	return w != _w.w;
}
std::ostream& operator<<(std::ostream& stream, const word& w) {
	stream << w.get_word();
	return stream;
}

const word lc(const word& _data) {
	word data(_data);
	data.set_word(lc(data.get_word()));
	return data;
}
const word uc(const word& _data) {
	word data(_data);
	data.set_word(uc(data.get_word()));
	return data;
}
const word lcfirst(const word& _data) {
	word data(_data);
	data.set_word(lcfirst(data.get_word()));
	return data;
}
const word ucfirst(const word& _data) {
	word data(_data);
	data.set_word(ucfirst(data.get_word()));
	return data;
}

}
