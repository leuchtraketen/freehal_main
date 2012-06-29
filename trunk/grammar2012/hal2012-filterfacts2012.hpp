/*
 * hal2012-filterfacts.hpp
 *
 *  Created on: 14.06.2012
 *      Author: tobias
 */

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

template<typename O, typename R>
ranking<O, R>::ranking() :
		map() {
}

template<typename O, typename R>
void ranking<O, R>::insert(O o, R r) {
	if (r > 0) {
		map.insert(make_pair(r, o));
	}
}

template<typename O, typename R>
O ranking<O, R>::get(int i) {
	return (i < values.size() ? values[i] : O());
}

template<typename O, typename R>
R ranking<O, R>::rank(int i) {
	if (keys.size() == 0)
		std::transform(map.begin(), map.end(), std::back_inserter(keys),
				boost::bind(&std::map<R, O>::value_type::first, _1));
	return (i < keys.size() ? keys[i] : R());
}

template<typename O, typename R>
vector<O> ranking<O, R>::best() {
	if (values.size() == 0)
		std::transform(map.begin(), map.end(), std::back_inserter(values),
				boost::bind(&std::map<R, O>::value_type::second, _1));

	vector<O> best;
	if (values.size() > 0) {
		R bestrank = rank(values[values.size()-1]);
		for (int i = values.size()-1; i >= 0; --i) {
			if (rank(i) == bestrank) {
				best.push_back(rank[i]);
			}
		}
	}
	return best;
}

template<typename O, typename R>
O ranking<O, R>::best_one() {
	vector<O> best = best();
	return best.size() > 0 ? best[0] : O();
}

template<typename O, typename R>
O ranking<O, R>::operator[](int i) {
	return get(i);
}

template<typename O, typename R>
size_t ranking<O, R>::size() {
	return map.size();
}

template<typename O, typename R>
O ranking<O, R>::best_one() {
	if (values.size() == 0)
		std::transform(map.begin(), map.end(), std::back_inserter(values),
				boost::bind(&std::map<R, O>::value_type::second, _1));
	return (i < values.size() ? values[i] : O());
}

}

