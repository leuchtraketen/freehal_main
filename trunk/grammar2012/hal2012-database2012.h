/*
 * hal2012-database2012.h
 *
 *  Created on: 07.06.2012
 *      Author: tobias
 */

#ifndef HAL2012_DATABASE2012_H_
#define HAL2012_DATABASE2012_H_

#include "hal2012-util2012.h"
#include "hal2012-xml2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-filterfacts2012.h"
#include "hal2012-serialization.h"

namespace grammar2012 {

// this resolves the chicken or egg causality dilemma between
// hal2012-xml2012.h and hal2012-database2012.h because both include both
class xml_fact;

template<typename DB>
class database {
private:
	bool verbose;
	bool buffered;
	string lang;
	fs::path path;
	tagger* t;

	// IO variables
	typedef boost::unordered_map<string, uintmax_t> filesmap;
	filesmap files;

	// I/O
	int resume();
	int suspend();
	int read_xml_fact_file(const fs::path, DB*);

public:
	// constructor, destructor
	database();
	~database();

	// I/O
	int prepare(const fs::path&);
	int find_by_word(vector<boost::shared_ptr<xml_fact> >&, const word&);
	int find_by_words(vector<boost::shared_ptr<xml_fact> >&, const vector<word>&);
	int find_by_fact(vector<boost::shared_ptr<xml_fact> >&, boost::shared_ptr<xml_fact>);

	// I/O flags
	void set_verbose(bool);
	bool is_verbose();
	void set_buffered(bool);
	bool is_buffered();

	// environment variables
	void set_lang(const string&);
	void set_path(const string&);
	const string get_lang() const;
	const string get_path() const;
	void set_tagger(tagger*);
	tagger* get_tagger() const;

	// cache algorithm
	template<typename T1, typename T2>
	const fs::path disk_find_file(const string, T1, T1, T1, T2);
	template<typename M>
	int disk_read_file(const fs::path&, M&);
	template<typename M>
	int disk_write_file(const fs::path&, const M&);

private:
	// serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & BOOST_SERIALIZATION_NVP(lang);
		//ar & BOOST_SERIALIZATION_NVP(path);
		ar & BOOST_SERIALIZATION_NVP(verbose);
		ar & BOOST_SERIALIZATION_NVP(buffered);
	}
};

}

#include "hal2012-database2012.hpp"

#endif /* HAL2012_DATABASE2012_H_ */
