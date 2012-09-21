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
#include "hal2012-xml2012.h"
#include "hal2012-util2012.h"

#ifndef HAL2012_CONSEQUENCE
#define HAL2012_CONSEQUENCE 1

namespace grammar2012 {

class consequence;

class consequence {
private:
	bool valid;
	boost::shared_ptr<xml_obj> cause;
	boost::shared_ptr<xml_obj> effect;

public:
	consequence();
	consequence(const consequence&);
	consequence(boost::shared_ptr<xml_fact>);
	consequence(boost::shared_ptr<xml_obj>);
	consequence* assign(boost::shared_ptr<xml_fact>);
	consequence* assign(boost::shared_ptr<xml_obj>);
	consequence* assign(xml_obj&);

	static bool match(boost::shared_ptr<consequence>, boost::shared_ptr<xml_fact>);
	static bool match(boost::shared_ptr<consequence>, boost::shared_ptr<xml_obj>);

	bool operator ==(xml_obj&) const;
	bool operator ==(boost::shared_ptr<xml_obj>) const;

	bool is_valid() const;
	string print_str() const;
	string print_xml() const;

	static const vector<word> get_if_words_list();
	static boost::shared_ptr<xml_obj> get_if_words_xmlobj();
};

}

#endif /* HAL2012_CONSEQUENCE */

