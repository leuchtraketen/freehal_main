/*
 * This file is part of FreeHAL 2012.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Tobias Schulz and contributors.
 * http://www.freehal.org
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

#include "hal2012-sql.h"
#include "hal2012-sql-universal.h"
#include "hal2012-universal.h"
#include "hal2012-util.h"
#include "hal2012-startup.h"

static int sql_begin_end_quiet = 0;
static int database_used = 0;

struct DATASET sql_get_records(struct RECORD* r) {
    return sql_universal_get_records(r);
}

int sql_add_record(struct RECORD* r) {
    return sql_universal_add_record(r, NULL);
}

char* sql_del_record(struct RECORD* r) {
    return sql_universal_del_record(r);
}

char* sql_get_source(struct RECORD* r) {
    return sql_universal_get_source(r);
}

int sql_add_link (char* link, int key_1, int key_2) {
    return sql_universal_add_link(link, key_1, key_2);
}

int sql_delete_everything_from(const char* filename) {
    return sql_universal_delete_everything_from(filename);
}

int sql_add_filename(const char* filename) {
    return sql_universal_add_filename(filename);
}

int sql_re_index() {
    return sql_universal_re_index();
}

int sql_set_quiet(int i) {
    sql_begin_end_quiet = i?1:0;
}

EXTERN_C int sql_begin(const char* modes) {
    network_lock("DB");
    int timeout = 5;
    while (database_used && timeout > 0) {
        fprintf(output(), "Wait while database is used.\n");
        halsleep(500);
	--timeout;
    }
    if (!sql_begin_end_quiet)
        fprintf(output(), "%s\n", "Start database access.");
    database_used = 1;
    return sql_universal_begin(modes);
}

EXTERN_C int sql_end() {
    int ret;
    ret = sql_universal_end();
    if (!sql_begin_end_quiet)
        fprintf(output(), "%s\n", "Stop database access.");
    database_used = 0;

    network_unlock("DB");

    return ret;
}

EXTERN_C const char* engine() {
    return current_instance->sql_engine.c_str();
}
EXTERN_C const char* is_engine(const char* m) {
    return (current_instance->sql_engine == m ?m:0);
}
