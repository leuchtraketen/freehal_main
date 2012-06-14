namespace grammar2012 {

template<typename DB>
int halxml_readfacts(DB* d, const string& prestr,
		const fs::path& filename_ref) {

	time_t start = 0;
	time(&start);

	vector<string> lines;
	split_lines(lines, prestr);

	int i;
	int xml_facts_size = 0;
	for (i = 0; i < lines.size(); ++i) {
		if (lines[i] == "<") {
			++i;
			if (lines[i] == "fact") {
				++xml_facts_size;
			}
		}
	}
	i = 0;
	int xml_facts_so_far = 0;
	while (i < lines.size()) {
		if (lines[i] == "<") {
			++i;
			if (lines[i] == "fact") {
				++i;
				boost::shared_ptr<xml_fact> xml_fact(halxml_readxml_fact(lines, i));

				use_xml_fact(d, xml_fact, xml_facts_size, &start,
						filename_ref, xml_facts_so_far);
				++xml_facts_so_far;
			}
		}
		++i;
	}

	return xml_facts_so_far;
}

template<typename DB>
int use_xml_fact(DB* d, boost::shared_ptr<xml_fact> xfact, int xml_facts_size, time_t* start_ref,
		const fs::path& filename_ref, int k) {
	xfact->filename = filename_ref;
	stringstream sst;
	sst << k + 1;
	sst >> xfact->line;

	if (k + 1 == xml_facts_size || ((k + 1) % 200) == 0) {
		time_t now = 0;
		time(&now);
		if (now - *start_ref == 0) {
			now = *start_ref + 1;
		}
		{
			long int xml_facts_per_second = k / (now - *start_ref);
			static long int time_needed = 1;
			time_needed = time_needed * 0.5
					+ 0.5 * (xml_facts_size - k) / (xml_facts_per_second + 1);

			cout << (k + 1) << " of " << xml_facts_size << " added ("
					<< xml_facts_per_second << " facts / sec, "
					<< now - *start_ref << " sec, " << time_needed
					<< " sec needed, " << (100.0 / xml_facts_size * (k + 1))
					<< "% done)          " << (d->is_buffered() ? "\n" : "\r")
					<< flush;
		}
	}
	if (k && ((k + 1) % 15000) == 0) {
		cout << endl;
	}

	d->insert_fact(xfact);

	return 0;
}


}
