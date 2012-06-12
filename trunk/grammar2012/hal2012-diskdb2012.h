/*
 * hal2012-diskdb2012.h
 *
 *  Created on: 07.06.2012
 *      Author: tobias
 */

#ifndef HAL2012_DATABASE2012_INDEXDB_H_
#define HAL2012_DATABASE2012_INDEXDB_H_

#include "hal2012-database2012.h"

namespace grammar2012 {

template<typename DB> class database;

class diskdb {
private:
	database<diskdb>* d;
	fs::path filename;
	bool complete;

public:
	diskdb(database<diskdb>*);
	~diskdb();

	int insert(xml_fact*);
	int set_filename(const fs::path&);
	int to_disk(const string& = "");
	int from_disk(const string&);
	int from_file(const fs::path&);
	int find_by_word(const word&);
	int copy_to(vector<boost::shared_ptr<xml_fact> >&);

	bool is_verbose();
	bool is_buffered();
	bool is_complete();
	void set_complete(bool);

	typedef boost::unordered_map<std::size_t, boost::shared_ptr<xml_fact> > indexmap_int;
	typedef boost::unordered_multimap<string, boost::shared_ptr<xml_fact> > indexmap_str;
	typedef boost::unordered_multimap<string, boost::shared_ptr<xml_fact> > indexmap_3chars;

	indexmap_int uniquefacts;
	indexmap_str index_word;
	indexmap_3chars index_3chars;
};

}

#endif /* HAL2012_DATABASE2012_INDEXDB_H_ */
