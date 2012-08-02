/*
 * hal2012-diskdb2012.h
 *
 *  Created on: 07.06.2012
 *      Author: tobias
 */

#ifndef HAL2012_DATABASE2012_INDEXDB_H_
#define HAL2012_DATABASE2012_INDEXDB_H_

#include "hal2012-database2012.h"
#include "hal2012-xml2012.h"

namespace grammar2012 {

template<typename DB> class database;
class diskdb_key;

class diskdb {
private:
	database<diskdb>* db;
	fs::path filename;
	bool complete;
	bool do_add_synonyms;

public:
	diskdb(database<diskdb>*);
	~diskdb();

	// build the database
	int insert_fact(boost::shared_ptr<xml_fact>);
	int insert_synonym(boost::shared_ptr<xml_fact>);
	void set_add_synonyms(bool);
	int set_filename(const fs::path&);
	int to_disk(const diskdb_key&);

	// get facts from the database
	int from_disk(const diskdb_key&);
	int from_disk(const fs::path&);
	int get_facts(const word&);
	int copy_facts_to(vector<boost::shared_ptr<xml_fact> >&);

	// get synonyms from the database
	int get_synonyms(vector<word>&, const word&);
	int get_synonyms(boost::shared_ptr<vector<string> >, const word&);
	const fs::path get_path_of_synonyms(const word&);

	// console output
	bool is_verbose();
	bool is_buffered();
	bool is_complete();
	void set_complete(bool);

	// static variables
	static diskdb_key ALL_KEYS;

	// data
	typedef boost::unordered_set<boost::shared_ptr<xml_fact> > xml_facts_set;
	typedef boost::unordered_multimap<string, boost::shared_ptr<xml_fact> > indexmap_str;
	typedef boost::unordered_map<string, boost::shared_ptr<xml_facts_set> > indexmap_3chars;
	typedef boost::unordered_map<string,
			boost::shared_ptr<boost::unordered_set<string> > > synonymsmap;

	xml_facts_set uniquefacts;
	indexmap_str index_word;
	indexmap_3chars index_3chars;
	synonymsmap synonyms;
};

class diskdb_key {
private:
	string _key;
	string _word;
public:
	diskdb_key();
	diskdb_key(const diskdb_key&);
	diskdb_key(const word&);
	diskdb_key(const string&);
	diskdb_key(const char, const char, const char, const char);
	char operator[](int) const;

	const string& get_key() const;
	const string& get_word() const;
	bool is_empty() const;
};

}

#endif /* HAL2012_DATABASE2012_INDEXDB_H_ */
