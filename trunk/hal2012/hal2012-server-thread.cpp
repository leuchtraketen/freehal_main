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
#include "hal2012-server-thread.h"
#include "hal2012-startup.h"
#include "hal2012-util.h"
#include "hal2012-talk.h"
#include "hal2012-ipc.h"
#include "hal2012-pro.h"
#include "hal2012-sql.h"
#include "hal2012-disk.h"
#include "hal2012-grammar2012.h"

#include <pthread.h>

#ifdef WINDOWS
#include <windows.h>
int win32_bind (SOCKET s, const struct sockaddr* addr, int);
namespace boost {
//    #include <sys/socket.h>
}
#endif

EXTERN_C void extract();

extern instance* current_instance;
string output_by_signal;
vector<string>* init_xml_files;
int init_thread_ended = 0;

/// Normal FreeHAL communication with GUIs

void hal2012_server_start() {
}

void hal2012_server_statement(const string& s, const string& username, bool do_learn, bool do_talk) {
    cout << "Begin of statement process." << endl;
    static string* to_display_talk = 0;
    static string* to_display_learn = 0;
    if (!to_display_talk) {
        to_display_talk = new string;
        time_t t;
        time(&t);
        char* time = ctime(&t);
        time[strlen(time)-1] = 0;
        (*to_display_talk) += "<i>" + string(time) + "</i><br /><br/>";
    }
    if (!to_display_learn) {
        to_display_learn = new string;
        time_t t;
        time(&t);
        char* time = ctime(&t);
        time[strlen(time)-1] = 0;
        (*to_display_learn) += "<i>" + string(time) + "</i><br /><br/>";
    }
    string* to_display = (do_learn) ? to_display_learn : to_display_talk;
    cout << "Initialize variables." << endl;

    if (s == "/q" || s == "/quit" || s == "/e" || s == "/exit" || s == "Beenden" || s == "beenden" || s == "exit" || s == "Exit" || s == "quit" || s == "Quit") {
        halusleep(5000);
        exit(0);
    }
    if (s == "/del fact" || s == "/del fakt" || s == "/DEL FAKT" || s == "/DEL FACT" || s == "/df" || s == "/DF" || s == "/d f" || s == "/D F") {
        struct RECORD r;
        strcpy(r.pkey, "a");
        printf("pkey (in hal2012-server, 2): %s\n", r.pkey);
        sql_begin("");
        char* source = sql_del_record(&r);
        if (source) {
            fact_delete_from_source(source);
            free(source);
        }
        sql_end();

        (*to_display) += "<b>" + username + "</b>: " + s + "<br />";
        (*to_display) += "<b>FreeHAL</b>: Deleted.<br />";
        cout << "Got something to display from '" << username << "'." << endl;
        cout << "    " << (*to_display) << endl;
        cerr << "DISPLAY:" << (*to_display) << endl;
        unlink("_output2");
        hal2012_clean();
        return;
    }
    if (s == "de" || s == "deutsch" || s == "Deutsch" || s == "german" || s == "German" || s == "Deutsch!" || s == "deutsch!") {
        current_instance->tlanguage = "de";

        (*to_display) += "<b>" + username + "</b>: " + s + "<br />";
        (*to_display) += "<b>FreeHAL</b>: Language is " + current_instance->tlanguage + "<br />";
        cout << "Got something to display from '" << username << "'." << endl;
        cout << "    " << (*to_display) << endl;
        cerr << "DISPLAY:" << (*to_display) << endl;
        unlink("_output2");
        hal2012_clean();
        return;
    }
    if (s == "en" || s == "english" || s == "English" || s == "englisch" || s == "Englisch" || s == "English!" || s == "english!") {
        current_instance->tlanguage = "en";

        (*to_display) += "<b>" + username + "</b>: " + s + "<br />";
        (*to_display) += "<b>FreeHAL</b>: Language is " + current_instance->tlanguage + "<br />";
        cout << "Got something to display from '" << username << "'." << endl;
        cout << "    " << (*to_display) << endl;
        cerr << "DISPLAY:" << (*to_display) << endl;
        unlink("_output2");
        hal2012_clean();
        return;
    }

    {
        ofstream change_text_language("_change_text_language");
        change_text_language << current_instance->tlanguage;
        change_text_language.close();
    }

    string input;
    if (do_learn) {
        if (do_talk) {
            input += "do_learn_do_talk: ";
        }
        else {
            input += "do_learn_no_talk: ";
        }
    }
    else {
        if (do_talk) {
            input += "no_learn_do_talk: ";
        }
        else {
            input += "no_learn_no_talk: ";
        }
    }
    input += s;

    string output;
    int timeout = 3;
    static string last_input = string();
    static long last_input_time = 0;
    while (output.size() == 0 && timeout > 0) {

        time_t now;
        time(&now);
        cout << "input: now:  " << input      << endl;
        cout << "input: last: " << last_input << endl;

        if (input.size() > 0 && (now - last_input_time) <= 3600 && last_input == input ) {
            return;
        }
        last_input = input;
        last_input_time = now;

        cout << "Delete nonsense." << endl;
        hal2012_clean();
        output_by_signal = "";

        if (!do_talk) {
            (*to_display) += "<b>Input</b>: " + s + "<br />";
            replace_all((*to_display), "\r", "");
            replace_all((*to_display), "\n", "<br />");
            cerr << "LEARNED:0:" << (*to_display) << endl;
        }
        else {
            (*to_display) += "<b>" + username + "</b>: " + s + "<br />";
            replace_all((*to_display), "\r", "");
            replace_all((*to_display), "\n", "<br />");
            cerr << "DISPLAY:" << (*to_display) << endl;
        }

        cout << "Start threads (language: " << current_instance->tlanguage << "). input=\"" << input << "\"" << endl;
        pthread_t answer_thread = hal2012_answer(input, current_instance->planguage, current_instance->tlanguage, current_instance->base_dir, NOT_JOIN, MULTI);
        pthread_join(answer_thread, NULL);


        while (output_by_signal.size() == 0) {
            halusleep(100);
        }

        output = output_by_signal;

        --timeout;
        hal2012_clean();
    }
    if (output.size() == 0) {
        output = "<i>FreeHAL sleeps... Unable to get an answer. Try again later.</i>";
    }
    else {
        last_input_time = 0;
    }

    if (!do_talk) {
        (*to_display) += "<b>Learned</b>: " + output + "<br />";
        replace_all((*to_display), "\r", "");
        replace_all((*to_display), "\n", "<br />");
        cerr << "LEARNED:1:" << (*to_display) << endl;
    }
    else {
        (*to_display) += "<b>FreeHAL</b>: " + output + "<br />";

        cout << "Got something to display from '" << username << "'." << endl;
        cout << "    " << (*to_display) << endl;
        cerr << "DISPLAY:" << (*to_display) << endl;
    }

    hal2012_clean();
    cout << "End of statement process." << endl;
}

int get_file_size(const char* filename) {
    struct stat* stbuf = (struct stat*)malloc(50000);
    stat(filename, stbuf);
    int file_size = stbuf ? stbuf->st_size : 0;
    free(stbuf);
    return file_size;
}

void hal2012_server_client_connection() {
    ofstream booted_file("booted");
    booted_file.close();
    vector<boost::thread*>* threads = new vector<boost::thread*>();

    cerr << "READY:EVERYTHING_INITIALIZED" << endl;
    cerr << "JELIZA_FULL_VERSION:" << FULL_VERSION << endl;

    /// for svn versions
    cerr << "NAME:" << FULL_NAME << " SVN-Rev. " << FULL_VERSION << " (database " << (current_instance->sql_engine == "ram" ? "in memory" : (current_instance->sql_engine == "disk" ? "at disk" : "at disk (traditional)")) << ")" << endl;

    /// for stable versions
    /// cerr << "NAME:" << FULL_NAME << " Step 2   24.12.09   (database " << (0 == strcmp(sql_engine, "ram") ? "in memory" : (0 == strcmp(sql_engine, "disk") ? "at disk" : "at disk (traditional)")) << ")" << endl;

    cerr << "Perl:." << endl;

    string flags;
    getline(cin, flags);
    if (strstr(flags.c_str(), "init_thread_ended=1")) {
        init_thread_ended = 1;
    }
    cerr << "init_thread_ended=" << init_thread_ended << endl;

    string username;
    getline(cin, username);
    replace_all(username, "USER:", "");
    trim(username);

    cout << "Got username." << endl;
    cout << "    " << username << endl;

    if (init_thread_ended == 0) {
        init_thread_ended = -1;

        init_xml_files = new vector<string>();

        pthread_t thread_init;
        pthread_create (&thread_init, NULL, hal2012_init_thread, (void*)NULL);
    }

    int time = 0;
    while (init_thread_ended <= 0) {
        if (time % 5000 == 0 || !time) {
            cerr << "DISPLAY:" << "FreeHAL reads the database. This can take some minutes.";
            if (init_xml_files->size() > 1) cerr << "<br><br><b>completed files</b>:<br>";
            int k;
            for (k = 0; k < init_xml_files->size(); ++k) {
                cerr << "<br>";
                cerr << (k+1 == init_xml_files->size() ? "<br><b>in progress</b>: " : "- " );

                std::vector<std::string>* path_splitted = simple_split(init_xml_files->at(k), "/");
                cerr << path_splitted->at(path_splitted->size()-1);
                delete(path_splitted);
            }
            cerr << endl;
        }

        halusleep(250);
        time += 250;
    }

    if (init_xml_files) { delete(init_xml_files); init_xml_files = 0; }

    cerr << "DISPLAY:" << "<i> </i>" << endl;

    string line;
    while (cin && getline(cin, line)) {
        ofstream booted_file("booted");
        booted_file.close();

        std::vector<std::string>* result = simple_split( line, ":" );

        std::vector<std::string>* result_underscore = simple_split( line, "_" );
        if ( result_underscore->at(0) == string("HERE") && result->size() >= 2 ) {
            cout << "Got a HERE_IS_* statement at the wrong place in code" << endl;
        }

        if ( result->at(0) == string("EXIT") ) {
            halusleep(5000);
            exit(0);
        }

        if ( result->at(0) == string("CSV") ) {
            char* request = strdup(result->at(1).c_str());
            struct DATASET set = hal2012_get_csv(request);
            char* csv_data = (char*)hal2012_make_csv(&set);
            ofstream out_csv_data("_csv-data.txt");
            int pos;
            int last_pos = 0;
            for (pos = 0; csv_data[pos]; ++pos) {
                if (csv_data[pos] == '\n') {
                    csv_data[pos] = '\0';

                    out_csv_data << csv_data + last_pos << endl;
                    cout << "CSV_RESULT:" << csv_data + last_pos << endl;

                    if (csv_data[pos+1]) last_pos = pos+1;
                    else                 last_pos = -1;
                }
            }
            if (last_pos >= 0) {
                out_csv_data << csv_data + last_pos << endl;
                cout << "CSV_RESULT:" << csv_data + last_pos << endl;
            }
            out_csv_data.close();
            cerr << "CSV_RESULT:." << endl;
            if (request) free(request);
            if (csv_data) free(csv_data);
        }

        if ( result->at(0) == string("DELETE") && result->at(1) == string("FACT") && result->at(2) == string("PK") ) {
            struct RECORD r;
            strcpy(r.pkey, result->at(3).c_str());
            printf("pkey (in hal2012-server, 1): %s\n", r.pkey);
            sql_begin("");
            char* source = sql_del_record(&r);
            if (source) {
                fact_delete_from_source(source);
                free(source);
            }
            sql_end();
            cerr << "DELETED:SUCCESS" << endl;
        }

        if ( result->at(0) == string("REINDEX") ) {
            printf("Running re-index function.\n");
            printf("Start.\n");
            sql_begin("");
            sql_re_index();
            sql_end();
            printf("Stop.\nNew Index.\n");
            cerr << "REINDEX:SUCCESS" << endl << "REINDEX:SUCCESS" << endl;
        }

        if ( result->at(0) == string("READ") && result->at(1) == string("XML") ) {
            hal2012_add_xml_file(result->at(2));
        }

        if ( result->at(0) == string("LIST") && result->at(1) == string("XML") ) {
            hal2012_add_xml_file(result->at(2));
        }

        if ( result->at(0) == string("GET") && result->at(1) == string("PROFACT") && result->at(2) == string("PK") ) {
            struct RECORD r;
            strcpy(r.pkey, result->at(3).c_str());
            while (r.pkey[0] && (r.pkey[strlen(r.pkey)-1] == '\r' || r.pkey[strlen(r.pkey)-1] == '\n')) {
                r.pkey[strlen(r.pkey)-1] = '\0';
            }
            printf("get .pro entry (in hal2012-server, 1): %s\n", r.pkey);
            sql_begin("");
            char* source = sql_get_source(&r);
            printf("source: '%s'\n", source ? source : "(null)");
            if (source) {
                char* editable_fact_data = fact_read_from_source(source);
                if (editable_fact_data) {
                    cerr << "PROFACT:" << editable_fact_data << endl;
                    free(editable_fact_data);
                }
                free(source);
            }
            else {
                cerr << "PROFACT:" << "Datei nicht gefunden..." << endl;
            }
            sql_end();
        }

        if ( result->at(0) == string("REPLACE") && result->at(1) == string("PROFACT") && result->at(2) == string("PK") ) {
            struct RECORD r;
            strcpy(r.pkey, result->at(3).c_str());
            char replacement[2001];
            strncpy(replacement, result->at(5).c_str(), 2000);
            while (replacement[0] && (replacement[strlen(replacement)-1] == '\r' || replacement[strlen(replacement)-1] == '\n')) {
                replacement[strlen(replacement)-1] = '\0';
            }
            printf("replace .pro entry (in hal2012-server, 1): %s by '%s'\n", r.pkey, replacement);
            sql_begin("");
            char* source = sql_get_source(&r);
            if (source) {
                fact_replace_in_source(source, replacement);
                char* editable_fact_data = fact_read_from_source(source);
                if (editable_fact_data) {
                    cerr << "PROFACT:" << editable_fact_data << endl;
                    free(editable_fact_data);
                }
                char* filename = strdup(source);
                if (strstr(filename, ":")) {
                    strstr(filename, ":")[0] = '\0';
                    sql_end();
                    hal2012_add_xml_file(filename);
                    // hal2012_add_pro_file() free's filename
                    sql_begin("");
                }
                free(source);
            }
            else {
                cerr << "PROFACT:" << "Datei nicht gefunden..." << endl;
            }
            sql_end();
        }

        if ( init_thread_ended > 0 && result->at(0) == string("QUESTION") && result->at(1) != string("QUESTION") && result->size() >= 2 && result->at(1).size() > 0 && !(result->at(1).size() < 3 && ' ' == result->at(1)[0]) ) {
            string input;
            for (int i = 1; i < result->size(); ++i) {
                if (i != 1) {
                    input += ":";
                }
                input += result->at(i);
            }
            replace_all(input, "::", ":");
            hal2012_server_statement(input, username, true, true);
        }

        if ( init_thread_ended > 0 && result->size() >= 2 && result->at(1).size() > 0 && !(result->at(1).size() < 3 && ' ' == result->at(1)[0]) ) {
            string input;
            for (int i = 1; i < result->size(); ++i) {
                if (i != 1) {
                    input += ":";
                }
                input += result->at(i);
            }
            replace_all(input, "::", ":");

            if (result->at(0) == string("LEARN") && result->at(1) != string("LEARN")) {
                hal2012_server_statement(input, username, true, false);
            }
            else if (result->at(0) == string("TALK") && result->at(1) != string("TALK")) {
                hal2012_server_statement(input, username, false, true);
            }
            else if (result->at(0) == string("TALK_LEARN") && result->at(1) != string("TALK_LEARN")) {
                hal2012_server_statement(input, username, true, true);
            }
        }

        delete result;
        delete result_underscore;
    }
}

void hal2012_server_run() {
    hal2012_server_client_connection();
}

void hal2012_server_stop() {
}

string hal2012_server_get_value_from_socket(const string& s1, const string& s2) {
    cout << "Get stream." << endl;

    cout << "Print into stream." << endl;
    cerr << "GET_" << s1 << ":" << s2 << endl;

    cout << "Wait for stream." << endl;
    std::vector<std::string>* result;
    string line;
    while (cin && getline(cin, line)) {
        if ( line.size() != 0 ) {
            cout << line << endl;
            result = simple_split(line, ":" );
            if ( result->at(0) == string("HERE_IS_") + s1 && result->size() >= 2 ) {
                break;
            }
        }
    }
    cout << "End of waiting for stream." << endl;
    int i;
    string value = "";
    for (i = 1; i < result->size(); ++i) {
        value += result->at(i);
        if (i+1 < result->size()) {
            value += ":";
        }
    }
    return value;
}

int set_nonblocking(int fd)
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined (__MINGW) || defined(__MINGW32__)
#else
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
#endif
}

void* hal2012_init_thread (void*) {
    time_t start = 0;
    time(&start);

    hal2012_init(current_instance->planguage, current_instance->tlanguage, current_instance->base_dir);
    init_thread_ended = 1;

    time_t end = 0;
    time(&end);

    int sec = end - start;
    struct tm* ts_start = localtime(&start);
    struct tm* ts_end   = localtime(&end);
    char*      ch_start = strdup(asctime(ts_start));
    char*      ch_end   = strdup(asctime(ts_end));
    if (strstr(ch_start, "\n")) {
        strstr(ch_start, "\n")[0] = '\0';
    }
    if (strstr(ch_end, "\n")) {
        strstr(ch_end, "\n")[0] = '\0';
    }

    ofstream protocol("database.log", ios::out | ios::app);
    protocol << "Rev. " << FULL_VERSION << ", " << ch_start << ":\t" << sec << " s \t= " << (sec/60) << " min" << endl;
    protocol.close();

    free(ch_start);
    free(ch_end);

}

int main(int argc, char** argv) {
    current_instance = new instance();
    current_instance->sql_engine = "disk";
    current_instance->tlanguage = "de";
    current_instance->planguage = "perl5";

    vector<string>* params = params_cin();

    for (int i = 0; i < params->size(); i += 2) {
        if (i+1 == params->size()) {
            fprintf(output(), "Option has no value: %s.\n", params->at(i).c_str());
            break;
        }
        if (params->at(i).find("--") == string::npos) {
            fprintf(output(), "Illegal option: %s.\n", params->at(i).c_str());
            break;
        }
        const string key = params->at(i);
        const string value = params->at(i+1);

        if (key == "--language") {
            current_instance->tlanguage = value;
        }
        if (key == "--sql") {
            current_instance->sql_engine = value;
        }
        if (key == "--interpreter") {
            current_instance->planguage = value;
        }
    }

    hal2012_clean();
    extract();

    fprintf(output(), "Language is %s.\n", current_instance->tlanguage.c_str());
    fprintf(output(), "Interpreter is %s.\n", current_instance->planguage.c_str());
    fprintf(output(), "Database Tool is %s.\n", current_instance->sql_engine.c_str());
    current_instance->base_dir = ".";

    {
        string sqlite_filename = current_instance->base_dir + "/lang_" + current_instance->tlanguage + "/database.db";
        sql_sqlite_set_filename(strdup(sqlite_filename.c_str()));
    }

    hal2012_server_start();

    hal2012_server_run();

    hal2012_server_stop();
}

void* hal2012_handle_signal(void* _p) {
    pair<const string, const string>* parameters = (pair<const string, const string>*)_p;
    const string& type = parameters->first;
    const string& text = parameters->second;

    if (type == "output_pos") {
        FILE* _doing = fopen("_doing__pos", "w+b");
        halclose(_doing);
        fprintf(output(), "\nUnknown part of speech:\n\n%s\n", text.c_str());
        const string& pos = hal2012_server_get_value_from_socket("WORD_TYPE", text);
        hal2012_send_signal("input_pos", pos);
        unlink("_doing__pos");
    }
/*
    else if (0 == strcmp(type, "output_link")) {
        if (text.size() < 99) {
            char link[99] = {0};
            int f1 = 0;
            int f2 = 0;
            sscanf(text, "%98[a-zA-Z]%d\n%d", (char*)&link, &f1, &f2);
            fprintf(output(), "--%s--\n%s\n%i\n%i", text.c_str(), link, f1, f2);
            hal2012_add_link(link, f1, f2);
        }
    }
*/
    else if (type == "add_pro_file") {
        hal2012_add_pro_file(text);
        hal2012_send_signal("add_pro_file", "");
    }
    else if (type == "add_xml_file") {
        if (init_xml_files) {
            string file = text;
            replace_all(file, "\\", "/");
            init_xml_files->push_back(file);
        }
        hal2012_add_xml_file(text);
        hal2012_send_signal("add_xml_file", "");
    }
    else if (type == "database_request") {
        struct DATASET set = hal2012_get_csv(text.c_str());
        char* csv_data = hal2012_make_csv(&set);
        hal2012_send_signal("database_request", csv_data);
        fprintf(output(), "Release memory now.\n");
        free(csv_data);
        fprintf(output(), "Memory is released.\n");
    }
    else if (type == "grammar2012") {

	grammar2012::grammar* g = new grammar2012::grammar();
	g->read_grammar("grammar.txt");
	g->set_verbose(false);
	g->expand();
	const string perl_eval = g->parse(text);
        hal2012_send_signal("grammar2012", perl_eval);

    }
    else if (type == "output") {
        output_by_signal = text;
    }
    else if (type == "_exit_by_user") {
        halusleep(5000);
        exit(0);
    }

    delete parameters;
}

