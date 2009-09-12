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

#include "hal2009-sql-sqlite.c"
#include "hal2009-sql-semtree.c"

static int database_used = 0;

struct DATASET sql_get_records(struct RECORD* r) {
    if (sql_engine && matchstr(sql_engine, "sqlite")) {
        return sql_sqlite_get_records(r);
    }
    else { //if (sql_engine && matchstr(sql_engine, "semtree")) {
        return sql_semtree_get_records(r);
    }
}

int sql_add_record(struct RECORD* r) {
    if (sql_engine && matchstr(sql_engine, "sqlite")) {
        return sql_sqlite_add_record(r, NULL);
    }
    else { //if (sql_engine && matchstr(sql_engine, "semtree")) {
        return sql_semtree_add_record(r, NULL);
    }
}

char* sql_del_record(struct RECORD* r) {
    if (sql_engine && matchstr(sql_engine, "sqlite")) {
        return sql_sqlite_del_record(r);
    }
    if (sql_engine && matchstr(sql_engine, "semtree")) {
        return sql_semtree_del_record(r);
    }
}

int sql_add_link (char* link, int key_1, int key_2) {
    if (sql_engine && matchstr(sql_engine, "sqlite")) {
        return sql_sqlite_add_link(link, key_1, key_2);
    }
    if (sql_engine && matchstr(sql_engine, "semtree")) {
        return sql_semtree_add_link(link, key_1, key_2);
    }
}

int sql_begin() {
    while (database_used) {
        fprintf(output(), "Wait while database is used.\n");
        sleep(1);
    }
    fprintf(output(), "%s\n", "Start database access.");
    database_used = 1;
    if (sql_engine && matchstr(sql_engine, "sqlite")) {
        return sql_sqlite_begin();
    }
    else { //if (sql_engine && matchstr(sql_engine, "semtree")) {
        return sql_semtree_begin();
    }
}

int sql_end() {
    int ret;
    if (sql_engine && matchstr(sql_engine, "sqlite")) {
        ret = sql_sqlite_end();
    }
    else { //if (sql_engine && matchstr(sql_engine, "semtree")) {
        ret = sql_semtree_end();
    }
    fprintf(output(), "%s\n", "Stop database access.");
    database_used = 0;
    
    return ret;
}

