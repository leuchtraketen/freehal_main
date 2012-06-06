



int get_file_path(
    const char *filename, char* upload_dir, int fanout, char* path
) {
    dir_hier_path(filename, upload_dir, fanout, path, true);
    if (boinc_file_exists(path)) {
        return 0;
    }
    char* p = strrchr(path, '/');
    *p = 0;
    if (boinc_file_exists(path)) {
        return ERR_NOT_FOUND;
    }
    return ERR_OPENDIR;
}


int wu_delete_files(WORKUNIT& wu) {
    char* p;
    char filename[256], pathname[256], buf[BLOB_SIZE+3];
    bool no_delete=false;
    int count_deleted = 0, retval, mthd_retval = 0;

    if (strstr(wu.name, "nodelete")) return 0;

    strcpy(buf, wu.xml_doc);
    
    p = strtok(buf, "\n");
    strcpy(filename, "");
    while (p) {
        if (parse_str(p, "<name>", filename, sizeof(filename))) {
        } else if (match_tag(p, "<file_info>")) {
            no_delete = false;
            strcpy(filename, "");
        } else if (match_tag(p, "<no_delete/>")) {
            no_delete = true;
        } else if (match_tag(p, "</file_info>")) {
            if (!no_delete) {
                retval = get_file_path(
                    filename, config.download_dir, config.uldl_dir_fanout,
                    pathname
                );
            if (strstr(filename, ".zip")) {
                count_deleted++;
            }
            else if (strstr(filename, "SLOW")) {
                count_deleted++;
            }
            else if (strstr(filename, ".pm")) {
                count_deleted++;
            }
            else {
                if (retval == ERR_OPENDIR) {
                    log_messages.printf(MSG_CRITICAL,
                        "[WU#%d] missing dir for %s\n",
                        wu.id, filename
                    );
                    mthd_retval = ERR_UNLINK;
                } else if (retval) {
                    log_messages.printf(MSG_CRITICAL,
                        "[WU#%d] get_file_path: %s: %d\n",
                        wu.id, filename, retval
                    );
                } else {
                    log_messages.printf(MSG_NORMAL,
                        "[WU#%d] deleting %s\n", wu.id, filename
                    );
                    retval = unlink(pathname);
                    if (retval) {
                        log_messages.printf(MSG_CRITICAL,
                            "[WU#%d] unlink %s failed: %d\n",
                            wu.id, filename, retval
                        );
                        mthd_retval = ERR_UNLINK;
                    } else {
                        count_deleted++;
                    }
                }
            }
            }
        }
        p = strtok(0, "\n");
    }
    log_messages.printf(MSG_DEBUG,
        "[WU#%d] deleted %d file(s)\n", wu.id, count_deleted
    );
    return 0;
//    return mthd_retval;
}


int purge_results(DB_WORKUNIT& wu, int& number_results) {
    int retval= 0;
    DB_RESULT result;
    char buf[256];

    number_results=0;

    sprintf(buf, "where workunitid=%d", wu.id);
    while (!result.enumerate(buf)) {
        retval = result.delete_from_db();
        if (retval) return retval;
        log_messages.printf(MSG_DEBUG,
            "Purged result [%d] from database\n", result.id
        );
        number_results++;
    }
    return 0;
}









