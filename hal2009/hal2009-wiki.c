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

char *strstri(char *t, const char *s) {
    int i, j;
    for (i=0; t[i] != '\0'; i++) {
        for(j=0; s[j] != '\0'; j++) {
            if (toupper(s[j])==toupper(t[i+j]))
                continue; 
            else 
                break;
        }

        /*if the whole string was successfully compared, break the loop*/
        if (s[j] == '\0')
            break;
    }
    if (s[j] == '\0')
        return (i+t); /*returns a pointer to the first occurrence of s within t, just like the original strstr*/
    else
        return '\0'; /*return NULL*/
}

halstring* remove_between(const halstring* s_text, char start, char stop) {
    if (!s_text || !s_text->s) {
        return 0;
    }
    
    halstring* t_text = calloc(sizeof(halstring), 1);
    t_text->do_free   = 1;
    int size          = strlen(s_text->s);
    t_text->s         = calloc(size+3, 1);
    
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


char* delete_bad_chars(const char* s, int with_star, int with_underscore) {
    int size          = strlen(s);
    char* n           = calloc(size+3, 1);
    
    int i;
    int j;
    for (i = 0, j = 0; i < size; ++i) {
        if ((with_star && s[i] == '*') || (with_underscore && (s[i] == '_' && i == 0) || (s[i] == '_' && i+1 >= size))) {
            continue;
        }
        n[j] = s[i];
        if (with_underscore && s[i] == '_') {
            n[j] = ' ';
        }
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

const char* define_general_verb(char* sentence, const char* entity) {
    if (strstr(sentence, " ist ") && (strstr(sentence, "ein") < strstr(sentence, " ist ") || strstr(sentence, "Ein") < strstr(sentence, " ist "))) {
        return "equal_in";
    }
    else if (strstr(sentence, "ezeichnung ")) {
        return "equal_in";
    }
    else if (strstr(sentence, " sind ")) {
        return "equal_pl";
    }
    else if (strstr(sentence, " heissen ")) {
        return "equal_pl";
    }
    else if (strstr(sentence, " waren ")) {
        return "equal_pl";
    }
    else if (strstr(sentence, " nennt man ")) {
        return "equal_pl";
    }
    return "equal_sg";
}

char* transform_sentence(char* sentence, const char* entity) {
    
    printf("sentence: %s\n", sentence);
    
    char _verb_1[100];
    strcpy(_verb_1, "");
    snprintf(_verb_1, 99, " ist %s ", entity);
    
    char* verb_str = 0;
    if (verb_str = strstr(sentence, "hrt zu ")) {
        verb_str += 7;
    }
    else if (verb_str = strstr(sentence, " ist ")) {
        verb_str += 5;
    }
    else if (verb_str = strstr(sentence, " war ")) {
        verb_str += 5;
    }
    else if (verb_str = strstr(sentence, " heissen ")) {
        verb_str += 9;
    }
    else if (verb_str = strstr(sentence, " versteht man ")) {
        verb_str += 14;
    }
    else if (verb_str = strstr(sentence, " bezeichnet man ")) {
        verb_str += 16;
    }
    else if (verb_str = strstr(sentence, " nennt man ")) {
        verb_str += 11;
    }
    else if (verb_str = strstr(sentence, " bezeichnet ")) {
        verb_str += 12;
    }
    else if (verb_str = strstr(sentence, _verb_1)) {
        verb_str += strlen(_verb_1);
    }
    else if (verb_str = strstr(sentence, " waren ")) {
        verb_str += 7;
    }
    else if (verb_str = strstr(sentence, " sind sie ")) {
        verb_str += 10;
    }
    else if (verb_str = strstr(sentence, " sind ")) {
        verb_str += 6;
    }
    else if (verb_str = strstr(sentence, " wird ")) {
        verb_str += 6;
        if (strstr(sentence, " bezeichnet, ")) {
            strcpy(strstr(sentence, " bezeichnet, "), strstr(sentence, " bezeichnet, ") + strlen(" bezeichnet, ") - 1);
        }
        else if (strstr(sentence, " bezeichnet")) {
            strstr(sentence, " bezeichnet")[0] = '\0';
        }
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
        if (i+1 < size && verb_str[i] == ',' && verb_str[i+1] == ' ') {
            continue;
        }
        
        object[j] = verb_str[i];
        ++j;
    }
    
    int maybe_end = j;
    int number_of_spaces = 0;
    for (; i < size; ++i) {
        if (       verb_str[i] == '.'
                && (i+2 >= size || verb_str[i+1] < '0' || verb_str[i+1] > '9')
                && (i<=2 || verb_str[i-1] < '0' || verb_str[i-1] > '9')
                && (i<=5 || (
                            verb_str+i-4 != strstri(verb_str+i-4, "bspw")
                        &&  verb_str+i-3 != strstri(verb_str+i-3, "mio")
                        &&  verb_str+i-3 != strstri(verb_str+i-3, "mrd")
                        &&  verb_str+i-2 != strstri(verb_str+i-2, "mr")
                        &&  verb_str+i-3 != strstri(verb_str+i-3, "mrs")
                        &&  verb_str+i-2 != strstri(verb_str+i-2, "dr")
                        &&  verb_str+i-4 != strstri(verb_str+i-4, "dres")
                ))
        ) {
            break;
        }
        if (verb_str[i] == ',') {
            if (number_of_spaces > 15) {
                break;
            }
            number_of_spaces -= 5;
        }
        if (number_of_spaces > 15 && i >= 3 && i + 4 < size && (verb_str+i == strstr(verb_str+i, " und") || verb_str+i == strstr(verb_str+i, " oder")) && *(verb_str+i-1) != '-') {
            break;
        }
        if (i+1 < size && verb_str[i] == ',' && verb_str[i+1] == ' ') {
            continue;
        }
        
        if (verb_str[i] == ' ') {
            ++number_of_spaces;
        }
        
        object[j] = verb_str[i];
        ++j;
    }
    
    if (number_of_spaces > 20) {
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

halstring** search_results        = 0;
int         search_results_lines  = 0;
halstring*  search_results_line   = 0;
halstring*  search_results_entity = 0;

int is_no_wiki_word(const char* word) {
    return (
        can_be_a_pointer(word) && (
            0 == strcmp(word, "ich")
         || 0 == strcmp(word, "du")
         || 0 == strcmp(word, "er")
         || 0 == strcmp(word, "sie")
         || 0 == strcmp(word, "es")
         || 0 == strcmp(word, "wir")
         || 0 == strcmp(word, "ihr")
         || 0 == strcmp(word, "sie")
         || 0 == strcmp(word, "i")
         || 0 == strcmp(word, "you")
         || 0 == strcmp(word, "he")
         || 0 == strcmp(word, "she")
         || 0 == strcmp(word, "it")
         || 0 == strcmp(word, "we")
         || 0 == strcmp(word, "us")
         || 0 == strcmp(word, "they")
         || 0 == strcmp(word, "them")
        )
         ?  1
         :  0
    );
}

struct fact** search_facts_wiki_page(const char*, const char*);

struct fact** search_facts_wiki(const char* entity, short todo) {
    if (is_a_trivial_word(entity)) {
        return 0;
    }
    if (is_no_wiki_word(entity)) {
        return 0;
    }
    
    char* entity_without_bad_chars = 0;
    char* entity_without_stars = 0;
    char* entity_without_articles = 0;
    char* entity_to_save_without_articles = 0;
    char* entity_upper = 0;
    char* entity_to_save_upper = 0;
    char* _url = 0;
    char* url = 0;
    halstring** lines = 0;
    int number_of_lines = 0;
    
    entity_without_bad_chars        = delete_bad_chars(entity, 1, 1);
    entity_without_articles         = delete_articles(entity_without_bad_chars);
    entity_upper                    = upper(entity_without_articles);
    
    entity_without_stars            = delete_bad_chars(entity, 1, 0);
    entity_to_save_without_articles = delete_articles(entity_without_stars);
    entity_to_save_upper            = upper(entity_to_save_without_articles);
    /// if NEW
    if (todo == NEW) {
        char* __url             = concat(".wikipedia.org/w/index.php?title=Spezial%3ASuche&fulltext=Volltext&search=", entity_upper);
        _url                    = concat(hal2009_get_text_language(), __url);
        free(__url);
        url                     = replace_spaces(_url);
        printf("url: %s\n", url);
        halstring* file         = download_from_url(url);
        /// entity_upper is free'd at the end of the function
        free( _url);
        free(  url);
        
        if (!file) {
            free(entity_without_stars);
            free(entity_upper);
            return 0;
        }

        if (search_results) {
            int current_line;
            for (current_line = 0; current_line < search_results_lines; ++current_line) {
                if (0 == search_results[current_line])
                    break;
                if (0 == search_results[current_line]->s)
                    break;
                halfree(search_results[current_line]->s);
                halfree(search_results[current_line]);
                search_results[current_line] = 0;
                continue;
            }
            search_results = 0;
        }
        if (search_results_entity) {
            free(search_results_entity);
            search_results_entity = 0;
        }
    
        search_results_entity   = strdup(entity_to_save_upper);

        number_of_lines = 0;
        lines = stringtoarray(file, '\n', &number_of_lines);
        search_results = lines;
        search_results_lines = number_of_lines;
        
        if (!file) {
            printf("Was not successful: download_from_url\n");
            return 0;
        }
        if (file->do_free) {
            free(file->s);
        }
    }
    /// if CONTINUE
    else if (search_results && search_results_entity) {
        lines = search_results;
        number_of_lines = search_results_lines;
    }
    else {
        free(entity_without_stars);
        free(entity_upper);
        return 0;
    }
    free(entity_without_stars);
    free(entity_without_bad_chars);
    free(entity_upper);
    free(entity_to_save_without_articles);
    free(entity_to_save_upper);
    
    
    struct fact** facts = 0;
    
    int current_line;
    if (todo == NEW) {
        current_line = 0;
    }
    else {
        current_line = search_results_line;
    }
    printf("(%i lines / search_results_line = %i / current_line = %i)\n", number_of_lines, search_results_line, current_line);
    for (++current_line; current_line < number_of_lines; ++current_line) {
        printf("- %i\n", current_line);
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
        
        
        if (strstr(lines[current_line]->s, "<li><a href=\"/wiki/")) {
            char* start = strstr(lines[current_line]->s, "\"");
            if (!start) continue;
            ++start;
            
            char* stop = strstr(start, "\"");
            if (!stop) continue;
            stop[0] = '\0';
            
            printf("page: \"%s\"\n", start);
            
            facts = search_facts_wiki_page(start, search_results_entity);
            if (facts) {
                search_results_line = current_line;
                break;
            }
        }
    }
    
    
    return facts;
}

struct fact** search_facts_wiki_page(const char* __url, const char* entity_upper) {
    if (!entity_upper || strlen(entity_upper) < 3) {
        return 0;
    }
    
    printf("page: __url: %s\n", __url);
    char* ___url                  = concat(".wikipedia.org", __url);
    char* _url                    = concat(hal2009_get_text_language(), ___url);
    free(___url);
    printf("page: _url: %s\n", _url);
    char*  url                    = replace_spaces(_url);
    printf("page: url: %s\n", url);
    halstring* file               = download_from_url(url);
    /// entity_upper is free'd at the end of the function
    free( _url);
    free(  url);
    
    if (!file) {
        printf("page: Was not successful: download_from_url\n");
        return 0;
    }
    
    struct fact** facts = calloc(sizeof(struct fact*), 2);

    int number_of_lines = 0;
    halstring** lines = stringtoarray(file, '\n', &number_of_lines);
    int current_line;
    printf("page: (%i lines)\n", number_of_lines);
    int in_header = 1;
    int in_table = 0;
    int in_script = 0;
    int in_ul = 0;
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
        if (strstr(lines[current_line]->s, "<ul")) {
            ++in_ul;
        }
        if (strstr(lines[current_line]->s, "</ul>")) {
            --in_ul;
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
            if (in_ul && strstr(lines[current_line]->s, "<a href=\"/wiki/")) {
                char* start = strstr(lines[current_line]->s, "\"");
                if (!start) continue;
                ++start;
                
                char* stop = strstr(start, "\"");
                if (!stop) continue;
                stop[0] = '\0';
                
                printf("page: \"%s\"\n", start);
                
                struct fact** temp = search_facts_wiki_page(start, entity_upper);
                if (temp) {
                    if (facts) free(facts);
                    facts = temp;
                    break;
                }
            }
            if (strstr(lines[current_line]->s, "action=edit") && !strstr(lines[current_line]->s, "redlink")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "bezeichnet:")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "bezeichnet") && !strstr(lines[current_line]->s, ". ")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "benannt:")) {
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
            if (strstr(lines[current_line]->s, "<li")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "Vorlage:Infobox")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "Siehe auch:")) {
                continue;
            }
            if (strstr(lines[current_line]->s, "iehe:")) {
                continue;
            }
            
            
            lines[current_line] = remove_between(lines[current_line], '(', ')');
            lines[current_line] = remove_between(lines[current_line], '[', ']');
            lines[current_line] = remove_between(lines[current_line], '<', '>');
            lines[current_line] = remove_between(lines[current_line], '&', ';');
            
            if (!can_be_a_pointer(lines[current_line]) || !can_be_a_pointer(lines[current_line]->s)) {
                continue;
            }
            
            char* ascii_text = ascii(lines[current_line]->s);
            if (ascii_text) {
                lines[current_line]->s = ascii_text;
            }
            
            while (lines[current_line]->s[0] == ' ' || lines[current_line]->s[0] == '\t') {
                strcpy(lines[current_line]->s, lines[current_line]->s+1);
            }
            
            if (strlen(lines[current_line]->s) < 30) {
                continue;
            }
            
            const char* general_verb = define_general_verb(lines[current_line]->s, entity_upper);
            char* object      = transform_sentence(lines[current_line]->s, entity_upper);
            if (strstr(object, ":")-object > strlen(object)-5 && strstr(object, ":")-object < strlen(object)+1) {
                free(object);
                break;
            }
            
            struct fact* fact  = calloc(sizeof(struct fact), 1);
            fact->pk           = 0;
            fact->verbs        = divide_words(general_verb);
            fact->subjects     = divide_words(entity_upper);
            fact->objects      = divide_words(object);
            fact->adverbs      = divide_words("...");
            fact->extra        = divide_words("");
            fact->questionword = strdup("");
            fact->from         = strdup("");
            fact->truth        = 1.0;
            
            /// write to prot file
            {
                char file[501];
                char* first_character = strdup(entity_upper);
                first_character[1] = '\0';
                snprintf(file, 500, "lang_%s/wiki/%s-wiki.prot", hal2009_get_text_language(), first_character);
                free(first_character);
                char line[501];
                char* object_working = object;
                char* end;
                while (end = strstr(object_working, ",")) {
                    
                    char* i;
                    short found_big_letter = 0;
                    for (i = object_working; i != end; ++i) {
                        if (i[0] >= 'A' && i[0] <= 'Z' && (strstr(i, " ") == 0 || strstr(i, " ") > end)) {
                            ++found_big_letter;
                        }
                    }
                    
                    if (found_big_letter) {
                        end[0] = '\0';
                        break;
                    }
                    else {
                        end[0] = ' ';
                        object_working = end+1;
                    }
                }
                char* i;
                for (i = object; i[0]; ++i) {
                    if ((strstr(i, " und") == i || strstr(i, " oder") == i) && i - object > 3 && *(i-1) != '-') {
                        i[0] = '\0';
                    }
                }
                snprintf(line, 500, "%s ~~~ %s\n", entity_upper, object);
                if (strlen(entity_upper) >= 3 && entity_upper[0] != '0') {
                    FILE* prot_out = fopen(file, "a");
                    if (prot_out) {
                        fprintf(prot_out, "%s", line);
                        fclose(prot_out);
                    }
                }
            }
            
            facts[0] = fact;
            
            if (strlen(object) && object[0] != ' ' && object[0] != '\t') {
                free(object);
                break;
            }
            free(object);
        }
    }
    for (; current_line < number_of_lines; ++current_line) {
        if (0 == lines[current_line])
            continue;
        if (0 == lines[current_line]->s)
            continue;
        
        {
            halfree(lines[current_line]->s);
            halfree(lines[current_line]);
            lines[current_line] = 0;
            continue;
        }
    }
    
    
    if (file->do_free) {
        free(file->s);
    }
    
    return facts;
}

