// This file is part of BOINC.
// http://boinc.berkeley.edu
// Copyright (C) 2008 University of California
//

// BOINC is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// BOINC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BOINC.  If not, see <http://www.gnu.org/licenses/>.
// sample_work_generator.cpp: an example BOINC work generator.
// This work generator has the following properties
// (you may need to change some or all of these):
//
// - Runs as a daemon, and creates an unbounded supply of work.
//   It attempts to maintain a "cushion" of 100 unsent job instances.
//   (your app may not work this way; e.g. you might create work in batches)
// - Creates work for the application "uppercase".
// - Creates a new input file for each job;
//   the file (and the workunit names) contain a timestamp
//   and sequence number, so that they're unique.

#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

#include "boinc_db.h"
#include "error_numbers.h"
#include "backend_lib.h"
#include "parse.h"
#include "util.h"
#include "svn_version.h"

#include "sched_config.h"
#include "sched_util.h"
#include "sched_msgs.h"
#include "str_util.h"

#define CUSHION 400
    // maintain at least this many unsent results
#define REPLICATION_FACTOR  1

// globals
//
char* wu_template;
DB_APP app;
int start_time;
int instanceno;
char instanceno_str[10];
int seqno;
extern int mode_to_create;

using namespace std;

// create one new job
//
int make_job(int mode, const char* s_0, const char* s_1, const char* s_3, const char* s_4, const char* s_5) {
    DB_WORKUNIT wu;
    char name[256], path[256];
    const char* infiles[6];
    int retval;


    // make a unique name (for the job and its input file)
    //

    char input_data_file[256];

if (seqno > 100) {
    start_time = time(0);
    seqno = 0;
}

++seqno;
if (mode == 1) {
    sprintf(name, "fh_nci_%d_%d_%d", instanceno, start_time-1277313993, seqno);
    sprintf(input_data_file, "fh_nci_%d_%d.zip", instanceno, seqno);
}
if (mode == 2) {
    sprintf(name, "fh_ci_%d_%d_%d", instanceno, start_time-1277313993, seqno);
    sprintf(input_data_file, "fh_ci_%d_%d.zip", instanceno, seqno);
}

    char seqno_str[99];
    sprintf(seqno_str, "%d", seqno);

    // Create the input file.
    // Put it at the right place in the download dir hierarchy
    //

if (seqno % 500 == 0)
    system("date");

    // Fill in the job parameters
    //
    wu.clear();
    wu.appid = app.id;
    strcpy(wu.name, name);

    wu.rsc_fpops_est = 1000000000;
    wu.rsc_fpops_bound = 10000000000000000000;
    wu.rsc_memory_bound = 50000000;
    wu.rsc_disk_bound = 3000000000;
    wu.delay_bound = 200000;
    wu.min_quorum = REPLICATION_FACTOR;
    wu.target_nresults = REPLICATION_FACTOR;
    wu.max_error_results = REPLICATION_FACTOR;
    wu.max_total_results = REPLICATION_FACTOR;
    wu.max_success_results = REPLICATION_FACTOR;

    infiles[0] = s_0;
    infiles[1] = s_1;
    infiles[3] = s_3;
    infiles[4] = s_4;
    infiles[5] = s_5;

    // Register the job with BOINC
    //

    retval = config.download_path(input_data_file, path);
    if (!retval) {
        printf("path=%s\n", path);

        string cmd = "/usr/local/bin/new_job_input_data ";
        cmd += path;
        cmd += " ";
        cmd += instanceno_str;
        cmd += " ";
        cmd += seqno_str;

        log_messages.printf(MSG_DEBUG, "system: %s\n", cmd.c_str());
        system(cmd.c_str());

        {
            ifstream i(path);
            if (!i.is_open()) {
                log_messages.printf(MSG_DEBUG, "abort; not open.\n");
                return 0;
            }
        }
        infiles[2] = input_data_file;
    }
    else {
        log_messages.printf(MSG_DEBUG, "abort; not created.\n");
        return 0;
    }

    if (seqno == 99) system("touch /home/boincadm/www/projects/freehal_at_home/download/312/report.txt");

    create_work(
            wu,
            wu_template,
            "templates/newFreeHAL_result",
            config.project_path("templates/newFreeHAL_result"),
            infiles,
            6,
            config
    );

    return 0;
}

void main_loop() {
    int retval;

    while (1) {
        check_stop_daemons();
        log_messages.printf(MSG_DEBUG, "Loop.\n");
        {
            ifstream i("/tmp/new-results-recent___");
            if (i.is_open()) {
                log_messages.printf(MSG_DEBUG, "Too recent.\n");
                sleep(10);
                i.close();
                continue;
            }
        }

        int n;
        retval = count_unsent_results(n, 0);
        if (n > CUSHION) {
            log_messages.printf(MSG_DEBUG, "No new WUs needed.\n");
            sleep(10);
        } else {
            int njobs = (CUSHION-n)/REPLICATION_FACTOR;
            if (njobs < 0) njobs *= -1;

            if (mode_to_create != 1) {
                if (njobs > CUSHION/3) njobs = CUSHION/3;
            }
            if (mode_to_create == 1) {
                system("ratio-workunit-type-nci > /tmp/r-nci.txt");
                ifstream ratio_i("/tmp/r-nci.txt");
                int ratio = 100;
                if (ratio_i) {
                    ratio_i >> ratio;
                    ratio_i.close();
                }
                njobs *= (float(ratio) / 100.0);

                if (ratio > 90 && njobs < 50) njobs += 5;

                log_messages.printf(MSG_NORMAL, "ratio (nci): %i (=> %i jobs)\n", ratio, njobs);
            }
            if (mode_to_create == 2) {
                system("ratio-workunit-type-ci > /tmp/r-ci.txt");
                ifstream ratio_i("/tmp/r-ci.txt");
                int ratio = 100;
                if (ratio_i) {
                    ratio_i >> ratio;
                    ratio_i.close();
                }

                if (ratio > 90 && njobs < 50) njobs += 5;

                if (ratio > 90) njobs = 100;

                log_messages.printf(MSG_NORMAL, "ratio (ci): %i (=> %i jobs)\n", ratio, njobs);
            }

            log_messages.printf(MSG_DEBUG, "Making %d jobs\n", njobs);

    ifstream i_0("/var/www/data/wu-file_1");
    string s_0;
    getline(i_0, s_0);
    const char* _s_0 = s_0.c_str();

    ifstream i_1("/var/www/data/wu-file_2");
    string s_1;
    getline(i_1, s_1);
    const char* _s_1 = s_1.c_str();

    ifstream i_3("/var/www/data/wu-file_4");
    string s_3;
    getline(i_3, s_3);
    const char* _s_3 = s_3.c_str();

    ifstream i_4("/var/www/data/wu-file_5");
    string s_4;
    getline(i_4, s_4);
    const char* _s_4 = s_4.c_str();

    ifstream i_5("/var/www/data/wu-file_3");
    string s_5;
    getline(i_5, s_5);
    const char* _s_5 = s_5.c_str();

    i_0.close();
    i_1.close();
    i_3.close();
    i_4.close();
    i_5.close();



            for (int i=0; i<njobs; i++) {
                retval = make_job(mode_to_create, _s_0, _s_1, _s_3, _s_4, _s_5);
                if (retval) {
                    log_messages.printf(MSG_CRITICAL, "can't make job: %d\n", retval);
                    exit(retval);
                }
            }

            system("/usr/local/bin/move-prepared-workunits");

            // Now sleep for a few seconds to let the transitioner
            // create instances for the jobs we just created.
            // Otherwise we could end up creating an excess of jobs.
            sleep(10+0.1*njobs);
        }
    }
}

void usage(char *name) {
return;    fprintf(stderr, "This is an example BOINC work generator.\n"
        "This work generator has the following properties\n"
        "(you may need to change some or all of these):\n"
        "- Runs as a daemon, and creates an unbounded supply of work.\n"
        "  It attempts to maintain a \"cushion\" of 100 unsent job instances.\n"
        "  (your app may not work this way; e.g. you might create work in batches)\n"
        "- Creates work for the application \"uppercase\".\n"
        "- Creates a new input file for each job;\n"
        "  the file (and the workunit names) contain a timestamp\n"
        "  and sequence number, so that they're unique.\n\n"
        "Usage: %s [OPTION]...\n\n"
        "Options:\n"
        "  [ -d X ]                        Sets debug level to X.\n"
        "  [ -no X ]                       Sets the instance no X.\n"
        "  [ -h | -help | --help ]         Shows this help text.\n"
        "  [ -v | --version | --version ]  Shows version information.\n",
        name
    );
}

int main(int argc, char** argv) {
    int i, retval;

    mode_to_create = 2;

    for (i=1; i<argc; i++) {
        if (is_arg(argv[i], "d")) {
            if (!argv[++i]) {
                log_messages.printf(MSG_CRITICAL, "%s requires an argument\n\n", argv[--i]);
                usage(argv[0]);
                exit(1);
            }

            int dl = atoi(argv[i]);
            log_messages.set_debug_level(dl);
            if (dl == 4) g_print_queries = true;
        } else if (is_arg(argv[i], "no")) {
            if(!argv[++i]) {
                log_messages.printf(MSG_CRITICAL, "%s requires an argument\n\n", argv[--i]);
                usage(argv[0]);
                exit(1);
            }

            instanceno = atoi(argv[i]);
            strcpy(instanceno_str, argv[i]);
        } else if (is_arg(argv[i], "modetocreate")) {
            if(!argv[++i]) {
                printf("%s requires an argument\n\n", argv[--i]);
                usage(argv[0]);
                exit(1);
            }

            mode_to_create = atoi(argv[i]);
        } else if (is_arg(argv[i], "h") || is_arg(argv[i], "help")) {
            usage(argv[0]);
            exit(0);
        } else if (is_arg(argv[i], "v") || is_arg(argv[i], "version")) {
            printf("%s\n", SVN_VERSION);
            exit(0);
        } else {
            log_messages.printf(MSG_CRITICAL, "unknown command line argument: %s\n\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }

    retval = config.parse_file();
    if (retval) {
        log_messages.printf(MSG_CRITICAL,
            "Can't parse config.xml: %s\n", boincerror(retval)
        );
        exit(1);
    }

    retval = boinc_db.open(config.db_name, config.db_host, config.db_user, config.db_passwd);

    if (retval) {
        log_messages.printf(MSG_CRITICAL, "can't open db\n");
        exit(1);
    }
    if (app.lookup("where name='newFreeHAL'")) {
        log_messages.printf(MSG_CRITICAL, "can't find app\n");
        exit(1);
    }
    if (mode_to_create == 1) {
        if (read_file_malloc(config.project_path("templates/newFreeHAL_wu_r9_nci"), wu_template)) {
            log_messages.printf(MSG_CRITICAL, "can't read WU template (ci)\n");
            exit(1);
        }
    }
    else {
        if (read_file_malloc(config.project_path("templates/newFreeHAL_wu_r9_ci"), wu_template)) {
            log_messages.printf(MSG_CRITICAL, "can't read WU template (nci)\n");
            exit(1);
        }
    }

    start_time = time(0);
    seqno = 0;

    log_messages.printf(MSG_NORMAL, "Starting\n");

    log_messages.printf(MSG_NORMAL, "Sleeping beginning-interval...\n");

    int _i = 15;
    while (_i > 0) {
        log_messages.printf(MSG_NORMAL, "%i\n", _i);
        --_i;
        sleep(1);
    }

    main_loop();
}
