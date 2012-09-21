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

#include "hal2012-consequence2012.h"
#include "hal2012-database2012.h"
#include "hal2012-filterfacts2012.h"

namespace grammar2012 {

consequence::consequence() :
		valid(false), cause(), effect() {
}
consequence::consequence(const consequence& c) :
		valid(c.valid), cause(c.cause), effect(c.effect) {

}
consequence::consequence(boost::shared_ptr<xml_fact> f) :
		valid(false), cause(), effect() {
	assign(f);
}
consequence::consequence(boost::shared_ptr<xml_obj> o) :
		valid(false), cause(), effect() {
	assign(o);
}

consequence* consequence::assign(boost::shared_ptr<xml_fact> f) {
	assign(boost::dynamic_pointer_cast<xml_obj>(f));
	return this;
}
consequence* consequence::assign(boost::shared_ptr<xml_obj> o) {
	assign(*o);
	return this;
}
consequence* consequence::assign(xml_obj& o) {
	effect.reset(new xml_obj(LIST));
	effect->set_name("effect");
	cause.reset(new xml_obj(LIST));
	cause->set_name("cause");

	vector<boost::shared_ptr<xml_obj> > embed = o.get_embedded();
	foreach (boost::shared_ptr<xml_obj> obj, embed) {
		if (obj->get_name() == "clause") {
			if ((obj->part("questionword") && consequence::get_if_words_xmlobj())
					>= 1) {
				cause << obj->get_embedded();
			} else {
				effect << obj;
			}
		} else {
			effect << obj;
		}
	}

	valid = cause && effect;

	return this;
}

bool consequence::match(boost::shared_ptr<consequence> pattern,
		boost::shared_ptr<xml_fact> f) {
	return match(pattern, boost::dynamic_pointer_cast<xml_obj>(f));
}
bool consequence::match(boost::shared_ptr<consequence> pattern,
		boost::shared_ptr<xml_obj> o) {

	boost::shared_ptr<xml_obj> subject1(pattern->cause->part("subject"));
	boost::shared_ptr<xml_obj> subject2(o->part("subject"));
	boost::shared_ptr<xml_obj> object1(pattern->cause->part("object"));
	boost::shared_ptr<xml_obj> object2(o->part("object"));
	boost::shared_ptr<xml_obj> adverbs1(pattern->cause->part("adverbs"));
	boost::shared_ptr<xml_obj> adverbs2(o->part("adverbs"));

	if ((subject1 && subject2) && (object1 && object2)
			&& (adverbs1 && adverbs2)) {
		return true;
	}
	return false;
}

bool consequence::is_valid() const {
	return valid;
}

const vector<word> consequence::get_if_words_list() {
	static vector<word> words = vector<word>();
	if (words.size() == 0) {
		words.push_back(word("if"));
		words.push_back(word("when"));
		words.push_back(word("wenn"));
		words.push_back(word("falls"));
	}
	return words;
}
boost::shared_ptr<xml_obj> consequence::get_if_words_xmlobj() {
	const vector<word> words = get_if_words_list();
	static boost::shared_ptr<xml_obj> if_words_obj =
			boost::shared_ptr<xml_obj>();
	if (!if_words_obj) {
		if_words_obj.reset(new xml_obj(LIST));
		if_words_obj->set_name("synonyms");
		foreach(const word & syn, words) {
			boost::shared_ptr<xml_obj> text_obj(new xml_obj(LIST));
			text_obj->set_name("text");
			boost::shared_ptr<xml_obj> t(new xml_obj(TEXT));
			t->set_text(syn.get_word());
			text_obj << t;
			if_words_obj << text_obj;
		}
	}
	return if_words_obj;
}

string consequence::print_str() const {
	stringstream ss;
	ss << "cause: " << cause->print_str() << endl;
	ss << "effect: " << effect->print_str() << endl;
	return ss.str();
}
string consequence::print_xml() const {
	stringstream ss;
	ss << "<consequence>" << endl;
	ss << cause->print_xml() << effect->print_xml();
	ss << "</consequence>" << endl;
	return ss.str();
}

}
