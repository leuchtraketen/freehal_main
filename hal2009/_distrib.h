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


#include <stdio.h>
#include <stdlib.h>

void* hal2009_answer__1(void* x) {
    char* input, language;
    input = calloc(strlen(x), 1);
    language = calloc(strlen(x), 1);
    sscanf(x, "%s:%s", language, input);
    
    char* copy_of_programming_language = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_programming_language, global_distrib__programming_language);
    char* copy_of_language             = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_language,             language);
    char* copy_of_base_dir             = (char*)halmalloc(16, "hal2009_server_statement");
    strcpy(copy_of_base_dir,             global_distrib__base_dir);
    char* copy_of_input                = (char*)halmalloc(1025, "hal2009_server_statement");
    strncpy(copy_of_input,               input, 1024);
    
    static initialized = 0;
    if (!initialized) {
        fprintf(stdout, "Programming language is %s\n", programming_language);
        fprintf(stdout, "Text language is %s\n", language);
        pthread_t signal_thread = hal2009_start_signal_handler(strdup(programming_language), strdup(language), MULTI);
        fprintf(stdout, "Shell started...\nThis is %s.\n\n", FULL_NAME);
    }
    
    pthread_t answer_thread = hal2009_answer(copy_of_input, copy_of_programming_language, copy_of_language, copy_of_base_dir, NOT_JOIN, MULTI);
    pthread_join(answer_thread, NULL);
    
    return (void*)"danke";
}

void* fb(void* x) {
    printf("b says %s\n", (const char*)x);
    return (void*)itoa(((long int)atoi((const char*)x)+1));
}

void* fc(void* x) {
    printf("c says %s\n", (const char*)x);
    return (void*)itoa(((long int)atoi((const char*)x)+1));
}



/// Macros for distributon service

#define N_FUNC		3

#define PTR_TO_STR 	{ \
				(fp)hal2009_answer__1, \
				(fp)fb, \
				(fp)fc, \
			}

#define STR_TO_PTR 	{ \
				"hal2009_answer__1", \
				"fb", \
				"fc", \
			}

