#include <iostream>
#include <fstream>
using namespace std;

#include "credit-amount.h"

#include "validator-inc.cpp"
#include "val-ass-del-pur.cpp"

int got_enough_host(int hostid) {
 struct tm *time_val;
  time_t unix_time;
  unix_time = time(NULL);
  time_val = localtime(&unix_time);
  if (time_val == NULL) {
        log_messages.printf(MSG_NORMAL,
            "somthing going wrong with localtime!!!!!"
        );
  }
  
    printf("Today is the %d/%d/%d\n",
             time_val->tm_mon + 1,
             time_val->tm_mday,
             time_val->tm_year + 1900);


  char path[499];
  sprintf(path, "/var/www/projects/freehal_at_home/date-limits/%d/%d/%d/h-%d.limit", time_val->tm_year + 1900, time_val->tm_mon + 1, time_val->tm_mday, hostid);
  {
    ifstream i(path);
    if (!i) {
      char _cmd[499];
      sprintf(_cmd, "mkdir -p /var/www/projects/freehal_at_home/date-limits/%d/%d/%d", time_val->tm_year + 1900, time_val->tm_mon + 1, time_val->tm_mday);
    system(_cmd);
      ofstream o(path);
          o << 0;
      o.close();
    }
    else {
       int size = 0;
        i >> size;
    if (size == 0) {
      char _cmd[499];
      sprintf(_cmd, "mkdir -p /var/www/projects/freehal_at_home/date-limits/%d/%d/%d", time_val->tm_year + 1900, time_val->tm_mon + 1, time_val->tm_mday);
    system(_cmd);
      ofstream o(path);
          o << 0;
      o.close();
    }
      i.close();
        }
  }
  {
    ifstream i(path);
    int size = 0;
    i >> size;
    i.close();
    size += CREDIT_AMOUNT;
    ofstream o(path);
    o << size;
    o.close();
    if (size > 15000) {
        log_messages.printf(MSG_NORMAL,
            "host %d GOT ENOUGH: %d \t\t\t\tnew: %d\n", hostid, size, CREDIT_AMOUNT
        );
        return 1;
    }
        log_messages.printf(MSG_NORMAL,
            "host %d     not   GOT ENOUGH: %d \t\t\t\tnew: %d\n", hostid, size, CREDIT_AMOUNT
        );
  }
    return 0;
}

int got_enough_user(int userid) {
 struct tm *time_val;
  time_t unix_time;
  unix_time = time(NULL);
  time_val = localtime(&unix_time);
  if (time_val == NULL) {
        log_messages.printf(MSG_NORMAL,
            "somthing going wrong with localtime!!!!!"
        );
  }
  

  char path[499];
  sprintf(path, "/var/www/projects/freehal_at_home/date-limits/%d/%d/%d/u-%d.limit", time_val->tm_year + 1900, time_val->tm_mon + 1, time_val->tm_mday, userid);
  {
    ifstream i(path);
    if (!i) {
      char _cmd[499];
      sprintf(_cmd, "mkdir -p /var/www/projects/freehal_at_home/date-limits/%d/%d/%d", time_val->tm_year + 1900, time_val->tm_mon + 1, time_val->tm_mday);
    system(_cmd);
      ofstream o(path);
          o << 0;
      o.close();
    }
    else {
       int size = 0;
        i >> size;
    if (size == 0) {
      char _cmd[499];
      sprintf(_cmd, "mkdir -p /var/www/projects/freehal_at_home/date-limits/%d/%d/%d", time_val->tm_year + 1900, time_val->tm_mon + 1, time_val->tm_mday);
    system(_cmd);
      ofstream o(path);
          o << 0;
      o.close();
    }
      i.close();
        }
  }
  {
    ifstream i(path);
    int size = 0;
    i >> size;
    i.close();
    size += CREDIT_AMOUNT;
    ofstream o(path);
    o << size;
    o.close();
    if (size > 100000) {
        log_messages.printf(MSG_NORMAL,
            "user %d GOT ENOUGH: %d \t\t\t\tnew: %d\n", userid, size, CREDIT_AMOUNT
        );
        return 1;
    }
        log_messages.printf(MSG_NORMAL,
            "user %d     not   GOT ENOUGH: %d \t\t\t\tnew: %d\n", userid, size, CREDIT_AMOUNT
        );
  }
    return 0;
}

int is_ok(DB_RESULT& result) {
    if (result.received_time - result.sent_time > 3*60) {
        log_messages.printf(MSG_NORMAL,
            "WU %d run time is ok: %d min\n", result.id, (result.received_time - result.sent_time)/60
        );
        return 1;
    }
        log_messages.printf(MSG_NORMAL,
            "STOP: USER IS CHEATING: user=%d, host=%d, WU run time is ok: %d min\n", result.userid, result.hostid, (result.received_time - result.sent_time)/60
        );

    return 0;
}

int update_credit_amount(DB_RESULT& result) {
    DB_HOST host;
    int retval;
    char buf[256];

    retval = host.lookup_id(result.hostid);
    if (retval) {
        log_messages.printf(MSG_CRITICAL,
            "VALIDATE_FAST: [RESULT#%d] lookup of host %d failed %d\n",
            result.id, result.hostid, retval
        );
        return retval;
    }

    compute_credit_rating(host);

    if (host.claimed_credit_per_cpu_sec > 0)
        CREDIT_AMOUNT = 35 + host.claimed_credit_per_cpu_sec*3600 * 1.98103 + rand() % 25;

if (strstr(result.name, "_ci_")) {
        time_t diff = result.received_time - result.sent_time;
        if (diff > 3600) diff = 3600;
        if (diff < 300) diff = 3600;
        CREDIT_AMOUNT = CREDIT_AMOUNT * diff / 3600;
}

if (CREDIT_AMOUNT < 17) CREDIT_AMOUNT = 17;


    snprintf(T_CREDIT_AMOUNT, 10, "%d", CREDIT_AMOUNT);
}

double grant(DB_RESULT& result) {
    DB_HOST host;
    int retval;
    char buf[256];

    retval = host.lookup_id(result.hostid);
    if (retval) {
        log_messages.printf(MSG_CRITICAL,
            "VALIDATE_FAST: [RESULT#%d] lookup of host %d failed %d\n",
            result.id, result.hostid, retval
        );
        return retval;
    }

    result.granted_credit = CREDIT_AMOUNT;
/*
if (strstr(result.name, "_ci_")) {
    time_t diff = result.received_time - result.sent_time;
    if (diff > 3600) diff = 3600;
    if (diff < 300) diff = 3600;
    result.granted_credit = CREDIT_AMOUNT * diff / 3600;
}

if (result.granted_credit < 1) result.granted_credit = 17;
*/

    log_messages.printf(MSG_NORMAL,
        "grant %fl credits for result %d\n", (float)result.granted_credit, result.id
    );

    grant_credit_fast(host, result.sent_time, result.cpu_time, result.granted_credit);

    return result.granted_credit;
}

int main(int argc, char** argv) {
    int retval;
    check_stop_daemons();

    retval = config.parse_file();
    if (retval) {
        log_messages.printf(MSG_CRITICAL,
            "Can't parse config.xml: %s\n", boincerror(retval)
        );
        exit(1);
    }

    log_messages.printf(MSG_NORMAL,
        "Starting validate_fast, debugg level %d\n", log_messages.debug_level
    );
    if (wu_id_modulus) {
        log_messages.printf(MSG_NORMAL,
            "Modulus %d, remainder %d\n", wu_id_modulus, wu_id_remainder
        );
    }

    install_stop_signal_handler();

    retval = boinc_db.open(
        config.db_name, config.db_host, config.db_user, config.db_passwd
    );
    if (retval) {
        log_messages.printf(MSG_CRITICAL, "boinc_db.open failed: %d\n", retval);
        exit(1);
    }

const char* LIMIT = argv[1];
int mod_n = 0, mod_i = 0;
if (LIMIT) {
    if (0 == strcmp(LIMIT, "-mod")) {
        mod_n = atoi(argv[2]);
        mod_i = atoi(argv[3]);
        LIMIT = 0;
    }
}

int n_workunits = 0;
while (1) {

int sleep_time = 5 - n_workunits;
sleep_time = sleep_time < 5 ? sleep_time : 4;
sleep_time = sleep_time > 0  ? sleep_time : 1;
sleep(sleep_time);

n_workunits = 0;

    std::vector<int> wus;
    std::vector<int> ress;

    DB_RESULT res;

    char _buf[2560];
    sprintf(_buf,
//      "where client_state = 5 and server_state = 5 and validate_state IN (0, 1) limit %s",
        "where server_state = 5 and validate_state IN (0, 1) limit %s",
        LIMIT ? LIMIT : "50"
    );
    log_messages.printf(MSG_NORMAL,
        "VALIDATE_FAST: SQL (1): %s\n", _buf
    );
    int i = 0;
    while (1) {
        retval = res.enumerate(_buf);
        if (retval)
            break;

        if ( mod_n && (res.id % mod_n) != mod_i ) {
//log_messages.printf(MSG_NORMAL, "modulus: %d mod %d = %d != %d\n", res.id, mod_n, (res.id % mod_n), mod_i);

        wus.push_back(res.workunitid);
        ress.push_back(res.id);

            continue;
    }
//log_messages.printf(MSG_NORMAL, "modulus: %d mod %d = %d == %d\n", res.id, mod_n, (res.id % mod_n), mod_i);

    if (res.id == 0)
        continue;

    if (std::find(ress.begin(), ress.end(), res.id) != ress.end()) {
            if(0) log_messages.printf(MSG_NORMAL,
                "[RES#%d] BUG: result %d already processed and deleted!\n", res.id, res.id
            );
            continue;
    }

        wus.push_back(res.workunitid);
        ress.push_back(res.id);

update_credit_amount(res);

    int host_got_enough = got_enough_host(res.hostid);
    int user_got_enough = got_enough_user(res.userid);

    double granted_credit = 0;
        if (res.outcome == RESULT_OUTCOME_SUCCESS && is_ok(res) && !host_got_enough && !user_got_enough) {
        granted_credit = grant(res);
    }

  time_t unix_time;
  unix_time = time(NULL);
    if (!is_ok(res) || host_got_enough || user_got_enough || res.outcome != 1) {
        ofstream log("/var/www/projects/freehal_at_home/wu-logs/deleted.log", ios::app);
        log << res.userid << "_" << res.hostid << "," << res.id << "," << res.name << (host_got_enough?"GEH":"") << (user_got_enough?"GEU":"") << "," << res.outcome << "," << 0 << ","
            << res.received_time << "," << res.sent_time << "," << unix_time << endl;
        log.close();
        char errfile[1000];
        snprintf(errfile, 999, "/var/www/projects/freehal_at_home/wu-logs/err/%d-%d-%d", res.userid, res.hostid, res.id);
        ofstream errlog(errfile, ios::app);
        errlog << res.userid << "_" << res.hostid << "," << res.id << "," << res.name << "," << res.outcome << "," << 0 << ","
            << res.received_time << "," << res.sent_time << "," << unix_time << endl;
        errlog << res.stderr_out << endl;
        errlog.close();
//        continue;
    }
    else {
        ofstream log("/var/www/projects/freehal_at_home/wu-logs/deleted.log", ios::app);
        log << res.userid << "_" << res.hostid << "," << res.id << "," << res.name << "," << res.outcome << "," << granted_credit << ","
            << res.received_time << "," << res.sent_time << "," << unix_time << endl;
        log.close();

        if (res.received_time-res.sent_time > 60*90) {
        char errfile[1000];
        snprintf(errfile, 999, "/var/www/projects/freehal_at_home/wu-logs/out/%d-%d-%d", res.userid, res.hostid, res.id);
        ofstream errlog(errfile, ios::app);
        errlog << res.userid << "_" << res.hostid << "," << res.id << "," << res.name << "," << res.outcome << "," << 0 << ","
            << res.received_time << "," << res.sent_time << "," << unix_time << endl;
        errlog << res.stderr_out << endl;
        errlog.close();
        }
    }
/*    }

    int i;
    for(i = 0; i < wus.size(); ++i) {*/
        DB_WORKUNIT wu;
        retval = wu.lookup_id(wus[i]);

        retval = wu_delete_files(wu);
        if (retval) {
            log_messages.printf(MSG_CRITICAL,
                "[WU#%d] VALIDATE_FAST: FAILED: file deletion: %d\n", wu.id, retval
            );
        } else {
            log_messages.printf(MSG_NORMAL,
                "[WU#%d] VALIDATE_FAST: file deleted: %d\n", wu.id, retval
            );
        }

        int n;
        retval = purge_results(wu, n);
        if (retval) {
            log_messages.printf(MSG_CRITICAL,
                "[WU#%d] VALIDATE_FAST: FAILED: results purge:%d\n", wu.id, retval
            );
        }
        log_messages.printf(MSG_NORMAL,
            "[WU#%d] VALIDATE_FAST: Purged results from database\n", wu.id
        );

        retval = wu.delete_from_db();
        if (retval) {
            log_messages.printf(MSG_CRITICAL,
                "[WU#%d] VALIDATE_FAST: FAILED: workunit purge:%d\n", wu.id, retval
            );
        }
        log_messages.printf(MSG_NORMAL,
            "[WU#%d] VALIDATE_FAST: Purged workunit from database\n", wu.id
        );

        retval = res.delete_from_db();
        if (retval) {
            log_messages.printf(MSG_CRITICAL,
                "[WU#%d] VALIDATE_FAST: FAILED: result purge (II):%d\n", wu.id, retval
            );
        }
        log_messages.printf(MSG_NORMAL,
            "[WU#%d] VALIDATE_FAST: Purged result from database (II)\n", wu.id
        );

        ++i;

    {
            char path[256];
            retval = config.download_path(wu.name, path);
            if (!retval) {
                printf("path=%s\n", path);
        }
            unlink(path);
    }



    }

    if (LIMIT) {
        exit(0);
    }
}

}

const char *BOINC_RCSID_634dbda0b9 = "$Id: validator.cpp 19054 2009-09-16 03:10:22Z davea $";
