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

// validator - check and validate results, and grant credit
//  --app appname
//  [-d N] [--debug_level N]    // log verbosity (1=least, 4=most)
//  [--one_pass_N_WU N]         // Validate only N WU in one pass, then exit
//  [--one_pass]                // make one pass through WU table, then exit
//  [--mod n i]                 // process only WUs with (id mod n) == i
//  [--max_granted_credit X]    // limit maximum granted credit to X
//  [--max_claimed_credit Y]    // invalid if claims more than Y
//  [--grant_claimed_credit]    // just grant whatever is claimed 
//  [--update_credited_job]     // add userid/wuid pair to credited_job table
//  [--credit_from_wu]          // get credit from WU XML

#include "config.h"
#include <unistd.h>
#include <climits>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <string>
#include <signal.h>

#include "boinc_db.h"
#include "util.h"
#include "str_util.h"
#include "error_numbers.h"

#include "filesys.h"


#include "credit.h"
#include "sched_config.h"
#include "sched_util.h"
#include "sched_msgs.h"
#include "validator.h"
#include "validate_util.h"
#include "validate_util2.h"
#ifdef GCL_SIMULATOR
#include "gcl_simulator.h"
#endif

#define LOCKFILE "validate.out"
#define PIDFILE  "validate.pid"

#define SELECT_LIMIT    10000
#define SLEEP_PERIOD    0.5

int sleep_interval = SLEEP_PERIOD;

typedef enum {
    NEVER,
    DELAYED,
    IMMEDIATE,
    NO_CHANGE
} TRANSITION_TIME;

char app_name[256];
DB_APP app;
int wu_id_modulus=0;
int wu_id_remainder=0;
int one_pass_N_WU=0;
bool one_pass = false;
double max_granted_credit = 0;
double max_claimed_credit = 0;
bool grant_claimed_credit = false;
bool update_credited_job = false;
bool credit_from_wu = false;
WORKUNIT* g_wup;
vector<DB_APP_VERSION> app_versions;
    // cache of app_versions; used by v2 credit system

bool is_unreplicated(WORKUNIT& wu) {
    return (wu.target_nresults == 1 && app.target_nresults > 1);
}

// Here when a result has been validated.
// - update consecutive_valid
// - udpdate turnaround stats
// - insert credited_job record if needed
//
int is_valid(DB_HOST& host, RESULT& result, WORKUNIT& wu, DB_HOST_APP_VERSION& hav) {
    DB_CREDITED_JOB credited_job;
    int retval;
    char buf[256];

    double turnaround = result.received_time - result.sent_time;
    compute_avg_turnaround(host, turnaround);

    // increment consecutive_valid, but only if unreplicated
    //
    if (!is_unreplicated(wu)) {
        hav.consecutive_valid++;
        log_messages.printf(MSG_DEBUG,
            "[HAV#%d] consecutive valid now %d\n",
            hav.app_version_id, hav.consecutive_valid
        );
    }

    if (update_credited_job) {
        credited_job.userid = host.userid;
        credited_job.workunitid = long(wu.opaque);
        retval = credited_job.insert();
        if (retval) {
            log_messages.printf(MSG_CRITICAL,
                "[RESULT#%d] Warning: credited_job insert failed (userid: %d workunit: %f err: %d)\n",
                result.id, host.userid, wu.opaque, retval
            );
        } else {
            log_messages.printf(MSG_DEBUG,
                "[RESULT#%d %s] added credited_job record [WU#%d OPAQUE#%f USER#%d]\n",
                result.id, result.name, wu.id, wu.opaque, host.userid
            );
        }
    }

    return 0;
}

static inline void is_invalid(DB_HOST_APP_VERSION& hav) {
    hav.consecutive_valid = 0;
}

// handle a workunit which has new results
//
int handle_wu(
    DB_VALIDATOR_ITEM_SET& validator, std::vector<VALIDATOR_ITEM>& items
) {
    int canonical_result_index = -1;
    bool update_result, retry;
    TRANSITION_TIME transition_time = NO_CHANGE;
    int retval = 0, canonicalid = 0, x;
    double credit = 0;
    unsigned int i;

    WORKUNIT& wu = items[0].wu;
    g_wup = &wu;

    if (wu_id_modulus && wu.id % wu_id_modulus != wu_id_remainder) return 0;

    if (wu.canonical_resultid) {
        log_messages.printf(MSG_NORMAL,
            "[WU#%d %s] Already has canonical result %d\n",
            wu.id, wu.name, wu.canonical_resultid
        );
        ++log_messages;

        // Here if WU already has a canonical result.
        // Get unchecked results and see if they match the canonical result
        //
        for (i=0; i<items.size(); i++) {
            RESULT& result = items[i].res;

            if (result.id == wu.canonical_resultid) {
                canonical_result_index = i;
            }
        }
        if (canonical_result_index == -1) {
            log_messages.printf(MSG_CRITICAL,
                "[WU#%d %s] Can't find canonical result %d\n",
                wu.id, wu.name, wu.canonical_resultid
            );
            return 0;
        }

        RESULT& canonical_result = items[canonical_result_index].res;

        // scan this WU's results, and check the unchecked ones
        //
        for (i=0; i<items.size(); i++) {
            RESULT& result = items[i].res;

            if (result.server_state != RESULT_SERVER_STATE_OVER) continue;
            if (result.outcome !=  RESULT_OUTCOME_SUCCESS) continue;
            switch (result.validate_state) {
            case VALIDATE_STATE_INIT:
            case VALIDATE_STATE_INCONCLUSIVE:
                break;
            default:
                continue;
            }
            log_messages.printf(MSG_NORMAL,
                 "[WU#%d] handle_wu(): testing result %d\n",
                 wu.id, result.id
             );

            check_pair(result, canonical_result, retry);
            if (retry) transition_time = DELAYED;
            update_result = false;

            if (result.outcome == RESULT_OUTCOME_VALIDATE_ERROR) {
                update_result = true;
            }

            // this might be last result, so let transitioner
            // trigger file delete etc. if needed
            //
            transition_time = IMMEDIATE;

            DB_HOST host;
            retval = host.lookup_id(result.hostid);
            if (retval) {
                log_messages.printf(MSG_CRITICAL,
                    "[RESULT#%d] lookup of host %d failed %d\n",
                    result.id, result.hostid, retval
                );
                continue;
            }
            HOST host_initial = host;

            bool update_hav = false;
            DB_HOST_APP_VERSION hav;
            retval = hav_lookup(hav, result.hostid,
                generalized_app_version_id(result.app_version_id, result.appid)
            );
            if (retval) {
                hav.host_id = 0;
            }
            DB_HOST_APP_VERSION hav_orig = hav;
            vector<DB_HOST_APP_VERSION> havv;
            havv.push_back(hav);

            vector<RESULT> rv;
            switch (result.validate_state) {
            case VALIDATE_STATE_VALID:
                update_result = true;
                update_hav = true;
                log_messages.printf(MSG_NORMAL,
                    "[RESULT#%d %s] pair_check() matched: setting result to valid\n",
                    result.id, result.name
                );
                retval = is_valid(host, result, wu, havv[0]);
                if (retval) {
                    log_messages.printf(MSG_NORMAL,
                        "[RESULT#%d %s] is_valid() error: %d\n",
                        result.id, result.name, retval
                    );
                }
                // do credit computation, but grant credit of canonical result
                //
                rv.push_back(result);
                assign_credit_set(
                    wu, rv, app, app_versions, havv, max_granted_credit, credit
                );
                result.granted_credit = canonical_result.granted_credit;
                grant_credit(
                    host, result.sent_time, result.cpu_time,
                    result.granted_credit
                );
                break;
            case VALIDATE_STATE_INVALID:
                update_result = true;
                update_hav = true;
                log_messages.printf(MSG_NORMAL,
                    "[RESULT#%d %s] pair_check() didn't match: setting result to invalid\n",
                    result.id, result.name
                );
                is_invalid(havv[0]);
            }
            if (hav.host_id && update_hav) {
                havv[0].update_validator(hav_orig);
            }
            host.update_diff_validator(host_initial);
            if (update_result) {
                log_messages.printf(MSG_NORMAL,
                    "[RESULT#%d %s] granted_credit %f\n",
                    result.id, result.name, result.granted_credit
                );

                retval = validator.update_result(result);
                if (retval) {
                    log_messages.printf(MSG_CRITICAL,
                        "[RESULT#%d %s] Can't update result: %d\n",
                        result.id, result.name, retval
                    );
                }
            }
        }
    } else {
        vector<RESULT> results;
        vector<DB_HOST_APP_VERSION> host_app_versions, host_app_versions_orig;
        int nsuccess_results;

        // Here if WU doesn't have a canonical result yet.
        // Try to get one

        log_messages.printf(MSG_NORMAL,
            "[WU#%d %s] handle_wu(): No canonical result yet\n",
            wu.id, wu.name
        );
        ++log_messages;

        // make a vector of the successful results,
        // and a parallel vector of host_app_versions
        //
        for (i=0; i<items.size(); i++) {
            RESULT& result = items[i].res;

            if ((result.server_state == RESULT_SERVER_STATE_OVER) &&
                (result.outcome == RESULT_OUTCOME_SUCCESS)
            ) {
                results.push_back(result);
                DB_HOST_APP_VERSION hav;
                retval = hav_lookup(hav, result.hostid,
                    generalized_app_version_id(result.app_version_id, result.appid)
                );
                if (retval) {
                    hav.host_id=0;   // flag that it's missing
                }
                host_app_versions.push_back(hav);
                host_app_versions_orig.push_back(hav);
            }
        }

        log_messages.printf(MSG_DEBUG,
            "[WU#%d %s] Found %d successful results\n",
            wu.id, wu.name, (int)results.size()
        );
        if (results.size() >= (unsigned int)wu.min_quorum) {
            log_messages.printf(MSG_DEBUG,
                "[WU#%d %s] Enough for quorum, checking set.\n",
                wu.id, wu.name
            );

            double dummy;
            retval = check_set(
                app, app_versions, results, wu, canonicalid, dummy, retry
            );
            if (retval) {
                log_messages.printf(MSG_CRITICAL,
                    "[WU#%d %s] check_set returned %d, exiting\n",
                    wu.id, wu.name, retval
                );
                return retval;
            }
            if (retry) transition_time = DELAYED;

            if (credit_from_wu) {
                retval = get_credit_from_wu(wu, results, credit);
                if (retval) {
                    log_messages.printf(MSG_CRITICAL,
                        "[WU#%d %s] get_credit_from_wu returned %d\n",
                        wu.id, wu.name, retval
                    );
                    return retval;
                }
            }

            if (canonicalid) {
                retval = assign_credit_set(
                    wu, results, app, app_versions, host_app_versions,
                    max_granted_credit, credit
                );
                if (retval) {
                    log_messages.printf(MSG_CRITICAL,
                        "[WU#%d %s] assign_credit_set() returned %d\n",
                        wu.id, wu.name, retval
                    );
                    return retval;
                }
            }

            if (max_granted_credit && credit>max_granted_credit) {
                credit = max_granted_credit;
            }

            // scan results.
            // update as needed, and count the # of results
            // that are still outcome=SUCCESS
            // (some may have changed to VALIDATE_ERROR)
            //
            nsuccess_results = 0;
            for (i=0; i<results.size(); i++) {
                RESULT& result = results[i];
                DB_HOST_APP_VERSION& hav = host_app_versions[i];
                DB_HOST_APP_VERSION& hav_orig = host_app_versions_orig[i];

                update_result = false;
                bool update_host = false;
                if (result.outcome == RESULT_OUTCOME_VALIDATE_ERROR) {
                    transition_time = IMMEDIATE;
                    update_result = true;
                } else {
                    nsuccess_results++;
                }

                DB_HOST host;
                HOST host_initial;
                switch (result.validate_state) {
                case VALIDATE_STATE_VALID:
                case VALIDATE_STATE_INVALID:
                    retval = host.lookup_id(result.hostid);
                    if (retval) {
                        log_messages.printf(MSG_CRITICAL,
                            "[RESULT#%d] lookup of host %d failed %d\n",
                            result.id, result.hostid, retval
                        );
                        continue;
                    }
                    host_initial = host;
                }

                switch (result.validate_state) {
                case VALIDATE_STATE_VALID:
                    update_result = true;
                    update_host = true;
                    retval = is_valid(host, result, wu, host_app_versions[i]);
                    if (retval) {
                        log_messages.printf(MSG_DEBUG,
                            "[RESULT#%d %s] is_valid() failed: %d\n",
                            result.id, result.name, retval
                        );
                    }
                    grant_credit(
                        host, result.sent_time, result.cpu_time,
                        result.granted_credit
                    );
                    log_messages.printf(MSG_NORMAL,
                        "[RESULT#%d %s] Valid; granted %f credit [HOST#%d]\n",
                        result.id, result.name, result.granted_credit,
                        result.hostid
                    );
                    break;
                case VALIDATE_STATE_INVALID:
                    update_result = true;
                    update_host = true;
                    log_messages.printf(MSG_NORMAL,
                        "[RESULT#%d %s] Invalid [HOST#%d]\n",
                        result.id, result.name, result.hostid
                    );
                    is_invalid(host_app_versions[i]);
                    break;
                case VALIDATE_STATE_INIT:
                    log_messages.printf(MSG_NORMAL,
                        "[RESULT#%d %s] Inconclusive [HOST#%d]\n",
                        result.id, result.name, result.hostid
                    );
                    result.validate_state = VALIDATE_STATE_INCONCLUSIVE;
                    update_result = true;
                    break;
                }

                if (hav.host_id) {
                    retval = hav.update_validator(hav_orig);
                }
                if (update_host) {
                    retval = host.update_diff_validator(host_initial);
                }
                if (update_result) {
                    retval = validator.update_result(result);
                    if (retval) {
                        log_messages.printf(MSG_CRITICAL,
                            "[RESULT#%d %s] result.update() failed: %d\n",
                            result.id, result.name, retval
                        );
                    }
                }
            }

            if (canonicalid) {
                // if we found a canonical result,
                // trigger the assimilator, but do NOT trigger
                // the transitioner - doing so creates a race condition
                //
                transition_time = NEVER;
                log_messages.printf(MSG_DEBUG,
                    "[WU#%d %s] Found a canonical result: id=%d\n",
                    wu.id, wu.name, canonicalid
                );
                wu.canonical_resultid = canonicalid;
                wu.canonical_credit = credit;
                wu.assimilate_state = ASSIMILATE_READY;

                // don't need to send any more results
                //
/*
                for (i=0; i<items.size(); i++) {
                    RESULT& result = items[i].res;

                    if (result.server_state != RESULT_SERVER_STATE_UNSENT) {
                        continue;
                    }

                    result.server_state = RESULT_SERVER_STATE_OVER;
                    result.outcome = RESULT_OUTCOME_DIDNT_NEED;
                    retval = validator.update_result(result);
                    if (retval) {
                        log_messages.printf(MSG_CRITICAL,
                            "[RESULT#%d %s] result.update() failed: %d\n",
                            result.id, result.name, retval
                        );
                    }
                }
*/
            } else {
                // here if no consensus.

                // check if #success results is too large
                //
                if (nsuccess_results > wu.max_success_results) {
                    wu.error_mask |= WU_ERROR_TOO_MANY_SUCCESS_RESULTS;
                    transition_time = IMMEDIATE;
                }

                // if #success results >= target_nresults,
                // we need more results, so bump target_nresults
                // NOTE: nsuccess_results should never be > target_nresults,
                // but accommodate that if it should happen
                //
                if (nsuccess_results >= wu.target_nresults) {
                    wu.target_nresults = nsuccess_results+1;
                    transition_time = IMMEDIATE;
                }
            }
        }
    }

    --log_messages;

    switch (transition_time) {
    case IMMEDIATE:
        wu.transition_time = time(0);
        break;
    case DELAYED:
        x = time(0) + 6*3600;
        if (x < wu.transition_time) wu.transition_time = x;
        break;
    case NEVER:
        wu.transition_time = INT_MAX;
        break;
    case NO_CHANGE:
        break;
    }

    wu.need_validate = 0;
    
    /****
retval = validator.update_workunit(wu);
    if (retval) {
        log_messages.printf(MSG_CRITICAL,
            "[WU#%d %s] update_workunit() failed: %d; exiting\n",
            wu.id, wu.name, retval
        );
        return retval;
    }
    ****/
    return 0;
}

