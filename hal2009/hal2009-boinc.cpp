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

#define NOT_HEADER_ONLY 1
#define USE_CXX 1
#include "hal2009.h"

#include <BOINC/boinc_api.h>

#include <getopt.h>
#include <signal.h>

#define OPTION_SIZE 512

#if defined (__MINGW) || defined(__MINGW32__)
#include <windows.h>
#else
#include <unistd.h>
#endif

// C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


extern "C" {
    void extract();
}
int sql_sqlite_set_filename(const char*);
int hal2009_execute_file(char* file, char* planguage);
int hal2009_add_pro_file (char* filename);
struct DATASET hal2009_get_csv(char* csv_request);
char* hal2009_make_csv(struct DATASET* set);

#ifndef UCHAR_MAX
#   define UCHAR_MAX sizeof(char)
#endif

int is_slow() {
    struct stat stFileInfo;
    int intStat;
    // Attempt to get the file attributes
    intStat = stat("SLOW", &stFileInfo);
    if (intStat == 0) {
        return 1;
    }
    return 0;
}

int boinc_sleep_if_slow(long t) {
    if (is_slow()) {
        halusleep(t);
        return 0;
    }
    return 1;
}

unsigned time_seed() {
    time_t now = time (0);
    unsigned char *p = (unsigned char *)&now;
    unsigned seed = 0;
    size_t i;
 
    for ( i = 0; i < sizeof now; i++ )
        seed = seed * ( UCHAR_MAX + 2U ) + p[i];
    return seed;
}
double uniform_deviate ( int seed ) {
    return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

void* cpu_thread (void* p) {

//  long max_seconds = 60 * (is_slow() ? 90 : 10 );

    long max_seconds = 60 * (is_slow() ? 240 : 60 );
    long checkpoint_sec = 60;
    long checkpoint_cpu = 0;

    time_t start;
    time(&start);

    long time_to_checkpoint = start - 1;

    short from_checkpoint = 0;
    {
        ifstream cpf_i("_status");
        if (cpf_i) {
            long diff = 0;
            cpf_i >> diff;
            cpf_i.close();
            start -= diff;
            cerr << "checkpoint revert: " << diff << "s..." << endl;
            from_checkpoint = 1;
        }
    }


    while (1) {
        double cpu_time;
        time_t now;
        time(&now);

        double frac = 1.0 / (max_seconds) * (now - start);
        boinc_fraction_done(frac);
        if ((now - start) % checkpoint_sec == 0 ) {
            checkpoint_cpu = now - start;
        }
        boinc_report_app_status(now - start, checkpoint_cpu, frac);

//        if ( (from_checkpoint || is_slow()) && ((now - start) / (max_seconds)) >= 1 ) {
        if ( ((now - start) / (max_seconds)) >= 1 ) {
            srand (time_seed());
            int N = 30;
            int M = 600;
            boinc_sleep_if_slow(1000*(M + uniform_deviate (rand()) * (N - M)));
            boinc_finish(0);
        }

        if ( time_to_checkpoint - now < 0 ) {
            if ( boinc_time_to_checkpoint() ) {
                time_to_checkpoint = now + 60*5;
                long diff = (now - start);
                ofstream cpf_o("_status");
                cpf_o << diff;
                cpf_o.close();
                cerr << "checkpoint: " << diff << "s..." << endl;
                boinc_checkpoint_completed();
            }
        }


        halusleep(1000);
    }
    if (is_slow()) {
        boinc_finish(0);
    }
}

void* app_thread (void* p) {
    {
        char* sqlite_filename = (char*)calloc(OPTION_SIZE + 1, 1);
        strcat(sqlite_filename, "./database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }

    hal2009_clean();

    pthread_t signal_thread = hal2009_start_signal_handler("perl5", "de", SINGLE);
    hal2009_execute_file("hal2009-boinc.hal", "perl5");
    if (is_slow()) {
        pthread_join((pthread_t)(signal_thread), NULL);
    }
    else {
        fprintf(stdout, "hal2009_execute_file done.\n");
        fprintf(stderr, "hal2009_execute_file done.\n");
        boinc_finish(0);
    }
}

#ifndef SIGTRAP
#define SIGTRAP 5
#endif

int main (int argc, char** argv) {
    signal(SIGTRAP, SIG_IGN);
    BOINC_OPTIONS options;
    memset(&options, 0, sizeof(options));
    options.main_program = true;
    options.check_heartbeat = false;
    options.handle_process_control = false;
    options.send_status_msgs = true;
    boinc_init_options(&options);
    fprintf(stderr, "freeHAL 2010: starting...\n");
    
    string resolved_name;
    boinc_resolve_filename_s("boincXYZ.pro", resolved_name);
    {
        resolved_name += "t";
        if (resolved_name == "boincXYZ.prot") {
            cerr << "Don't need to resolve file name " << resolved_name << "." << endl;
        }
        else {
            cerr << "Resolved file name " << resolved_name << "." << endl;
            ifstream r("boincXYZ.prot");
            ofstream w(resolved_name.c_str());
            w << r.rdbuf();
            r.close();
            w.close();
        }
    }

    sql_engine = (char*)calloc(64, 1);
    strcpy(sql_engine, "disk");

    srand (time_seed()+(long)((void*)(sql_engine))%17);
    int N = 30;
    int M = 600;
    boinc_sleep_if_slow(1000*(N + rand() % (M - N)));
    void* rand_1 = malloc(17);
    boinc_sleep_if_slow(1000*((long)((void*)(rand_1))%257));

    extract();

    void* rand_2 = malloc(7);
    boinc_sleep_if_slow(1000*((long)((void*)(rand_2))%257));

    // pthread_t thread_cpu;
    // int nul = NULL;
    // pthread_create (&thread_cpu, NULL, cpu_thread, &nul);
    // pthread_join((pthread_t)(thread_cpu), NULL);

    if (is_slow()) {
        pthread_t thread_app;
        int nul = NULL;
        pthread_create (&thread_app, NULL, app_thread, &nul);
        cpu_thread(&nul);
    }
    else {
        pthread_t thread_app;
        int nul = NULL;
        pthread_create (&thread_app, NULL, app_thread, &nul);
        cpu_thread(&nul);
    }

    boinc_finish(0);
    return 0;
}

void hal2009_handle_signal(void* arg) {
    char* type = (char*)((void**)arg)[0];
    char* text = (char*)((void**)arg)[1];

    if (0 == strcmp(type, "_output__pos")) {
        fprintf(output(), "\nUnknown part of speech:\n\n%s\n", text);
        strcpy(text, "n");
        FILE* target = fopen("_input__pos", "w+b");
        halwrite(text, 1, strlen(text), target);
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__genus")) {
        fprintf(output(), "\nUnknown part of speech:\n\n%s\n", text);
        strcpy(text, "q");
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
    else if (0 == strcmp(type, "_output__add_pro_file")) {
        hal2009_add_pro_file(text);
        FILE* target = fopen("_input__add_pro_file", "w+b");
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__get_csv")) {
        struct DATASET set = hal2009_get_csv(text);
        FILE* target = fopen("_input__get_csv", "w+b");
        char* csv_data = hal2009_make_csv(&set);
        halwrite(csv_data, 1, strlen(csv_data), target);
        halclose(target);
        fprintf(output(), "Release memory now.\n");
        free((void*)csv_data);
        fprintf(output(), "Memory is released.\n");
    }
    else if (0 == strcmp(type, "_output")) {
        fprintf(output(), "\nFreeHAL: %s\n", text);
    }
}



