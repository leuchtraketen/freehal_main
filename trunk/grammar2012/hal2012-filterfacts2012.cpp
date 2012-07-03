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

bool is_index_word::operator()(const word& word) const {
	tags* tags = word.get_tags();
	if (tags == 0) {
		// cout << "tags == 0" << endl;
		return word != "a" && word != "the" && word != "in" && word != "verb"
				&& word != "name" && word != "von" && word != "der"
				&& word != "die" && word != "das" && word != "ein"
				&& word != "eine";
	} else {
		cout << "tags != 0 => " << word.get_word() << ": " << tags->first
				<< endl;
		return tags->first == "n" || tags->first == "adj";
	}
}

bool is_synonym::operator()(const boost::shared_ptr<xml_obj>& xfact_p) const {
	double count_subject = count_words(xfact_p->part("subject"));
	double count_object = count_words(xfact_p->part("object"));
	return ((xfact_p->part("verb") && xml_obj::from_text("=")) == 1
			&& count_subject > 0 && count_object > 0
			&& ((xfact_p->part("subject") && xml_obj::from_text("(a)")) == 0
					|| count_subject == 1)
			&& ((xfact_p->part("object") && xml_obj::from_text("(a)")) == 0
					|| count_object == 1));
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

double operator &(const word& _word1, const word& _word2) {
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
	const vector<word>& words = o2->get_words();

	foreach (const word& word2, words) {
		double matches = word1 && word2;
		if (matches > 0)
			return matches;
	}
	return 0;
}

double operator &(const word& word1, const boost::shared_ptr<xml_obj>& o2) {
	const vector<word>& words = o2->get_words();

	foreach (const word& word2, words) {
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
		const vector<word>& words = o1->get_words();
		foreach (const word& word, words) {
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
		if (o1->get_name() == "synonyms")
			matches /= count;
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
		if (o1->get_name() == "synonyms")
			matches /= count;
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " && " << o2->print_str()
				<< " = " << matches << endl;
	return matches;
}

double operator &(const boost::shared_ptr<xml_obj>& o1,
		const boost::shared_ptr<xml_obj>& o2) {
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " & " << o2->print_str()
				<< endl;
	double matches = 0;
	if (o1->get_mode() == TEXT && o2->get_mode() == TEXT) {
		const vector<word>& words = o1->get_words();
		foreach (const word& word, words) {
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
		if (o1->get_name() == "synonyms")
			matches /= count;
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
		if (o1->get_name() == "synonyms")
			matches /= count;
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " & " << o2->print_str()
				<< " = " << matches << endl;
	return matches;
}
double operator &(const boost::shared_ptr<xml_obj>& o1,
		const vector<boost::shared_ptr<xml_obj> >& v2) {
	double matches = 0;
	foreach (boost::shared_ptr<xml_obj> o2, v2) {
		matches += o1 & o2;
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << o1->print_str() << " & " << v2.data()
				<< " = " << matches << endl;
	return matches;
}
double operator &(const vector<boost::shared_ptr<xml_obj> >& v1,
		const vector<boost::shared_ptr<xml_obj> >& v2) {
	double matches = 0;
	foreach (boost::shared_ptr<xml_obj> o1, v1) {
		matches += o1 & v2;
	}
	if (filterlist::is_verbose())
		cout << "---- compare: " << v1.data() << " & " << v2.data() << " = "
				<< matches << endl;
	return matches;
}
double count_words(boost::shared_ptr<xml_obj> o) {
	if (o->get_mode() == TEXT) {
		const vector<word>& words = o->get_words();
		return (double) words.size();

	} else {
		std::vector<boost::shared_ptr<xml_obj> > content;
		o >> content;
		double c = 0;
		foreach (boost::shared_ptr<xml_obj> subobj, content) {
			if (subobj->get_name() != "questionword"
					//	&& subobj->get_name() != "adverbs"
					&& subobj->get_name() != "extra"
					&& subobj->get_name() != "truth"
					&& subobj->get_name() != "clause") {
				c += count_words(subobj);
			}
		}
		if (o->get_name() == "link_|" || o->get_name() == "synonyms")
			c /= content.size();
		return c;
	}
}

double count_tags(boost::shared_ptr<xml_obj> o, const tags tag) {
	//if (o->get_mode() == TEXT) {
	const vector<word>& words = o->get_words();
	int amount = 0;
	foreach (const word& w, words) {
		cout << "freehal|| w.has_tags() = " << w.has_tags() << endl;
		if (w.has_tags()) {
			cout << "freehal|| tag.first.size() > 0 = "
					<< (tag.first.size() > 0) << endl;
			cout << "freehal|| tag.first == w.get_tags()->first = "
					<< (tag.first == w.get_tags()->first) << endl;
			if (tag.first.size() > 0 && tag.first == w.get_tags()->first) {
				if (tag.second.size() == 0)
					++amount;
				else if (tag.first == w.get_tags()->first)
					++amount;
			}
		}
	}
	if (algo::contains(o->print_xml(), "freehal"))
		cout << "count_tags(): o->get_name() = " << o->get_name()
				<< ", o->print_str() = " << o->print_str() << " amount = "
				<< amount << endl;
	return (double) amount;
	/*
	 } else {
	 std::vector<boost::shared_ptr<xml_obj> > content;
	 o >> content;
	 double c = 0;
	 foreach (boost::shared_ptr<xml_obj> subobj, content) {
	 if (subobj->get_name() != "questionword"
	 //	&& subobj->get_name() != "adverbs"
	 && subobj->get_name() != "extra"
	 && subobj->get_name() != "truth"
	 && subobj->get_name() != "clause") {
	 c += count_tags(subobj, tag);
	 }
	 }
	 if (o->get_name() == "link_|" || o->get_name() == "synonyms")
	 c /= content.size();
	 return c;
	 }*/
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

	if (filterlist::is_verbose())
		cout << "matches (before filterlist): " << matches << endl;

	boost::shared_ptr<filterlist> filters = filterlist::get();
	matches = (*filters)(make_pair(o1, o2), matches);

	if (filterlist::is_verbose())
		cout << "matches (after filterlist): " << matches << endl;

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
	//double avgcount = 0; // sqrt((9 * count(o1) + count(o2)) / 10.0);
	// cout << "count(o1): " << count(o1) << endl;
	// cout << "count(o2): " << count(o2) << endl;
	//if (avgcount > 0)
	//	matches /= avgcount;
	// cout << "matches: " << matches << endl;
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

filterlist::filterlist() :
		verbose(true), buffered(false) {
	add(new filter_no_names());
	add(new filter_not());
	add(new filter_question_who());
	add(new filter_question_what());
	add(new filter_question_extra());
}

void filter_no_names::rule() {
	if (filterlist::is_verbose())
		cout << "matches (filter: no_names): " << m << endl;

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

void filter_not::rule() {
	if (filterlist::is_verbose())
		cout << "matches (filter: not): " << m << endl;

	//if ((a->part("questionword") && xml_obj::from_text("who")) == 1) {
	if ((b->part("adverbs") && xml_obj::from_text("nicht")) >= 1) {
		m *= 0.25;
	}
	if ((b->part("truth") && xml_obj::from_text("0")) >= 1) {
		m *= 0.1;
	}
	//}
}

void filter_question_who::rule() {
	if (filterlist::is_verbose())
		cout << "matches (filter: question_who): " << m << endl;
	//cout << "  (a->part(questionword) && xml_obj::from_text(who)) == 1, = "
	//		<< (a->part("questionword") && xml_obj::from_text("who")) << endl;

	if ((a->part("questionword") && xml_obj::from_text("who")) == 1) {
		if (count_words(b->part("clause")) > 0) {
			m *= 0.75;
		}
		if (count_words(b->part("adverbs")) > 0) {
			m *= 0.3;
		}
		if (count_words(b->part("object")) > 0) {
			if ((b->part("object") && xml_obj::from_text("(a)")) > 0) {
				m *= 0.5;
			} else if (count_tags(b->part("object"), tags("n", "")) == 0) {
				m *= 0.3;
			}
		} else {
			m *= 0.3;
		}
	}
}

void filter_question_what::rule() {
	if (filterlist::is_verbose())
		cout << "matches (filter: question_what): " << m << endl;
	if ((a->part("questionword") && xml_obj::from_text("what")) == 1) {
		if (count_words(b->part("clause")) > 0) {
			m *= 0.9;
		}
		if (count_words(b->part("object")) > 0) {
			if ((b->part("object") && xml_obj::from_text("(a)")) > 0) {
				m *= 2;
			} else {
				m *= 0.7;
			}
		} else {
			m *= 0.3;
		}
	}
}

void filter_question_extra::rule() {
	if (filterlist::is_verbose())
		cout << "matches (filter: question_extra): " << m << endl;

	if (count_words(a->part("extra")) > 0) {
		vector<boost::shared_ptr<xml_obj> > extra1;
		a->part(extra1, "extra");
		vector<boost::shared_ptr<xml_obj> > subject2;
		b->part(subject2, "subject");
		vector<boost::shared_ptr<xml_obj> > object2;
		b->part(object2, "object");
		vector<boost::shared_ptr<xml_obj> > adverbs2;
		b->part(adverbs2, "adverbs");

		if (filterlist::is_verbose())
			cout << "question_extra: (extra1 & subject2) = "
					<< (extra1 & subject2) << ", (extra1 & object2) = "
					<< (extra1 & object2) << ", (extra1 & adverbs2) = "
					<< (extra1 & adverbs2) << ", fact = " << b << endl;

		double _m = (extra1 & subject2) + (extra1 & object2)
				+ (extra1 & adverbs2);
		if (_m > 0)
			m += _m;
		else
			m *= 0.75;
		cout << "m = " << m << ", _m = " << _m << endl;
	}
}

}

