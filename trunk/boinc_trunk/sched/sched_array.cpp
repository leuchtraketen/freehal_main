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

// scheduler code related to sending work

#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdio>

#include "config.h"
#include "sched_main.h"
#include "sched_types.h"
#include "sched_shmem.h"
#include "sched_hr.h"
#include "sched_config.h"
#include "sched_util.h"
#include "sched_msgs.h"
#include "sched_send.h"
#include "sched_version.h"


#include "credit-amount.h"

#ifdef _USING_FCGI_
#include "boinc_fcgi.h"
#define FCGI_fopen FCGI::fopen
#define FCGI_fclose FCGI::fclose
#define FCGI_fwrite FCGI::fwrite
#define FCGI_fread FCGI::fread
#else
#define FCGI_FILE FILE
#define FCGI_fopen fopen
#define FCGI_fclose fclose
#define FCGI_fwrite fwrite
#define FCGI_fread fread
#endif


#include "sched_array.h"

int nwus = 0;

bool accept_any_cpu_intensity = false;
bool complain_about_cpu_intensity = false;

const char* r_mode_str = 0;
const char* r_behave_str = 0;

const char* mode_str() {
    if (r_mode_str) {
        return r_mode_str;
    }
    int mode = 1;
    int i;
    {
        char filename[300];
        sprintf(filename, "/home/boincadm/www/projects/freehal_at_home/limits/%d-%s", g_reply->user.id,    strlen(g_reply->host.venue) ?
        (
            (
                    0 == strcmp(g_reply->host.venue,    "home")
                ||    0 == strcmp(g_reply->host.venue,    "school")
                ||    0 == strcmp(g_reply->host.venue,    "work")
            )
            ? g_reply->host.venue
            : "home"
        )
        : "home"
        );

        log_messages.printf(MSG_NORMAL,
            "filename: %s\n",
            filename
        );

        FCGI_FILE* f = FCGI_fopen(filename, "r");
        int i = 1;
        if (!f || !FCGI_fread(&i, sizeof(int), 1, f)) {
            i = 1;
            f = FCGI_fopen(filename, "w");
            if (f) FCGI_fwrite(&i, sizeof(int), 1, f);
            else log_messages.printf(MSG_NORMAL, "Unable to open %s\n", filename);
            if (f) FCGI_fclose(f);

            f = FCGI_fopen(filename, "r");
            if (f) FCGI_fread(&i, sizeof(int), 1, f);
        }
        if (f) FCGI_fclose(f);
        log_messages.printf(MSG_NORMAL, "User selected mode %d\n", i);

        mode = i;
    }

    if (mode == 1) {
        log_messages.printf(MSG_NORMAL, "User selected mode _nci_\n", i);
        r_mode_str = "_nci_";
        return "_nci_";
    }
    if (mode == 2) {
        log_messages.printf(MSG_NORMAL, "User selected mode _ci_\n", i);
        r_mode_str = "_ci_";
        return "_ci_";
    }
    if (mode == 3) {
        log_messages.printf(MSG_NORMAL, "User selected mode _nci_onepercore_\n", i);
        r_mode_str = "_nci_onepercore_";
        return "_nci_onepercore_";
    }
    if (mode == 4) {
        log_messages.printf(MSG_NORMAL, "User selected mode _ci_onepercore_\n", i);
        r_mode_str = "_ci_onepercore_";
        return "_ci_onepercore_";
    }
    if (mode == 5) {
        log_messages.printf(MSG_NORMAL, "User selected mode _nci_onepercore_noht_\n", i);
        r_mode_str = "_nci_onepercore_noht_";
        return "_nci_onepercore_noht_";
    }
    if (mode == 6) {
        log_messages.printf(MSG_NORMAL, "User selected mode _ci_onepercore_noht_\n", i);
        r_mode_str = "_ci_onepercore_noht_";
        return "_ci_onepercore_noht_";
    }
    log_messages.printf(MSG_NORMAL, "Automatically selected mode _nci_\n", i);
    r_mode_str = "_nci_";
    return "_nci_";
}


const char* behave_str() {
    if (r_behave_str) {
        return r_behave_str;
    }
    int behave = 1;
    int i;
    {
        char filename[300];
        sprintf(filename, "/home/boincadm/www/projects/freehal_at_home/limits/behave-%d-%s", g_reply->user.id,    strlen(g_reply->host.venue) ?
        (
            (
                    0 == strcmp(g_reply->host.venue,    "home")
                ||    0 == strcmp(g_reply->host.venue,    "school")
                ||    0 == strcmp(g_reply->host.venue,    "work")
            )
            ? g_reply->host.venue
            : "home"
        )
        : "home"
        );

        log_messages.printf(MSG_NORMAL,
            "filename: %s\n",
            filename
        );

        FCGI_FILE* f = FCGI_fopen(filename, "r");
        int i = 1;
        if (!f || !FCGI_fread(&i, sizeof(int), 1, f)) {
            i = strstr(mode_str(), "_nci_") ? 1 : 0;
            f = FCGI_fopen(filename, "w");
            if (f) FCGI_fwrite(&i, sizeof(int), 1, f);
            else log_messages.printf(MSG_NORMAL, "Unable to open %s\n", filename);
            if (f) FCGI_fclose(f);

            f = FCGI_fopen(filename, "r");
            if (f) FCGI_fread(&i, sizeof(int), 1, f);
        }
        FCGI_fclose(f);
        log_messages.printf(MSG_NORMAL, "User selected behave mode %d\n", i);

        behave = i;
    }

    if (behave == 1) {
        log_messages.printf(MSG_NORMAL, "User selected behave mode _nci_\n", i);
        r_behave_str = "_nci_";
        return "_nci_";
    }
    if (behave == 2) {
        log_messages.printf(MSG_NORMAL, "User selected behave mode _ci_\n", i);
        r_behave_str = "_ci_";
        return "_ci_";
    }
    r_behave_str = mode_str();
    log_messages.printf(MSG_NORMAL, "Automatically selected behave mode %s\n", r_behave_str);
    return r_behave_str;
}

void message_for_users() {
/*
    g_reply->insert_message(
                       "Since June you can select whether you want to get 'cpu intensive' or 'non cpu intensive' work at FreeHAL.",
                       "high"
                    );
    g_reply->insert_message(
                       "'non cpu intensive' means: 4 hours, 0.1% to 0.5% CPU, < 50 MB RAM, credits depend on your benchmarks",
                       "high"
                    );
    g_reply->insert_message(
                       "'cpu intensive' means: about 1 hour or less, 80% to 100% CPU, < 50 MB RAM, credits depend on your benchmarks",
                       "high"
                    );
*/
}

// do fast checks on this job, i.e. ones that don't require DB access
// if any check fails, return false
//
static bool quick_check(
    WU_RESULT& wu_result, WORKUNIT& wu, BEST_APP_VERSION* &bavp,
    APP* &app, int& last_retval
) {
    int retval;

    if (wu_result.state != WR_STATE_PRESENT && wu_result.state != g_pid) {
        return false;
    }
    app = ssp->lookup_app(wu_result.workunit.appid);
    if (app == NULL) {
        return false; // this should never happen
    }

    g_wreq->no_jobs_available = false;

    // Find the app and best app_version for this host.
    //
    bavp = get_app_version(wu, true, g_wreq->reliable_only);
    if (!bavp) {
        if (config.debug_array) {
            log_messages.printf(MSG_NORMAL,
                "[array] No app version\n"
            );
        }
        return false;
    }

    if (!( (strstr(mode_str(), "_ci_") && strstr(wu.name, "_ci_")) || (strstr(mode_str(), "_nci_") && strstr(wu.name, "_nci_")) )) {
        if (complain_about_cpu_intensity) {
            log_messages.printf(MSG_NORMAL, "Wrong mode: %s\n", wu.name);
            if (strstr(mode_str(), "_ci_")) {
                system("/usr/local/bin/add-workunit-type-ci-100");
            }
            else {
                system("/usr/local/bin/add-workunit-type-nci-100");
            }
        }
        static bool already_sent = false;
        if (complain_about_cpu_intensity && !already_sent) {
            already_sent = true;
            if (strstr(mode_str(), "_ci_")) {
                g_reply->insert_message(
                       "Sorry! You selected 'cpu intensive', but I have no workunits matching your requirements at the moment.",
                       "high"
                );
            }
            else {
                g_reply->insert_message(
                       "Sorry! You selected 'non cpu intensive', but I have no workunits matching your requirements at the moment.",
                       "high"
                );
            }
        }
    }



    message_for_users();

    return true;
}

// do slow checks (ones that require DB access)
//
static bool slow_check(WU_RESULT& wu_result, WORKUNIT& wu, APP* app) {
    int n, retval;
    DB_RESULT result;
    char buf[256];

    // Don't send if we've already sent a result of this WU to this user.
    //
    if (config.one_result_per_user_per_wu) {
        sprintf(buf,
            "where workunitid=%d and userid=%d",
            wu_result.workunit.id, g_reply->user.id
        );
        retval = result.count(n, buf);
        if (retval) {
            log_messages.printf(MSG_CRITICAL,
                "send_work: can't get result count (%d)\n", retval
            );
            return false;
        } else {
            if (n>0) {
                if (config.debug_send) {
                    log_messages.printf(MSG_NORMAL,
                        "[send] [USER#%d] already has %d result(s) for [WU#%d]\n",
                        g_reply->user.id, n, wu_result.workunit.id
                    );
                }
                return false;
            }
        }
    } else if (config.one_result_per_host_per_wu) {
        // Don't send if we've already sent a result
        // of this WU to this host.
        // We only have to check this
        // if we don't send one result per user.
        //
        sprintf(buf,
            "where workunitid=%d and hostid=%d",
            wu_result.workunit.id, g_reply->host.id
        );
        retval = result.count(n, buf);
        if (retval) {
            log_messages.printf(MSG_CRITICAL,
                "send_work: can't get result count (%d)\n", retval
            );
            return false;
        } else {
            if (n>0) {
                if (config.debug_send) {
                    log_messages.printf(MSG_NORMAL,
                        "[send] [HOST#%d] already has %d result(s) for [WU#%d]\n",
                        g_reply->host.id, n, wu_result.workunit.id
                    );
                }
                return false;
            }
        }
    }

    if (app_hr_type(*app)) {
        if (already_sent_to_different_platform_careful(
            wu_result.workunit, *app
        )) {
            if (config.debug_send) {
                log_messages.printf(MSG_NORMAL,
                    "[send] [HOST#%d] [WU#%d %s] is assigned to different platform\n",
                    g_reply->host.id, wu.id, wu.name
                );
            }
            // Mark the workunit as infeasible.
            // This ensures that jobs already assigned to a platform
            // are processed first.
            //
            wu_result.infeasible_count++;
            return false;
        }
    }
    return true;
}

// Check for pathological conditions that mean
// result is not sendable at all.
//
static bool result_still_sendable(DB_RESULT& result, WORKUNIT& wu) {
    int retval = result.lookup_id(result.id);
    if (retval) {
        log_messages.printf(MSG_CRITICAL,
            "[RESULT#%d] result.lookup_id() failed %d\n",
            result.id, retval
        );
        return false;
    }
    if (result.server_state != RESULT_SERVER_STATE_UNSENT) {
        log_messages.printf(MSG_NORMAL,
            "[RESULT#%d] expected to be unsent; instead, state is %d\n",
            result.id, result.server_state
        );
        return false;
    }
    if (result.workunitid != wu.id) {
        log_messages.printf(MSG_CRITICAL,
            "[RESULT#%d] wrong WU ID: wanted %d, got %d\n",
            result.id, wu.id, result.workunitid
        );
        return false;
    }
    return true;
}

// Make a pass through the wu/results array, sending work.
// The choice of jobs is limited by flags in g_wreq, as follows:
// infeasible_only:
//      send only results that were previously infeasible for some host
// reliable_only: 
//      send only retries
// user_apps_only:
//      Send only jobs for apps selected by user
// beta_only:
//      Send only jobs for beta-test apps
//
// Return true if no more work is needed.
//
static bool scan_work_array() {
    log_messages.printf(MSG_NORMAL, "Running scan_work_array....\n");
    WORKUNIT wu;
    char buf[256];
    int i, j, retval, rnd_off, last_retval=0;;
    APP* app;
    BEST_APP_VERSION* bavp;
    bool no_more_needed = false;
    DB_RESULT result;

    lock_sema();
    
    rnd_off = rand() % ssp->max_wu_results;
    for (j=0; j<ssp->max_wu_results; j++) {
//    if (j % 45 == 0)
//        log_messages.printf(MSG_NORMAL, ".\n");
        i = j; // (j+rnd_off) % ssp->max_wu_results;

        WU_RESULT& wu_result = ssp->wu_results[i];
        WORKUNIT wu = wu_result.workunit;

    if (strstr(mode_str(), "_ci_") && !strstr(wu.name, "_ci_")) {
        if (complain_about_cpu_intensity || accept_any_cpu_intensity) {
            log_messages.printf(MSG_NORMAL, "user requested ci, workunit #%d %s is nci\n", j, wu.name);
            if (!accept_any_cpu_intensity) continue;
        }
        else {
            continue;
        }
    }
    if (strstr(mode_str(), "_nci_") && !strstr(wu.name, "_nci_")) {
        if (complain_about_cpu_intensity || accept_any_cpu_intensity) {
            log_messages.printf(MSG_NORMAL, "user requested nci, workunit #%d %s is ci\n", j, wu.name);
            if (!accept_any_cpu_intensity) continue;
        }
        else {
            continue;
        }
    }

        // do fast (non-DB) checks
        //
//        app = ssp->lookup_app(wu_result.workunit.appid);
//        if (app == NULL) continue; // this should never happen
        if (!quick_check(wu_result, wu, bavp, app, last_retval)) {
            log_messages.printf(MSG_NORMAL, "quick_check failed\n");
            continue;
        }

        // mark wu_result as checked out and release semaphore.
        // from here on in this loop, don't continue on failure;
        // instead, goto dont_send (so that we reacquire semaphore)
        //
        // Note: without the semaphore we don't have mutual exclusion;
        // ideally we should use a transaction from now until when
        // we commit to sending the results.

        wu_result.state = g_pid;
        unlock_sema();

        if (!slow_check(wu_result, wu, app)) {
            // if we couldn't send the result to this host,
            // set its state back to PRESENT
            //
            wu_result.state = WR_STATE_PRESENT;

            log_messages.printf(MSG_NORMAL, "slow_check failed\n");
        } else {
            result.id = wu_result.resultid;

            // mark slot as empty AFTER we've copied out of it
            // (since otherwise feeder might overwrite it)
            //
            wu_result.state = WR_STATE_EMPTY;

            // reread result from DB, make sure it's still unsent
            // TODO: from here to end of add_result_to_reply()
            // (which updates the DB record) should be a transaction
            //
            if (result_still_sendable(result, wu)) {
                retval = add_result_to_reply(result, wu, bavp, false);
                ++nwus;

                // add_result_to_reply() fails only in pathological cases -
                // e.g. we couldn't update the DB record or modify XML fields.
                // If this happens, don't replace the record in the array
                // (we can't anyway, since we marked the entry as "empty").
                // The feeder will eventually pick it up again,
                // and hopefully the problem won't happen twice.
            }
            else {
                log_messages.printf(MSG_NORMAL, "result_still_sendable failed\n");
            }
        }
        lock_sema();
        if (!work_needed(false)) {
            no_more_needed = true;
log_messages.printf(MSG_NORMAL,
                "[send] No more work needed.\n"
            );

            break;
        }
    }
    unlock_sema();
    log_messages.printf(MSG_NORMAL, "Ran scan_work_array....\n");
    return no_more_needed;
}

// Send work by scanning the job array multiple times,
// with different selection criteria on each scan.
// This has been superceded by send_work_matchmaker()
//
void send_work_old() {
    int i = 5;
    while (i >= 0 && work_needed(false)) {
    --i;

    // activate this to send the other one if the selected one is not available!!!
    //
    // accept_any_cpu_intensity = (i == 0);
    complain_about_cpu_intensity = (i == 0);

    g_wreq->beta_only = false;
    g_wreq->user_apps_only = true;
    g_wreq->infeasible_only = false;

    // give top priority to results that require a 'reliable host'
    //
    if (g_wreq->has_reliable_version) {
        g_wreq->reliable_only = true;
        if (scan_work_array()) return;
        g_wreq->reliable_only = false;
        g_wreq->best_app_versions.clear();
    }

    // give 2nd priority to results for a beta app
    // (projects should load beta work with care,
    // otherwise your users won't get production work done!
    //
    if (g_wreq->allow_beta_work) {
        g_wreq->beta_only = true;
        if (config.debug_send) {
            log_messages.printf(MSG_NORMAL,
                "[send] [HOST#%d] will accept beta work.  Scanning for beta work.\n",
                g_reply->host.id
            );
        }
        if (scan_work_array()) return;
    }
    g_wreq->beta_only = false;

    // give next priority to results that were infeasible for some other host
    //
    g_wreq->infeasible_only = true;
    if (scan_work_array()) return;;

    // that takes care of high-priority cases.  Now do general scan.
    //
    g_wreq->infeasible_only = false;
    if (scan_work_array()) return;

    // If user has selected apps but will accept any,
    // and we haven't found any jobs for selected apps, try others
    //
    if (!g_wreq->njobs_sent && g_wreq->allow_non_preferred_apps ) {
        g_wreq->user_apps_only = false;
        preferred_app_message_index = g_wreq->no_work_messages.size();
        if (config.debug_send) {
            log_messages.printf(MSG_NORMAL,
                "[send] [HOST#%d] is looking for work from a non-preferred application\n",
                g_reply->host.id
            );
        }
        scan_work_array();
    }

    } // while
}

const char *BOINC_RCSID_d9f764fd14="$Id: sched_array.cpp 21181M 2012-01-02 16:24:51Z (lokal) $";
