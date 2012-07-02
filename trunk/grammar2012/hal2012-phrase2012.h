/*
 * hal2012-parser2012.h
 *
 *  Created on: 01.07.2012
 *      Author: tobias
 */

#ifndef HAL2012_PHRASE2012_H_
#define HAL2012_PHRASE2012_H_

#include "hal2012-util2012.h"
#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-xml2012.h"
#include "hal2012-database2012.h"
#include "hal2012-serialization.h"

#define EXTERN_C extern "C"
EXTERN_C char* check_config(const char* name, const char* _default);

namespace grammar2012 {

class phraser;

class phraser: public freehal_base {
private:
	tagger* t;
	grammar* g;

	const string join(vector<word>&);
	void arrange(vector<word>&, boost::shared_ptr<xml_fact>);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
	}

public:
	phraser();
	~phraser();
	const string phrase(boost::shared_ptr<xml_fact>);

	void set_tagger(tagger*);
	void set_grammar(grammar*);
	tagger* get_tagger() const;
	grammar* get_grammar() const;
};

vector<word>& operator <<(vector<word>&,
		const vector<boost::shared_ptr<xml_obj> >&);

vector<word>& operator <<(vector<word>&, boost::shared_ptr<xml_obj>);

}

#endif /* HAL2012_PARSER2012_H_ */
