
#include "validator-inc.cpp"
#include "val-ass-del-pur.cpp"

// make one pass through the workunits with need_validate set.
// return true if there were any
//
bool do_validate_scan() {
    DB_VALIDATOR_ITEM_SET validator;
    std::vector<VALIDATOR_ITEM> items;
    bool found=false;
    int retval, i=0;

    // loop over entries that need to be checked
    //
    while (1) {
        retval = validator.enumerate(
            app.id, SELECT_LIMIT, wu_id_modulus, wu_id_remainder, items
        );
        if (retval) {
            if (retval != ERR_DB_NOT_FOUND) {
                log_messages.printf(MSG_DEBUG,
                    "DB connection lost, exiting\n"
                );
                exit(0);
            }
            break;
        }
        retval = handle_wu(validator, items);
        if (!retval) found = true;
        if (++i == one_pass_N_WU) break;
    }
    return found;
}

int main_loop() {
    int retval;
    bool did_something;
    char buf[256];

    retval = boinc_db.open(
        config.db_name, config.db_host, config.db_user, config.db_passwd
    );
    if (retval) {
        log_messages.printf(MSG_CRITICAL, "boinc_db.open failed: %d\n", retval);
        exit(1);
    }

    sprintf(buf, "where name='%s'", app_name);

    while (1) {
        check_stop_daemons();

        // look up app within the loop,
        // in case its min_avg_pfc has been changed by the feeder
        //
        retval = app.lookup(buf);
        if (retval) {
            log_messages.printf(MSG_CRITICAL, "can't find app %s\n", app_name);
            exit(1);
        }
        did_something = do_validate_scan();
        if (!did_something) {
            write_modified_app_versions(app_versions);
            if (one_pass) break;
#ifdef GCL_SIMULATOR
            char nameforsim[64];
            sprintf(nameforsim, "validator%i", app.id);
            continue_simulation(nameforsim);
            signal(SIGUSR2, simulator_signal_handler);
            pause();
#else
            sleep(sleep_interval);
#endif
        }
    }
    return 0;
}

// For use by project-supplied routines check_set() and check_match()
//
int debug_level=0;

int main(int argc, char** argv) {
    int i, retval;

    const char *usage = 
      "\nUsage: %s -app <app-name> [OPTIONS]\n"
      "Start validator for application <app-name>\n\n"
      "Optional arguments:\n"
      "  --one_pass_N_WU N       Validate at most N WUs, then exit\n"
      "  --one_pass              Make one pass through WU table, then exit\n"
      "  --mod n i               Process only WUs with (id mod n) == i\n"
      "  --max_claimed_credit X  If a result claims more credit than this, mark it as invalid\n"
      "  --max_granted_credit X  Grant no more than this amount of credit to a result\n"
      "  --grant_claimed_credit  Grant the claimed credit, regardless of what other results for this workunit claimed\n"
      "  --update_credited_job   Add record to credited_job table after granting credit\n"
      "  --credit_from_wu        Credit is specified in WU XML\n"
      "  --sleep_interval n      Set sleep-interval to n\n"
      "  -d n, --debug_level n   Set log verbosity level, 1-4\n\n";

    if ((argc > 1) && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
        printf (usage, argv[0] );
        exit(1);
    }


    check_stop_daemons();

    for (i=1; i<argc; i++) {
        if (is_arg(argv[i], "one_pass_N_WU")) {
            one_pass_N_WU = atoi(argv[++i]);
            one_pass = true;
        } else if (is_arg(argv[i], "sleep_interval")) {
            sleep_interval = atoi(argv[++i]);
        } else if (is_arg(argv[i], "one_pass")) {
            one_pass = true;
        } else if (is_arg(argv[i], "app")) {
            strcpy(app_name, argv[++i]);
        } else if (is_arg(argv[i], "d") || is_arg(argv[i], "debug_level")) {
            debug_level = atoi(argv[++i]);
            log_messages.set_debug_level(debug_level);
            if (debug_level == 4) g_print_queries = true;
        } else if (is_arg(argv[i], "mod")) {
            wu_id_modulus = atoi(argv[++i]);
            wu_id_remainder = atoi(argv[++i]);
        } else if (is_arg(argv[i], "max_granted_credit")) {
            max_granted_credit = atof(argv[++i]);
        } else if (is_arg(argv[i], "max_claimed_credit")) {
            max_claimed_credit = atof(argv[++i]);
        } else if (is_arg(argv[i], "grant_claimed_credit")) {
            grant_claimed_credit = true;
        } else if (is_arg(argv[i], "update_credited_job")) {
            update_credited_job = true;
        } else if (is_arg(argv[i], "credit_from_wu")) {
            credit_from_wu = true;
        } else {
            fprintf(stderr,
                "Invalid option '%s'\nTry `%s --help` for more information\n",
                argv[i], argv[0]
            );
            log_messages.printf(MSG_CRITICAL, "unrecognized arg: %s\n", argv[i]);
            exit(1);
        }
    }

    if (app_name[0] == 0) {
        log_messages.printf(MSG_CRITICAL,
            "must use '--app' to specify an application\n"
        );
        printf (usage, argv[0] );
        exit(1);      
    }

    retval = config.parse_file();
    if (retval) {
        log_messages.printf(MSG_CRITICAL,
            "Can't parse config.xml: %s\n", boincerror(retval)
        );
        exit(1);
    }

    log_messages.printf(MSG_NORMAL,
        "Starting validator, debug level %d\n", log_messages.debug_level
    );
    if (wu_id_modulus) {
        log_messages.printf(MSG_NORMAL,
            "Modulus %d, remainder %d\n", wu_id_modulus, wu_id_remainder
        );
    }

    install_stop_signal_handler();

    main_loop();
}

const char *BOINC_RCSID_634dbda0b9 = "$Id: validator.cpp 21230M 2012-01-02 16:24:52Z (lokal) $";
