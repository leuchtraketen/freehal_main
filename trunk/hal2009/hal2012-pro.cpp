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

#include "hal2012.h"
#include "hal2012-pro.h"
#include "hal2012-xml.h"
#include "hal2012-util.h"

int hal2012_add_pro_file (const string& filename) {
    string filename_xml = filename;
    filename_xml = replace(filename_xml, ".pro", ".xml");
    ofstream xml_out(filename_xml.c_str());

    cout << "Convert .pro file " << filename << " to .xml file " << filename_xml << "." << endl;
    int position_in_insertions = 0;
    time_t start = 0;
    time(&start);
    ifstream input(filename.c_str());
    cout << " _______________________________" << endl;
    cout << "|                               |" << endl << "|";
    if ( input ) {
        string line;
        int k = 0;
        int line_number = 0;
        while (std::getline(input, line)) {
            ++line_number;

            if (line.find("#") != string::npos) {
                continue;
            }

            line = ascii(line);

            /// Make lower case
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);

            /// Add the fact
            struct RECORD r;
            r.truth = 0.5;

            line = replace(line, "nothing", " ");
            line = replace(line, "nichts", "nothing");
            line = replace(line, "\"", "'");
            line = replace(line, ";;", "^");
            line = replace(line, ";", " ");
            line = replace(line, "()", "");
            line = replace(line, " <> ", "^");
            line = replace(line, "<>", "^");
            line = replace(line, " ^", "^");
            line = replace(line, " ^", "^");
            line = replace(line, " ^", "^");
            line = replace(line, "^ ", "^");
            line = replace(line, "^ ", "^");
            line = replace(line, "^ ", "^");
            line = replace(line, "^^", "^ ^");
            line = replace(line, "^^", "^ ^");
            line = replace(line, "^^", "^ ^");
            line = replace(line, "^(true)", "(true)");
            line = replace(line, "^(maybe)", "(maybe)");
            line = replace(line, "^(false)", "(false)");
            line = replace(line, "\r", "");
            line = replace(line, "\n", "");

            std::vector<std::string> buffer;
            boost::split(buffer, line, boost::is_any_of("^"));
            int g = 0;
            for (;buffer.size() < 5;) { buffer.push_back(string()); }
            for (;(buffer.size()-4)%5 != 0;) { buffer.push_back(string()); }

            strcpy(r.questionword, "");

            strcpy(r.verb, buffer[0].c_str());
            strcpy(r.subjects, buffer[1].c_str());
            strcpy(r.objects, buffer[2].c_str());
            strcpy(r.adverbs, buffer[3].c_str());

            remove_negation(r.verb, &(r.truth), &(r.only_logic));
            remove_negation(r.subjects, &(r.truth), &(r.only_logic));
            remove_negation(r.objects, &(r.truth), &(r.only_logic));
            remove_negation(r.adverbs, &(r.truth), &(r.only_logic));

            strcpy(r.filename, filename.c_str());
            snprintf(r.line, 100, "%d", line_number);
            {
                stringstream ss;
                ss << line_number;
                string _s;
                ss >> _s;
                strcpy(r.line, _s.c_str());
            }
            strcpy(r.extra, "");

            r.verb_flag_want    = 0;
            r.verb_flag_must    = 0;
            r.verb_flag_can     = 0;
            r.verb_flag_may     = 0;
            r.verb_flag_should  = 0;

            if (strstr(r.verb, "/")) {
                if (strstr(r.verb, "/want")) {
                    r.verb_flag_want = 1;
                    strcpy(strstr(r.verb, "/want"), strstr(r.verb, "/want")+5);
                }
                if (strstr(r.verb, "/must")) {
                    r.verb_flag_must = 1;
                    strcpy(strstr(r.verb, "/must"), strstr(r.verb, "/must")+5);
                }
                if (strstr(r.verb, "/can")) {
                    r.verb_flag_can = 1;
                    strcpy(strstr(r.verb, "/can"), strstr(r.verb, "/can")+4);
                }
                if (strstr(r.verb, "/may")) {
                    r.verb_flag_may = 1;
                    strcpy(strstr(r.verb, "/may"), strstr(r.verb, "/may")+4);
                }
                if (strstr(r.verb, "/should")) {
                    r.verb_flag_should = 1;
                    strcpy(strstr(r.verb, "/should"), strstr(r.verb, "/should")+7);
                }
            }

            r.prio = 50;

            int i;
            i = 0;
            while (i+1 < MAX_CLAUSES) {
                r.clauses[i] = 0;
                ++i;
            }
            i = 0;
            r.num_clauses = 0;
            while (buffer.size() > 4+i*5 && i+1 < MAX_CLAUSES) {
                /*if ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))) {
                    if (strlen(buffer) <= 6 && strlen(buffer) >= 2) {
                        if (buffer[0] == '0' || buffer[0] == '1' || buffer[0] == '2' || buffer[0] == '3' || buffer[0] == '4' || buffer[0] == '5' || buffer[0] == '6' || buffer[0] == '7' || buffer[0] == '8' || buffer[0] == '9') {
                            if (buffer[1] == '0' || buffer[1] == '1' || buffer[1] == '2' || buffer[1] == '3' || buffer[1] == '4' || buffer[1] == '5' || buffer[1] == '6' || buffer[1] == '7' || buffer[1] == '8' || buffer[1] == '9') {
                                break;
                            }
                        }
                    }
                }*/

                r.clauses[i] = (struct RECORD*)malloc(sizeof(struct RECORD));
                struct RECORD* sub_clause = (struct RECORD*)r.clauses[i];
                sub_clause->truth = 0.5;

                strcpy(sub_clause->verb, buffer[4+i*5+0].c_str());
                strcpy(sub_clause->subjects, buffer[4+i*5+1].c_str());
                strcpy(sub_clause->objects, buffer[4+i*5+2].c_str());
                strcpy(sub_clause->adverbs, buffer[4+i*5+3].c_str());
                strcpy(sub_clause->questionword, buffer[4+i*5+4].c_str());

                remove_negation(sub_clause->verb, &(sub_clause->truth), &(sub_clause->only_logic));
                remove_negation(sub_clause->subjects, &(sub_clause->truth), &(sub_clause->only_logic));
                remove_negation(sub_clause->objects, &(sub_clause->truth), &(sub_clause->only_logic));
                remove_negation(sub_clause->adverbs, &(sub_clause->truth), &(sub_clause->only_logic));
                remove_negation(sub_clause->questionword, &(sub_clause->truth), &(sub_clause->only_logic));

                strcpy(sub_clause->filename, filename.c_str());
                snprintf(sub_clause->line, 100, "%d", line_number);
                strcpy(sub_clause->extra, "");

                // a common error in malformed pro files
                if (0 == strcmp(sub_clause->questionword, "50")) {
                    free(r.clauses[i]);
                    r.clauses[i] = NULL;
                    break;
                }

                if  (!sub_clause->subjects && !sub_clause->objects && !sub_clause->verb && !sub_clause->questionword) {
                    free(r.clauses[i]);
                    r.clauses[i] = NULL;
                    break;
                }

                sub_clause->verb_flag_want    = 0;
                sub_clause->verb_flag_must    = 0;
                sub_clause->verb_flag_can     = 0;
                sub_clause->verb_flag_may     = 0;
                sub_clause->verb_flag_should  = 0;

                sub_clause->prio  = 50;

                r.clauses[i] = sub_clause;
                r.clauses[i+1] = NULL;
                ++(r.num_clauses);
                ++i;
            }
            r.clauses[i] = NULL;

            if (0 == strlen(r.verb)) {
                continue;
            }
            if (0 == strlen(r.subjects)) {
                strcpy(r.subjects, "");
            }
            if (0 == strlen(r.objects)) {
                strcpy(r.objects, "");
            }
            if (0 == strlen(r.adverbs)) {
                strcpy(r.adverbs, "");
            }

            int err = 0;
            if ((strstr(r.subjects, "_") && strlen(r.subjects) > 2) || (strstr(r.objects, "_") && strlen(r.objects) > 2)) {
                // Modify hash

                strcpy(r.subjects, delete_underscores(r.subjects).c_str());
                strcpy(r.objects, delete_underscores(r.objects).c_str());

                ++num_facts_added_during_this_run;

                char* _xml = record_to_xml(&r);
                xml_out << _xml;
                free(_xml);
            }
            else {
                char* _xml = record_to_xml(&r);
                xml_out << _xml;
                free(_xml);

                if (err) {
                    cout << "Error: " << line << endl;
                }
                ++num_facts_added_during_this_run;
            }

            int k = 0;
            while (k+1 < MAX_CLAUSES && k < i && r.clauses[k]) {
                cxxhalfree(r.clauses[k]);
                ++k;
            }

            ++position_in_insertions;
            if (position_in_insertions % 25 == 0) {
                cout << "." << flush;
            }
            if (position_in_insertions % 750 == 0) {
                time_t now = 0;
                time(&now);
                if (now - start <= 2) {
                    cout << " | ---" << endl << "|";
                }
                else {
                    long int facts_per_second = position_in_insertions / (now - start);
                    cout << " | " << facts_per_second << " facts/sec (" << position_in_insertions << " facts, " << now - start << " sec)" << endl << "|";
                }
                cout << flush;
            }
        }
    }

    int position_in_insertions_simulated = position_in_insertions;
    while (position_in_insertions_simulated % 750 != 0) {
        ++position_in_insertions_simulated;
        if (position_in_insertions_simulated % 25 == 0) {
            cout << " " << flush;
        }
    }

    xml_out.close();

    cout << " | ---" << endl << "|";
    time_t now = 0;
    time(&now);
    long int facts_per_second = position_in_insertions / ((now - start)>0?(now - start):1);
    cout << "_______________________________| " << facts_per_second << " facts/sec (" << position_in_insertions << " facts, " << now - start << " sec)" << endl << endl << flush;

    cout << "Converted " << (num_facts_added_during_this_run > 0 ? num_facts_added_during_this_run : 0) << " facts.";
    cout << endl << "(" << (num_facts_added_during_this_run + num_facts_not_added_during_this_run_because_exist + num_facts_not_added_during_this_run_because_other_error) << " facts)" << endl << endl;

    //hal2012_add_xml_file (filename_xml);

    return 0;
}

int remove_negation (char* _line, double* truth_ref, int* only_logic) {
    string line = _line;

    if (string::npos != line.find("kein")) {
        line = replace(line, "kein", "ein");
        (*truth_ref) = 0.0;
    }
    if (string::npos != line.find(" nicht ")) {
        line = replace(line, " nicht ", " ");
        (*truth_ref) = 0.0;
    }
    if (string::npos != line.find(" nicht") && (line.size()-6) == line.find(" nicht")) {
        line = replace(line, " nicht", " ");
        (*truth_ref) = 0.0;
    }
    if (string::npos != line.find("nicht ")) {
        line = replace(line, "nicht ", " ");
        (*truth_ref) = 0.0;
    }
    if (string::npos != line.find("nicht") && line.size() <= 7) {
        line = replace(line, "nicht", " ");
        (*truth_ref) = 0.0;
    }
    if (line == "nicht") {
        line = replace(line, "nicht", "");
        (*truth_ref) = 0.0;
    }
    if (string::npos != line.find(" not ")) {
        line = replace(line, " not ", "");
        (*truth_ref) = 0.0;
    }
    if (string::npos != line.find(" 50")) {
        line = replace(line, " 50", "");
        line = replace(line, " <> 50", "");
        (*truth_ref) = 0.5;
    }
    if (string::npos != line.find("(true)")) {
        line = replace(line, " (true)", "");
        line = replace(line, "(true)", "");
        (*truth_ref) = 1.0;
    }
    if (string::npos != line.find("(maybe)")) {
        line = replace(line, " (maybe)", "");
        line = replace(line, "(maybe)", "");
// don't change anything
//      (*truth_ref) = 0.5;
    }
    if (string::npos != line.find("(false)")) {
        line = replace(line, " (false)", "");
        line = replace(line, "(false)", "");
        (*truth_ref) = 0.0;
    }

    if ((*only_logic) != 1) {
        (*only_logic) = 0;
    }

    if (string::npos != line.find("(logic)")) {
        line = replace(line, " (logic)", "");
        line = replace(line, "(logic)", "");
        (*only_logic) = 1;
    }

    strcpy(_line, line.c_str());
}

string delete_underscores(const string& _text) {
    string text = _text;

    text = replace(text, "_", " ");
    text = replace(text, "  ", " ");
    trim(text);

    return text;
}

