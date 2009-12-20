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

#include "hal2009-universal.h"
#include "hal2009-ram.h"
#include "hal2009-disk.h"

#include "hal2009-universal.c"
#include "hal2009-ram.c"
#include "hal2009-disk.c"

int sql_universal_set_filename(const char* filename) {
}

int sql_universal_begin() {
    return universal_begin();
}

int sql_universal_end() {
    return universal_end();
}

char* sql_universal_del_record(struct RECORD* r) {
}

int sql_universal_add_record(struct RECORD* r, const char* relation_to) {
    if (relation_to && relation_to[0]) {
        return 0;
    }
    else {
        struct fact* fact = add_fact(r->subjects, r->objects, r->verb, r->adverbs, r->extra, r->questionword, r->from, r->truth, r->verb_flag_want, r->verb_flag_must, r->verb_flag_can, r->verb_flag_may, r->verb_flag_should);
        
        if (fact && fact->pk) {
            FILE* input_key = fopen("_input_key", "w+b");
            if (input_key) {
                fprintf(input_key, "%d", fact->pk);
                fclose(input_key);
            }
        }
        if (r->clauses && r->clauses[0] && fact && fact->pk) {
            int n;
            int broken = 0;
            for (n = 0; n <= r->num_clauses && n+1 < MAX_CLAUSES && r->clauses && r->clauses[n]; ++n) {
                if (broken) {
                    continue;
                }
                if (!add_clause(fact->pk,
                        ((struct RECORD*)(r->clauses[n]))->subjects,
                        ((struct RECORD*)(r->clauses[n]))->objects,
                        ((struct RECORD*)(r->clauses[n]))->verb,
                        ((struct RECORD*)(r->clauses[n]))->adverbs,
                        ((struct RECORD*)(r->clauses[n]))->extra,
                        ((struct RECORD*)(r->clauses[n]))->questionword,
                        ((struct RECORD*)(r->clauses[n]))->from,
                        ((struct RECORD*)(r->clauses[n]))->truth,
                        ((struct RECORD*)(r->clauses[n]))->verb_flag_want,
                        ((struct RECORD*)(r->clauses[n]))->verb_flag_must,
                        ((struct RECORD*)(r->clauses[n]))->verb_flag_can,
                        ((struct RECORD*)(r->clauses[n]))->verb_flag_may,
                        ((struct RECORD*)(r->clauses[n]))->verb_flag_should
                )) {
                
                    broken = 1;
                }
            }
        }
        if (fact && fact->pk && is_engine("disk")) {
            free(fact);
        }
    }
    return 0;
}

struct DATASET sql_universal_get_records(struct RECORD* r) {
    if (r->pkey && r->pkey[0] && to_number(r->pkey)) {
        // return search_clauses_as_dataset(to_number(r->pkey));
        return as_dataset(0);
    }
    return search_facts_as_dataset(r->subjects, r->objects, r->verb, r->adverbs, r->extra, r->questionword, r->context);
}

