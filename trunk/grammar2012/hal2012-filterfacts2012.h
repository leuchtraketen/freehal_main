/*
 * hal2012-filterfacts.h
 *
 *  Created on: 10.06.2012
 *      Author: tobias
 */

#ifndef HAL2012_FILTERFACTS_H_
#define HAL2012_FILTERFACTS_H_

#include "hal2012-tagger2012.h"
#include "hal2012-util2012.h"

namespace grammar2012 {

template<class Element>
int push(vector<Element>& target, Element& e);
template<class Container, class Predicate>
int filter(Container& source, Container& target, Predicate pred);

class all_in {
public:
	vector<boost::shared_ptr<xml_obj> >& content;
	all_in(vector<boost::shared_ptr<xml_obj> >&);
};
class any_in {
public:
	vector<boost::shared_ptr<xml_obj> >& content;
	any_in(vector<boost::shared_ptr<xml_obj> >&);
};

double operator &&(const word& _word1, const word& _word2);
double operator &(const word& _word1, const word& _word2);
double operator &&(const word& word1, const boost::shared_ptr<xml_obj>& o2);
double operator &(const word& word1, boost::shared_ptr<xml_obj>& o2);
double operator &&(const boost::shared_ptr<xml_obj>& o1,
		const boost::shared_ptr<xml_obj>& o2);
double operator &(const boost::shared_ptr<xml_obj>& o1,
		const boost::shared_ptr<xml_obj>& o2);
double operator &(const boost::shared_ptr<xml_obj>& o1,
		const vector<boost::shared_ptr<xml_obj> >& v2);
double operator &(const vector<boost::shared_ptr<xml_obj> >& v1,
		const vector<boost::shared_ptr<xml_obj> >& v2);

double operator %(boost::shared_ptr<xml_obj>, boost::shared_ptr<xml_obj>);
double operator %(boost::shared_ptr<xml_fact>, boost::shared_ptr<xml_fact>);
double operator /(boost::shared_ptr<xml_obj>, boost::shared_ptr<xml_obj>);
double operator /(boost::shared_ptr<xml_fact>, boost::shared_ptr<xml_fact>);
double count_words(boost::shared_ptr<xml_obj>);
double count_tags(boost::shared_ptr<xml_obj>);

template<typename O, typename R>
class ranking {
private:
	std::multimap<R, O> map;
	vector<R> keys;
	vector<O> values;

public:
	ranking();
	void insert(O, R);
	O get(int);
	R rank(int);
	O operator[](int);
	size_t size();

	vector<O> best();
	O best_one();
};

class is_index_word: std::unary_function<word, bool> {
public:
	bool operator()(const word&) const;
};

class is_synonym: std::unary_function<boost::shared_ptr<xml_obj>, bool> {
public:
	bool operator()(const boost::shared_ptr<xml_obj>&) const;
};

class filterrule: public binary_function<
		pair<boost::shared_ptr<xml_obj>, boost::shared_ptr<xml_obj> >, double,
		double> {
protected:
	boost::shared_ptr<xml_obj> a;
	boost::shared_ptr<xml_obj> b;
	double m;

public:
	virtual double operator()(
			pair<boost::shared_ptr<xml_obj>, boost::shared_ptr<xml_obj> >,
			double);
	virtual void rule() = 0;
	virtual ~filterrule();
};

class filterlist: public filterrule {
private:
	vector<boost::shared_ptr<filterrule> > rules;
	static boost::shared_ptr<filterlist> instance;

	bool verbose;
	bool buffered;

	filterlist();

public:
	void rule();
	static boost::shared_ptr<filterlist> get();

	void add(boost::shared_ptr<filterrule>);
	void add(filterrule*);

	static void set_verbose(bool);
	static bool is_verbose();
	static void set_buffered(bool);
	static bool is_buffered();
};

class filter_no_names: public filterrule {
public:
	void rule();
};

class filter_not: public filterrule {
public:
	void rule();
};

class filter_question_who: public filterrule {
public:
	void rule();
};

class filter_question_what: public filterrule {
public:
	void rule();
};

class filter_question_extra: public filterrule {
public:
	void rule();
};

}

#include "hal2012-filterfacts2012.hpp"

#endif /* HAL2012_FILTERFACTS_H_ */
