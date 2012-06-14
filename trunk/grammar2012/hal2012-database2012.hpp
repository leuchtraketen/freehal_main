namespace grammar2012 {

template<typename DB> database<DB>::database() :
		freehal_base() {
}
template<typename DB> database<DB>::~database() {
}

template<typename DB> void database<DB>::set_lang(const string& _lang) {
	freehal_base::set_lang(_lang);
	resume();
}
template<typename DB> void database<DB>::set_path(const fs::path& _path) {
	freehal_base::set_path (_path);
	resume();
}

template<typename DB> void database<DB>::set_tagger(tagger* _t) {
	t = _t;
}
template<typename DB> tagger* database<DB>::get_tagger() const {
	return t;
}

template<typename DB> bool database<DB>::is_configured() const {
	if (!freehal_base::is_configured()) {
		return false;
	} else if (t == 0) {
		cout << "Error! parser2012: tagger is undefined." << endl;
		return false;
	} else {
		return true;
	}
}

template<typename DB>
int database<DB>::resume() {
	if (!lang.empty() && !path.empty()) {
		fs::path resumefile = get_language_directory() / "db/files.dat";
		try {
			fs::ifstream ifs(resumefile);
			if (ifs.is_open()) {
				boost::archive::xml_iarchive ia(ifs);
				ia >> boost::serialization::make_nvp("files", files);
			}

		} catch (boost::archive::archive_exception& ex) {
			cout << "Error! resume: exception during deserialization: "
					<< ex.what() << endl;
		}
	}
}
template<typename DB>
int database<DB>::suspend() {
	if (!lang.empty() && !path.empty()) {
		fs::path resumefile = get_language_directory() / "db/files.dat";
		try {

			fs::ofstream ofs(resumefile, ios::binary);
			boost::archive::xml_oarchive oa(ofs);
			oa << boost::serialization::make_nvp("files", files);
		} catch (boost::archive::archive_exception& ex) {
			cout << "Error! exception during serialization: " << ex.what()
					<< endl;
		}
	}
}

template<typename DB>
int database<DB>::prepare(const fs::path& p) {
	if (!is_configured())
		return 1;

	try {
		if (fs::exists(p)) {
			// is p a regular file?
			if (fs::is_regular_file(p)) {
				uintmax_t size = fs::file_size(p);
				if (files.find(p.generic_string()) == files.end()
						|| files.at(p.generic_string()) != size) {

					DB* idb = new DB(this);
					read_xml_fact_file(p, idb);
					idb->to_disk(DB::ALL_KEYS);
					delete idb;

					files.insert(
							filesmap::value_type(p.generic_string(), size));
				}
			}
			// is p a directory?
			else if (fs::is_directory(p)) {
				vector < fs::path > dir;
				copy(fs::directory_iterator(p), fs::directory_iterator(),
						back_inserter(dir));
				sort(dir.begin(), dir.end());
				for (vector<fs::path>::iterator iter(dir.begin());
						iter != dir.end(); ++iter) {
					const fs::path& p = *iter;
					if (algo::ends_with(p.filename().generic_string(),
							".xml")) {
						this->prepare(p);
					}
				}
			} else {
				cout << "Error! prepare: " << p
						<< " exists, but is neither a regular file nor a directory"
						<< endl;
			}
		} else {
			cout << "Error! prepare: " << p << " does not exist" << endl;
		}
	} catch (const fs::filesystem_error& ex) {
		cout << "Error! " << ex.what() << endl;
	}

	suspend();
}

template<typename DB> template<typename T1, typename T2>
const fs::path database<DB>::disk_find_file(const string type, T1 a, T1 b, T1 c, T1 d,
		T2 filename) {

	stringstream ss;
	ss << a << "/" << b << "/" << c << "/" << d;

	fs::path dir(get_language_directory() / "db" / type / ss.str());
	create_directories(dir);

	fs::path file(dir / filename);
	return file;
}

template<typename DB> template<typename M>
int database<DB>::disk_read_file(const fs::path& p, M& list) {
	if (!is_configured())
		return 1;

	if (fs::exists(p) && fs::is_regular_file(p)) {
		try {
			fs::ifstream ifs(p, std::ios_base::binary | std::ios_base::in);
			if (ifs.is_open()) {
				std::stringstream buf(
						std::ios_base::binary | std::ios_base::in
								| std::ios_base::out);
				buf << ifs.rdbuf();
				ifs.close();

				boost::archive::text_iarchive ia(buf);
				ia >> boost::serialization::make_nvp("list", list);
				return 0;
			}

		} catch (boost::archive::archive_exception& ex) {
			cout << "Error! disk_read_file: " << p
					<< " exception during deserialization: " << ex.what()
					<< endl;
			return 1;
		}
	}
	return 1;
}

template<typename DB> template<typename M>
int database<DB>::disk_write_file(const fs::path& p, const M& list) {
	if (!is_configured())
		return 1;

	try {
		fs::ofstream ofs(p);
		if (ofs.is_open()) {
			boost::archive::text_oarchive oa(ofs);
			oa << boost::serialization::make_nvp("list", list);
			return 0;
		}

	} catch (boost::archive::archive_exception& ex) {
		cout << "Error! disk_write_file: " << p
				<< " exception during serialization: " << ex.what() << endl;
		return 1;
	}
	return 1;
}

template<typename DB>
int database<DB>::find_by_word(vector<boost::shared_ptr<xml_fact> >& list,
		const word& word) {
	if (!is_configured())
		return 1;

	DB* idb = new DB(this);
	idb->set_add_synonyms(false);
	idb->get_facts(word);
	idb->copy_facts_to(list);
	delete idb;

	std::for_each(list.begin(), list.end(),
			boost::bind(&database<DB>::insert_synonyms, this, _1));
	if (t != 0) {
		bool verbose_copy = t->is_verbose();
		t->set_verbose(false);
		std::for_each(list.begin(), list.end(),
				boost::bind(&xml_fact::prepare_tags, _1, t));
		t->set_verbose(verbose_copy);
	}

	return 0;
}

template<typename DB>
int database<DB>::find_by_words(vector<boost::shared_ptr<xml_fact> >& list,
		const vector<word>& words) {
	if (!is_configured())
		return 1;

	DB* idb = new DB(this);
	idb->set_add_synonyms(false);
	foreach (const word& word, words) {
		idb->get_facts(word);
	}
	idb->copy_facts_to(list);
	delete idb;

	std::for_each(list.begin(), list.end(),
			boost::bind(&database<DB>::insert_synonyms, this, _1));
	if (t != 0) {
		bool verbose_copy = t->is_verbose();
		t->set_verbose(false);
		std::for_each(list.begin(), list.end(),
				boost::bind(&xml_fact::prepare_tags, _1, t));
		t->set_verbose(verbose_copy);
	}

	return 0;
}

template<typename DB>
int database<DB>::find_by_fact(vector<boost::shared_ptr<xml_fact> >& list,
		boost::shared_ptr<xml_fact> fact) {
	if (!is_configured())
		return 1;

	this->insert_synonyms(boost::dynamic_pointer_cast<xml_obj>(fact));

	cout << "tagger: " << t << endl;
	if (t != 0) {
		bool verbose_copy = t->is_verbose();
		t->set_verbose(false);
		fact->prepare_tags(t);
		t->set_verbose(verbose_copy);
	}

	vector<word> words;
	fact->get_words(words);
	vector<word> usefulwords;
	filter(words, usefulwords, is_index_word());

	return find_by_words(list, usefulwords);
}

template<typename DB>
int database<DB>::read_xml_fact_file(const fs::path filename, DB* idb) {
	if (!is_configured())
		return 1;
	{
		fs::ifstream i;
		i.open(filename);

		if (!i) {
			cout
					<< "Error! Could not open part of speech file (read_xml_fact_file): "
					<< filename << endl;
			return 1;
		}

		if (is_verbose()) {
			if (is_buffered())
				cout << "read fact file: " << filename << endl;
			else
				cout << "read fact file: " << filename << "\r";
		}
	}

	{
		string instr = halxml_readfile(filename);
		if (instr.size() == 0) {
			cerr
					<< "Error! Could not open part of speech file (halxml_readfile): "
					<< filename << endl;
			return 1;
		}

		string prestr;
		halxml_ordertags(instr, prestr);

		if (is_verbose())
			cout << "scan fact file: " << filename << endl;

		idb->set_complete(true);
		idb->set_filename(filename);
		idb->set_add_synonyms(true);
		halxml_readfacts(idb, prestr, filename);

		cout << endl;
	}

	return 0;
}

template<typename DB>
int database<DB>::get_synonyms(vector<word>& syns, const word& w) {
	if (!is_configured())
		return 1;

	syns.push_back(w);

	DB* idb = new DB(this);
	idb->set_add_synonyms(false);
	idb->get_synonyms(syns, w);
	delete idb;

	return 0;
}

template<typename DB>
boost::shared_ptr<xml_obj> database<DB>::insert_synonyms(
		boost::shared_ptr<xml_obj> xobj) {
	if (!is_configured())
		return xobj;

	//cout << "before: " << xobj->print_str() << endl;
	if (xobj->get_mode() == LIST && xobj->get_name() != "truth"
			&& xobj->get_name() != "flags") {
		std::vector<boost::shared_ptr<xml_obj> >& embedded =
				xobj->get_embedded();
		foreach(boost::shared_ptr < xml_obj > &o, embedded)
		{
			if (o->get_mode() == LIST && o->get_name() == "text") {
				vector<word> words;
				o->get_words(words);
				o.reset(new xml_obj(LIST));
				o->set_name("list");
				foreach(word & w, words)
				{
					vector<word> syns;
					this->get_synonyms(syns, w);
					boost::shared_ptr<xml_obj> subtree(new xml_obj(LIST));
					subtree->set_name("synonyms");
					foreach(word & syn, syns)
					{
						boost::shared_ptr<xml_obj> text_obj(new xml_obj(LIST));
						text_obj->set_name("text");
						boost::shared_ptr<xml_obj> t(new xml_obj(TEXT));
						t->set_text(syn.get_word());
						text_obj << t;
						subtree << text_obj;
					}
					o << subtree;
				}
			} else {
				insert_synonyms (o);
			}
		}
	}
	//cout << "after: " << xobj->print_str() << endl;
	return xobj;
}

}
