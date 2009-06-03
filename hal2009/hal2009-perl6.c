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

#include "hal2009.h"


/* PERL 6 convert functions */

static inline void convert_to_perl6_structure (halstring* hals) {
    
    if (strstr(hals->s, "compile source " )) {
        hals = replace(hals, "compile source ", "");
        convert_to_perl6_convert_file(hals->s);
        strcpy(hals->s, "");
        return;
    }
    
    if (strstr(hals->s, "require source " )) {
        hals = replace(hals, "require source ", "require(\"");
        strcat(hals->s, ".pl6\");");
    }
    
    if (strstr(hals->s, "print into " )) {
        hals = replace(hals, "print into ", " ");
        hals = replace(hals, " data ", ".print: ");
        strcat(hals->s, ";");
    }
    else if (strstr(hals->s, "print " )) {
        hals = replace(hals, "print ", "do print with ");
    }
    
    if (strstr(hals->s, "~" )) {
        hals = replace(hals, "~", "$");
    }
    hals = replace(hals, " is not ", " not is ");
    hals = replace(hals, "global hash ", "hash main::");
    hals = replace(hals, "global array ", "array main::");
    hals = replace(hals, "global var ", "var main::");
    hals = replace(hals, "first item", "item [ 0 ]");
    hals = replace(hals, "is an empty array ", "() ~~ @");
    hals = replace(hals, "is an empty hash ", "() ~~ %");
    hals = replace(hals, "that array is empty: ", "() ~~ (");
    hals = replace(hals, "that hash is empty: ", "() ~~ (");
    hals = replace(hals, "that array not is empty: ", "() !~~ (");
    hals = replace(hals, "that hash not is empty: ", "() !~~ (");
    hals = replace(hals, "end-check", ")");
    hals = replace(hals, "push into", "do push with into");
    hals = replace(hals, "into array ", "@");
    hals = replace(hals, "into hash ", "%");
    hals = replace(hals, "items of array ", "@");
    hals = replace(hals, "items of hash ", "%");
    hals = replace(hals, "of array ", "@");
    hals = replace(hals, "of hash ", "%");
    hals = replace(hals, "items of that array: ", "(");
    hals = replace(hals, "items of that hash: ", "(");
    hals = replace(hals, "items of that array ", "(");
    hals = replace(hals, "items of that hash ", "(");
    hals = replace(hals, "end-items-of", ")");
    hals = replace(hals, "end items of", ")");
    hals = replace(hals, "shift ", "do shift with ");
    hals = replace(hals, "from array ", "@");
    hals = replace(hals, "from hash ", "%");
    hals = replace(hals, "to array-returning ", "to ");
    hals = replace(hals, "to items-returning ", "to ");
    hals = replace(hals, "var ", "$");
    hals = replace(hals, "const ", "$");
    hals = replace(hals, " flag ", " $");
    hals = replace(hals, " from array ", " @");
    hals = replace(hals, " array ", " @");
    hals = replace(hals, "variable ", "$");
    hals = replace(hals, "length(", "chars(");
    hals = replace(hals, "an empty hash", "()");
    hals = replace(hals, "an empty array", "()");
    hals = replace(hals, " from hash ", " %");
    hals = replace(hals, " hash ", " %");
    hals = replace(hals, " item ", "");
    if (strstr(hals->s, "exists:" )) {
        hals = replace(hals, " exists: ", " ");
        hals = replace(hals, ", end test", " ~~ :e");
    }
    hals = replace(hals, "A-Z", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    hals = replace(hals, "a-z", "abcdefghijklmnopqrstuvwxyz");
    hals = replace(hals, "0-9", "0123456789");
    if (strstr(hals->s, "DEBUG VAR: " )) {
        hals = replace(hals, "DEBUG VAR: ", "# <Debugging variable>-feature not available in FreeHAL's Perl6 # DEBUG VAR: ");
    }
    if (strstr(hals->s, "file name " )) {
        hals = replace(hals, "file name ", " ");
    }
    if (strstr(hals->s, " concat " )) {
        hals = replace(hals, " concat ", " ~ ");
    }
    if (strstr(hals->s, " file handle for " )) {
        hals = replace(hals, " file handle for ", " open with ");
    }
    if (strstr(hals->s, " handle for " )) {
        hals = replace(hals, " handle for ", " open with ");
    }
    if (strstr(hals->s, " read" )) {
        hals = replace(hals, " read", " :r");
    }
    if (strstr(hals->s, " write" )) {
        hals = replace(hals, " write", " :w");
    }
    if (strstr(hals->s, " append" )) {
        hals = replace(hals, " append", " :a");
    }
    if (strstr(hals->s, "do regex using " )) {
        hals = replace(hals, "do regex using ", "");
        hals = replace(hals, ": ", ".subst(");
        //hals = realloc(hals, strlen(hals) + 1 + 2);
        strcat(hals->s, ");");
    }
    if (strstr(hals->s, "do wait" )) {
        hals = replace(hals, "do wait", "select undef, undef, undef,");
        hals = replace(hals, "seconds", ";");
        hals = replace(hals, "second", ";");
    }
    if (strstr(hals->s, "define action " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, "define action ", "sub ");
        hals = replace(hals, " without arguments", "(*@_, *%_");
        hals = replace(hals, " with ", "(");
        hals = replace(hals, ":", ") {");
    }
    if (strstr(hals->s, "done" )) {
        hals = replace(hals, "done", "}");
    }
    if (strstr(hals->s, "else if " )) {
        hals = replace(hals, "else if ", "elsif ");
    }
    if (strstr(hals->s, "else do" )) {
        hals = replace(hals, "else do", "else {");
    }
    if (strstr(hals->s, "end if" )) {
        hals = replace(hals, "end if", "}");
    }
    hals = replace(hals, " matches ", " ~~ ");
    hals = replace(hals, " match ", " ~~ ");
    hals = replace(hals, " is ", " == ");
    hals = replace(hals, " and ", " && ");
    hals = replace(hals, " or ", " || ");
    if (strstr(hals->s, "if " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, ":", ") {");
        hals = replace(hals, "if ", "if (");
    }
    if (strstr(hals->s, "end for" )) {
        hals = replace(hals, "end for", "}");
    }
    if (strstr(hals->s, "for " )) {
        hals = replace(hals, ":", " {");
    }
    if (strstr(hals->s, "while " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, "while ", "while (");
        hals = replace(hals, ":", ") {");
    }
    if (strstr(hals->s, "end while" )) {
        hals = replace(hals, "end while", "}");
    }
    if (strstr(hals->s, "loop " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, ":", " {");
    }
    if (strstr(hals->s, "end loop" )) {
        hals = replace(hals, "end loop", "}");
    }
    if (strstr(hals->s, "new line" )) {
        hals = replace(hals, "new line", "qq{\\n}");
    }
    if ( (strstr(hals->s, "do " ) || strstr(hals->s, " to " )) && (strstr(hals->s, " with" ) || strstr(hals->s, " using" )) ) {
        hals = replace(hals, "do ", "");
        hals = replace(hals, " without arguments", "(");
        hals = replace(hals, " with ", "(");
        hals = replace(hals, " by using ", "(");
        hals = replace(hals, " using ", "(");
        hals = replace(hals, " -> ", "(");
        //hals = realloc(hals, strlen(hals) + 1 + 2);
        strcat(hals->s, ");");
    }
    hals = replace(hals, "new $main::", "our $");
    hals = replace(hals, "new %main::", "our %");
    hals = replace(hals, "new @main::", "our @");
    hals = replace(hals, "new $", "my $");
    hals = replace(hals, "new %", "my %");
    hals = replace(hals, "new @", "my @");
    if (strstr(hals->s, " the next line from " )) {
        hals = replace(hals, " the next line from ", " =");
    }
    if (strstr(hals->s, "set " ) && strstr(hals->s, " to" )) {
        hals = replace(hals, "set ", "");
        hals = replace(hals, " set to ", " = ");
        hals = replace(hals, " to ", " = ");
        if (strstr(hals->s, "multi-line" )) {
            hals = replace(hals, "multi-line", "");
        }
        else {
            strcat(hals->s, ";");
        }
    }
    if (strstr(hals->s, "go to next" ) || strstr(hals->s, "go to last" )) {
        hals = replace(hals, "go to ", "");
        strcat(hals->s, ";");
    }
    if (strstr(hals->s, "for each " ) && (strstr(hals->s, " in " ) || strstr(hals->s, " from " ))) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, " my ", " ");
        hals = replace(hals, ":", " :");
        hals = replace(hals, "for each ", "for ");

        int number_of_words;
        int estimated_string_length = line_size + 100;
        int spaces_before_begin_of_line = 0;
        while ( hals->s[0] == ' ' ) {
            ++hals->s;
            ++spaces_before_begin_of_line;
        }
        halstring** words = stringtoarray(hals, ' ', &number_of_words);
        //halfree(hals);

        halstring* temp;
        temp = words[1];
        words[1] = words[3];
        words[3] = temp;

        int i;
        char* new_hals;
        new_hals = halmalloc(estimated_string_length, "convert_to_perl6_structure");
        *new_hals = 0;
        for (i = 0; i < number_of_words; ++i) {
            strcat(new_hals, words[i]->s);
            strcat(new_hals, " ");
            halfree(words[i]->s);
            halfree(words[i]);
        }
        halfree(words);
        hals->s = new_hals;
        // for =$file -> $line {
        hals = replace(hals, " file ", " ");
        hals = replace(hals, " in ", " -> ");
        if (strstr(hals->s, " from " )) {
            hals = replace(hals, "for ", "for =");
        }
        hals = replace(hals, " from ", " -> ");
        hals = replace(hals, ":", " {");
//        hals = realloc(hals, strlen(hals) + 1 + 2);
//        strcat(hals, " {");
    }
    else if (strstr(hals->s, "for " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, ":", " {");
    }
    hals = replace(hals, " { ", "{");
    hals = replace(hals, " } ", "} ");
    hals = replace(hals, ";;", ";");
    hals = replace(hals, "( ", "(");
    hals = replace(hals, " )", ")");
    //*hals_orig = hals;
    return;
}

char* convert_to_perl6 (char* hals) {
    int number_of_lines;
    halstring string;
    string.s = hals;
    string.do_free = 0;
    halstring** lines = stringtoarray(&string, '\n', &number_of_lines);
    
    long new_code_size = (line_size + 100) * (number_of_lines + 1);
    char* newcode;
    newcode = halmalloc(new_code_size, "convert_to_perl6"); // be sure we have enough space
    zero_p(newcode, new_code_size);
    
    int current_line;
    for (current_line = 0; current_line < number_of_lines; ++current_line) {
        char* newline;
        newline = halmalloc(line_size + 100, "convert_to_perl6"); // be sure we have enough space
        zero_p(newline, line_size + 100);
        strcpy(newline, lines[current_line]->s);
        halstring s;
        s.s = newline;
        s.do_free = 1;
        
        convert_to_perl6_structure(&s);
        strcat(newcode, s.s);
        strcat(newcode, "\n");
        if ( lines[current_line]->do_free ) halfree(lines[current_line]->s);
        halfree(lines[current_line]);
        if ( s.do_free ) halfree(s.s);
        
        if (current_line % 20 == 0) fprintf(output(), "%s", ".");
    }
    fprintf(output(), "%s", "\n");
    halfree(lines);
    halfree(hals);
    
    return newcode;
}


#include <parrot/parrot.h>
#include <parrot/embed.h>
#include <parrot/imcc.h>

int convert_to_perl6_convert_file(char* filename) {
    FILE* source = fopen(filename, "r");
    if ( !source ) {
        fprintf(output(), "Parrot source file not found: %s", filename);
        return 1;
    }

    fseek(source, 0, SEEK_END);
    int file_size = ftell(source) + 1;
    char* code = halmalloc(file_size, "convert_to_perl6_convert_file");
    fseek(source, 0, SEEK_SET);
    fread(code, 1, file_size, source);
    halclose(source);
    
    FILE* target;
    {
        char targetname[300];
        strcpy(targetname, filename);
        strcat(targetname, ".pl6");
        
        target = fopen(targetname, "w");
    }
    if ( !target ) {
        fprintf(output(), "Parrot Perl6 source file: unable to create: %s", filename);
        return 1;
    }
    fprintf(output(), "Compile %s...", filename);
    char* targetcode;
    {
        targetcode = convert_to_perl6(code);
    }
    // DEBUG: fprintf(output(), "%s\n", targetcode);
    halwrite(targetcode, 1, strlen(targetcode), target);
    halclose(target);
    halfree(targetcode);
    
    return 0;
}

void execute_perl6(char* filename) {
    Interp  *interp;
    int      status;
    const char *sourcefile;

    /* internationalization setup */
    /* setlocale(LC_ALL, ""); */

    PARROT_BINDTEXTDOMAIN(PACKAGE, LOCALEDIR);
    PARROT_TEXTDOMAIN(PACKAGE);

    Parrot_set_config_hash();

    interp = Parrot_new(NULL);
    imcc_initialize(interp);

    /* We parse the arguments, but first store away the name of the Parrot
       executable, since parsing destroys that and we want to make it
       available. */
    fprintf(output(), "%s\n", "Set name...");
    Parrot_set_executable_name(interp, string_from_cstring(interp, "FreeHAL", 0));

    fprintf(output(), "%s\n", "Compile FreeHAL...");
    convert_to_perl6_convert_file(filename);
    
    {
        int argc = 3;
        char** argv = malloc(100+strlen(filename));
        argv[0] = "FreeHAL";
        argv[1] = "perl6.pbc";
        strcpy(argv[2], filename);
        strcat(argv[2], ".pl6");

        fprintf(output(), "%s\n", "Parse command line arguments...");
        sourcefile = parseflags(interp, &argc, &argv);
        fprintf(output(), "%s\n", "Run IMCC...");
        status     = imcc_run(interp, sourcefile, argc, argv);
        UNUSED(status);
    }

    fprintf(output(), "%s\n", "Destroy Parrot...");
    /* Clean-up after ourselves */
    Parrot_destroy(interp);
}

