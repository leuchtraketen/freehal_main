/*
 * This file is part of FreeHAL 2010.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010 Tobias Schulz and contributors.
 * http://freehal.org
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

#include "hal2009.h"

#include "hal2009-xml.h"
#include "hal2009-sql.h"
#include "hal2009-perl5.h"
#include "hal2009-perl6.h"

char* record_to_xml(struct RECORD* r);

static int position_in_insertions = 0;
char* sql_engine = 0;

extern "C" int _debugf(const char* c, ...) {
}

extern "C" {
int halusleep(double seconds) {
    seconds /= 1000;
#ifdef _WIN32
    Sleep((int)(1000*seconds));
#else
    double end_time = time(NULL) + seconds - 0.01;
    while (1) {
        if (seconds >= 1) {
            sleep((unsigned int) seconds);
        } else {
            usleep((int)fmod(seconds*1000000, 1000000));
        }
        seconds = end_time - time(NULL);
        if (seconds <= 0) break;
    }
#endif
}
int halsleep(double seconds) {
    halusleep(seconds*1000);
}
}

int hal2009_add_link (char* link, int key_1, int key_2) {
    sql_add_link(link, key_1, key_2);
}

int hal2009_delete_everything_from(const char* filename) {
    return sql_delete_everything_from(filename);
}

int hal2009_add_filename(const char* filename) {
    return sql_add_filename(filename);
}

int fact_replace_in_source (const char* source, const char* _replacement) {
    string replacement = string(_replacement) + "\n";

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

    ifstream file(filename.c_str());
    if (file) {
        std::vector<std::string> lines;
        stringstream ss;
        ss << file.rdbuf();
        string lines_str = ss.str();
        boost::split(lines, lines_str, boost::is_any_of("\n\r"));

        std::vector<std::string> data;
        int line_number;
        for (line_number = 1; line_number <= lines.size(); ++line_number) {
            if (line_number != line_int) {
                data.push_back(lines[line_number-1]);
            }
            else {
                data.push_back(replacement);
            }
        }
        file.close();

        ofstream file_o(filename.c_str());
        int i;
        for (i = 0; i <= data.size(); ++i) {
            file_o << data[i];
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

    ifstream file(filename.c_str());
    if (file) {
        std::vector<std::string> lines;
        stringstream ss;
        ss << file.rdbuf();
        string lines_str = ss.str();
        boost::split(lines, lines_str, boost::is_any_of("\n\r"));

        int line_number;
        for (line_number = 1; line_number <= lines.size(); ++line_number) {
            if (line_number == line_int) {
                return strdup(lines[line_number-1].c_str());
            }
        }
        file.close();
    }
    else {
        printf("Unable to open file.\n");
        return 0;
    }

    return 0;
}

string& replace (string& in, const string& rep, const string& wit) {
  int pos;
  int last_pos = 0;
  while (true) {
    pos = in.find(rep, last_pos);
    if (pos == -1) {
      break;
    } else {
      in.erase(pos, rep.length());
      in.insert(pos, wit);
      last_pos = pos + wit.size();
    }
  }
  return in;
}

/* UTF8/Latin --> ASCII */
string ascii(const string& not_ascii) {
    string ascii(not_ascii);

    ascii = replace(ascii, "Ã¼", "ue");
	ascii = replace(ascii, "Ã", "ss");
	ascii = replace(ascii, "Ã¤", "ae");
	ascii = replace(ascii, "ü", "ue");
	ascii = replace(ascii, "ß", "ss");
	ascii = replace(ascii, "ä", "ae");
	ascii = replace(ascii, "ö", "oe");
	ascii = replace(ascii, "Ü", "Ue");
	ascii = replace(ascii, "Ä", "Ae");
	ascii = replace(ascii, "Ö", "Oe");
	ascii = replace(ascii, "Ã¢ÂÂ", "\"");
	ascii = replace(ascii, "Ã¢ÂÂ", "\"");
	ascii = replace(ascii, "&lt;/br&gt;", " ");

    string _ascii = ascii;
    ascii = "";
    int x;
    for (x = 0; x < _ascii.size(); x++) {
        short y = (short)(_ascii[x]);

        if (y == 228) {
            ascii += "ae";
        }
        else if (y == 196) {
            ascii += "Ae";
        }
        else if (y == 252) {
            ascii += "ue";
        }
        else if (y == 220) {
            ascii += "Ue";
        }
        else if (y == 246) {
            ascii += "oe";
        }
        else if (y == 214) {
            ascii += "Oe";
        }
        else if (y == 223) {
            ascii += "ss";
        }
        else if (y == 226) {
            x += 2;
        }
        else if (y > 129) {
            x += 1;
        }

        else {
            char _char[2];
            _char[0] = y;
            _char[1] = '\0';
            ascii += _char;
        }
    }

    return ascii;
}

int hal2009_add_xml_file (const string& filename) {
    cout << "Clearing DB: .xml file " << filename << "." << endl;
    sql_begin("faster");
    hal2009_delete_everything_from(filename.c_str());
    hal2009_add_filename(filename.c_str());

    sql_end();

    cout << "Add .xml file " << filename << "." << endl;

    add_xml_fact_file(filename.c_str());

    return 0;
}

struct DATASET hal2009_get_csv(char* csv_request) {
    fprintf(output(), "Get CSV data: %s.\n", csv_request);
    sql_begin("");
    position_in_insertions = 0;

    struct RECORD r;
    char* buffer;

    buffer = strtok(csv_request, "^"); strcpy(r.verb,               (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.subjects,           (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.objects,            (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.adverbs,            (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.extra,              (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.context,            (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.pkey,               (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");

    buffer = strtok(NULL,        "^"); r.verb_flag_want    =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_must    =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_can     =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_may     =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_should  =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);

    buffer = strtok(NULL,        "^");
    if (buffer && 0 == strcmp(buffer, "everything")) {
        r.everything_q = EVERYTHING;
    }

    if (0 == strlen(r.verb)) {
        strcpy(r.verb, "0");
    }
    if (0 == strlen(r.subjects)) {
        strcpy(r.subjects, "0");
    }
    if (0 == strlen(r.objects)) {
        strcpy(r.objects, "0");
    }
    if (0 == strlen(r.adverbs)) {
        strcpy(r.adverbs, "0");
    }
    if (0 == strlen(r.extra)) {
        strcpy(r.extra, "0");
    }
    if (0 == strlen(r.pkey)) {
        strcpy(r.pkey, "0");
    }
    if (0 == strlen(r.context)) {
        strcpy(r.context, "default");
    }

    struct DATASET set;
    set = sql_get_records(&r);

    fprintf(output(), "\n");
    fflush(stdout);
    sql_end();

    return set;
}

char* hal2009_make_csv(struct DATASET* set) {
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
int hal2009_compile(string file, string planguage) {
    if (planguage == "perl6") {
        perl6_convert_file(file.size() > 0 ? file : "hal2009.hal");
    }
    else if (planguage == "perl5") {
        perl5_convert_file(file.size() > 0 ? file : "hal2009.hal");
    }
}

int hal2009_execute_file(string file, string planguage) {
    if (planguage == "perl6") {
        perl6_execute(file.size() > 0 ? file : "hal2009.hal");
    }
    else if (planguage == "perl5") {
        perl5_execute(file.size() > 0 ? file : "hal2009.hal");
    }
}

/*
    The main execute function
*/
int hal2009_execute_code(string code, string planguage) {
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

void hal2009_clean() {
    int number_of_files_to_delete = 13;
    char files_to_delete[15][100] = { "_check_files", "_output", "_output__pos", "_input__pos", "_output__add_pro_file", "_input__add_pro_file", "_output__get_csv", "_input__get_csv", "_exit", "_exit_by_user", "_exit_hal2009_signal_handler", "_do_not_need_a_signal_handler", "_input_server", "_change_text_language" };

    int i;
    for (i = 0; i < number_of_files_to_delete; ++i) {
        unlink(files_to_delete[i]);
    }
}

/*struct params_signal_handler {
    char* planguage;
    char* tlanguage;
    short start_type;
};

void* hal2009_signal_handler(void* parameters) {
    struct params_signal_handler* params = parameters;
    char* planguage  = params->planguage;
    char* tlanguage  = params->tlanguage;
    char* start_type = params->start_type;
    free(params);

    void* temporary_memory = halmalloc(5120, "hal2009_signal_handler");

    if ( stat("_do_not_need_a_signal_handler", temporary_memory) == 0 ) {
        halfree(temporary_memory);
        halfree(planguage);
        halfree(tlanguage);
        return 0;
    }
    FILE* signal_handler_lock = fopen("_do_not_need_a_signal_handler", "w");
    halclose(signal_handler_lock);

    int number_of_files_to_check = 8;
    char files_to_check[9][100] = { "_output", "_output__link", "_output__pos", "_output__add_pro_file", "_output__get_csv", "_exit", "_cgi_request", "_exit_by_user", };
    int i;

    short check_files = 0;
    while ( 1 ) {
        if (check_files > 0) --check_files;

        {
            FILE* source = NULL;
            if ( source = fopen("_check_files", "r") ) {
                check_files += 10;
                halclose(source);
                unlink("_check_files");
            }
        }

        if (check_files <= 0) {
            halusleep(1000);
        }

        for (i = 0; i < number_of_files_to_check; ++i) {
            FILE* source = NULL;
            if ( source = fopen("_change_text_language", "r") ) {
                halusleep(500);
                halclose(source);
                source = fopen("_change_text_language", "r");
                if (source && tlanguage) fread(tlanguage, 1, 2, source);
                halclose(source);
                unlink("_change_text_language");
            }

            if ( source = fopen("_exit_hal2009_signal_handler", "r") ) {
                halclose(source);
                unlink("_exit_hal2009_signal_handler");
                unlink("_do_not_need_a_signal_handler");
                halfree(temporary_memory);
                halfree(tlanguage);
                halfree(planguage);
                return 0;
            }

            if ( source = fopen(files_to_check[i], "r+b") ) {
                halusleep(500);
                halclose(source);
                source = fopen(files_to_check[i], "r+b");
            }
            if ( source ) {
                fprintf(output(), "File opened: %s\n", files_to_check[i]);
                char* code   = calloc(10240+1, 1);
                strcpy(code, "");
                char* buffer = calloc(10240, 1);
                if ( fread(buffer, 1, 10240, source) ) {
                    code[strlen(code)] = 0;
                    strcat(code, buffer);
                    code[strlen(code)] = 0;
                }
                halclose(source);
                if (!strlen(code) && strcmp(files_to_check[i], "_exit")) {
                    halusleep(1000);
                    source = fopen(files_to_check[i], "r+b");
                    fprintf(output(), "File reopened: %s\n", files_to_check[i]);
                    if ( fread(buffer, 1, 10240, source) ) {
                        code[strlen(code)] = 0;
                        strcat(code, buffer);
                        code[strlen(code)] = 0;
                    }
                    halclose(source);
                }
                fprintf(output(), "File closed: %s\n", files_to_check[i]);
                unlink(files_to_check[i]);
                halfree(buffer);

                char* parameters[3] = {strdup(files_to_check[i]), strdup(code), start_type};
                /// pthread_t thread_no_1;
                /// pthread_create (&thread_no_1, NULL, hal2009_handle_signal, (void*)parameters);
                hal2009_handle_signal((void*)parameters);
                halfree(code);

                if (0 == strcmp(files_to_check[i], "_printf")) {
                    unlink("_exit_hal2009_signal_handler");
                    unlink("_do_not_need_a_signal_handler");
                    halfree(temporary_memory);
                }
            }
        }
    }

    halfree(temporary_memory);
    halfree(tlanguage);
    halfree(planguage);
    unlink("_do_not_need_a_signal_handler");
    return 0;
}

pthread_t hal2009_start_signal_handler(char* planguage, char* tlanguage, short start_type) {
    pthread_t thread_no_2;
//    char* parameters[3] = { planguage, tlanguage, start_type };
    struct params_signal_handler* parameters = calloc(sizeof(struct params_signal_handler)+1, 1);
    parameters->planguage = planguage;
    parameters->tlanguage = tlanguage;
    parameters->start_type = start_type;
    pthread_create (&thread_no_2, NULL, hal2009_signal_handler, (void*)parameters);
    return thread_no_2;
}*/

class params_answer {
  public:
    params_answer()
    : input(), planguage(), tlanguage(), base_dir(), start_type(SINGLE)
    {}
  
    string input;
    string planguage;
    string tlanguage;
    string base_dir;
    short start_type;
};

pthread_t hal2009_answer(string _input, string planguage, string tlanguage, string base_dir, int join, short start_type) {
    cout << "input: " << _input << endl;
//    string input = ascii(_input);
string input = _input;
    cout << "input: " << input << endl;

    hal2009_send_signal("input", input.c_str());

    pthread_t thread_no_1;
    params_answer* parameters = new params_answer();
        cout << "input: " << input << endl;
    parameters->input = planguage;
    parameters->planguage = planguage;
    parameters->tlanguage = tlanguage;
    parameters->base_dir = base_dir;
    parameters->start_type = start_type;
    cout << "Start thread now. Language is " << tlanguage << endl;
    pthread_create (&thread_no_1, NULL, hal2009_answer_thread, (void*)parameters);

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
void* hal2009_answer_thread(void* parameters) {
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
    startfile << "compile source hal2009-lang-" << tlanguage << ".hal" << endl;
    startfile << "require source hal2009-lang-" << tlanguage << ".hal" << endl;
    startfile << "compile source hal2009.hal" << endl;
    startfile << "require source hal2009.hal" << endl;
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

void hal2009_init(string planguage, string tlanguage, string base_dir) {

    stringstream startfile;
    startfile << "compile source hal2009-lang-" << tlanguage << ".hal" << endl;
    startfile << "require source hal2009-lang-" << tlanguage << ".hal" << endl;
    startfile << "compile source hal2009.hal" << endl;
    startfile << "require source hal2009.hal" << endl;
    startfile << "set new var current_language to '" << tlanguage << "'" << endl;
    startfile << "set new var path to '" << base_dir << "'" << endl;
    startfile << "do init with var current_language, var path" << endl;

    string content = startfile.str();
    hal2009_execute_code(content, planguage);
}

int hal2009_set_text_language(const char* s) {
    if (!s) {
        return INVALID;
    }
    if (hal2009_text_language) {
        free(hal2009_text_language);
        hal2009_text_language = 0;
    }
    hal2009_text_language = strdup(s);
}

const char* hal2009_get_text_language() {
    if (!hal2009_text_language) {
        hal2009_text_language = strdup("de");
    }
    return hal2009_text_language;
}

char* check_config (const char* name, const char* _default) {
    const char* config_file = "config.txt";
    ifstream i;
    if (!i.is_open()) {
        i.open("config.txt");
        if (i.is_open()) config_file = "config.txt";
    }
    if (!i.is_open()) {
        i.open("../config.txt");
        if (i.is_open()) config_file = "../config.txt";
    }
    if (!i.is_open()) {
        i.open("../../config.txt");
        if (i.is_open()) config_file = "../../config.txt";
    }

    if (i) {
        string temp;
        while (std::getline(i, temp)) {
            if (temp.find(name) != string::npos) {
                temp = replace(temp, " =", "=");
                temp = replace(temp, "= ", "=");
                temp = replace(temp, "\n", "=");
                temp = replace(temp, "\r", "=");
                temp = replace(temp, name, "");
                temp = replace(temp, "=", "");

                i.close();
                if (string(name).find("limit") != string::npos) {
                    cout << config_file << ": " << name << " = " << temp << ", default " << _default << endl;
                }

                return strdup(temp.c_str());
            }
        }
        i.close();
    }

    FILE* o = fopen(config_file, "a");
    if (o) {
        fprintf(o, "%s = %s\n", name, _default);
        fclose(o);
    }

    return strdup(_default);
}

EXTERN_C char* halgetline(FILE *handle) {
    char *line;
    char zeichen;
    int length;

    length = 0;
    line = NULL;
    zeichen = fgetc(handle);

    if(zeichen == '\n') {
        line = (char*)malloc(sizeof(char)*2);
        line[0] = zeichen;
        line[1] = '\0';
        return line;
    }

    if(zeichen == EOF) {
        return NULL;
    }

    line = (char*)malloc(LINE_SIZE+1);
    int index = 0;
    line[index] = zeichen;
    ++index;
    do {
        zeichen = fgetc(handle);
        if(zeichen == '\n') {
            line[index] = zeichen;
            ++index;
            break;
        }

        if (EOF == zeichen) { /* feof() verwenden? */
            break;
        }

        line[index] = zeichen;
        ++index;
    }
    while(1 && index < LINE_SIZE - 1);

    line[index] = 0;

    return line;
}

