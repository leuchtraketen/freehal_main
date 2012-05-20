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

#include "hal2012-talk.h"
#include "hal2012-util.h"
#include "hal2012-xml.h"
#include "hal2012-sql.h"
#include "hal2012-perl5.h"
#include "hal2012-perl6.h"
#include "hal2012-startup.h"

string hal2012_text_language;

int hal2012_add_link (char* link, int key_1, int key_2) {
    sql_add_link(link, key_1, key_2);
}

int hal2012_delete_everything_from(const char* filename) {
    return sql_delete_everything_from(filename);
}

int hal2012_add_filename(const char* filename) {
    return sql_add_filename(filename);
}

int fact_replace_in_source (const char* source, const char* _replacement) {
    string replacement = string(_replacement?_replacement:"") + "\n";

    std::vector<std::string> source_parts;
    boost::split(source_parts, source, boost::is_any_of(":"));
    string filename = source_parts[0];
    string line = source_parts[1];

    cout << "File:" << filename << endl;
    cout << "Line:" << line << endl;

    stringstream ss;
    ss << line;
    int line_int;
    ss >> line_int;

    // Manipulate file

    string entry;
    ifstream file(filename.c_str());
    if (file) {
        std::vector<std::string> lines;
        stringstream ss;
        ss << file.rdbuf();
        string lines_str = ss.str();
        lines_str = replace(lines_str, "\r\n", "\n");
        lines_str = replace(lines_str, "\r", "");
        boost::split(lines, lines_str, boost::is_any_of("\n"));

        std::vector<std::string> data;
        int line_number;
        int fact_number = 0;
        for (line_number = 1; line_number <= lines.size(); ++line_number) {
            if (strstr(lines[line_number-1].c_str(), "<fact>")) {
                ++fact_number;
            }
            if (fact_number == line_int) {
                static int added = 0;
                if (_replacement && added) {
                    added = 1;
                    data.push_back(replace(replacement, "\\n", "\n"));
                }
            }
            else {
                lines[line_number-1] = replace(lines[line_number-1], "\n", "");
                data.push_back(lines[line_number-1]);
            }
        }
        file.close();

        ofstream file_o(filename.c_str());
        int i;
        for (i = 0; i < data.size(); ++i) {
            file_o << data[i] << endl;
        }
        file_o.close();
    }
    else {
        printf("Unable to open file.\n");
        return 0;
    }

    return 0;
}

int fact_delete_from_source (const char* source) {
    int r = fact_replace_in_source (source, 0);
    return r;
}

char* fact_read_from_source (const char* source) {
    std::vector<std::string> source_parts;
    boost::split(source_parts, source, boost::is_any_of(":"));
    string filename = source_parts[0];
    string line = source_parts[1];

    cout << "File:" << filename << endl;
    cout << "Line:" << line << endl;

    stringstream ss;
    ss << line;
    int line_int;
    ss >> line_int;

    // Manipulate file

    string entry;
    ifstream file(filename.c_str());
    if (file) {
        std::vector<std::string> lines;
        stringstream ss;
        ss << file.rdbuf();
        string lines_str = ss.str();
        boost::split(lines, lines_str, boost::is_any_of("\n\r"));

        int line_number;
        int fact_number = 0;
        for (line_number = 1; line_number <= lines.size(); ++line_number) {
            if (strstr(lines[line_number-1].c_str(), "<fact>")) {
                ++fact_number;
            }
            if (fact_number == line_int) {
                entry += lines[line_number-1] + "\\n";
            }
//            if (line_number == line_int) {
//                return strdup(lines[line_number-1].c_str());
        }
        file.close();
        return strdup(entry.c_str());
    }
    else {
        printf("Unable to open file.\n");
        return 0;
    }

    return 0;
}

int hal2012_add_xml_file (const string& filename) {
    cout << "Clearing DB: .xml file " << filename << "." << endl;
    sql_begin("faster");
    hal2012_delete_everything_from(filename.c_str());
    hal2012_add_filename(filename.c_str());

    sql_end();

    cout << "Add .xml file " << filename << "." << endl;

    add_xml_fact_file(filename.c_str());

    return 0;
}

const string filter_empty_str(const string& s) {
    return (s.size() && s != " " && s != "*")?s:string();
}

int filter_empty_int(const string& _s) {
    const string& s = filter_empty_str(_s);
    if (!s.size()) return 0;
    stringstream sst;
    sst << s;
    int i;
    sst >> i;
    return i;
}

struct DATASET hal2012_get_csv(const char* csv_request) {
    fprintf(output(), "Get CSV data: %s.\n", csv_request);
    sql_begin("");

    struct RECORD r;

    std::vector<std::string>* buffer = simple_split(csv_request, "^");
    if (buffer && buffer->size() >= 13) {
        strcpy(r.verb, "");
        strcpy(r.subjects, "");
        strcpy(r.objects, "");
        strcpy(r.adverbs, "");
        strcpy(r.extra, "");
        strcpy(r.context, "");
        strcpy(r.pkey, "");

        r.verb_flag_want = 0;
        r.verb_flag_must = 0;
        r.verb_flag_can = 0;
        r.verb_flag_may = 0;
        r.verb_flag_should = 0;
    }
    else {
        strcpy(r.verb, filter_empty_str(buffer->at(0)).c_str());
        strcpy(r.subjects, filter_empty_str(buffer->at(1)).c_str());
        strcpy(r.objects, filter_empty_str(buffer->at(2)).c_str());
        strcpy(r.adverbs, filter_empty_str(buffer->at(3)).c_str());
        strcpy(r.extra, filter_empty_str(buffer->at(4)).c_str());
        strcpy(r.context, filter_empty_str(buffer->at(5)).c_str());
        strcpy(r.pkey, filter_empty_str(buffer->at(6)).c_str());

        r.verb_flag_want = filter_empty_int(buffer->at(7));
        r.verb_flag_must = filter_empty_int(buffer->at(8));
        r.verb_flag_can = filter_empty_int(buffer->at(9));
        r.verb_flag_may = filter_empty_int(buffer->at(10));
        r.verb_flag_should = filter_empty_int(buffer->at(11));

        if (buffer->size() > 12 && buffer->at(12) == "everything") {
            r.everything_q = EVERYTHING;
        }
    }

    if (0 == strlen(r.verb)) strcpy(r.verb, "0");
    if (0 == strlen(r.subjects)) strcpy(r.subjects, "0");
    if (0 == strlen(r.objects)) strcpy(r.objects, "0");
    if (0 == strlen(r.adverbs)) strcpy(r.adverbs, "0");
    if (0 == strlen(r.extra)) strcpy(r.extra, "0");
    if (0 == strlen(r.pkey)) strcpy(r.pkey, "0");
    if (0 == strlen(r.context)) strcpy(r.context, "default");

    struct DATASET set;
    set = sql_get_records(&r);

    fprintf(output(), "\n");
    fflush(stdout);
    sql_end();

    return set;
}

char* hal2012_make_csv(struct DATASET* set) {
    if (set->err == TOOMUCH) {
        return strdup("/err:TOOMUCH\n");
    }

    fprintf(output(), "Compute CSV data, %li records, %li columns\n", set->size, set->column_count);
    int j;
    int m;
    long int size = 4096 + 1*set->size;
    long int size_raw = 0;
    for (j = 0; j < set->size; ++j) {
        if (set->data[j]) {
            for (m = 0; m < set->column_count; ++m) {
                if (set->data[j][m]) {
                    /// fprintf(output(), "Data: %s\n", set->data[j][m]);
                    size     += strlen(set->data[j][m]) + 10;
                    size_raw += strlen(set->data[j][m]);
                }
            }
        }
    }

    fprintf(output(), "I need %li kB of memory, I have %li bytes of raw data\n", size/1024, size_raw);
    fprintf(output(), "Allocate it.\n", size/1024);
    char* csv_data = (char*)calloc(size, 1);
    if (0 == csv_data) {
        fprintf(output(), "Out of memory. I'll return nothing.\n");
        csv_data = (char*)calloc(2, 1);
        return csv_data;
    }
    fprintf(output(), "Copy data.\n");
    int records_count = 0;
    for (j = 0; j < set->size; ++j) {
        if (set->data[j] && set->data[j][0] && *(set->data[j][0])) {
            for (m = 0; m < set->column_count; ++m) {
                if (set->data[j][m]) {
                    /*
                    int p;
                    for (p = 0; p < strlen(set->data[j][m]); ++p) {
                        if (set->data[j][m][p] == '\n' || set->data[j][m][p] == '\r') {
                            set->data[j][m][p] = ' ';
                        }
                    }
                    */
                    strcat(csv_data, set->data[j][m]);
                    free(set->data[j][m]);
                }
                if (m+1 < set->column_count) {
                    strcat(csv_data, "^");
                }
            }
            strcat(csv_data, "\n");
            ++records_count;
        }
        if (set->data[j]) {
            free(set->data[j]);
        }
    }
    fprintf(output(), "Data copied (%d records).\n", records_count);

    return csv_data;
}

/*
    The main compile function
*/
int hal2012_compile(const string& file, const string& planguage) {
    if (planguage == "perl6") {
        perl6_convert_file(file.size() > 0 ? file : "hal2012.hal");
    }
    else if (planguage == "perl5") {
        perl5_convert_file(file.size() > 0 ? file : "hal2012.hal");
    }
}

int hal2012_execute_file(const string& file, const string& planguage) {
    if (planguage == "perl6") {
        perl6_execute(file.size() > 0 ? file : "hal2012.hal");
    }
    else if (planguage == "perl5") {
        perl5_execute(file.size() > 0 ? file : "hal2012.hal");
    }
}

/*
    The main execute function
*/
int hal2012_execute_code(const string& code, const string& planguage) {
    ofstream codefile("temp.hal");
    codefile << code;
    codefile.close();

    if (planguage == "perl6") {
        perl6_execute("temp.hal");
    }
    else if (planguage == "perl5") {
        perl5_execute("temp.hal");
    }
}

void hal2012_clean() {
    int number_of_files_to_delete = 13;
    char files_to_delete[15][100] = { "_check_files", "_output", "_output__pos", "_input__pos", "_output__add_pro_file", "_input__add_pro_file", "_output__get_csv", "_input__get_csv", "_exit", "_exit_by_user", "_exit_hal2012_signal_handler", "_do_not_need_a_signal_handler", "_input_server", "_change_text_language" };

    int i;
    for (i = 0; i < number_of_files_to_delete; ++i) {
        unlink(files_to_delete[i]);
    }
}

pthread_t hal2012_answer(const string& _input, const string& planguage, const string& tlanguage, const string& base_dir, int join, short start_type) {
    cout << "input: " << _input << endl;
    string input = _input;
    cout << "input: " << input << endl;

    hal2012_send_signal("input", input.c_str());

    hal2012_set_text_language(tlanguage);

    pthread_t thread_no_1;
    params_answer* parameters = new params_answer();
        cout << "input: " << input << endl;
    parameters->input = planguage;
    parameters->planguage = planguage;
    parameters->tlanguage = tlanguage;
    parameters->base_dir = base_dir;
    parameters->start_type = start_type;
    cout << "Start thread now. Language is " << tlanguage << endl;
    pthread_create (&thread_no_1, NULL, hal2012_answer_thread, (void*)parameters);

    if ( join == JOIN ) {
        pthread_join(thread_no_1, NULL);
    }
    //sleep(1);

    return thread_no_1;
}

/*
    A Thread invoked by the main answer function

    input: input from user
    planguage: the programming language
    tlanguage: the text (=input) language
*/
void* hal2012_answer_thread(void* parameters) {
    params_answer* params = (params_answer*)parameters;
    string input      = params->input;
    string planguage  = params->planguage;
    string tlanguage  = params->tlanguage;
    string base_dir   = params->base_dir;
    short start_type  = params->start_type;
    delete(params);

    if (planguage.size() == 0 || tlanguage.size() == 0) {
        cout << "planguage.size() == 0 || tlanguage.size() == 0" << endl;
        return 0;
    }

    static int stuck_here = 0;
    while (stuck_here) {
        halsleep(1);
    }
    stuck_here = 1;

    ofstream flowchart_log((base_dir + "/flowchart.log").c_str());
    flowchart_log.close();

    stringstream ss;
    ss << start_type;
    string start_type_str;
    ss >> start_type_str;

    string startname = base_dir + "/temp_" + tlanguage + "_" + start_type_str + ".hal";
    ofstream startfile(startname.c_str());
    startfile << "compile source hal2012-lang-" << tlanguage << ".hal" << endl;
    startfile << "require source hal2012-lang-" << tlanguage << ".hal" << endl;
    startfile << "compile source hal2012.hal" << endl;
    startfile << "require source hal2012.hal" << endl;
    startfile << "set new var current_language to '" << tlanguage << "'" << endl;
    startfile << "set new var path to '" << base_dir << "'" << endl;
    if (start_type == SINGLE) {
        startfile << "do init with var current_language, var path" << endl;
    }
    startfile << "do shell with var current_language, var path" << endl;
    startfile << "do exit with 0" << endl;
    startfile.close();

    if (planguage == "perl6") {
        perl6_execute(startname);
    }
    else if (planguage == "perl5") {
        perl5_execute(startname);
    }

    stuck_here = 0;
    pthread_exit(0);

    return 0;
}

void hal2012_init(const string& planguage, const string& tlanguage, const string& base_dir) {

    stringstream startfile;
    startfile << "compile source hal2012-lang-" << tlanguage << ".hal" << endl;
    startfile << "require source hal2012-lang-" << tlanguage << ".hal" << endl;
    startfile << "compile source hal2012.hal" << endl;
    startfile << "require source hal2012.hal" << endl;
    startfile << "set new var current_language to '" << tlanguage << "'" << endl;
    startfile << "set new var path to '" << base_dir << "'" << endl;
    startfile << "do init with var current_language, var path" << endl;

    string content = startfile.str();
    hal2012_execute_code(content, planguage);
}

int hal2012_set_text_language(const string& s) {
    if (s.size()==0) {
        return INVALID;
    }
    hal2012_text_language = s;
    return 0;
}

const string& hal2012_get_text_language() {
    if (hal2012_text_language.size()==0) {
        hal2012_text_language = "de";
    }
    return hal2012_text_language;
}

EXTERN_C char* generalize_verb(char* _verb) {
    string verb = _verb;

    {
        string verb_1 = "=|bi|is|bin|bist|ist|sind|seid|heisst|heisse|heissen|sei|war|wurde|wurden|werden|werde|wirst|wurdest|wurde|wuerdet|werdet|is|am|are";
        if (("|" + verb_1 + "|").find("|" + verb) != string::npos) {
            return strdup(verb_1.c_str());
        }
    }

    {
        string filename = "lang_" + string(hal2012_get_text_language()) + "/verbs.csv";
        ifstream file(filename.c_str());
        if (file.is_open()) {
            string buffer;
            while (getline(file, buffer)) {
                trim(buffer);

                char* to_return = 0;
                if ((","+buffer+",").find(","+verb+",") != string::npos) {
                    buffer = replace(buffer, ",", "|");
                    return(strdup(buffer.c_str()));
                }
            }
            file.close();
        }
    }
    return (strdup(_verb));
}

