/*
 * hal2012-phrase2012.cpp
 *
 *  Created on: 01.07.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-phrase2012.h"

namespace grammar2012 {

tagger* phrase_tagger_local = 0;

vector<word>& operator <<(vector<word>& words, tagger* t);

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
		if (is_verbose() && w.has_tags())
			cout << "phrase: word: " << w << ", tags: "
					<< print_tags(w.get_tags()) << endl;

		if (w.has_tags() && w.get_tags()->first == "n")
			w.set_word(ucfirst(w.get_word()));
	}

	if (words.size() >= 1)
		words[0].set_word(ucfirst(words[0].get_word()));

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
	regex_replace(joined, "[ _]+", " ");

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
		words << t << subject << verbs << object << adverbs;
	}
}

vector<word>& operator <<(vector<word>& words, tagger* t) {
	if (t != 0)
		phrase_tagger_local = t;
	return words;
}

void replace_virtual_words(vector<word>& final_words,
		const vector<word>& raw_words, const string& obj_name) {
	foreach (word w, raw_words) {
		if (phrase_tagger_local != 0) {
			if (w == "is-a") {
				w = phrase_tagger_local->get_verb("be", "3s");
			}
			if (w == "(a)") {
				string genus;
				foreach (const word& inner_w, raw_words) {
					if (inner_w.has_tags()
							&& inner_w.get_tags()->second.size() > 0)
						genus = inner_w.get_tags()->second;
				}
				if (!genus.empty())
					w = phrase_tagger_local->get_article(w.get_word(), genus,
							obj_name);
			}
		}
		final_words.push_back(w);
	}
}

vector<word>& operator <<(vector<word>& words, boost::shared_ptr<xml_obj> obj) {
	vector<word> final_words;
	vector<word> raw_words;
	obj->get_words(raw_words);
	replace_virtual_words(final_words, raw_words, obj->get_name());
	words.insert(words.end(), final_words.begin(), final_words.end());
	return words;
}

vector<word>& operator <<(vector<word>& words,
		const vector<boost::shared_ptr<xml_obj> >& objs) {
	foreach (boost::shared_ptr<xml_obj> obj, objs) {
		words << obj;
	}
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

