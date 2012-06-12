/*
 * This file is part of FreeHAL 2012.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Tobias Schulz and contributors.
 * http://www.freehal.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "hal2012-tagger2012.h"
#include "hal2012-util2012.h"

#ifndef HAL2012_XML
#define HAL2012_XML 1

namespace grammar2012 {

class xml_obj;
class xml_fact;
class word;

enum xml_obj_mode {
	LIST, TEXT, ANY
};
class xml_obj {
public:
	xml_obj(xml_obj_mode);
	xml_obj();
	virtual ~xml_obj();

	template<typename Tree>
	friend Tree* halxml_readtree(Tree*, const string&, vector<string>&, int&);

// universal
public:
	void set_name(const string&);
	string get_name();
	string print_xml() const;
	string print_xml(int, int) const;
	string print_str() const;
	string print_str(string) const;
	void free();

	int prepare_words(tagger* t);
	int get_words(vector<word>&) const;
	int get_words(vector<word>&);

protected:
	string name;
private:
	bool is_cached_words;
	bool is_cached_tags;
	vector<word> cache_words;

// list
public:
	int part(std::vector<boost::shared_ptr<xml_obj> >&, string) const;
	void trim();
	size_t size();

	friend boost::shared_ptr<xml_obj>& operator <<(boost::shared_ptr<xml_obj>&,
			boost::shared_ptr<xml_obj>);
	friend boost::shared_ptr<xml_obj>& operator <<(boost::shared_ptr<xml_obj>&,
			vector<boost::shared_ptr<xml_obj> >&);
	friend boost::shared_ptr<xml_obj>& operator >>(boost::shared_ptr<xml_obj>&,
			vector<boost::shared_ptr<xml_obj> >&);
	friend xml_obj* operator <<(xml_obj*, vector<boost::shared_ptr<xml_obj> >&);
	friend xml_obj* operator <<(xml_obj*, boost::shared_ptr<xml_obj>);
	friend xml_obj* operator >>(xml_obj*, vector<boost::shared_ptr<xml_obj> >&);
protected:
	std::vector<boost::shared_ptr<xml_obj> > content;

// text
public:
	void set_text(const string&);
private:
	string text;

// mode
public:
	xml_obj_mode get_mode();
private:
	xml_obj_mode mode;

//serialization
public:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::make_nvp("name", name);
		ar & boost::serialization::make_nvp("mode", mode);
		ar & boost::serialization::make_nvp("text", text);
		ar & boost::serialization::make_nvp("content", content);
	}
};

class xml_fact: public xml_obj {
public:
	xml_fact();
	~xml_fact();

	string line;
	fs::path filename;

	//serialization
public:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & boost::serialization::make_nvp("filename", filename);
		ar & boost::serialization::make_nvp("line", line);
		ar & boost::serialization::make_nvp("content", content);
	}
};

class word {
private:
	string w;
	tags* tag;

public:
	word();
	word(const string);
	word(const string, tags*);

	void set_word(const string&);
	void set_tags(tags*);
	const string& get_word() const;
	tags* get_tags() const;
};


int halxml_ordertags(string& indata, string& predata);
string halxml_readfile(const fs::path& infile);
template<typename DB>
vector<xml_fact*> halxml_readfacts(DB* d, const string& prestr,
		const fs::path& filename_ref);
xml_fact* halxml_readxml_fact(vector<string>& lines, int& i);
xml_fact* record_to_xml_fact(struct RECORD* r, int level);
xml_obj* halxml_readtree(const string& tag_name, vector<string>& lines, int& i);
int use_xml_fact(void* _d, xml_fact* xfact, int xml_facts_size,
		time_t* start_ref, const fs::path& filename_ref, int k);
std::size_t hash_value(const xml_obj& o);

boost::shared_ptr<xml_obj>& operator <<(boost::shared_ptr<xml_obj>&,
		boost::shared_ptr<xml_obj>);
boost::shared_ptr<xml_obj>& operator <<(boost::shared_ptr<xml_obj>&,
		vector<boost::shared_ptr<xml_obj> >&);
boost::shared_ptr<xml_obj>& operator >>(boost::shared_ptr<xml_obj>&,
		vector<boost::shared_ptr<xml_obj> >&);
xml_obj* operator <<(xml_obj*, vector<boost::shared_ptr<xml_obj> >&);
xml_obj* operator <<(xml_obj*, boost::shared_ptr<xml_obj>);
xml_obj* operator >>(xml_obj*, vector<boost::shared_ptr<xml_obj> >&);
std::ostream& operator<<(std::ostream& stream, const xml_fact& xfact);
std::ostream& operator<<(std::ostream& stream, const xml_obj& xobj);

}

#include "hal2012-xml2012.hpp"

#endif /* HAL2012_XML */

