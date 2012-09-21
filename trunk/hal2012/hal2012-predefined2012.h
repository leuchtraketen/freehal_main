/*
 * hal2012-parser2012.h
 *
 *  Created on: 01.07.2012
 *      Author: tobias
 */

#ifndef HAL2012_PREDEFINED2012_H_
#define HAL2012_PREDEFINED2012_H_

#include "hal2012-util2012.h"
#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-xml2012.h"
#include "hal2012-parser2012.h"
#include "hal2012-serialization.h"

#define EXTERN_C extern "C"
EXTERN_C char* check_config(const char* name, const char* _default);

namespace grammar2012 {

class predefined;

class predefined: public freehal_base {
private:
	tagger* t;
	grammar* g;

	void try_greeting(const string&, string&);
	void try_thanks(const string&, string&);
	void try_random_question(const string&, string&);
	void try_random_statement(const string&, string&);

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
	}

public:
	predefined();
	~predefined();
	const string get_predefined_output(const string&);
	const string get_random_output(sentence*);

	void set_tagger(tagger*);
	void set_grammar(grammar*);
	tagger* get_tagger() const;
	grammar* get_grammar() const;
};
}

#endif /* HAL2012_PREDEFINED2012_H_ */
