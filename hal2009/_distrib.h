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

#define USE_CXX 1
#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1
#include "hal2009.h"

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

char* global_distrib__base_dir                        = ".";
char* global_distrib__programming_language            = "perl5";
char* global_distrib__output                          = (char*)calloc(5001, 1);
short global_distrib__initialized_signal_handler      = 0;

void* hal2009_answer__1(void* x) {
    char* input;
    char* language;
    input = (char*)calloc(strlen((const char*)x), 1);
    language = (char*)calloc(strlen((const char*)x), 1);
    strcpy(input, (const char*)x+3);
    ((char*)x)[2] = '\0';
    strcpy(language, (const char*)x);
    
    char* copy_of_programming_language = (char*)cxxhalmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_programming_language, global_distrib__programming_language);
    char* copy_of_language             = (char*)cxxhalmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_language,             language);
    char* copy_of_base_dir             = (char*)cxxhalmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_base_dir,             global_distrib__base_dir);
    char* copy_of_input                = (char*)cxxhalmalloc(1025, "hal2009_server_statement");
    strncpy(copy_of_input,               input, 1024);
    
    
    {
        char* sqlite_filename = (char*)calloc(5120 + 1, 1);
        strcat(sqlite_filename, copy_of_base_dir);
        strcat(sqlite_filename, "/lang_");
        strcat(sqlite_filename, language);
        strcat(sqlite_filename, "/database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }

    if (!global_distrib__initialized_signal_handler) {
        hal2009_clean();

        fprintf(stdout, "Programming language is %s\n", copy_of_programming_language);
        fprintf(stdout, "Text language is %s\n", copy_of_language);
        pthread_t signal_thread = hal2009_start_signal_handler(strdup(copy_of_programming_language), strdup(copy_of_language), MULTI);
        fprintf(stdout, "Shell started...\nThis is %s.\n\n", FULL_NAME);
        sleep(2);
        
        global_distrib__initialized_signal_handler = 1;
    }
    
    pthread_t answer_thread = hal2009_answer(copy_of_input, copy_of_programming_language, copy_of_language, copy_of_base_dir, NOT_JOIN, MULTI);
    //pthread_join(answer_thread, NULL);

    while (global_distrib__output == 0 || global_distrib__output[0] == 0) {
        usleep(500);
    }
    char* result = (char*)strdup(global_distrib__output);
    if (global_distrib__output)
        strcpy(global_distrib__output, "");

    return (void*)result;
}

void hal2009_handle_signal(void* arg) {
    char* type       = (char*)((void**)arg)[0];
    char* text       = (char*)((void**)arg)[1];
    void* start_type = (void*)((void**)arg)[2];
    
    if (0 == strcmp(type, "_output__pos")) {
        fprintf(output(), "\nUnknown part of speech: %s\n\nPlease type it in: ", text);
        scanf("%s", text);
        FILE* target = fopen("_input__pos", "w+b");
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
        free((void*)csv_data);
        fprintf(output(), "Memory is released.\n");
    }
    else if (0 == strcmp(type, "_output")) {
        fprintf(output(), "\nFreeHAL: %s\n", text);
        strncpy(global_distrib__output, text, 5000);
    }
    else if (0 == strcmp(type, "_exit") && start_type == (void*)SINGLE) {
        // exit(0);
    }
}




/// Macros for distributon service

#define N_FUNC		1

#define PTR_TO_STR 	{ \
				(fp)hal2009_answer__1, \
			}

#define STR_TO_PTR 	{ \
				"hal2009_answer__1", \
			}

