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
#include "hal2009-main.h"
#include "hal2009-ipc.h"
#include "hal2009-pro.h"

extern "C" void extract();

#include <getopt.h>

int usage (string exe_name) {
    fprintf(stderr, "%s%s%s",
"Usage: ", exe_name.c_str(), " OPTION...\n"
"FreeHAL is a self-learning conversation simulator, an artificial \n"
"intelligence using semantic nets to organize its knowledge.\n"
"\n"
"  -m METHOD        The way to run and use FreeHAL (Method)\n"
"  -i INPUT         A statement or question FreeHAL has to answer.\n"
"  -b DIRECTORY     The base directory with data files\n"
"  -q SQL_ENGINE    The SQL_ENGINE to use\n"
"  -l LANGUAGE      The language of text input \n"
"                   (must be understood by FreeHAL)\n"
"  -p PROGRAMMING-  FreeHAL should compile itself to PROGRAMMINGLANGUAGE\n"
"     LANGUAGE      \n"
"  -e FILE          Execute FILE as HAL2009-Code by using the language from \"-p\"\n"
"  -c FILE          Compile FILE from HAL2009-Code to the language from \"-p\"\n"
"  -d FILE          Generae documentation for source file FILE (HAL2009-Code)\n"
"\n"
"PROGRAMMINGLANGUAGE can be:\n"
"     perl5\n"
" or  perl6\n"
"\n"
"SQL_ENGINE can be:\n"
"     sqlite\n"
"\n"
"LANGUAGE can be:\n"
"     de\n"
" or  en\n"
"\n"
"Without options, this help is displayed.\n"
"\n"
"Report bugs to <info@freehal.org>.\n"


    );
}

#define OPTION_SIZE 512

int main (int argc, char** argv) {
    sql_engine = (char*)calloc(OPTION_SIZE + 1, 1);

    string method, input, programming_language, compile_file, execute_file, base_dir, language, exe_name;

    exe_name = argv[0];
    programming_language = "perl5";
    language = "de";
    base_dir = ".";
    sql_engine = "disk";

    int c;
    while ((c = getopt (argc, argv, "b:q:i:m:p:l:c:e:d:s")) != -1) {
        switch (c) {
            case 'b':
                base_dir = optarg;
                break;
            case 'q':
                sql_engine = optarg;
                break;
            case 'p':
                programming_language = optarg;
                break;
            case 'i':
                input = optarg;
                break;
            case 'l':
                language = optarg;
                break;
            case 'm':
                method = optarg;
                break;
            case 'c':
                compile_file = optarg;
                break;
            case 'e':
                execute_file = optarg;
                break;
            case 's':
                input = "?";
                break;

            case '?':
                break;
            default:
                abort ();
        }
    }

    if (method.size() == 0 && input.size() == 0 && compile_file.size() == 0 && execute_file.size() == 0) {
        usage(exe_name);
        exit(1);
    }

    {
        string sqlite_filename = base_dir + "/lang_" + language + "/database.db";
        sql_sqlite_set_filename(strdup(sqlite_filename.c_str()));
    }

    hal2009_clean();
    chdir(base_dir.c_str());

    if (compile_file.size() > 0) {
        hal2009_compile(compile_file, programming_language);
    }
    else if (execute_file.size() > 0) {
        extract();
        hal2009_execute_file(execute_file, programming_language);
    }
    else if (input.size() > 0 && input == "?") {
        extract();
        shell(programming_language, language, base_dir);
    }
    else if (input.size() > 0) {
        extract();
        halsleep(2);
        cout << "input: " << input << endl;
        pthread_t answer_thread = hal2009_answer(input, programming_language, language, base_dir, NOT_JOIN, SINGLE);
        //pthread_join(signal_thread, NULL);
        pthread_join(answer_thread, NULL);
    }
    else if (method.size() > 0) {
        fprintf(output(), "TODO: Start method here...");
        // start method here
    }

    return 0;
}

void shell(string programming_language, string language, string base_dir) {
    cout << "Programming language is " << programming_language << endl;
    cout << "Text language is " << language << endl;
    cout << "Shell started..." << endl << "This is " << FULL_NAME << endl << endl;
    while (1) {
        cout << "Human: " << flush;

        string input;
        getline(cin, input);
        if (input.size() == 0) continue;

        pthread_t answer_thread = hal2009_answer(input, programming_language, language, base_dir, NOT_JOIN, MULTI);
        pthread_join(answer_thread, NULL);
        halsleep(1);
    }
}

void* hal2009_handle_signal(void* arg) {
    char* type       = ((char**)arg)[0];
    char* text       = ((char**)arg)[1];
    short start_type = ((short*)arg)[2];

    if (0 == strcmp(type, "output_pos")) {
        fprintf(output(), "\nUnknown part of speech: %s\n\nPlease type it in: ", text);
        scanf("%s", text);
        hal2009_send_signal("input_pos", text);
    }
    else if (0 == strcmp(type, "output_link")) {
        if (strlen(text) < 99) {
            char link[99] = {0};
            int f1 = 0;
            int f2 = 0;
            sscanf(text, "%98[a-zA-Z]%d\n%d", &link, &f1, &f2);
            fprintf(output(), "--%s--\n%s\n%i\n%i", text, link, f1, f2);
            hal2009_add_link(link, f1, f2);
        }
    }
    else if (0 == strcmp(type, "add_pro_file")) {
        hal2009_add_pro_file(text);
        hal2009_send_signal("add_pro_file", "");
    }
    else if (0 == strcmp(type, "add_xml_file")) {
        hal2009_add_xml_file(text);
        hal2009_send_signal("add_xml_file", "");
    }
    else if (0 == strcmp(type, "database_request")) {
        struct DATASET set = hal2009_get_csv(text);
        char* csv_data = hal2009_make_csv(&set);
        hal2009_send_signal("database_request", csv_data);
        fprintf(output(), "Release memory now.\n");
        free(csv_data);
        fprintf(output(), "Memory is released.\n");
    }
    else if (0 == strcmp(type, "output")) {
        fprintf(output(), "\n\n--------------------------------------------------------------------------------\n\n", text);
        fprintf(output(), "FreeHAL: %s\n\n--------------------------------------------------------------------------------\n\n", text);
    }
    else if (0 == strcmp(type, "_exit") && start_type == SINGLE) {
        //exit(0);
    }

    free(type);
    free(text);
}



