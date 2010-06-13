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

#define HAL2009_CGI_MODE 1

#include "hal2009.h"
#include "hal2009-wiki.c"

#include <getopt.h>

#define OPTION_SIZE 512

char* base_dir;
char* programming_language;
char* language;
char current_user[900];

int main (int argc, char** argv) {
    char* method = calloc(OPTION_SIZE + 1, 1);
    char* compile_file = calloc(OPTION_SIZE + 1, 1);
    char* execute_file = calloc(OPTION_SIZE + 1, 1);
    char* doc_file = calloc(OPTION_SIZE + 1, 1);
    base_dir = calloc(OPTION_SIZE + 1, 1);
    programming_language = calloc(OPTION_SIZE + 1, 1);
    language = calloc(OPTION_SIZE + 1, 1);
    sql_engine = calloc(OPTION_SIZE + 1, 1);
    char* exe_name = argv[0];
    int c;

    fprintf(output(), "%s", "Content-type: text/html\n\n");

    strcpy(programming_language, "perl5");
    strcpy(language, "de");
    strcpy(base_dir, ".");
    strcpy(sql_engine, "disk");
    if (argc >= 2) {
        strcpy(sql_engine, argv[1]);
    }

    strcpy(current_user, "anonymous");
    FILE* file = fopen("_cgi_user", "r");
    printf("user 0\n");
    if (file) {
        char* buffer;
    printf("user 1\n");
        while (file && (buffer = halgetline(file)) != NULL) {
    printf("user 2\n");
            if (buffer) {
    printf("user 3: %s\n", buffer);
                strcpy(current_user, buffer);
                free(buffer);
		break;
            }
        }
        fclose(file);
    }
    printf("user: %s\n", current_user);

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
    
    pthread_t signal_thread = hal2009_start_signal_handler(strdup(programming_language), strdup(language), MULTI);
    hal2009_init(strdup(programming_language), strdup(language), strdup(base_dir));

    while (1) {
        halsleep(1);
    }
    
    return 0;
}

void for__input__add_pro_file(void* arg) {
    char* text = (char*)arg;
    
    hal2009_add_pro_file(text);
    FILE* target = fopen("_input__add_pro_file", "w+b");
    halclose(target);
    
    if (text) free(text);
}

void for__input__get_csv(void* text) {
    struct DATASET set = hal2009_get_csv(text);
    if (text) free(text);
    const char* csv_data = hal2009_make_csv(&set);
    FILE* target = fopen("_input__get_csv", "w+b");
    halwrite(csv_data, 1, strlen(csv_data), target);
    halclose(target);
    fprintf(output(), "Release memory now.\n");
    free(csv_data);
    fprintf(output(), "Memory is released.\n");
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
    else if (0 == strcmp(type, "_output__genus")) {
        fprintf(output(), "\nUnknown genus:\n\n%s\n", text);
        strcpy(text, "q");
        FILE* target = fopen("_input__genus", "w+b");
        halwrite(text, 1, strlen(text), target);
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__add_pro_file")) {
        pthread_t thread;
        pthread_create (&thread, NULL, for__input__add_pro_file, (void*)strdup(text));
    }
    else if (0 == strcmp(type, "_output__get_csv")) {
        pthread_t thread;
        pthread_create (&thread, NULL, for__input__get_csv, (void*)strdup(text));
    }
    else if (0 == strcmp(type, "_output")) {
	char _temp[1000];
	snprintf(_temp, 999, "log/o-%s", current_user);

        FILE* target = fopen(_temp, "w+b");
        if (target) {
            halwrite(text, 1, strlen(text), target);
        }
        halclose(target);
        fprintf(output(), "\nFreeHAL: %s\n", text);
        fprintf(output(), "\nFreeHAL -> log: log/o-%s\n", current_user);
    }
    else if (0 == strcmp(type, "_cgi_request")) {
        unlink("_cgi_answer");
        char* input = calloc(OPTION_SIZE + 1, 1);
        snprintf(input, OPTION_SIZE, "no_learn_do_talk: %s", text);

        pthread_t answer_thread = hal2009_answer(strdup(input), strdup(programming_language), strdup(language), strdup(base_dir), NOT_JOIN, MULTI);
    }
}



