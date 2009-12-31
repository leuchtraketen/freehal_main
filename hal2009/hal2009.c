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

static int position_in_insertions = 0;
char* sql_engine = 0;

char* ascii(const char* not_ascii);

int _debugf(const char* c, ...) {
}

int halusleep(double seconds) {
    seconds /= 1000;
#ifdef _WIN32
    Sleep((int)(1000*seconds));
#else
    double end_time = time(NULL) + seconds - 0.01;
    while (1) {
        if (seconds >= 1) {
            sleep((unsigned int) seconds);
        } else {
            usleep((int)fmod(seconds*1000000, 1000000));
        }
        seconds = end_time - time(NULL);
        if (seconds <= 0) break;
    }
#endif
}
int halsleep(double seconds) {
    halusleep(seconds*1000);
}


void* halfilecalloc( long s, long si, char* str ) {
    void* p = calloc( s, si );
    char fn[500];
    sprintf(fn, "alloc/%d", p);
    FILE* f = fopen(fn, "w");
    fprintf(f, "%d %s", s*si/1024, str);
    halclose(f);
    return p;
}

void* halfilefree( void* s ) {
    if (s) free(s);
    char fn[500];
    sprintf(fn, "alloc/%d", s);
    unlink(fn);
    return 0;
}

char* halgetline(FILE *handle) {
    char *line;
    char zeichen;
    int length;

    length = 0;
    line = NULL;
    zeichen = fgetc(handle);

    if(zeichen == '\n') {
        line = malloc(sizeof(char)*2);
        line[0] = zeichen;
        line[1] = '\0';
        return line;
    }

    if(zeichen == EOF) {
        return NULL;
    }
   
    line = malloc(LINE_SIZE+1);
    int index = 0;
    line[index] = zeichen;
    ++index;
    do {
        zeichen = fgetc(handle);
        if(zeichen == '\n') {
            line[index] = zeichen;
            ++index;
            break;
        }
    
        if (EOF == zeichen) { /* feof() verwenden? */
            break;
        }

        line[index] = zeichen;
        ++index;
    }
    while(1 && index < LINE_SIZE -1);
    
    line[index] = 0;
      
    return line;
}

int hal2009_add_link (char* link, int key_1, int key_2) {
    sql_add_link(link, key_1, key_2);
}

int fact_replace_in_source (const char* source, const char* _replacement) {
    char* replacement;
    if (_replacement) {
        replacement = calloc(strlen(_replacement)+5, 1);
        strcpy(replacement, _replacement);
        strcat(replacement, "\r\n");
    }
    else {
        replacement = -2;
    }
    
    char filename[999];
    char line[999];
    filename[0] = '\0';
    line[0] = '\0';
    
    int i;
    int len = strlen(source);
    for (i = 0; i < 999 && i < len; ++i) {
        if (source[i] == ':')
            break;
        filename[i] = source[i];
    }
    filename[i] = '\0';
    
    int l_i;
    for (l_i = 0, ++i; i < 999 && i < len; ++i, ++l_i) {
        line[l_i] = source[i];
    }
    line[l_i] = '\0';
    
    printf("File: %s\n", filename);
    printf("Line: %s\n", line && line[0] ? line : "no line");
    
    int line_int = line && line[0] ? atoi(line) : 0;
    
    // Manipulate file
    
    FILE* file = fopen(filename, "r");
    if (file) {
        char* buffer;
        int lines = 1;
        while (file && (buffer = halgetline(file)) != NULL) {
            ++lines;
        }
        --lines;
        fclose(file);
        
        file = fopen(filename, "r");
        if (file) {
            char** data = calloc(lines+2, sizeof(char*));
            int line_number = 1;
            while (file && (buffer = halgetline(file)) != NULL && line_number <= lines) {
                
                if (line_number != line_int) {
                    data[line_number] = strdup(buffer);
                }
                else {
                    data[line_number] = replacement;
                }
                ++line_number;
            }
            fclose(file);
            
            file = fopen(filename, "w");
            if (file) {
                for (line_number = 1; line_number <= lines; ++line_number) {
                    if (!data[line_number]) {
                        fprintf(file, "\r\n");
                    }
                    else if (data[line_number] == -2) {
                        fprintf(file, "\r\n");
                    }
                    else if (data[line_number]) {
                        fprintf(file, "%s", data[line_number]);
                        free(data[line_number]);
                    }
                }
                fclose(file);
            }
        }
    }
    else {
        printf("Unable to open file.\n");
        
        if (_replacement) {
            return replacement;
        }
        
        return 1;
    }
    
    if (_replacement) {
        return replacement;
    }
    
    return 0;
}

int fact_delete_from_source (const char* source) {
    int r = fact_replace_in_source (source, 0);
    return r;
}

char* fact_read_from_source (const char* source) {
    char filename[999];
    char line[999];
    filename[0] = '\0';
    line[0] = '\0';
    
    int i;
    int len = strlen(source);
    for (i = 0; i < 999 && i < len; ++i) {
        if (source[i] == ':')
            break;
        filename[i] = source[i];
    }
    filename[i] = '\0';
    
    int l_i;
    for (l_i = 0, ++i; i < 999 && i < len; ++i, ++l_i) {
        line[l_i] = source[i];
    }
    line[l_i] = '\0';
    
    printf("File: %s\n", filename);
    printf("Line: %s\n", line && line[0] ? line : "no line");
    
    int line_int = line && line[0] ? atoi(line) : 0;
    
    // Manipulate file
    
    FILE* file = fopen(filename, "r");
    if (file) {
        char* buffer;
        char* found;
        int lines = 1;
        while (file && (buffer = halgetline(file)) != NULL) {
            ++lines;
        }
        --lines;
        fclose(file);
        
        file = fopen(filename, "r");
        if (file) {
            int line_number = 1;
            while (file && (buffer = halgetline(file)) != NULL && line_number <= lines) {
                
                if (line_number == line_int) {
                    found = strdup(buffer);
                }
                ++line_number;
            }
            fclose(file);
        }
        
        return found;
    }
    printf("Unable to open file.\n");
    
    return strdup("");
}

int remove_negation (char* line, double* truth_ref) {
    halstring sline;
    sline.do_free = 0;
    halstring* sline_ref = &sline;
    char l[4196];
    sline.s = l;
    strncpy(l, line, 4196);
    
    if (strstr(line, "kein")) {
        sline_ref = replace(sline_ref, "kein", "ein");
        (*truth_ref) = 0.0;
    }
    if (strstr(line, "nicht")) {
        sline_ref = replace(sline_ref, "nicht", "");
        (*truth_ref) = 0.0;
    }
    if (strstr(line, " not ")) {
        sline_ref = replace(sline_ref, " not ", "");
        (*truth_ref) = 0.0;
    }
    if (strstr(line, " 50")) {
        sline_ref = replace(sline_ref, " 50", "");
        sline_ref = replace(sline_ref, " <> 50", "");
        (*truth_ref) = 0.5;
    }
    if (strstr(line, "(true)")) {
        sline_ref = replace(sline_ref, " (true)", "");
        sline_ref = replace(sline_ref, "(true)", "");
        (*truth_ref) = 1.0;
    }
    if (strstr(line, "(maybe)")) {
        sline_ref = replace(sline_ref, " (maybe)", "");
        sline_ref = replace(sline_ref, "(maybe)", "");
        (*truth_ref) = 0.5;
    }
    if (strstr(line, "(false)")) {
        sline_ref = replace(sline_ref, " (false)", "");
        sline_ref = replace(sline_ref, "(false)", "");
        (*truth_ref) = 0.0;
    }
    
    strncpy(line, sline_ref->s, 4196);
    if (line[0] == ' ') {
        strcpy(line, line+1);
    }
    
    if (sline_ref->do_free) halfree(sline_ref->s);
}

int hal2009_add_pro_file (char* filename) {
    fprintf(output(), "Add .pro file %s.\n", filename);
    sql_begin();
    position_in_insertions = 0;
    time_t start = 0;
    time(&start);
    FILE* input = fopen(filename, "rb");
    fprintf(output(), " _______________________________\n");
    fprintf(output(), "|                               |\n|");
    if ( input ) {
        char* wholeline;
        int k = 0;
        int line_number = 0;
        while ((wholeline = halgetline(input)) != NULL) {
            ++line_number;

            if (wholeline[0]) {
                char* a = ascii(wholeline);
                free(wholeline);
                wholeline = a;
            }
            
            int d;
            int f;
            int first_record_in_this_line = 1;
            char* line = calloc(line_size + 100+1, 1);
            
            for (d = 0, f = 0; d <= strlen(wholeline); ++d, ++f) {
                line[f] = wholeline[d];
                if (!wholeline[d] || (d+3 < strlen(wholeline) && wholeline[d] == ' ' && wholeline[d+1] == '~' && wholeline[d] == ' ')) {
                    line[f] = 0;
                    f = -1;
                    d += 2;

                    int last_pk = 0;
                    if (!first_record_in_this_line) {
                        FILE* target = fopen("_input_key", "r");
                        if (target) {
                            fscanf(target, "%d", &last_pk);
                            fclose(target);
                        }
                    }
                    
                    /// Make lower case
                    int i_size = strlen(line);
                    int ij;
                    for (ij = 0; ij < i_size; ++ij) {
                        line[ij] = (char)(tolower((int)(line[ij])));
                    }
                    
                    /// Compute the hash
                    int hash_clauses = 1;
                    int j;
                    for (j = 0; j < strlen(line); ++j) {
                        hash_clauses += line[j];
                        hash_clauses += j % 6;
                    }
                    //hash_clauses /= strlen(line)+1;
                    if (hash_clauses < 0)
                        hash_clauses = -hash_clauses;
                    
                    /// Add the fact
                    struct RECORD r;
                    r.truth = 0.5;
                    halstring sline;
                    sline.do_free = 1;
                    halstring* sline_ref = &sline;
                    sline.s = line;
                    sline_ref = replace(sline_ref, "nothing", " ");
                    sline_ref = replace(sline_ref, "nichts", "nothing");
                    sline_ref = replace(sline_ref, "\"", "'");
                    sline_ref = replace(sline_ref, ";;", "YYY");
                    sline_ref = replace(sline_ref, ";", " ");
                    sline_ref = replace(sline_ref, "YYY", ";");
                    sline_ref = replace(sline_ref, "()", "");
                    sline_ref = replace(sline_ref, " <> ", "^");
                    sline_ref = replace(sline_ref, "<>", "^");
                    sline_ref = replace(sline_ref, " ^", "^");
                    sline_ref = replace(sline_ref, " ^", "^");
                    sline_ref = replace(sline_ref, " ^", "^");
                    sline_ref = replace(sline_ref, "^ ", "^");
                    sline_ref = replace(sline_ref, "^ ", "^");
                    sline_ref = replace(sline_ref, "^ ", "^");
                    sline_ref = replace(sline_ref, "^^", "^ ^");
                    sline_ref = replace(sline_ref, "^^", "^ ^");
                    sline_ref = replace(sline_ref, "^^", "^ ^");
                    line = sline_ref->s;
                    
                    char* buffer;
                    
                    strcpy(r.questionword, "");
                    
                    buffer = strtok(line, "^"); strcpy(r.verb,              (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                    remove_negation(r.verb, &(r.truth));
                    buffer = strtok(NULL, "^"); strcpy(r.subjects,          (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                    remove_negation(r.subjects, &(r.truth));
                    buffer = strtok(NULL, "^"); strcpy(r.objects,           (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                    remove_negation(r.objects, &(r.truth));
                    buffer = strtok(NULL, "^"); strcpy(r.adverbs,           (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                    remove_negation(r.adverbs, &(r.truth));
                    buffer = filename;          strcpy(r.from,              (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                    strcat(r.from, ":");
                    sprintf(r.from+strlen(r.from), "%d", line_number);
                    strcpy(r.extra, "");
                    
                    r.verb_flag_want    = 0;
                    r.verb_flag_must    = 0;
                    r.verb_flag_can     = 0;
                    r.verb_flag_may     = 0;
                    r.verb_flag_should  = 0;
                    
                    if (strstr(r.verb, "/")) {
                        if (strstr(r.verb, "/want")) {
                            r.verb_flag_want = 1;
                            strcpy(strstr(r.verb, "/want"), strstr(r.verb, "/want")+5);
                        }
                        if (strstr(r.verb, "/must")) {
                            r.verb_flag_must = 1;
                            strcpy(strstr(r.verb, "/must"), strstr(r.verb, "/must")+5);
                        }
                        if (strstr(r.verb, "/can")) {
                            r.verb_flag_can = 1;
                            strcpy(strstr(r.verb, "/can"), strstr(r.verb, "/can")+4);
                        }
                        if (strstr(r.verb, "/may")) {
                            r.verb_flag_may = 1;
                            strcpy(strstr(r.verb, "/may"), strstr(r.verb, "/may")+4);
                        }
                        if (strstr(r.verb, "/should")) {
                            r.verb_flag_should = 1;
                            strcpy(strstr(r.verb, "/should"), strstr(r.verb, "/should")+7);
                        }
                    }
                    
                    r.prio = 50;
                    
                    buffer = strtok(NULL, "^");
                    int i;
                    i = 0;
                    while (buffer && i+1 < MAX_CLAUSES) {
                        r.clauses[i] = 0;
                        ++i;
                    }
                    i = 0;
                    r.num_clauses = 0;
                    while (buffer && i+1 < MAX_CLAUSES) {
                        if ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))) {
                            if (strlen(buffer) <= 6 && strlen(buffer) >= 2) {
                                if (buffer[0] == '0' || buffer[0] == '1' || buffer[0] == '2' || buffer[0] == '3' || buffer[0] == '4' || buffer[0] == '5' || buffer[0] == '6' || buffer[0] == '7' || buffer[0] == '8' || buffer[0] == '9') {
                                    if (buffer[1] == '0' || buffer[1] == '1' || buffer[1] == '2' || buffer[1] == '3' || buffer[1] == '4' || buffer[1] == '5' || buffer[1] == '6' || buffer[1] == '7' || buffer[1] == '8' || buffer[1] == '9') {
                                        break;
                                    }
                                }
                            }
                        }
                        
                        r.clauses[i] = malloc(sizeof(struct RECORD));
                        struct RECORD* sub_clause = r.clauses[i];
                        sub_clause->truth = 0.5;

                                                    strcpy(sub_clause->verb,          (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                        remove_negation(sub_clause->verb, &(sub_clause->truth));
                        buffer = strtok(NULL, "^"); strcpy(sub_clause->subjects,      (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                        remove_negation(sub_clause->subjects, &(sub_clause->truth));
                        buffer = strtok(NULL, "^"); strcpy(sub_clause->objects,       (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                        remove_negation(sub_clause->objects, &(sub_clause->truth));
                        buffer = strtok(NULL, "^"); strcpy(sub_clause->adverbs,       (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                        remove_negation(sub_clause->adverbs, &(sub_clause->truth));
                        buffer = strtok(NULL, "^"); strcpy(sub_clause->questionword,  (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                        buffer = filename;          strcpy(sub_clause->from,          (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) >= 1))?buffer:"\0");
                        strcat(sub_clause->from, ":");
                        sprintf(sub_clause->from+strlen(sub_clause->from), "%d", line_number);
                        strcpy(sub_clause->extra, "");
                        
                        // a common error in malformed pro files
                        if (0 == strcmp(sub_clause->questionword, "50")) {
                            free(r.clauses[i]);
                            r.clauses[i] = NULL;
                            break;
                        }
                        
                        sub_clause->verb_flag_want    = 0;
                        sub_clause->verb_flag_must    = 0;
                        sub_clause->verb_flag_can     = 0;
                        sub_clause->verb_flag_may     = 0;
                        sub_clause->verb_flag_should  = 0;


                        sub_clause->prio  = 50;
                        
                        r.clauses[i] = sub_clause;
                        r.clauses[i+1] = NULL;
                        ++(sub_clause->num_clauses);
                        ++i;
                        buffer = strtok(NULL, "^");
                    }
                    r.clauses[i] = NULL;
                    
                    halfree(line);
                    
                    if (0 == strlen(r.verb)) {
                        continue;
                    }
                    if (0 == strlen(r.subjects)) {
                        strcpy(r.subjects, "");
                    }
                    if (0 == strlen(r.objects)) {
                        strcpy(r.objects, "");
                    }
                    if (0 == strlen(r.adverbs)) {
                        strcpy(r.adverbs, "");
                    }
                    hash_clauses += r.num_clauses * 13;
                    r.hash_clauses = hash_clauses;

                    int err;
                    
                    if (strstr(r.subjects, "_") && strlen(r.subjects) > 2) {
                        // Modify hash
                        r.hash_clauses = hash_clauses-5;
                        
                        int size_subjects = strlen(r.subjects);
                        if (r.subjects[0] == '_') {
                            char* subj_bak = strdup(r.subjects);
                            strcpy(r.subjects, subj_bak+1);
                            if (subj_bak) free(subj_bak);
                        }
                        
                        int j;
                        for (j = 0; j < LINE_SIZE-1; ++j) {
                            if (r.subjects[j] == '_') {
                                if (j > size_subjects-3) {
                                    r.subjects[j] = '\0';
                                }
                                else {
                                    r.subjects[j] = ' ';
                                }
                            }
                        }
                        
                        int size_objects = strlen(r.objects);
                        if (r.objects[0] == '_') {
                            char* obj_bak = strdup(r.objects);
                            strcpy(r.objects, obj_bak+1);
                            if (obj_bak) free(obj_bak);
                        }
                        
                        for (j = 0; j < LINE_SIZE-1; ++j) {
                            if (r.objects[j] == '_') {
                                if (j > size_objects-3) {
                                    r.objects[j] = '\0';
                                }
                                else {
                                    r.objects[j] = ' ';
                                }
                            }
                        }

                        ++num_facts_added_during_this_run;
                        err = sql_add_record(&r);
                    }
                    else {
                        err = sql_add_record(&r);
                        if (err) {
                            printf("Error: %s\n", wholeline);
                        }
                        ++num_facts_added_during_this_run;
                    }
                    
                    int k = 0;
                    while (k+1 < MAX_CLAUSES && k < i && r.clauses[k]) {
                        halfree(r.clauses[k]);
                        ++k;
                    }
                    
                    if (err) {
                        if (err == NO_CONNECTION) {
                            fprintf(output(), "\n");
                            fflush(stdout);
                            sql_end();
                            return err;
                        }
                    }

                    ++position_in_insertions;
                    if (position_in_insertions % 25 == 0) {
                        fprintf(output(), ".");
                        fflush(stdout);
                    }
                    if (position_in_insertions % 750 == 0) {
                        time_t now = 0;
                        time(&now);
                        if (now - start <= 2) {
                            fprintf(output(), " | ---\n|");
                        }
                        else {
                            long int facts_per_second = position_in_insertions / (now - start);
                            fprintf(output(), " | %i facts/sec (%li facts, %i sec)\n|", facts_per_second, position_in_insertions, now - start);
                        }
                        fflush(stdout);
                    }

                    halfree(line);
                    line = calloc(line_size + 100+1, 1);
                    
                    if (last_pk) {
                        //printf("last_pk: %d\n", last_pk);
                        int current_pk;
                        
                        FILE* target = fopen("_input_key", "r");
                        if (target) {
                            fscanf(target, "%d", &current_pk);
                            fclose(target);
                            //printf("current_pk: %d\n", current_pk);
                            
                            hal2009_add_link("order", last_pk, current_pk);
                        }
                        else {
                            //printf("could not open _input_key\n", last_pk);
                        }
                    }
                    
                    first_record_in_this_line = 0;
                }
                
            }
            
            if (first_record_in_this_line) {
                printf ("Error in line: %s\n%s\n", wholeline, "No database entry made.");
            }

            if (line) {
                free(line);
            }
            free(wholeline);
        }
    }
    
    int position_in_insertions_simulated = position_in_insertions;
    while (position_in_insertions_simulated % 750 != 0) {
        ++position_in_insertions_simulated;
        if (position_in_insertions_simulated % 25 == 0) {
            fprintf(output(), " ");
            fflush(stdout);
        }
    }
    fprintf(output(), " | ---\n|");
    time_t now = 0;
    time(&now);
    long int facts_per_second = position_in_insertions / ((now - start)>0?(now - start):1);
    fprintf(output(), "_______________________________| %i facts/sec (%li facts, %i sec)\n", facts_per_second, position_in_insertions, now - start);
    fflush(stdout);
    fprintf(output(), "\n");
    fflush(stdout);
    sql_end();
    
    fprintf(output(), "\nAdded %d facts.", num_facts_added_during_this_run > 0 ? num_facts_added_during_this_run : 0);
    fprintf(output(), "\nNot added %d facts because they already exist in the database.", num_facts_not_added_during_this_run_because_exist);
    fprintf(output(), "\nNot added %d facts because of an other error.", num_facts_not_added_during_this_run_because_other_error);
    fprintf(output(), "\n(%d facts)", num_facts_added_during_this_run
        + num_facts_not_added_during_this_run_because_exist
        + num_facts_not_added_during_this_run_because_other_error);
    fprintf(output(), "\n\n");
    

    return 0;
}

struct DATASET hal2009_get_csv(char* csv_request) {
    fprintf(output(), "Get CSV data: %s.\n", csv_request);
    sql_begin();
    position_in_insertions = 0;

    struct RECORD r;
    char* buffer;

    buffer = strtok(csv_request, "^"); strcpy(r.verb,               (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.subjects,           (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.objects,            (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.adverbs,            (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.extra,              (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.context,            (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");
    buffer = strtok(NULL,        "^"); strcpy(r.pkey,               (buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?buffer:"\0");

    buffer = strtok(NULL,        "^"); r.verb_flag_want    =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_must    =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_can     =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_may     =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    buffer = strtok(NULL,        "^"); r.verb_flag_should  =        ((buffer && ((buffer[0] != ' ' && buffer[0] != '*') || strlen(buffer) > 1))?atoi(buffer):0);
    
    buffer = strtok(NULL,        "^");
    if (buffer && 0 == strcmp(buffer, "everything")) {
        r.everything_q = EVERYTHING;
    }

    if (0 == strlen(r.verb)) {
        strcpy(r.verb, "0");
    }
    if (0 == strlen(r.subjects)) {
        strcpy(r.subjects, "0");
    }
    if (0 == strlen(r.objects)) {
        strcpy(r.objects, "0");
    }
    if (0 == strlen(r.adverbs)) {
        strcpy(r.adverbs, "0");
    }
    if (0 == strlen(r.extra)) {
        strcpy(r.extra, "0");
    }
    if (0 == strlen(r.pkey)) {
        strcpy(r.pkey, "0");
    }
    if (0 == strlen(r.context)) {
        strcpy(r.context, "default");
    }

    struct DATASET set;
    set = sql_get_records(&r);

    fprintf(output(), "\n");
    fflush(stdout);
    sql_end();

    return set;
}

const char* hal2009_make_csv(struct DATASET* set) {
    fprintf(output(), "Compute CSV data, %li records, %li columns\n", set->size, set->column_count);
    int j;
    int m;
    long int size = 4096 + 1*set->size;
    long int size_raw = 0;
    for (j = 0; j < set->size; ++j) {
        if (set->data[j]) {
            for (m = 0; m < set->column_count; ++m) {
                if (set->data[j][m]) {
                    /// fprintf(output(), "Data: %s\n", set->data[j][m]);
                    size     += strlen(set->data[j][m]) + 10;
                    size_raw += strlen(set->data[j][m]);
                }
            }
        }
    }

    fprintf(output(), "I need %li kB of memory, I have %li bytes of raw data\n", size/1024, size_raw);
    fprintf(output(), "Allocate it.\n", size/1024);
    char* csv_data = calloc(size, 1);
    if (0 == csv_data) {
        fprintf(output(), "Out of memory. I'll return nothing.\n");
        csv_data = calloc(2, 1);
        return csv_data;
    }
    fprintf(output(), "Copy data.\n");
    for (j = 0; j < set->size; ++j) {
        if (set->data[j] && set->data[j][0] && *(set->data[j][0])) {
            for (m = 0; m < set->column_count; ++m) {
                if (set->data[j][m]) {
                    strcat(csv_data, set->data[j][m]);
                    free(set->data[j][m]);
                }
                if (m+1 < set->column_count) {
                    strcat(csv_data, "^");
                }
            }
            strcat(csv_data, "\n");
        }
        if (set->data[j]) {
            free(set->data[j]);
        }
    }
    fprintf(output(), "Data copied.\n");

    return csv_data;
}

/* UTF8/Latin --> ASCII */
char* ascii(const char* not_ascii) {
    halstring s;
    s.s = not_ascii;
    s.do_free = 0;

    halstring* s_ref = &s;
    s_ref = replace(s_ref, "Ã¼", "ue");
	s_ref = replace(s_ref, "Ã", "ss");
	s_ref = replace(s_ref, "Ã¤", "ae");
	s_ref = replace(s_ref, "ü", "ue");
	s_ref = replace(s_ref, "ß", "ss");
	s_ref = replace(s_ref, "ä", "ae");
	s_ref = replace(s_ref, "ö", "oe");
	s_ref = replace(s_ref, "Ü", "Ue");
	s_ref = replace(s_ref, "Ä", "Ae");
	s_ref = replace(s_ref, "Ö", "Oe");
	s_ref = replace(s_ref, "Ã¢ÂÂ", "\"");
	s_ref = replace(s_ref, "Ã¢ÂÂ", "\"");
	s_ref = replace(s_ref, "&lt;/br&gt;", " ");

    char* ascii = malloc(strlen(s_ref->s)*2+2);
    strcpy(ascii, "");
    unsigned int length = strlen(s_ref->s);
    unsigned int x;
	for (x = 0; x < length; x++) {
	    short y = (short)(s_ref->s[x]);
	    if (y == 228) {
	        strcat(ascii, "ae");
	    }
	    else if (y == 196) {
	        strcat(ascii, "Ae");
	    }
	    else if (y == 252) {
	        strcat(ascii, "ue");
	    }
	    else if (y == 220) {
	        strcat(ascii, "Ue");
	    }
	    else if (y == 246) {
	        strcat(ascii, "oe");
	    }
	    else if (y == 214) {
	        strcat(ascii, "Oe");
	    }
	    else if (y == 223) {
	        strcat(ascii, "ss");
	    }
        else if (y == 226) {
            x += 2;
        }
        else if (y > 129) {
            x += 1;
        }

        else {
            char array[2];
            array[0] = y;
            array[1] = '\0';
	        strcat(ascii, array);
	    }
	}
    
    if (s_ref->do_free) halfree(s_ref->s);

   
    return ascii;
}

/* a replace function */
halstring* replace(halstring *src, const char *from, const char *to) {
    if (0 == strstr(src->s, from)) {
        return src;
    }
    
    char* full_string_pointer = src->s;
    src->s = halmalloc(line_size + 100, "replace");
    strcpy(src->s, full_string_pointer);
    char* string_to_free = src->s;
    
    size_t size = strlen(src->s) + 1;
    size_t fromlen = strlen(from);
    size_t tolen = strlen(to);

    char *value = halmalloc(size, "replace");

    char *dst = value;

    if ( value != NULL ) {
        for ( ;; ) {
            const char *match = strstr(src->s, from);
            if ( match != NULL ) {
                size_t count = match - src->s;
                char *temp;
                size += tolen - fromlen;
                temp = realloc(value, size);
                if ( temp == NULL ) {
                    halfree(value);
                    return NULL;
                }
                dst = temp + (dst - value);
                value = temp;
                memmove(dst, src->s, count);
                src->s += count;
                dst += count;
                memmove(dst, to, tolen);
                src->s += fromlen;
                dst += tolen;
            }
            else /* No match found. */
            {
                strcpy(dst, src->s);
                break;
            }
        }
    }
    
    /*
    if ( src->do_free ) {
        halfree(full_string_pointer);
    }

    src->s = value;
    */
    
    strcpy(full_string_pointer, value);
    src->s = full_string_pointer;
    halfreef(value, "replace");
    halfreef(string_to_free, "replace");
    
    return src;
}

/* a split function */
halstring **stringtoarray(halstring *string, char delimiter, int *size) { 
  halstring **array = halmalloc(sizeof(halstring*), "stringtoarray");
  char *ptr, *oldptr;
  int flag = 1;
  int count;
    
  *size = 0;
  ptr = string->s;

  for(count=0 ; flag ; ++count) 
  {
    for (oldptr=ptr;*ptr&&*ptr!=delimiter;*ptr++)
      ;
    if (!*ptr) flag = 0;
    *ptr++ = '\0';
    (*size)++;                                                       

    array = realloc(array, (count+2)*sizeof(halstring *));
    halstring* s = halmalloc(sizeof(halstring), "stringtoarray");
    //s->s = strdup(oldptr);
    s->s = halmalloc(line_size + 100, "stringtoarray");
    strncpy(s->s, oldptr, line_size + 99);
    s->do_free = 1;
    array[count] = s;
  
  }
  array[count] = 0;
  return array;
}

/*
    The main compile function
*/
int hal2009_compile(char* file, char* planguage) {
    if (0 == strcmp(planguage, "perl6")) {
        convert_to_perl6_convert_file(strlen(file) ? file : "hal2009.hal");
    }
    else if (0 == strcmp(planguage, "perl5")) {
        convert_to_perl5_convert_file(strlen(file) ? file : "hal2009.hal");
    }
}

/*
    The main compile function
*/
int hal2009_doc(char* filename) {
    FILE* source = fopen(filename, "r");
    if ( !source ) {
        fprintf(output(), "HAL2009 source file not found: %s", filename);
        return 1;
    }

    fseek(source, 0, SEEK_END);
    int file_size = ftell(source) + 1;
    char* code = halmalloc(file_size, "hal2009_doc");
    fseek(source, 0, SEEK_SET);
    fread(code, 1, file_size, source);
    halclose(source);

    FILE* target;
    {
        char targetname[100];
        strcpy(targetname, filename);
        strcat(targetname, ".html");
        
        target = fopen(targetname, "w");
    }
    if ( !target ) {
        fprintf(output(), "HAL2009 source file: unable to create: %s", filename);
        return 1;
    }
    fprintf(output(), "Generate Documentation for %s...", filename);
    
    char header_1[2048] = "<html>"
"<head>"
"<title>";
    
    char header_2[2048] = "</title>"
"<style type=\"text/css\">"
"*, body, html {"
"	margin: 0;"
"	padding: 0;"
"}"
"h1, h2, h3, h4, h5, h6 {"
"	color: #3d3def;"
"	margin-top: 20px;"
"}"
"h1 small {"
"	float: right;"
"	font-size: 0.5em;"
"}"
"body {"
"	padding: 20px;"
"}"
"b {"
"	display: block;"
"	margin: 20px;"
"	border: 1px solid #3d3def;"
"	padding: 20px;"
"}"
".pre {"
"   clear: both;"
"	display: block;"
"	margin-top: 20px;"
"	margin-bottom: 20px;"
"	margin-left: 30px;"
"	padding-left: 10px;"
"	padding-top: 5px;"
"	margin-right: 20px;"
"	border-left: 2px solid #3d3def;"
"}"
".p {"
"   clear: both;"
"	display: block;"
"	margin-left: 20px;"
"	margin-right: 20px;"
"}"
"span.f {"
"	color: #ff6600;"
"}"
"span.f1 { color: red; font-weight: bold; }"
"span.f2 { color: blue; font-weight: bold; }"
"span.f3 { color: green; font-weight: bold; }"
"span.f4 { color: maroon; font-weight: bold; }"
"span.f5 { color: black; font-weight: bold; }"
"</style>"
"</head>"
"<body";
    halwrite(header_1, 1, strlen(header_1), target);
    halwrite(filename, 1, strlen(filename), target);
    halwrite(header_2, 1, strlen(header_2), target);
    halwrite("<h1>", 1, 4, target);
    halwrite(filename, 1, strlen(filename), target);
    halwrite("</h1>", 1, 5, target);

    int number_of_lines;
    halstring string;
    string.s = code;
    string.do_free = 0;
    halstring** lines = stringtoarray(&string, '\n', &number_of_lines);
    
    int current_line;
    for (current_line = 0; current_line < number_of_lines; ++current_line) {
        char* newline;
        newline = halmalloc(line_size + 100, "hal2009_doc"); // be sure we have enough space
        zero_p(newline, line_size + 100);
        strcpy(newline, lines[current_line]->s);
        halstring s;
        s.s = newline;
        s.do_free = 1;

        halstring* s_ref = &s;
        
        {
            if (strstr(s.s, "# TEXT")) {
                if ( s.do_free ) halfree(s.s);
                s.s = "</pre><pre class='p'>";
                s.do_free = 0;
            }
            else if (strstr(s.s, "# CODE")) {
                if ( s.do_free ) halfree(s.s);
                s.s = "</pre><pre class='pre'>";
                s.do_free = 0;
            }
            else {
                s_ref = replace(s_ref, "# ~~~~~~ ", "<h2>");
                s_ref = replace(s_ref, "~~~~~~", "</h2>");
                s_ref = replace(s_ref, "# ~~~~~ ", "<h3>");
                s_ref = replace(s_ref, "~~~~~", "</h3>");
                s_ref = replace(s_ref, "# ~~~~ ", "<h4>");
                s_ref = replace(s_ref, "~~~~", "</h4>");
                s_ref = replace(s_ref, "# ~~~ ", "<h5>");
                s_ref = replace(s_ref, "~~~", "</h5>");
                s_ref = replace(s_ref, "# ~~ ", "<h6>");
                s_ref = replace(s_ref, "~~", "</h6>");
                s_ref = replace(s_ref, "# ", "");
                s_ref = replace(s_ref, "#", "");
            }
        }
        
        
        halwrite(s_ref->s, 1, strlen(s_ref->s), target);
        halwrite("\n", 1, 1, target);
        
        if ( lines[current_line]->do_free ) halfree(lines[current_line]->s);
        halfree(lines[current_line]);
        if ( s_ref->do_free ) halfree(s_ref->s);
        
        fprintf(output(), "%s", ".");
    }
    fprintf(output(), "%s", "\n");
    halfree(lines);
    halwrite("</pre>", 1, 6, target);
}

/*
    The main execute function
*/
int hal2009_execute_file(char* file, char* planguage) {
    if (0 == strcmp(planguage, "perl6")) {
        execute_perl6(strlen(file) ? file : "hal2009.hal");
    }
    else if (0 == strcmp(planguage, "perl5")) {
        execute_perl5(strlen(file) ? file : "hal2009.hal");
    }
}

/*
    The main execute function
*/
int hal2009_execute_code(char* code, char* planguage) {
    FILE* codefile = fopen("temp.hal", "w");
    halwrite(code, 1, strlen(code), codefile);
    halclose(codefile);
    if (0 == strcmp(planguage, "perl6")) {
        execute_perl6("temp.hal");
    }
    else if (0 == strcmp(planguage, "perl5")) {
        execute_perl5("temp.hal");
    }
}

void hal2009_clean() {
    int number_of_files_to_delete = 11;
    char files_to_delete[13][100] = { "_output", "_output__pos", "_input__pos", "_output__add_pro_file", "_input__add_pro_file", "_output__get_csv", "_input__get_csv", "_exit", "_exit_hal2009_signal_handler", "_do_not_need_a_signal_handler", "_input_server", "_change_text_language" };
    
    int i;
    for (i = 0; i < number_of_files_to_delete; ++i) {
        unlink(files_to_delete[i]);
    }
}

struct params_signal_handler {
    char* planguage;
    char* tlanguage;
    short start_type;
};

void* hal2009_signal_handler(void* parameters) {
    struct params_signal_handler* params = parameters;
    char* planguage  = params->planguage;
    char* tlanguage  = params->tlanguage;
    char* start_type = params->start_type;
    free(params);

    void* temporary_memory = halmalloc(5120, "hal2009_signal_handler");
    
    if ( stat("_do_not_need_a_signal_handler", temporary_memory) == 0 ) {
        halfree(temporary_memory);
        halfree(planguage);
        halfree(tlanguage);
        return 0;
    }
    FILE* signal_handler_lock = fopen("_do_not_need_a_signal_handler", "w");
    halclose(signal_handler_lock);
    
    
    int number_of_files_to_check = 7;
    char files_to_check[8][100] = { "_output", "_output__link", "_output__pos", "_output__add_pro_file", "_output__get_csv", "_exit", "_cgi_request" };
    int i;
    
    while ( 1 ) {
        for (i = 0; i < number_of_files_to_check; ++i) {
            FILE* source = NULL;
            if ( source = fopen("_change_text_language", "r") ) {
                halusleep(500);
                halclose(source);
                source = fopen("_change_text_language", "r");
                if (source && tlanguage) fread(tlanguage, 1, 2, source);
                halclose(source);
                unlink("_change_text_language");
            }

            if ( source = fopen("_exit_hal2009_signal_handler", "r") ) {
                halclose(source);
                unlink("_exit_hal2009_signal_handler");
                unlink("_do_not_need_a_signal_handler");
                halfree(temporary_memory);
                halfree(tlanguage);
                halfree(planguage);
                return 0;
            }

            if ( source = fopen(files_to_check[i], "r+b") ) {
                halusleep(500);
                halclose(source);
                source = fopen(files_to_check[i], "r+b");
            }
            if ( source ) {
                fprintf(output(), "File opened: %s\n", files_to_check[i]);
                char* code   = calloc(10240+1, 1);
                strcpy(code, "");
                char* buffer = calloc(10240, 1);
                if ( fread(buffer, 1, 10240, source) ) {
                    code[strlen(code)] = 0;
                    strcat(code, buffer);
                    code[strlen(code)] = 0;
                }
                halclose(source);
                if (!strlen(code) && strcmp(files_to_check[i], "_exit")) {
                    halusleep(1000);
                    source = fopen(files_to_check[i], "r+b");
                    fprintf(output(), "File reopened: %s\n", files_to_check[i]);
                    if ( fread(buffer, 1, 10240, source) ) {
                        code[strlen(code)] = 0;
                        strcat(code, buffer);
                        code[strlen(code)] = 0;
                    }
                    halclose(source);
                }
                fprintf(output(), "File closed: %s\n", files_to_check[i]);
                unlink(files_to_check[i]);
                halfree(buffer);

                pthread_t thread_no_1;
                char* parameters[3] = {files_to_check[i], code, start_type};
                /// pthread_create (&thread_no_1, NULL, hal2009_handle_signal, (void*)parameters);
                hal2009_handle_signal((void*)parameters);

                if (0 == strcmp(files_to_check[i], "_printf")) {
                    unlink("_exit_hal2009_signal_handler");
                    unlink("_do_not_need_a_signal_handler");
                    halfree(temporary_memory);
                }
            }
        }

        halusleep(1000);
    }
    
    halfree(temporary_memory);
    halfree(tlanguage);
    halfree(planguage);
    unlink("_do_not_need_a_signal_handler");
    return 0;
}

pthread_t hal2009_start_signal_handler(char* planguage, char* tlanguage, short start_type) {
    pthread_t thread_no_2;
//    char* parameters[3] = { planguage, tlanguage, start_type };
    struct params_signal_handler* parameters = calloc(sizeof(struct params_signal_handler)+1, 1);
    parameters->planguage = planguage;
    parameters->tlanguage = tlanguage;
    parameters->start_type = start_type;
    pthread_create (&thread_no_2, NULL, hal2009_signal_handler, (void*)parameters);
    return thread_no_2;
}

struct params_answer {
    char* input;
    char* planguage;
    char* tlanguage;
    char* base_dir;
    short start_type;
};

/*
    The main answer function

    input: input from user
    planguage: the programming language
    tlanguage: the text (=input) language
*/
pthread_t hal2009_answer(char* input, char* planguage, char* tlanguage, char* base_dir, int join, short start_type) {
    input = ascii(input);

    FILE* target = fopen("_input", "w");
    halwrite(input, 1, strlen(input), target);
    halwrite("\n", 1, strlen("\n"), target);
    halclose(target);
    
    pthread_t thread_no_1;
    struct params_answer* parameters = calloc(sizeof(struct params_answer)+1, 1);
    parameters->input = input;
    parameters->planguage = planguage;
    parameters->tlanguage = tlanguage;
    parameters->base_dir = base_dir;
    parameters->start_type = start_type;
    fprintf(output(), "Start thread now. Language is %s\n", tlanguage);
    pthread_create (&thread_no_1, NULL, hal2009_answer_thread, (void*)parameters);
    
    if ( join == JOIN ) {
        pthread_join(thread_no_1, NULL);
    }
    //sleep(1);

    return thread_no_1;
}

/*
    A Thread invoked by the main answer function

    input: input from user
    planguage: the programming language
    tlanguage: the text (=input) language
*/
void* hal2009_answer_thread(void* parameters) {
    struct params_answer* params = (struct params_answer*)parameters;
    char* input      = params->input;
    char* planguage  = params->planguage;
    char* tlanguage  = params->tlanguage;
    char* base_dir   = params->base_dir;
    short start_type = params->start_type;

    if (planguage == NULL || tlanguage == NULL) {
        return 0;
    }
    
    static int stuck_here = 0;
    while (stuck_here) {
        halsleep(1);
    }
    stuck_here = 1;

    reset_stdout();

    char* tempfile = calloc(strlen(base_dir)+1000, 1);
    
    snprintf(tempfile, strlen(base_dir)+1000-1, "%s/flowchart.log", base_dir);
    FILE* flowchart_log = fopen(tempfile, "w");
    if (flowchart_log)
        fclose(flowchart_log);
    *tempfile = 0;
    
    snprintf(tempfile, strlen(base_dir)+1000-1, "%s/temp_%s_%i.hal", base_dir, tlanguage, strlen(base_dir));
    FILE* startfile = fopen(tempfile, "w");
    char* content = halmalloc(1024, "hal2009_answer_thread");
    strcpy(content, ""
    "compile source hal2009-lang-");
    strcat(content, tlanguage);
    strcat(content, ".hal\n"
    "require source hal2009-lang-");
    strcat(content, tlanguage);
    strcat(content, ".hal\n"
    "compile source hal2009.hal\n"
    "require source hal2009.hal\n"
    "set new var current_language to '");
    strcat(content, tlanguage);
    strcat(content, "'\n"
    "set new var path to \"");
    strcat(content, base_dir);
    strcat(content, "\"\n");
    if (start_type == SINGLE) {
        strcat(content, ""
        "do init with var current_language, var path\n");
    }
    strcat(content, ""
    "do shell with var current_language, var path\n"
    "do exit with 0");
    fprintf(output(), "Start script is:\n%s\n", content);
    halwrite(content, sizeof(char), strlen(content), startfile);
    halclose(startfile);
    halfree(content);

    if (0 == strcmp(planguage, "perl6")) {
        execute_perl6(tempfile);
    }
    else if (0 == strcmp(planguage, "perl5")) {
        execute_perl5(tempfile);
    }
    halfree(tempfile);
    
    halfree(planguage);
    halfree(tlanguage);
    halfree(base_dir);
    halfree(input);
    
    halfree(parameters);
    
    {
        FILE* file_done = fopen("_done", "w");
        halwrite("1", 1, strlen("1"), file_done);
        halclose(file_done);
    }
    stuck_here = 0;
    pthread_exit(0);
    return 0;
}

void hal2009_init(char* planguage, char* tlanguage, char* base_dir) {

    char* content = (char*)halmalloc(1024, "main");
    strcpy(content, ""
    "compile source hal2009-lang-");
    strcat(content, tlanguage);
    strcat(content, ".hal\n"
    "require source hal2009-lang-");
    strcat(content, tlanguage);
    strcat(content, ".hal\n"
    "compile source hal2009.hal\n"
    "require source hal2009.hal\n"
//    "set STDOUT to handle for file name \"hal.log\", write\n"
    "set new var current_language to '");
    strcat(content, tlanguage);
    strcat(content, "'\n"
    "set new var path to \"");
    strcat(content, base_dir);
    strcat(content, "\"\n"
    "do init with var current_language, var path\n");
    hal2009_execute_code(content, planguage);
    halfree(content);
}

int hal2009_set_text_language(const char* s) {
    if (!s) {
        return INVALID;
    }
    if (hal2009_text_language) {
        free(hal2009_text_language);
        hal2009_text_language = 0;
    }
    hal2009_text_language = strdup(s);
}

const char* hal2009_get_text_language() {
    if (!hal2009_text_language) {
        hal2009_text_language = strdup("de");
    }
    return hal2009_text_language;
}

const char* check_config (const char* name, const char* _default) {
    const char* config_file = "config.txt";
    FILE* i = 0;
    if (!i) {
        i = fopen("config.txt", "r");
        if (i) config_file = "config.txt";
    }
    if (!i) {
        i = fopen("../config.txt", "r");
        if (i) config_file = "../config.txt";
    }
    if (!i) {
        i = fopen("../../config.txt", "r");
        if (i) config_file = "../../config.txt";
    }

    if (i) {
        char* temp;
        while (i && (temp = halgetline(i)) != NULL) {
            if (strstr(temp, name)) {
                halstring haltemp;
                halstring* haltemp_ref = &haltemp;
                haltemp_ref->do_free = 0;
                haltemp_ref->s = temp;
                
                haltemp_ref = replace(haltemp_ref, " =", "=");
                haltemp_ref = replace(haltemp_ref, "= ", "=");
                haltemp_ref = replace(haltemp_ref, "\n", "=");
                haltemp_ref = replace(haltemp_ref, "\r", "=");
                haltemp_ref = replace(haltemp_ref, name, "");
                haltemp_ref = replace(haltemp_ref, "=", "");

                char copy[4001];
                strncpy(copy, haltemp_ref->s, 4000);
                fclose(i);
                printf("%s: %s = %s, default %s\n", config_file, name, copy, _default);
                return copy;
            }
        }
    }
    fclose(i);
    
    FILE* o = fopen(config_file, "a");
    if (o) {
        fprintf(o, "%s = %s\n", name, _default);
        fclose(o);
    }
    
    return _default;
}
