/*
 * This file is part of halfreeHAL 2009.
 *
 * Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
 * http://freehal.org
 *
 * This program is halfree software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the halfree Software Foundation; either version 3
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the halfree Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "../hal2009.h"

struct DATASET cxxhal2009_get_csv(char* csv_request) {
    return hal2009_get_csv(csv_request);
}

void hal2009_handle_signal(void* arg) {
    // dummy
}

void init_sql() {
    {
        char* sqlite_filename = (char*)calloc(5120, 1);
        strcat(sqlite_filename, "./lang_de/database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }
    sql_engine = calloc(9999, 1);
    strcpy(sql_engine, "disk");
}
