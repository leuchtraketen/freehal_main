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
int push(vector<Element>& target, Element& e) {
	target.push_back(e);
}
template<class Container, class Predicate>
int filter(Container& source, Container& target, Predicate pred) {
	int filtered = 0;
	for (int i = 0; i < source.size(); ++i) {
		if (pred(source[i])) {
			push(target, source[i]);
			++filtered;
		}
	}
	return filtered;
}

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

double operator %(boost::shared_ptr<xml_obj> o1, boost::shared_ptr<xml_obj> o2);
double operator %(boost::shared_ptr<xml_fact> o1,
		boost::shared_ptr<xml_fact> o2);
double operator /(boost::shared_ptr<xml_obj> o1, boost::shared_ptr<xml_obj> o2);
double operator /(boost::shared_ptr<xml_fact> o1,
		boost::shared_ptr<xml_fact> o2);

template<typename O, typename R>
class ranking {
private:
	std::map<R, O> map;
	vector<R> keys;
	vector<O> values;

public:
	ranking() :
			map() {
	}
	void insert(O o, R r) {
		map.insert(make_pair(r, o));
	}
	O get(int i) {
		if (values.size() == 0)
			std::transform(map.begin(), map.end(), std::back_inserter(values),
					boost::bind(&std::map<R, O>::value_type::second, _1));
		return (i < values.size() ? values[i] : O());
	}
	R rank(int i) {
		if (keys.size() == 0)
			std::transform(map.begin(), map.end(), std::back_inserter(keys),
					boost::bind(&std::map<R, O>::value_type::first, _1));
		return (i < keys.size() ? keys[i] : R());
	}
	O operator[](int i) {
		return get(i);
	}
	size_t size() {
		return map.size();
	}
};

class is_index_word: std::unary_function<word, bool> {
public:
	is_index_word() :
			std::unary_function<word, bool>() {
	}
	bool operator()(const word& word) const {
		tags* tags = word.get_tags();
		if (tags == 0) {
			return true;
		} else {
			cout << tags->first << endl;
			return tags->first == "n" || tags->first == "adj";
		}
	}
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

}

#endif /* HAL2012_FILTERFACTS_H_ */
