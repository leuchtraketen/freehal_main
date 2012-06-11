/*
 * hal2012-filterfacts.h
 *
 *  Created on: 10.06.2012
 *      Author: tobias
 */

#ifndef HAL2012_FILTERFACTS_H_
#define HAL2012_FILTERFACTS_H_

#include "hal2012-util2012.h"

namespace grammar2012 {

template<class Element>
int push(vector<Element>& target, Element& e) {
	target.push_back(e);
}
template<class Container, class Element, class Predicate>
int filter(Container& source, Container& target, Predicate pred) {
	int filtered = 0;
	foreach (Element e, source) {
		if (pred(e)) {
			push(target, e);
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

size_t operator %(boost::shared_ptr<xml_obj> o1, boost::shared_ptr<xml_obj> o2);
size_t operator %(boost::shared_ptr<xml_fact> o1, boost::shared_ptr<xml_fact> o2);
double operator /(boost::shared_ptr<xml_obj> o1, boost::shared_ptr<xml_obj> o2);
double operator /(boost::shared_ptr<xml_fact> o1, boost::shared_ptr<xml_fact> o2);

}

#endif /* HAL2012_FILTERFACTS_H_ */
