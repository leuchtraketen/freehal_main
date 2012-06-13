/*
 * hal2012-filterfacts2012.cpp
 *
 *  Created on: 10.06.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-xml2012.h"
#include "hal2012-filterfacts2012.h"

namespace grammar2012 {

boost::shared_ptr<filterlist> filterlist::instance;

struct my_is_digit: std::unary_function<char, bool> {
	bool operator()(char c) const {
		return c >= '0' && c <= '9';
	}
};

std::unary_negate<my_is_digit> operator!(const my_is_digit& x) {
	return std::not1(x);
}

all_in::all_in(vector<boost::shared_ptr<xml_obj> >& _content) :
		content(_content) {
}
any_in::any_in(vector<boost::shared_ptr<xml_obj> >& _content) :
		content(_content) {
}

double operator &&(const word& _word1, const word& _word2) {
	const string& word1 = _word1.get_word();
	const string& word2 = _word2.get_word();

	if (word1 == word2 || lc(word1) == lc(word2)) {
		return 1;
	} else {
		return 0;
	}
}

double operator &(word& _word1, word& _word2) {
	if (_word1 && _word2) {
		bool equal = _word1 && _word2;
		tags* tags = 0;
		if (_word1.has_tags())
			tags = _word1.get_tags();
		else if (_word2.has_tags())
			tags = _word2.get_tags();

		double weight = tags == 0 ? 0.75 : tags->first == "n" ? 1 :
						tags->first == "adj" ? 0.5 : tags->first == "v" ? 0.75 :
						tags->first == "art" ? 0.1 : 0.4;

		return weight;
	} else {
		return 0;
	}
}

double operator &&(const word& word1, const boost::shared_ptr<xml_obj>& o2) {
	vector<word> words;
	o2->get_words(words);

	foreach (word& word2, words) {
		double matches = word1 && word2;
		if (matches > 0)
			return matches;
	}
	return 0;
}

double operator &(word& word1, boost::shared_ptr<xml_obj>& o2) {
	vector<word> words;
	o2->get_words(words);

	foreach (word& word2, words) {
		double matches = word1 & word2;
		if (matches > 0)
			return matches * (3.0 * words.size() + 1) / 4.0 / words.size();
	}
	return 0;
}

double operator &&(const boost::shared_ptr<xml_obj>& o1,
		const boost::shared_ptr<xml_obj>& o2) {
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " && " << o2->print_str()
				<< endl;
	double matches = 0;
	if (o1->get_mode() == TEXT && o2->get_mode() == TEXT) {
		vector<word> words;
		o1->get_words(words);
		foreach (word& word, words) {
			matches += word && o2;
		}
	} else if (o1->get_mode() == LIST) {
		std::vector<boost::shared_ptr<xml_obj> > content;
		o1 >> content;
		int count = 0;
		foreach (boost::shared_ptr<xml_obj> subobj, content) {
			double m = subobj && o2;
			if (m > 0) {
				matches += m;
				++count;
			}
		}

		if (o1->get_name() == "link_&")
			matches = (count == content.size() ? matches : 0);
	} else if (o2->get_mode() == LIST) {
		std::vector<boost::shared_ptr<xml_obj> > content;
		o2 >> content;
		int count = 0;
		foreach (boost::shared_ptr<xml_obj> subobj, content) {
			double m = o1 && subobj;
			if (m > 0) {
				matches += m;
				++count;
			}
		}

		if (o2->get_name() == "link_&")
			matches = (count == content.size() ? matches : 0);
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " && " << o2->print_str()
				<< " = " << matches << endl;
	return matches;
}

double operator &(boost::shared_ptr<xml_obj>& o1,
		boost::shared_ptr<xml_obj>& o2) {
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " & " << o2->print_str()
				<< endl;
	double matches = 0;
	if (o1->get_mode() == TEXT && o2->get_mode() == TEXT) {
		vector<word> words;
		o1->get_words(words);
		foreach (word& word, words) {
			matches += word & o2;
		}
	} else if (o1->get_mode() == LIST) {
		std::vector<boost::shared_ptr<xml_obj> > content;
		o1 >> content;
		int count = 0;
		foreach (boost::shared_ptr<xml_obj> subobj, content) {
			double m = subobj & o2;
			if (m > 0) {
				matches += m;
				++count;
			}
		}

		if (o1->get_name() == "link_&")
			matches = (count == content.size() ? matches : 0);
	} else if (o2->get_mode() == LIST) {
		std::vector<boost::shared_ptr<xml_obj> > content;
		o2 >> content;
		int count = 0;
		foreach (boost::shared_ptr<xml_obj> subobj, content) {
			double m = o1 & subobj;
			if (m > 0) {
				matches += m;
				++count;
			}
		}

		if (o2->get_name() == "link_&")
			matches = (count == content.size() ? matches : 0);
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " & " << o2->print_str()
				<< " = " << matches << endl;
	return matches;
}
double operator &(boost::shared_ptr<xml_obj>& o1,
		vector<boost::shared_ptr<xml_obj> >& v2) {
	double matches = 0;
	foreach (boost::shared_ptr<xml_obj> o2, v2) {
		matches += o1 & o2;
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " & " << v2.data()
				<< " = " << matches << endl;
	return matches;
}
double operator &(vector<boost::shared_ptr<xml_obj> >& v1,
		vector<boost::shared_ptr<xml_obj> >& v2) {
	double matches = 0;
	foreach (boost::shared_ptr<xml_obj> o1, v1) {
		matches += o1 & v2;
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << v1.data() << " & " << v2.data() << " = "
				<< matches << endl;
	return matches;
}
double count(boost::shared_ptr<xml_obj> o) {
	if (o->get_mode() == TEXT) {
		vector<word> words;
		o->get_words(words);
		return (double) words.size();

	} else if (o->get_name() == "questionword" || o->get_name() == "adverbs"
			|| o->get_name() == "extra" || o->get_name() == "truth"
			|| o->get_name() == "clause") {
		return 0;

	} else {
		std::vector<boost::shared_ptr<xml_obj> > content;
		o >> content;
		double c = 0;
		foreach (boost::shared_ptr<xml_obj> subobj, content) {
			c += count(subobj);
		}
		if (o->get_name() == "link_|")
			c /= content.size();
		return c;
	}
}

double operator %(boost::shared_ptr<xml_obj> o1,
		boost::shared_ptr<xml_obj> o2) {
	double matches = 0;

	vector<boost::shared_ptr<xml_obj> > verbs1;
	o1->part(verbs1, "verb");
	vector<boost::shared_ptr<xml_obj> > verbs2;
	o2->part(verbs2, "verb");

	double verbs_match = verbs1 & verbs2;
	if (verbs_match == 0)
		return matches;

	vector<boost::shared_ptr<xml_obj> > subject1;
	o1->part(subject1, "subject");
	vector<boost::shared_ptr<xml_obj> > subject2;
	o2->part(subject2, "subject");
	vector<boost::shared_ptr<xml_obj> > object1;
	o1->part(object1, "object");
	vector<boost::shared_ptr<xml_obj> > object2;
	o2->part(object2, "object");
	vector<boost::shared_ptr<xml_obj> > adverbs1;
	o1->part(adverbs1, "adverbs");
	vector<boost::shared_ptr<xml_obj> > adverbs2;
	o2->part(adverbs2, "adverbs");

	double subject_match = (subject1 & subject2) + (subject1 & object2)
			+ (subject1 & adverbs2);
	matches += subject_match;

	double object_match = (object1 & subject2) + (object1 & object2)
			+ (object1 & adverbs2);
	matches += object_match;

	double adverbs_match = (adverbs1 & subject2) + (adverbs1 & object2)
			+ (adverbs1 & adverbs2);
	matches += adverbs_match;

	boost::shared_ptr<filterlist> filters = filterlist::get();
	matches = (*filters)(make_pair(o1, o2), matches);

	return matches;
}
double operator %(boost::shared_ptr<xml_fact> o1,
		boost::shared_ptr<xml_fact> o2) {
	return boost::dynamic_pointer_cast<xml_obj>(o1)
			% boost::dynamic_pointer_cast<xml_obj>(o2);
}
double operator /(boost::shared_ptr<xml_obj> o1,
		boost::shared_ptr<xml_obj> o2) {
	double matches = o1 % o2;
	double avgcount = (9 * count(o1) + count(o2)) / 10.0;
	if (avgcount > 0)
		matches /= avgcount;
	return matches;
}
double operator /(boost::shared_ptr<xml_fact> o1,
		boost::shared_ptr<xml_fact> o2) {
	return boost::dynamic_pointer_cast<xml_obj>(o1)
			/ boost::dynamic_pointer_cast<xml_obj>(o2);
}

double filterrule::operator()(
		pair<boost::shared_ptr<xml_obj>, boost::shared_ptr<xml_obj> > p,
		double _m) {
	m = _m;
	a = p.first;
	b = p.second;
	rule();
	return m;
}

filterrule::~filterrule() {
}

void filterlist::rule() {

	for (int i = 0; i < rules.size(); ++i) {
		filterrule* func = rules[i].get();
		m = (*func)(make_pair(a, b), m);
	}
}

void filterlist::add(boost::shared_ptr<filterrule> rule) {
	rules.push_back(rule);
}
void filterlist::add(filterrule* rule) {
	rules.push_back(boost::shared_ptr<filterrule>(rule));
}

boost::shared_ptr<filterlist> filterlist::get() {
	if (!instance)
		instance.reset(new filterlist());
	return instance;
}

filterlist::filterlist() :
		verbose(true), buffered(false) {
	add(new filter_no_names());
}

void filterlist::set_verbose(bool v) {
	get()->verbose = v;
}
bool filterlist::is_verbose() {
	return get()->verbose;
}

void filterlist::set_buffered(bool v) {
	get()->buffered = v;
}
bool filterlist::is_buffered() {
	return get()->buffered;
}

void filter_no_names::rule() {

	if (filterlist::is_verbose()) {
		cout << "filter_no_names: (verb && is-a) == 1, ="
				<< (b->part("verb") && xml_obj::from_text("is-a")) << endl;
		cout << "filter_no_names: (object && (a) name) == 2, = "
				<< (b->part("object") && xml_obj::from_text("(a) name"))
				<< endl;
	}

	if ((b->part("verb") && xml_obj::from_text("is-a")) == 1) {
		if ((b->part("object") && xml_obj::from_text("(a) name")) == 2) {
			m *= 0.1;
		}
	}
}

}

