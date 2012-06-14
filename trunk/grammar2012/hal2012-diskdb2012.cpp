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

diskdb_key diskdb::ALL_KEYS = diskdb_key();

diskdb::diskdb(database<diskdb>* _db) :
		db(0), filename(), complete(false), do_add_synonyms(false) {
	db = _db;
}
diskdb::~diskdb() {
	foreach (indexmap_int::value_type i, uniquefacts) {
//		if (i.second)
//			delete i.second;
	}
}

void diskdb::set_add_synonyms(bool b) {
	do_add_synonyms = b;
}

int diskdb::set_filename(const fs::path& _filename) {
	filename = _filename;
	return 0;
}

int diskdb::insert_fact(boost::shared_ptr<xml_fact> xfact_p) {

	if (is_synonym()(boost::dynamic_pointer_cast<xml_obj>(xfact_p))) {
		insert_synonym(xfact_p);
		return 0;
	}

	size_t hash = hash_value(*xfact_p);
	uniquefacts.insert(indexmap_int::value_type(hash, xfact_p));

	vector<word> words;
	xfact_p->get_words(words);
	foreach (word& _word, words) {
		string word = _word.get_word();
		if (word.size() == 0)
			continue;
		word = lc(word);

		index_word.insert(indexmap_str::value_type(word, xfact_p));

		if (is_index_word()(word)) {
			diskdb_key key(word);
			indexmap_3chars::iterator iter(index_3chars.find(key.get_key()));
			if (iter == index_3chars.end()) {
				boost::shared_ptr<xml_facts_set> set(new xml_facts_set());
				set->insert(xfact_p);
				index_3chars.insert(
						indexmap_3chars::value_type(key.get_key(), set));
			} else {
				iter->second->insert(xfact_p);
			}
		}
	}

	return 0;
}

int diskdb::insert_synonym(boost::shared_ptr<xml_fact> xfact_p) {

	if (do_add_synonyms == false)
		return 1;

	if (is_synonym()(boost::dynamic_pointer_cast<xml_obj>(xfact_p))) {

		const string text1 = xfact_p->part("subject")->print_text();
		const string text2 = xfact_p->part("object")->print_text();

		synonymsmap::iterator syns1_iter = synonyms.find(text1);
		synonymsmap::iterator syns2_iter = synonyms.find(text2);
		boost::shared_ptr<boost::unordered_set<string> > syns;
		if (syns1_iter != synonyms.end()) {
			if (syns2_iter != synonyms.end()) {
				boost::shared_ptr<boost::unordered_set<string> > syns1 =
						syns1_iter->second;
				boost::shared_ptr<boost::unordered_set<string> > syns2 =
						syns2_iter->second;
				if (syns1.get() != syns2.get()) {
					if (0)
						cout << "synonyms: " << text1 << " and " << text2
								<< " found, and they have different values..."
								<< endl;
					syns1->insert(syns2->begin(), syns2->end());
					syns = syns1;
					synonyms.insert(synonymsmap::value_type(text2, syns));
				} else {
					syns = syns1;
					if (0)
						cout << "synonyms: " << text1 << " and " << text2
								<< " found, and they have the same values..."
								<< endl;
				}
			} else {
				boost::shared_ptr<boost::unordered_set<string> > syns1 =
						syns1_iter->second;
				syns = syns1;
				synonyms.insert(synonymsmap::value_type(text2, syns));
				if (0)
					cout << "synonyms: only " << text1 << " and not " << text2
							<< " found..." << endl;
			}
		} else {
			if (syns2_iter != synonyms.end()) {
				boost::shared_ptr<boost::unordered_set<string> > syns2 =
						syns2_iter->second;
				syns = syns2;
				synonyms.insert(synonymsmap::value_type(text1, syns));
				if (0)
					cout << "synonyms: not " << text1 << " and only " << text2
							<< " found..." << endl;
			} else {
				if (0)
					cout << "synonyms: neither " << text1 << " nor " << text2
							<< " found..." << endl;
				syns.reset(new boost::unordered_set<string>());
				synonyms.insert(synonymsmap::value_type(text1, syns));
				synonyms.insert(synonymsmap::value_type(text2, syns));
			}
		}
		syns->insert(text1);
		syns->insert(text2);
		if (0)
			cout << "synonym: " << text1 << ", " << text2 << endl;

	}

	return 0;
}

int diskdb::copy_facts_to(vector<boost::shared_ptr<xml_fact> >& list) {

	foreach (indexmap_int::value_type v, uniquefacts) {
		list.push_back(v.second);
	}

	return 0;
}

const fs::path diskdb::get_path_of_synonyms(const word& w) {
	diskdb_key key(w);
	const fs::path path = db->disk_find_file("synonyms", key[0], key[1], key[2],
			key[3], key.get_word());
	return path;
}

int diskdb::get_synonyms(boost::shared_ptr<vector<string> > list,
		const word& w) {
	if (0)
		if (db->is_verbose())
			cout << "find synonyms of: " << w.get_word() << endl << flush;

	const fs::path path = get_path_of_synonyms(w);
	if (fs::is_regular_file(path)) {
		db->disk_read_file(path, *list);
	}

	return 0;
}

int diskdb::get_synonyms(vector<word>& syns, const word& w) {

	boost::shared_ptr<vector<string> > list(new vector<string>());
	get_synonyms(list, w);
	if (list->size() > 0) {
		foreach (const string& s, *list) {
			if (s != w.get_word())
				syns.push_back(word(s, w.get_tags()));
		}
	}

	return 0;
}

int diskdb::get_facts(const word& word) {
	diskdb_key key(word);
	if (db->is_verbose())
		cout << "find facts by key: " << key.get_key() << endl << flush;

	this->from_disk(key);

	return 0;
}

int diskdb::from_disk(const diskdb_key& key) {
	const fs::path path = db->disk_find_file("index", key[0], key[1], key[2],
			key[3], "");
	this->from_disk(path);
	return 0;
}

int diskdb::from_disk(const fs::path& path) {
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
				this->from_disk(p);
			}
			if (is_verbose() && !is_buffered())
				cout << "index scan done. " << uniquefacts.size()
						<< " facts found." << endl;
			return 0;

		} else if (fs::is_regular_file(path)) {
			boost::shared_ptr<vector<string> > xmllist(new vector<string>());
			db->disk_read_file(path, *xmllist);
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

int diskdb::to_disk(const diskdb_key& key) {
	if (key.is_empty()) {
		// store facts
		for (indexmap_3chars::iterator found(index_3chars.begin());
				found != index_3chars.end(); ++found) {

			diskdb_key key(found->first);
			if (db->is_buffered())
				cout << "store index: " << key.get_key() << endl;
			else
				cout << "store index: " << key.get_key() << "\r" << flush;

			const fs::path path = db->disk_find_file("index", key[0], key[1],
					key[2], key[3], filename.filename().generic_string());
			// cout << path << endl;
			if (is_complete()) {
				boost::unordered_set<string> set;
				foreach (boost::shared_ptr<xml_fact> xfact_p, *(found->second)) {
					set.insert(xfact_p->print_xml());
				}
				vector<string> list(set.begin(), set.end());
				db->disk_write_file(path, list);
			} else {
				vector<string> list;
				db->disk_read_file(path, list);
				boost::unordered_set<string> set(list.begin(), list.end());
				foreach (boost::shared_ptr<xml_fact> xfact_p, *(found->second)) {
					set.insert(xfact_p->print_xml());
				}
				list.assign(set.begin(), set.end());
				db->disk_write_file(path, list);
			}

		}
		if (0) {
			vector<char> chars;
			for (char a = 'a'; a <= 'z'; ++a) {
				chars.push_back(a);
			}
			chars.push_back('_');

			foreach (char a, chars) {
				foreach (char b, chars) {
					foreach (char c, chars) {
						foreach (char d, chars) {
							diskdb_key key(a, b, c, d);

							if (db->is_verbose() && c == '_' && d == '_') {
								if (db->is_buffered() && b == '_')
									cout << "store index: " << a << endl;
								else
									cout << "store index: " << a << b << "\r"
											<< flush;
							}

							this->to_disk(key);
						}
					}
				}
			}
			if (db->is_verbose())
				cout << "store index: done" << endl << flush;
		}

		// store synonyms
		{
			foreach (synonymsmap::value_type v, synonyms) {
				const string& word = v.first;
				const boost::shared_ptr<boost::unordered_set<string> >& set =
						v.second;
				const fs::path path = get_path_of_synonyms(
						grammar2012::word(word));

				if (db->is_verbose()) {
					if (db->is_buffered())
						cout << "store synonyms: " << word << endl;
					else
						cout << "store synonyms: " << word << "\r" << flush;
				}

				vector<string> list;
				db->disk_read_file(path, list);
				set->insert(list.begin(), list.end());
				list.assign(set->begin(), set->end());
				db->disk_write_file(path, list);
			}

			if (db->is_verbose())
				cout << "store synonym: done " << endl << flush;
		}

	}
	return 0;
}

bool diskdb::is_verbose() {
	return db ? db->is_verbose() : true;
}
bool diskdb::is_buffered() {
	return db ? db->is_buffered() : false;
}

bool diskdb::is_complete() {
	return complete;
}
void diskdb::set_complete(bool _complete) {
	complete = _complete;
}

diskdb_key::diskdb_key() :
		_key(), _word() {
}
diskdb_key::diskdb_key(const diskdb_key& copy) :
		_key(copy._key), _word(copy._word) {
}
diskdb_key::diskdb_key(const string& w) :
		_key(), _word(w) {

	_key = lc(_word.substr(0, 4));
	while (_key.size() < 4)
		_key += "_";
}
diskdb_key::diskdb_key(const word& w) :
		_key(), _word(w.get_word()) {

	_key = lc(_word.substr(0, 4));
	while (_key.size() < 4)
		_key += "_";
}
diskdb_key::diskdb_key(const char a, const char b, const char c, const char d) :
		_key(), _word() {

	stringstream ss;
	ss << a << b << c << d;
	_key = lc(ss.str());
}

char diskdb_key::operator[](int i) const {
	return (i < _key.size() ? _key[i] : '\0');
}

const string& diskdb_key::get_key() const {
	return _key;
}

const string& diskdb_key::get_word() const {
	return _word;
}

bool diskdb_key::is_empty() const {
	return _key.empty();
}

}

