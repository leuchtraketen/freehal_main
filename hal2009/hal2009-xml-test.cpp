#include "hal2009-xml.h"

using namespace std;

int main() {

    string instr = halxml_readfile("example.dat");
    if (instr.size() == 0) {
        cerr << "Error: file could not be opened " << endl;
        exit(1);
    }
    string prestr;
    halxml_ordertags(instr, prestr);

    cout << prestr;
    vector<XML_Fact*> xml_facts = halxml_readfacts(prestr);

    int k;
    for (k = 0; k < xml_facts.size(); ++k) {
        XML_Fact* xfact = xml_facts[k];
        cout << xfact->print_xml();
        delete(xfact);
    }

    return 0;
}

extern "C" struct fact* add_fact(const char* subjects, const char* objects, const char* verbs, const char* adverbs, const char* extra, const char* questionword, const char* filename, const char* line, float truth, short verb_flag_want, short verb_flag_must, short verb_flag_can, short verb_flag_may, short verb_flag_should, short only_logic, short has_conditional_questionword) {}

