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

bool operator &(string& word1, string& word2) {
	cout << "---- compare: " << word1 << " & " << word2 << " = "
			<< (word1 == word2 ? 1 : 0) << endl;
	return word1 == word2 || lc(word1) == lc(word2);
}
bool operator &(string& word1, boost::shared_ptr<xml_obj>& o2) {
	vector<string> words;
	o2->get_words(words);
	foreach (string& word2, words) {
		if (word1 & word2)
			return true;
	}
	return false;
}
size_t operator &(boost::shared_ptr<xml_obj>& o1,
		boost::shared_ptr<xml_obj>& o2) {
	cout << "---- compare: " << o1->print_str() << " & " << o2->print_str()
			<< endl;
	size_t matches = 0;
	if (o1->get_mode() == TEXT && o2->get_mode() == TEXT) {
		vector<string> words;
		o1->get_words(words);
		foreach (string& word, words) {
			matches += word & o2;
		}
	} else if (o1->get_mode() == LIST) {
		std::vector<boost::shared_ptr<xml_obj> > content;
		o1 >> content;
		int count = 0;
		foreach (boost::shared_ptr<xml_obj> subobj, content) {
			size_t m = subobj & o2;
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
			size_t m = o1 & subobj;
			if (m > 0) {
				matches += m;
				++count;
			}
		}

		if (o2->get_name() == "link_&")
			matches = (count == content.size() ? matches : 0);
	}
	cout << "---- compare: " << o1->print_str() << " & " << o2->print_str()
			<< " = " << matches << endl;
	return matches;
}
size_t operator &(boost::shared_ptr<xml_obj>& o1,
		vector<boost::shared_ptr<xml_obj> >& v2) {
	size_t matches = 0;
	foreach (boost::shared_ptr<xml_obj> o2, v2) {
		matches += o1 & o2;
	}
	cout << "---- compare: " << o1->print_str() << " & " << v2.data() << " = "
			<< matches << endl;
	return matches;
}
size_t operator &(vector<boost::shared_ptr<xml_obj> >& v1,
		vector<boost::shared_ptr<xml_obj> >& v2) {
	size_t matches = 0;
	foreach (boost::shared_ptr<xml_obj> o1, v1) {
		matches += o1 & v2;
	}
	cout << "---- compare: " << v1.data() << " & " << v2.data() << " = "
			<< matches << endl;
	return matches;
}
double count(boost::shared_ptr<xml_obj> o) {
	if (o->get_mode() == TEXT) {
		vector<string> words;
		o->get_words(words);
		return (double) words.size();

	} else if (o->get_name() == "questionword" || o->get_name() == "adverbs"
			|| o->get_name() == "extra"|| o->get_name() == "truth") {
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

size_t operator %(boost::shared_ptr<xml_obj> o1,
		boost::shared_ptr<xml_obj> o2) {
	size_t matches = 0;

	vector<boost::shared_ptr<xml_obj> > verbs1;
	o1->part(verbs1, "verb");
	vector<boost::shared_ptr<xml_obj> > verbs2;
	o2->part(verbs2, "verb");

	size_t verbs_match = verbs1 & verbs2;
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

	size_t subject_match = (subject1 & subject2) + (subject1 & object2)
			+ (subject1 & adverbs2);
	matches += subject_match;

	size_t object_match = (object1 & subject2) + (object1 & object2)
			+ (object1 & adverbs2);
	matches += object_match;

	size_t adverbs_match = (adverbs1 & subject2) + (adverbs1 & object2)
			+ (adverbs1 & adverbs2);
	matches += adverbs_match;

	return matches;
}
size_t operator %(boost::shared_ptr<xml_fact> o1,
		boost::shared_ptr<xml_fact> o2) {
	return boost::dynamic_pointer_cast<xml_obj>(o1)
			% boost::dynamic_pointer_cast<xml_obj>(o2);
}
double operator /(boost::shared_ptr<xml_obj> o1,
		boost::shared_ptr<xml_obj> o2) {
	double matches = o1 % o2;
	double avgcount = (3 * count(o1) + count(o2)) / 4.0;
	if (avgcount > 0)
		matches /= avgcount;
	return matches;
}
double operator /(boost::shared_ptr<xml_fact> o1,
		boost::shared_ptr<xml_fact> o2) {
	return boost::dynamic_pointer_cast<xml_obj>(o1)
			/ boost::dynamic_pointer_cast<xml_obj>(o2);
}

}

