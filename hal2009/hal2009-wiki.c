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



#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1

#include <stdio.h>

#include "hal2009.h"
#include "hal2009-universal.h"

halstring* download_from_url(const char* url);

int wiki_begin() {
    return 1;
}

halstring* remove_between(halstring* s_text, char start, char stop) {
    halstring* t_text = calloc(sizeof(halstring), 1);
    t_text->do_free   = 1;
    int size          = strlen(s_text->s);
    t_text->s         = calloc(size, 1);
    
    int i;
    int j;
    int in_gap = 0;
    for (i = 0, j = 0; i < size; ++i) {
        if (s_text->s[i] == start) {
            ++in_gap;
        }
        
        if (0 == in_gap) {
            t_text->s[j] = s_text->s[i];
            ++j;
        }
        
        if (s_text->s[i] == stop) {
            --in_gap;
        }
    }
    
    if (s_text->do_free) {
        free(s_text->s);
    }
    
    return t_text;
}

char* replace_spaces(const char* s) {
    int size          = strlen(s);
    char* n           = calloc(size+2, 1);
    
    int i;
    for (i = 0; i < size; ++i) {
        n[i] = s[i] == ' ' ? '_' : s[i] == '\t' ? '_' : s[i] == '\r' ? '_' : s[i] == '\n' ? '_' : s[i];
    }
    
    return n;
}


char* delete_stars(const char* s) {
    int size          = strlen(s);
    char* n           = calloc(size+3, 1);
    
    int i;
    int j;
    for (i = 0, j = 0; i < size; ++i) {
        if (s[i] == '*') {
            continue;
        }
        n[j] = s[i];
        ++j;
    }
    n[j] = '\0';
    
    return n;
}

char* concat(const char* a, const char* b) {
    char* c = (char*)calloc(strlen(a)+strlen(b)+3, 1);
    strcpy(c, a);
    strcat(c, b);
    
    return c;
}

char* transform_sentence(char* sentence) {
    
    printf("sentence: %s\n", sentence);
    
    char* verb_str = 0;
    if (verb_str = strstr(sentence, "hrt zu ")) {
        verb_str += 7;
    }
    if (verb_str = strstr(sentence, " ist ")) {
        verb_str += 5;
    }
    else if (verb_str = strstr(sentence, " sind ")) {
        verb_str += 6;
    }
    else if (verb_str = sentence) {
        // do nothing
    }
    else {
        return 0;
    }
    
    int size     = strlen(verb_str);
    char* object = calloc(size+5, 1);
    int i;
    int j;
    int in_last_word = 0;
    for (i = 0, j = 0; i < size; ++i) {
        if (verb_str[i] == toupper(verb_str[i]) && i && verb_str[i-1] == ' ') {
            ++in_last_word;
        }
        
        if (in_last_word && verb_str[i] == ' ') {
            break;
        }
        if (j == 0 && (verb_str[i] == ' ' || verb_str[i] == '\t')) {
            continue;
        }
        
        object[j] = verb_str[i];
        ++j;
    }
    
    int maybe_end = j;
    int number_of_spaces = 0;
    for (; i < size; ++i) {
        if (verb_str[i] == ',') {
            break;
        }
        
        if (verb_str[i] == ' ') {
            ++number_of_spaces;
        }
        
        object[j] = verb_str[i];
        ++j;
    }
    
    if (number_of_spaces > 4) {
        object[maybe_end] = '\0';
    }
    
    printf("object:   %s\n", object);
    
    return object;
}

char* upper (const char* s) {
    int size          = strlen(s);
    char* n           = calloc(size+2, 1);
    
    int i;
    for (i = 0; i < size; ++i) {
        n[i] = (i == 0 || (i && (s[i-1] == ' ' || s[i-1] == '\t' || s[i-1] == '\n' || s[i-1] == '\r'))) ? toupper(s[i]) : s[i];
    }
    
    return n;
}

char* delete_articles(const char* s) {
    int size          = strlen(s);
    char* n           = calloc(size+2, 1);
    
    int offset = 0;
    
    if (size > 4) {
        if (s == strstr(s, "der"))      offset = 4;
        if (s == strstr(s, "die"))      offset = 4;
        if (s == strstr(s, "das"))      offset = 4;
        if (s == strstr(s, "den"))      offset = 4;
        if (s == strstr(s, "dem"))      offset = 4;
        if (s == strstr(s, "ein"))      offset = 4;
        if (s == strstr(s, "eine"))     offset = 5;
        if (s == strstr(s, "einer"))    offset = 6;
        if (s == strstr(s, "einem"))    offset = 6;
        if (s == strstr(s, "einen"))    offset = 6;
    }
    
    strcpy(n, s+offset);
    return n;
}

struct fact** search_facts_wiki(const char* entity) {
    char* entity_without_stars    = delete_stars(entity);
    printf("entity_without_stars: %s\n", entity_without_stars);
    char* entity_without_articles = delete_articles(entity_without_stars);
    printf("entity_without_stars: %s\n", entity_without_stars);
    char* entity_upper            = upper(entity_without_articles);
    printf("entity_upper: %s\n", entity_upper);
    char* _url                    = concat("de.wikipedia.org/wiki/", entity_upper);
    printf("_url: %s\n", _url);
    char*  url                    = replace_spaces(_url);
    printf("url: %s\n", url);
    halstring* file               = download_from_url(url);
    free(entity_upper);
    free(entity_without_stars);
    free( _url);
    free(  url);
    
    if (!file) {
        printf("Was not successful: download_from_url\n");
        return 0;
    }
    
    struct fact** facts = calloc(sizeof(struct fact*), 2);

    int number_of_lines = 0;
    halstring** lines = stringtoarray(file, '\n', &number_of_lines);
    int current_line;
    printf("(%i lines)\n", number_of_lines);
    int in_header = 1;
    int in_table = 0;
    int in_script = 0;
    for (current_line = 0; current_line < number_of_lines; ++current_line) {
        if (0 == lines[current_line])
            break;
        if (0 == lines[current_line]->s)
            break;
        if (strlen(lines[current_line]->s) <= 1) {
            halfree(lines[current_line]->s);
            halfree(lines[current_line]);
            lines[current_line] = 0;
            continue;
        }
        
        if (strstr(lines[current_line]->s, "class=\"noprint\"") || strstr(lines[current_line]->s, "Wechseln zu")) {
            in_header = 0;
            continue;
        }
        
        if (0 == in_header) {
            if (strstr(lines[current_line]->s, "<table")) {
                in_table += 1;
            }
            if (strstr(lines[current_line]->s, "</table")) {
                in_table -= 1;
            }
            if (strstr(lines[current_line]->s, "<script")) {
                in_script += 1;
            }
            if (strstr(lines[current_line]->s, "</script")) {
                in_script -= 1;
            }
            
            if (0 < in_script || 0 < in_table) {
                continue;
            }
            if (strstr(lines[current_line]->s, "action=edit") && !strstr(lines[current_line]->s, "redlink")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "bezeichnet:")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "Koordinaten")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "latitude")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "</div")) {
                continue;
            }
            
            lines[current_line] = remove_between(lines[current_line], '(', ')');
            lines[current_line] = remove_between(lines[current_line], '[', ']');
            lines[current_line] = remove_between(lines[current_line], '<', '>');
            
            if (strlen(lines[current_line]->s) < 5) {
                continue;
            }
            
            struct fact* fact  = calloc(sizeof(struct fact), 1);
            fact->pk           = 0;
            fact->verbs        = divide_words("ist");
            fact->subjects     = divide_words(entity);
            char* object       = transform_sentence(lines[current_line]->s);
            fact->objects      = divide_words(object);
            fact->adverbs      = divide_words("...");
            fact->extra        = divide_words("");
            fact->questionword = strdup("");
            fact->from         = strdup("");
            fact->truth        = 1.0;
            
            facts[0] = fact;
            
            if (strlen(object) && object[0] != ' ' && object[0] != '\t') {
                free(object);
                break;
            }
            free(object);
        }
    }
    
    for (current_line = 0; current_line < number_of_lines; ++current_line) {
        if (0 == lines[current_line])
            break;
        if (0 == lines[current_line]->s)
            break;
        halfree(lines[current_line]->s);
        halfree(lines[current_line]);
        lines[current_line] = 0;
        continue;
    }
    
    
    if (file->do_free) {
        free(file->s);
    }
    
    return facts;
}

