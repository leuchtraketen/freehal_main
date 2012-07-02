/*
 * hal2012-phrase2012.cpp
 *
 *  Created on: 01.07.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-phrase2012.h"

namespace grammar2012 {

phraser::phraser() :
		freehal_base() {
}
phraser::~phraser() {
}

const string phraser::phrase(boost::shared_ptr<xml_fact> xfact) {
	if (!freehal_base::is_configured()) {
		return "";
	} else if (t == 0) {
		cout << "Error! phrase2012: tagger is undefined." << endl;
		return "";
	} else if (g == 0) {
		cout << "Error! phrase2012: grammar is undefined." << endl;
		return "";
	}

	{
		if (is_verbose())
			cout << "phrase2012: xml input: " << xfact->print_str() << endl;
	}

	vector<word> words;
	{
		arrange(words, xfact);
		if (is_verbose())
			cout << "phrase2012: arranged words: " << xfact->print_str()
					<< endl;
	}

	string joined;
	{
		joined = join(words);
		if (is_verbose())
			cout << "phrase2012: joined output: " << xfact->print_str() << endl;
	}

	return joined;
}

const string phraser::join(vector<word>& words) {
	// upper case nouns
	foreach (word& w, words) {
		if (w.has_tags() && w.get_tags()->first == "n")
			w.set_word(ucfirst(w.get_word()));
		else if (w.has_tags())
			cout << w << ", tags=" << w.get_tags()->first << endl;
	}

	// words to strings
	vector<string> strings;
	std::transform(words.begin(), words.end(), std::back_inserter(strings),
			boost::bind(&word::get_word, _1));
	// join them with spaces as seperators
	string joined = boost::algorithm::join(strings, " ");

	// syntax
	regex_replace(joined, "[ ]+([.?!;,])", "\\1");
	if (!regex_find(joined, "[?!]")) {
		joined += ".";
	}
	joined = ucfirst(joined);

	return joined;
}

void phraser::arrange(vector<word>& words, boost::shared_ptr<xml_fact> xf) {
	vector<boost::shared_ptr<xml_obj> > verbs;
	xf->part(verbs, "verb");
	vector<boost::shared_ptr<xml_obj> > subject;
	xf->part(subject, "subject");
	vector<boost::shared_ptr<xml_obj> > object;
	xf->part(object, "object");
	vector<boost::shared_ptr<xml_obj> > adverbs;
	xf->part(adverbs, "adverbs");

	{
		words << subject << verbs << object << adverbs;
	}
}

vector<word>& operator <<(vector<word>& words,
		const vector<boost::shared_ptr<xml_obj> >& objs) {
	foreach (boost::shared_ptr<xml_obj> obj, objs) {
		obj->get_words(words);
	}
	return words;
}

vector<word>& operator <<(vector<word>& words, boost::shared_ptr<xml_obj> obj) {
	obj->get_words(words);
	return words;
}

void phraser::set_tagger(tagger* _t) {
	t = _t;
}
void phraser::set_grammar(grammar* _g) {
	g = _g;
}
tagger* phraser::get_tagger() const {
	return t;
}
grammar* phraser::get_grammar() const {
	return g;
}

}

