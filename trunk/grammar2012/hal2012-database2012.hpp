namespace grammar2012 {

template<typename DB> database<DB>::database() :
		verbose(true), buffered(false) {
}
template<typename DB> database<DB>::~database() {
}

template<typename DB> void database<DB>::set_verbose(bool v) {
	verbose = v;
}
template<typename DB> bool database<DB>::is_verbose() {
	return verbose;
}

template<typename DB> void database<DB>::set_buffered(bool v) {
	buffered = v;
}
template<typename DB> bool database<DB>::is_buffered() {
	return buffered;
}

template<typename DB> void database<DB>::set_lang(const string& _lang) {
	lang = _lang;
	resume();
}
template<typename DB> void database<DB>::set_path(const string& _path) {
	path = _path;
	resume();
}

template<typename DB> const string database<DB>::get_lang() const {
	return lang;
}
template<typename DB> const string database<DB>::get_path() const {
	return path;
}

template<typename DB>
int database<DB>::resume() {
	if (!lang.empty() && !path.empty()) {
		fs::path resumefile = path / "files.dat";
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
		fs::path resumefile = path / "files.dat";
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
	try {
		if (fs::exists(p)) {
			// is p a regular file?
			if (fs::is_regular_file(p)) {
				uintmax_t size = fs::file_size(p);
				if (files.find(p.generic_string()) == files.end()
						|| files.at(p.generic_string()) != size) {

					DB* idb = new DB(this);
					read_xml_fact_file(p, idb);
					idb->to_disk();
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
const fs::path database<DB>::disk_find_file(const string type, T1 a, T1 b, T1 c,
		T2 filename) {
	stringstream ss;
	ss << (path.empty() ? "" : path.generic_string() + "/");
	ss << "db/" << type << "/";
	ss << a << "/" << b << "/" << c;
	fs::path dir(ss.str());
	create_directories(dir);

	ss << "/" << filename;
	fs::path file(ss.str());
	return file;
}

template<typename DB> template<typename M>
int database<DB>::disk_read_file(const fs::path& p, M& list) {

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
		const string& word) {

	DB* idb = new DB(this);
	idb->find_by_word(word);
	idb->copy_to(list);
	delete idb;

	return 0;
}

template<typename DB>
int database<DB>::read_xml_fact_file(const fs::path filename, DB* idb) {
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
		vector<xml_fact*> xml_facts = halxml_readfacts(idb, prestr, filename);

		cout << endl;
	}

	return 0;
}

}
