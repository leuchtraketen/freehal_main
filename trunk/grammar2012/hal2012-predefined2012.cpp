/*
 * hal2012-phrase2012.cpp
 *
 *  Created on: 01.07.2012
 *      Author: tobias
 */

#include "hal2012-util2012.h"
#include "hal2012-predefined2012.h"

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace grammar2012 {

predefined::predefined() :
		freehal_base() {
}
predefined::~predefined() {
}

const string predefined::get_predefined_output(const string& input) {
	if (!freehal_base::is_configured()) {
		return "";
	} else if (t == 0) {
		cout << "Error! predefined2012: tagger is undefined." << endl;
		return "";
	} else if (g == 0) {
		cout << "Error! predefined2012: grammar is undefined." << endl;
		return "";
	}

	{
		if (is_verbose())
			cout << "predefined2012: input: " << input << endl;
	}
	string output;

	try_greeting(input, output);
	try_thanks(input, output);

	{
		if (is_verbose() && output.size() > 0)
			cout << "predefined2012: predefined output: " << output << endl;
	}

	return output;
}

const string predefined::get_random_output(sentence* s) {
	if (!freehal_base::is_configured()) {
		return "";
	} else if (t == 0) {
		cout << "Error! predefined2012: tagger is undefined." << endl;
		return "";
	} else if (g == 0) {
		cout << "Error! predefined2012: grammar is undefined." << endl;
		return "";
	}

	{
		if (is_verbose())
			cout << "predefined2012: input: " << s->to_str() << endl;
	}
	string output;

	if (s->get_mode() == QUESTION)
		try_random_question(s->get_input(), output);
	if (s->get_mode() == STATEMENT)
		try_random_statement(s->get_input(), output);

	{
		if (is_verbose() && output.size() > 0)
			cout << "predefined2012: random output: " << output << endl;
	}

	return output;
}

void predefined::try_greeting(const string& input, string& output) {
	if (output.size() > 0)
		return;

	if (regex_ifind(input, "(hallo|hi|hey|mahlzeit|"
			"(gute|guten|schoene|schoenen "
			"tag|morgen|abend|nachmittag|vormittag|nacht))")) {

		boost::posix_time::ptime pt =
				boost::posix_time::second_clock::local_time();
		int hour = pt.time_of_day().hours();
		std::cout << "predefined2012: hour = " << hour;

		if (hour < 5) {
			stringstream ss;
			ss << "Immer noch wach...? Es ist " << hour
					<< " Uhr nachts!";
			output = ss.str();
		} else if (hour < 12)
			output = "Guten Morgen!";
		else if (hour < 17)
			output = "Guten Tag!";
		else
			output = "Guten Abend!";
	}
}

void predefined::try_thanks(const string& input, string& output) {
	if (output.size() > 0)
		return;

	if (regex_ifind(input, "(danke)") && input.size() < 10) {
		vector<string> outputs;
		outputs.push_back("Nichts zu danken.");
		outputs.push_back("Gern geschehen.");
		outputs.push_back("Nichts zu danken...");
		outputs.push_back("Gern geschehen...");
		outputs.push_back("Nichts zu danken!");
		outputs.push_back("Gern geschehen!");
		srand(unsigned(time(NULL)) + (unsigned long)(&outputs));
		std::random_shuffle(outputs.begin(), outputs.end());
		output = outputs[0];
	}

	if (regex_ifind(input, "(bitte)") && input.size() < 10) {
		vector<string> outputs;
		outputs.push_back("Danke!");
		outputs.push_back("Danke.");
		outputs.push_back("Danke...");
		srand(unsigned(time(NULL)) + (unsigned long)(&outputs));
		std::random_shuffle(outputs.begin(), outputs.end());
		output = outputs[0];
	}
}

void predefined::try_random_question(const string& input, string& output) {
	if (output.size() > 0)
		return;

	vector<string> outputs;

	if (regex_ifind(" " + input + " ",
			"((\\sja\\s)|(\\snein\\s)|(\\swas\\s)|(\\swer\\s)|isunknown|(\\swie\\s)|(\\swo\\s)|(\\swann\\s)|(\\swen\\s)|(\\swem\\s)|(\\swelch))")) {
		outputs.push_back("Da kann ich dir leider keine Antwort geben.");
		outputs.push_back("Keine Ahnung.");
		outputs.push_back("Keine Ahnung, das kann ich dir nicht sagen.");
		outputs.push_back("Keine Ahnung, das kann ich nicht sagen.");
		outputs.push_back("Tut mir leid, das weiss ich nicht.");
		outputs.push_back("Sorry, Keine Ahnung.");
		outputs.push_back("Tut mir leid, das kann ich nicht sagen.");
		outputs.push_back("Hm, das kann ich nicht sagen.");
		outputs.push_back("Hm, das kann ich dir nicht sagen.");
		outputs.push_back("Das weiss ich nicht.");
		outputs.push_back("Ich weiss das nicht, kannst du es mir sagen?");
		outputs.push_back("Wenn ich das wuesste...");
		outputs.push_back("Ich stehe gerade auf dem Schlauch.");
		outputs.push_back("Ich stehe wohl gerade auf dem Schlauch.");
		outputs.push_back("Diese Eingabe kann ich nicht verarbeiten.");
		outputs.push_back("Auf diese Eingabe kann ich nicht antworten.");
		outputs.push_back("Darauf kann ich noch nicht antworten.");
		outputs.push_back("Das kann ich leider nicht beantworten.");
		outputs.push_back(
				"Fuer diese Eingabe fehlt mir das notwendige Wissen.");
		outputs.push_back("Auf alles habe ich leider keine Antwort...");
		outputs.push_back("Die Antwort auf diese Eingabe ist mir unbekannt.");
		outputs.push_back("Zur Zeit kann ich diese Eingabe nicht beantworten.");
		outputs.push_back(
				"Momentan ist es mir nicht moeglich diese Eingabe zu beantworten.");
		outputs.push_back(
				"Mein Wissensspeicher ist begrenzt, diese Eingabe kann ich nicht verarbeiten.");
		outputs.push_back(
				"Fuer diese Eingabe ist keine Antwort in meinen Daten vorhanden.");
		outputs.push_back(
				"Tut mir leid, aber auf diese Eingabe habe ich keine korrekte Antwort. ");
		outputs.push_back("Das kann ich momentan noch nicht beantworten.");
		outputs.push_back(
				"Fuer diese Eingabe fehlt mir momentan das notwendige Wissen, um diese genau zu beantworten.");
		outputs.push_back(
				"Bitte nicht boese sein, aber auf diese Eingabe habe ich keine Antwort.");
	}

	else if (regex_ifind(" " + input + " ", "((\\sja\\s)|(\\snein\\s))")) {
		outputs.push_back("Wieso nicht?");
		outputs.push_back("Ja.");
		outputs.push_back("Ja, klar.");
		outputs.push_back("Ja klar.");
		outputs.push_back("Natuerlich.");
	}

	else if (regex_ifind(" " + input + " ",
			"((\\swarum\\s)|(\\swieso\\s)|(\\swes))")) {
		outputs.push_back("Wieso nicht?");
		outputs.push_back("Keine Ahnung.");
		outputs.push_back("Keine Ahnung, das kann ich dir nicht sagen.");
		outputs.push_back("Keine Ahnung, das kann ich nicht sagen.");
		outputs.push_back("Hm, das kann ich nicht sagen.");
		outputs.push_back("Hm, das kann ich dir nicht sagen.");
		outputs.push_back("Das weiss ich nicht.");
		outputs.push_back("Ich weiss das nicht, kannst du es mir sagen?");
	}

	else {
		for (int i = 0; i < 20; ++i)
			outputs.push_back("Nein.");
		for (int i = 0; i < 5; ++i)
			outputs.push_back("Nein!");
		for (int i = 0; i < 5; ++i)
			outputs.push_back("Nein.,.");
		if (regex_ifind(" " + input + " ", "(\\shast\\s)")) {
			outputs.push_back("Nein, habe ich leider verliehen.");
			outputs.push_back("Nein, ich bin nuechtern.");
			outputs.push_back("Nein, ich habe eine Menge Freunde.");
			outputs.push_back("Nein, ich habe viele Freunde und Bekannte.");
		}
		if (regex_ifind(" " + input + " ", "(\\smagst\\s)")) {
			outputs.push_back("Nein, ich mag das nicht.");
		}
		outputs.push_back("Nein eigentlich so gut wie nie.");
		outputs.push_back("Nein, ganz im Gegenteil.");
		outputs.push_back("Nein, ganz und gar nicht.");
		outputs.push_back("Nein, keinesfalls.");
		outputs.push_back("Nein, ich sehe keinen Grund dazu .");
		outputs.push_back(
				"Nein, ich vertreibe mir mit chatten die Langeweile.");
		outputs.push_back("Nein, tu ich sicher nicht.");
		outputs.push_back("Nein, nicht direkt, darueber will ich nicht reden.");
		outputs.push_back("Nein, nur ganz selten.");
		outputs.push_back(
				"Nicht falsch verstehen, aber ich denke nicht das dich das etwas angeht.");
		if (regex_ifind(" " + input + " ", "(\\sviel\\s)")) {
			outputs.push_back("Nicht gerade wenig.");
			outputs.push_back("Nicht mehr und nicht weniger als andere.");
		}
		if (regex_ifind(" " + input + " ", "(\\sfeig)")) {
			outputs.push_back("Nicht feige, nur vorsichtig.");
		}
	}

	srand(unsigned(time(NULL)) + (unsigned long)(&outputs));
	std::random_shuffle(outputs.begin(), outputs.end());
	output = outputs[0];
}

void predefined::try_random_statement(const string& input, string& output) {
	if (output.size() > 0)
		return;

	vector<string> outputs;

	outputs.push_back("Ich habe dich leider nicht verstanden.");
	outputs.push_back("Hmm.");
	outputs.push_back("Ah.");
	outputs.push_back("Aha.");
	outputs.push_back("Hmm.");
	outputs.push_back("Gut.");
	outputs.push_back("Ja.");
	outputs.push_back("Oh.");
	outputs.push_back("Klar.");

	srand(unsigned(time(NULL)) + (unsigned long)(&outputs));
	std::random_shuffle(outputs.begin(), outputs.end());
	output = outputs[0];
}

void predefined::set_tagger(tagger* _t) {
	t = _t;
}
void predefined::set_grammar(grammar* _g) {
	g = _g;
}
tagger* predefined::get_tagger() const {
	return t;
}
grammar* predefined::get_grammar() const {
	return g;
}

}

