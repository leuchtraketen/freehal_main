/*
 * hal2012-database2012.cpp
 *
 *  Created on: 07.06.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-database2012.h"
#include "hal2012-diskdb2012.h"
#include "hal2012-serialization.h"

namespace grammar2012 {

diskdb::diskdb(database<diskdb>* _d) :
		d(0), filename(), complete(false) {
	d = _d;
}
diskdb::~diskdb() {
	foreach (indexmap_int::value_type i, uniquefacts) {
//		if (i.second)
//			delete i.second;
	}
}

int diskdb::set_filename(const fs::path& _filename) {
	filename = _filename;
	return 0;
}

int diskdb::insert(xml_fact* xfact) {
	size_t hash = hash_value(*xfact);
	boost::shared_ptr<xml_fact> xfact_p(xfact);
	uniquefacts.insert(indexmap_int::value_type(hash, xfact_p));

	vector<string> words;
	xfact->get_words(words);
	foreach (string word, words) {
		if (word.size() == 0)
			continue;

		index_word.insert(indexmap_str::value_type(word, xfact_p));

		string sub = word.substr(0, 3);
		while (sub.size() < 3)
			sub = "_" + sub;
		index_3chars.insert(indexmap_3chars::value_type(sub, xfact_p));
	}

	return 0;
}

int diskdb::copy_to(vector<boost::shared_ptr<xml_fact> >& list) {

	foreach (indexmap_int::value_type v, uniquefacts) {
		list.push_back(v.second);
	}

	return 0;
}

int diskdb::find_by_word(const string& word) {

	string key;
	{
		stringstream ss_key;
		ss_key << word.substr(0, 3);
		key = ss_key.str();
		while (key.size() < 3)
			key = "_" + key;
		if (d->is_verbose())
			cout << "find facts by key: " << key << endl << flush;
	}

	this->from_disk(key);

	return 0;
}

int diskdb::from_disk(const string& key) {
	const fs::path path = d->disk_find_file("index", key[0], key[1], key[2],
			"");
	this->from_file(path);
	return 0;
}

int diskdb::from_file(const fs::path& path) {
	if (fs::exists(path)) {

		if (fs::is_directory(path)) {
			if (is_verbose() && !is_buffered())
				cout << "scan index directory: " << path << "        \r";
			vector<fs::path> dir;
			copy(fs::directory_iterator(path), fs::directory_iterator(),
					back_inserter(dir));
			for (vector<fs::path>::iterator iter(dir.begin());
					iter != dir.end(); ++iter) {
				const fs::path& p = *iter;
				this->from_file(p);
			}
			if (is_verbose() && !is_buffered())
				cout << "index scan done. " << uniquefacts.size() << " facts found." << endl;
			return 0;

		} else if (fs::is_regular_file(path)) {
			boost::shared_ptr<vector<string> > xmllist(new vector<string>());
			d->disk_read_file(path, *xmllist);
			boost::shared_ptr<string> instr(new string());
			foreach (string xmlfact, *xmllist) {
				*instr += xmlfact;
			}
			xmllist.reset();
			boost::shared_ptr<string> prestr(new string());
			halxml_ordertags(*instr, *prestr);
			instr.reset();

			if (is_verbose() && !is_buffered())
				cout << "scan index file: " << path << "        \r";

			halxml_readfacts(this, *prestr, "");
			prestr.reset();
			if (is_verbose() && !is_buffered())
				cout << endl;
			return 0;

		} else
			return 1;
	} else {
		return 1;
	}
}

int diskdb::to_disk(const string& key) {
	if (key.empty()) {
		vector<char> chars;
		for (char a = 'a'; a <= 'z'; ++a) {
			chars.push_back(a);
		}
		chars.push_back('_');

		foreach (char a, chars) {
			foreach (char b, chars) {
				foreach (char c, chars) {
					stringstream ss_key;
					ss_key << a << b << c;
					string _key = ss_key.str();
					if (d->is_verbose() && c == '_') {
						cout << "store index: " << _key
								<< (d->is_buffered() ? "\n" : "\r") << flush;
					}

					this->to_disk(_key);
				}
			}
		}
		if (d->is_verbose())
			cout << "store index: done" << endl << flush;

	} else {
		pair<indexmap_3chars::iterator, indexmap_3chars::iterator> hashes =
				index_3chars.equal_range(key);
		if (hashes.first != hashes.second) {
			const fs::path path = d->disk_find_file("index", key[0], key[1],
					key[2], filename.filename().generic_string());

			if (is_complete()) {
				boost::unordered_set<string> set;
				for (indexmap_3chars::iterator hash_iter = hashes.first;
						hash_iter != hashes.second; ++hash_iter) {
					set.insert(hash_iter->second->print_xml());
				}
				vector<string> list(set.begin(), set.end());
				d->disk_write_file(path, list);
			} else {
				vector<string> list;
				d->disk_read_file(path, list);
				boost::unordered_set<string> set(list.begin(), list.end());
				for (indexmap_3chars::iterator hash_iter = hashes.first;
						hash_iter != hashes.second; ++hash_iter) {
					set.insert(hash_iter->second->print_xml());
				}
				list.assign(set.begin(), set.end());
				d->disk_write_file(path, list);
			}
		}
	}
	return 0;
}

bool diskdb::is_verbose() {
	return d ? d->is_verbose() : true;
}
bool diskdb::is_buffered() {
	return d ? d->is_buffered() : false;
}

bool diskdb::is_complete() {
	return complete;
}
void diskdb::set_complete(bool _complete) {
	complete = _complete;
}

}

