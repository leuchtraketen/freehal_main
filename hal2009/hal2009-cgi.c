/*
 * This file is part of FreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
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

#define HAL2009_CGI_MODE 1

#include "hal2009.h"
#include "hal2009-wiki.c"

#include <getopt.h>

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
    
    fprintf(output(), "%s", "Content-type: text/html\n\n");
    
    strcpy(programming_language, "perl5");
    strcpy(language, "de");
    strcpy(base_dir, ".");
    strcpy(sql_engine, "disk");
    if (argc > 1) {
        strcpy(sql_engine, argv[2]);
    }

    {
        char* sqlite_filename = calloc(OPTION_SIZE + 1, 1);
        strcat(sqlite_filename, base_dir);
        strcat(sqlite_filename, "/lang_");
        strcat(sqlite_filename, language);
        strcat(sqlite_filename, "/database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }
    
    strcpy(input, argv[1]);

    hal2009_clean();
    chdir(base_dir);
    
    char* copy_of_programming_language = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_programming_language, programming_language);
    char* copy_of_language             = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_language,             language);
    pthread_t signal_thread = hal2009_start_signal_handler(copy_of_programming_language, copy_of_language, SINGLE);
    sleep(2);
    pthread_t answer_thread = hal2009_answer(input, programming_language, language, base_dir, NOT_JOIN, SINGLE);
    //pthread_join(signal_thread, NULL);
    pthread_join(answer_thread, NULL);
    
    return 0;
}

void hal2009_handle_signal(void* arg) {
    char* type       = ((void**)arg)[0];
    char* text       = ((void**)arg)[1];
    char* start_type = ((void**)arg)[2];
    
    if (0 == strcmp(type, "_output__pos")) {
        fprintf(output(), "\nUnknown part of speech:\n\n%s\n", text);
        strcpy(text, "q");
        FILE* target = fopen("_input__pos", "w+b");
        halwrite(text, 1, strlen(text), target);
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__add_pro_file")) {
        hal2009_add_pro_file(text);
        FILE* target = fopen("_input__add_pro_file", "w+b");
        halclose(target);
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
        fprintf(output(), "\nFreeHAL: %s\n", text);
        ///pthread_exit(0);
        exit(0);
    }
    else if (0 == strcmp(type, "_exit") && start_type == SINGLE) {
        exit(0);
    }
}



