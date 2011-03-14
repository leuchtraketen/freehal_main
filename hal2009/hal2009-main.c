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
#include "hal2009-wiki-dummy.c"

#include <getopt.h>

int usage (char* exe_name) {
    fprintf(stderr, "%s%s%s",
"Usage: ", exe_name, " OPTION...\n"
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
    char* method = calloc(OPTION_SIZE + 1, 1);
    char* input = calloc(OPTION_SIZE + 1, 1);
    char* programming_language = calloc(OPTION_SIZE + 1, 1);
    char* compile_file = calloc(OPTION_SIZE + 1, 1);
    char* execute_file = calloc(OPTION_SIZE + 1, 1);
    char* doc_file = calloc(OPTION_SIZE + 1, 1);
    char* base_dir = calloc(OPTION_SIZE + 1, 1);
    char* language = calloc(OPTION_SIZE + 1, 1);
    sql_engine = calloc(OPTION_SIZE + 1, 1);
    char* exe_name = argv[0];
    int c;

    strcpy(programming_language, "perl5");
    strcpy(language, "de");
    strcpy(base_dir, ".");
    strcpy(sql_engine, "disk");

    while ((c = getopt (argc, argv, "b:q:i:m:p:l:c:e:d:s")) != INVALID_POINTER) {
        switch (c) {
            case 'b':
                strncpy(base_dir, optarg, OPTION_SIZE);
                break;
            case 'q':
                strncpy(sql_engine, optarg, OPTION_SIZE);
                break;
            case 'p':
                strncpy(programming_language, optarg, OPTION_SIZE);
                break;
            case 'i':
                strncpy(input, optarg, OPTION_SIZE);
                break;
            case 'l':
                strncpy(language, optarg, OPTION_SIZE);
                break;
            case 'm':
                strncpy(method, optarg, OPTION_SIZE);
                break;
            case 'c':
                strncpy(compile_file, optarg, OPTION_SIZE);
                break;
            case 'e':
                strncpy(execute_file, optarg, OPTION_SIZE);
                break;
            case 'd':
                strncpy(doc_file, optarg, OPTION_SIZE);
                break;
            case 's':
                strncpy(input, "?", OPTION_SIZE);
                break;

            case '?':
                break;
            default:
                abort ();
        }
    }

    if (method && input && compile_file && execute_file && doc_file && *method == 0 && *input == 0 && *compile_file == 0 && *execute_file == 0 && *doc_file == 0) {
        usage(exe_name);
        exit(1);
    }

    {
        char* sqlite_filename = calloc(OPTION_SIZE + 1, 1);
        strcat(sqlite_filename, base_dir);
        strcat(sqlite_filename, "/lang_");
        strcat(sqlite_filename, language);
        strcat(sqlite_filename, "/database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }

    hal2009_clean();
    chdir(base_dir);

    if (doc_file && *doc_file) {
        hal2009_doc(doc_file);
    }
    else if (compile_file && *compile_file) {
        hal2009_compile(compile_file, programming_language);
    }
    else if (execute_file && *execute_file) {
        extract();
        hal2009_start_signal_handler(programming_language, language, SINGLE);
        hal2009_execute_file(execute_file, programming_language);
    }
    else if (input && *input && 0 == strcmp(input, "?")) {
        extract();
        shell(programming_language, language, base_dir);
    }
    else if (input && *input) {
        extract();
        char* copy_of_programming_language = (char*)halmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_programming_language, programming_language);
        char* copy_of_language             = (char*)halmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_language,             language);
        pthread_t signal_thread = hal2009_start_signal_handler(copy_of_programming_language, copy_of_language, SINGLE);
        halsleep(2);
        pthread_t answer_thread = hal2009_answer(input, programming_language, language, base_dir, NOT_JOIN, SINGLE);
        //pthread_join(signal_thread, NULL);
        pthread_join(answer_thread, NULL);
    }
    else if (method && *method) {
        fprintf(output(), "TODO: Start method here...");
        // start method here
    }

    return 0;
}

void shell(char* programming_language, char* language, char* base_dir) {
    fprintf(stdout, "Programming language is %s\n", programming_language);
    fprintf(stdout, "Text language is %s\n", language);
    pthread_t signal_thread = hal2009_start_signal_handler(strdup(programming_language), strdup(language), MULTI);
    fprintf(stdout, "Shell started...\nThis is %s.\n\n", FULL_NAME);
    while (1) {
        fprintf(stdout, "Human: ");
        fflush(stdout);
        char* input = halgetline(stdin);
        if (0 == input || 0 == strlen(input)) continue;
        input[strlen(input)-1] = 0;
        char* copy_of_programming_language = (char*)halmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_programming_language, programming_language);
        char* copy_of_language             = (char*)halmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_language,             language);
        char* copy_of_base_dir             = (char*)halmalloc(16, "hal2009_server_statement");
        strcpy(copy_of_base_dir,             base_dir);
        char* copy_of_input                = (char*)halmalloc(1025, "hal2009_server_statement");
        strncpy(copy_of_input,               input, 1024);
        pthread_t answer_thread = hal2009_answer(copy_of_input, copy_of_programming_language, copy_of_language, copy_of_base_dir, NOT_JOIN, MULTI);
        pthread_join(answer_thread, NULL);
        halsleep(1);
    }
}

void* hal2009_handle_signal(void* arg) {
    char* type       = ((void**)arg)[0];
    char* text       = ((void**)arg)[1];
    char* start_type = ((void**)arg)[2];

    if (0 == strcmp(type, "_output__pos")) {
        fprintf(output(), "\nUnknown part of speech: %s\n\nPlease type it in: ", text);
        scanf("%s", text);
        FILE* target = fopen("_input__pos", "w+b");
        halwrite(text, 1, strlen(text), target);
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__genus")) {
        fprintf(output(), "\nUnknown genus: %s\n\nPlease type it in: ", text);
        scanf("%s", text);
        FILE* target = fopen("_input__genus", "w+b");
        halwrite(text, 1, strlen(text), target);
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__link")) {
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
        hal2009_add_pro_file(strdup(text));
        hal2009_send_signal("add_pro_file", "");
    }
    else if (0 == strcmp(type, "_output__get_csv")) {
        struct DATASET set = hal2009_get_csv(text);
        const char* csv_data = hal2009_make_csv(&set);
        FILE* target = fopen("_input__get_csv", "w+b");
        halwrite(csv_data, 1, strlen(csv_data), target);
        halclose(target);
        fprintf(output(), "Release memory now.\n");
        free(csv_data);
        fprintf(output(), "Memory is released.\n");
    }
    else if (0 == strcmp(type, "_output")) {
        fprintf(output(), "\n\n--------------------------------------------------------------------------------\n\n", text);
        fprintf(output(), "FreeHAL: %s\n\n--------------------------------------------------------------------------------\n\n", text);
    }
    else if (0 == strcmp(type, "_exit") && start_type == SINGLE) {
        exit(0);
    }

    free(type);
    free(text);
}



