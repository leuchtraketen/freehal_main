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

#include <uconfig.h>
#include <config.h>
#include <EXTERN.h>
#include <perl.h>
#include <stdio.h>

#include "hal2009.h"

void xs_init (pTHX);

#ifdef fopen
#undef fopen
#endif
#ifdef fwrite
#undef fwrite
#endif
#ifdef fread
#undef fread
#endif
#ifdef fclose
#undef fclose
#endif
#ifdef fseek
#undef fseek
#endif
#ifdef stat
#undef stat
#endif

/* PERL 5 convert functions */
static inline void convert_to_perl5_structure (halstring* hals) {
    hals = replace(hals, "\r", "");
    if (strstr(hals->s, "compile source " )) {
        hals = replace(hals, "compile source ", "");
        convert_to_perl5_convert_file(hals->s);
        strcpy(hals->s, "");
        return;
    }
    
    if (strstr(hals->s, "require source " )) {
        hals = replace(hals, "require source ", "require(\"");
        strcat(hals->s, ".pl\");");
    }
    
    if (strstr(hals->s, "print into " )) {
        hals = replace(hals, "print into ", "print {");
        hals = replace(hals, " data ", "} ");
        strcat(hals->s, ";");
    }
    else if (strstr(hals->s, "print " )) {
        hals = replace(hals, "print ", "do print with ");
    }
    
    hals = replace(hals, "# TEXT", "#---------------------------------------");
    hals = replace(hals, "# CODE", "#---------------------------------------");
    hals = replace(hals, "variable ", "var ");
    hals = replace(hals, "$15", "$16");
    hals = replace(hals, "$14", "$15");
    hals = replace(hals, "$13", "$14");
    hals = replace(hals, "$12", "$13");
    hals = replace(hals, "$11", "$12");
    hals = replace(hals, "$10", "$11");
    hals = replace(hals, "$9", "$10");
    hals = replace(hals, "$8", "$9");
    hals = replace(hals, "$7", "$8");
    hals = replace(hals, "$6", "$7");
    hals = replace(hals, "$5", "$6");
    hals = replace(hals, "$4", "$5");
    hals = replace(hals, "$3", "$4");
    hals = replace(hals, "$2", "$3");
    hals = replace(hals, "$1", "$2");
    hals = replace(hals, "$0", "$1");
    hals = replace(hals, "<[", "[");
    hals = replace(hals, "]>", "]");
    hals = replace(hals, " is not ", " not is ");
    hals = replace(hals, "global hash ", "hash ::");
    hals = replace(hals, "global array ", "array ::");
    hals = replace(hals, "global var ", "var ::");
    hals = replace(hals, "first item", "item [ 0 ]");
    hals = replace(hals, "is an empty array ", "0 == @$");
    hals = replace(hals, "is an empty hash ", "0 == %$");
    hals = replace(hals, "that array is empty: ", "0 == @{ ");
    hals = replace(hals, "that hash is empty: ", "0 == %{ ");
    hals = replace(hals, "that array not is empty: ", "0 != @{ ");
    hals = replace(hals, "that hash not is empty: ", "0 != %{ ");
    hals = replace(hals, "end check", " }");
    hals = replace(hals, "push into", "do push with into");
    hals = replace(hals, "shift from", "do shift with into");
    hals = replace(hals, "into array ", "@$");
    hals = replace(hals, "into hash ", "%$");
    hals = replace(hals, "items of array ", "@$");
    hals = replace(hals, "items of hash ", "%$");
    hals = replace(hals, "of array ", "@$");
    hals = replace(hals, "of hash ", "%$");
    hals = replace(hals, "items of that array: ", "@{");
    hals = replace(hals, "items of that hash: ", "%{");
    hals = replace(hals, "items of that array ", "@{");
    hals = replace(hals, "items of that hash ", "%{");
    hals = replace(hals, "end-items-of", "}");
    hals = replace(hals, "end items of", "}");
    hals = replace(hals, " is rw", " ");
    hals = replace(hals, " is copy", " ");
    hals = replace(hals, "var ", "$");
    hals = replace(hals, "const ", "$");
    hals = replace(hals, " flag ", " $");
    hals = replace(hals, "from array ", "$");
    hals = replace(hals, " array ", " $");
    hals = replace(hals, "an empty hash", "{}");
    hals = replace(hals, "an empty array", "[]");
    hals = replace(hals, "from hash ", "$");
    hals = replace(hals, " hash ", " $");
    hals = replace(hals, " item ", "->");
    if (strstr(hals->s, "exists:" )) {
        hals = replace(hals, " exists: ", " -e ");
        hals = replace(hals, ", end test", "");
    }
    hals = replace(hals, "A-Z", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    hals = replace(hals, "a-z", "abcdefghijklmnopqrstuvwxyz");
    hals = replace(hals, "0-9", "0123456789");
    if (strstr(hals->s, "DEBUG VAR: " )) {
        hals = replace(hals, "DEBUG VAR: ", "eval 'use Data::Dumper; print Dumper( ");
        hals = replace(hals, "STOP DEBUG", ");'; print new line ; ");
    }
    if (strstr(hals->s, "file name " )) {
        hals = replace(hals, "file name ", " ");
    }
    if (strstr(hals->s, " concat " )) {
        hals = replace(hals, " concat ", " . ");
    }
    if (strstr(hals->s, " file handle for " )) {
        hals = replace(hals, " file handle for ", " open with ");
    }
    if (strstr(hals->s, " handle for " )) {
        hals = replace(hals, " handle for ", " open with ");
    }
    if (strstr(hals->s, "do regex using " )) {
        hals = replace(hals, "do regex using ", "");
        hals = replace(hals, ": ", " =~ s");
        hals = replace(hals, "/, \"", "/");
        hals = replace(hals, "\"", "/");
        hals = replace(hals, "/, :", "/:");
        hals = replace(hals, ":global", "g");
        hals = replace(hals, ":i", "i");
        //hals = realloc(hals, strlen(hals) + 1 + 2);
        strcat(hals->s, ";");
    }
    if (strstr(hals->s, "define action " )) {
        hals = replace(hals, "define action ", "sub ");
        hals = replace(hals, " without arguments", " { \n    my (@_dummy");
        hals = replace(hals, " with ", " { \n    my (");
        hals = replace(hals, " do", ":");
        hals = replace(hals, ":", ") = @_;");
    }
    if (strstr(hals->s, "do wait" )) {
        hals = replace(hals, "do wait", "select undef, undef, undef,");
        hals = replace(hals, "seconds", ";");
        hals = replace(hals, "second", ";");
    }
    if (strstr(hals->s, "done" )) {
        hals = replace(hals, "done", "}");
    }
    if (strstr(hals->s, "else do" )) {
        hals = replace(hals, "else do", "else {");
    }
    if (strstr(hals->s, "else if " )) {
        hals = replace(hals, "else if ", "elsif ");
    }
    if (strstr(hals->s, "end if" )) {
        hals = replace(hals, "end if", "}");
    }
    hals = replace(hals, " matches $", " eq $");
    hals = replace(hals, " match $", " eq $");
    hals = replace(hals, " matches '", " eq '");
    hals = replace(hals, " match '", " eq '");
    hals = replace(hals, " matches \"", " eq \"");
    hals = replace(hals, " match \"", " eq \"");
    hals = replace(hals, " matches ", " =~ ");
    hals = replace(hals, " match ", " =~ ");
    hals = replace(hals, " is ", " == ");
    hals = replace(hals, " and ", " && ");
    hals = replace(hals, " or ", " || ");
    if (strstr(hals->s, "if " )) {
        hals = replace(hals, " do", ") {");
        hals = replace(hals, "if ", "if (");
    }
    if (strstr(hals->s, "end for" )) {
        hals = replace(hals, "end for", "}");
    }
    if (strstr(hals->s, "while " )) {
        hals = replace(hals, "while ", "while (");
        hals = replace(hals, " do", ") {");
    }
    if (strstr(hals->s, "end while" )) {
        hals = replace(hals, "end while", "}");
    }
    if (strstr(hals->s, "loop " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, "loop ", "for (");
        hals = replace(hals, ":", ") {");
    }
    if (strstr(hals->s, "end loop" )) {
        hals = replace(hals, "end loop", "}");
    }
    if (strstr(hals->s, "new line" )) {
        hals = replace(hals, "new line", "qq{\\n}");
    }
    if (strstr(hals->s, "open " )) {
        hals = replace(hals, "do ", " ");
        if (strstr(hals->s, " read" )) {
            hals = replace(hals, ", read", " ");
            hals = replace(hals, " read", " ");
            hals = replace(hals, "open", ", '<'");
        }
        if (strstr(hals->s, " write" )) {
            hals = replace(hals, ", write", " ");
            hals = replace(hals, " write", " ");
            hals = replace(hals, "open", ", '>'");
        }
        if (strstr(hals->s, " append" )) {
            hals = replace(hals, ", append", " ");
            hals = replace(hals, " append", " ");
            hals = replace(hals, "open", ", '>>'");
        }
        hals = replace(hals, "open", "");
        hals = replace(hals, " to ", "");
        hals = replace(hals, "set", "");
        hals = replace(hals, " without arguments", ", ");
        hals = replace(hals, " with ", ", ");
        hals = replace(hals, " by using ", ", ");
        hals = replace(hals, " using ", ", ");
        hals = replace(hals, " -> ", ", ");
        int spaces_before_begin_of_line = 0;
        while ( hals->s[0] == ' ' ) {
            ++spaces_before_begin_of_line;
            ++hals->s;
        }
        hals->s -= spaces_before_begin_of_line;
        strcat(hals->s, ");");
        char* new_hals;
        new_hals = halmalloc(strlen(hals->s)+30, "convert_to_perl5_structure");
        *new_hals = 0;
        int k;
        for (k = 0; k <= spaces_before_begin_of_line; ++k) {
            strcat(new_hals, " ");
        }
        strcat(new_hals, "open(");
        strcat(new_hals, spaces_before_begin_of_line + hals->s);
        hals->s = new_hals;
    }
    else if ( (strstr(hals->s, "do " ) || strstr(hals->s, " to " )) && (strstr(hals->s, " with" ) || strstr(hals->s, " using" )) ) {
        hals = replace(hals, "do ", "");
        hals = replace(hals, " without arguments", "(");
        hals = replace(hals, " with ", "(");
        hals = replace(hals, " by using ", "(");
        hals = replace(hals, " using ", "(");
        hals = replace(hals, " -> ", "(");
        strcat(hals->s, ")");
        if (strstr(hals->s, "split" )) {
            hals = replace(hals, " to ", " to [");
            strcat(hals->s, "]");
        }
        if (strstr(hals->s, "to array-returning " )) {
            hals = replace(hals, "to array-returning ", "to @{");
            strcat(hals->s, "}");
        }
        if (strstr(hals->s, "to items-returning " )) {
            hals = replace(hals, "to items-returning ", "to [");
            strcat(hals->s, "]");
        }
        strcat(hals->s, ";");
    }
    hals = replace(hals, "new $::", "our $");
    hals = replace(hals, "new %::", "our %");
    hals = replace(hals, "new @::", "our @");
    hals = replace(hals, "new $", "my $");
    hals = replace(hals, "new %", "my %");
    hals = replace(hals, "new @", "my @");
    if (strstr(hals->s, " the next line from " )) {
        hals = replace(hals, " the next line from ", " <");
        strcat(hals->s, ">");
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
    if (strstr(hals->s, "for each " ) && (strstr(hals->s, " from " ))) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, " :", ":");
        hals = replace(hals, ":", ">)) {");
        hals = replace(hals, "for each ", "while (chomp(");
        hals = replace(hals, " file ", " ");
        hals = replace(hals, " from ", " = <");
    }
    if (strstr(hals->s, "for each " ) && (strstr(hals->s, " in " ))) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, ":", ") {");
        hals = replace(hals, "for each ", "foreach ");
        hals = replace(hals, " file ", " ");
        hals = replace(hals, " in ", " (@");
    }
    if (strstr(hals->s, "for " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, "for", "for (");
        hals = replace(hals, ":", ") {");
    }
    hals = replace(hals, " { ", "{");
    hals = replace(hals, " } ", "} ");
    hals = replace(hals, "@$$", "@$");
    hals = replace(hals, "%$$", "%$");
    hals = replace(hals, ";;", ";");
    hals = replace(hals, "( ", "(");
    hals = replace(hals, " )", ")");
    hals = replace(hals, "[ ", "[");
    hals = replace(hals, " ]", "]");
    hals = replace(hals, "{ ", "{");
    //*hals_orig = hals;
    return;
}

char* convert_to_perl5 (char* hals) {
    int number_of_lines;
    halstring string;
    string.s = hals;
    string.do_free = 1;
    halstring** lines = stringtoarray(&string, '\n', &number_of_lines);
    
    long new_code_size = (LINE_SIZE + 100) * (number_of_lines + 1);
    char* newcode;
    newcode = halmalloc(new_code_size, "convert_to_perl5"); // be sure we have enough space
    zero_p(newcode, new_code_size);
    
    strcat(newcode, "use strict;\n\n");
    int current_line;
    fprintf(output(), "(%i new lines)", number_of_lines);
    for (current_line = 0; current_line < number_of_lines; ++current_line) {
        if (0 == lines[current_line])
            break;
        if (0 == lines[current_line]->s)
            break;
        if (strlen(lines[current_line]->s) <= 1) {
            halfree(lines[current_line]->s);
            halfree(lines[current_line]);
            continue;
        }
        
        char* newline;
        newline = halmalloc(LINE_SIZE + 100, "convert_to_perl5/9"); // be sure we have enough space
        zero_p(newline, LINE_SIZE + 100);
        strcpy(newline, lines[current_line]->s);
        halstring s;
        s.s = newline;
        s.do_free = 1;
        
        convert_to_perl5_structure(&s);
        strcat(newcode, s.s);
        strcat(newcode, "\n");
        if ( lines[current_line]->do_free )
            halfreef(lines[current_line]->s, "convert_to_perl5/1");
        halfreef(lines[current_line], "convert_to_perl5/2");
        if ( s.do_free )
            halfreef(s.s, "convert_to_perl5/3");
        
        if (current_line % 20 == 0)
            fprintf(output(), "%s", ".");
    }
    strcat(newcode, "\n1\n");
    fprintf(output(), "%s", "\n");
    halfree(lines);
    halfree(hals);
    
    return newcode;
}

int convert_to_perl5_convert_file(char* filename) {
    static char* last_filename;
    last_filename = strdup("");
    if (0 == strcmp(last_filename, filename)) {
        fprintf(output(), "Compiler: Abort, unnecessary!\n");
        return 0;
    }
    fprintf(output(), "Last file to compile: %s, new: %s\n", last_filename, filename);

    fprintf(output(), "Opening source file: %s\n", filename);
    FILE* source = fopen(filename, "r");
    if ( !source ) {
        fprintf(output(), "\nPerl5 source file not found(1): %s\n", filename);
        return 1;
    }

    struct stat stbuf;
    stat(filename, &stbuf);
    int file_size = &stbuf ? stbuf.st_size : 0;
    char* code = halmalloc(file_size+1, "convert_to_perl5_convert_file");
    
    fread(code, 1, file_size, source);
    halclose(source);
    
    FILE* target;
    {
        char targetname[100];
        strcpy(targetname, filename);
        strcat(targetname, ".pl");
        
        target = fopen(targetname, "w");
    }
    if ( !target ) {
        fprintf(output(), "\nPerl5 source file: unable to create: %s\n", filename);
        return 1;
    }
    fprintf(output(), "Compile %s (filesize is %i)...", filename, file_size);
    char* targetcode;
    {
        targetcode = convert_to_perl5(code);
    }
    // DEBUG: fprintf(output(), "%s\n", targetcode);
    halwrite(targetcode, 1, strlen(targetcode), target);
    halclose(target);
    halfree(targetcode);

    if (last_filename)
        free(last_filename);
    last_filename = strdup(filename);
    
    return 0;
}

void execute_perl5(char* filename) {
    static PerlInterpreter* my_perl = NULL;

    // COMPILE
    fprintf(output(), "%s\n", "Compile FreeHAL...");
    convert_to_perl5_convert_file(filename);
    
    // INIT
    fprintf(output(), "%s\n", "Running Perl5 -> C-Init...");
    PERL_SYS_INIT3(NULL, NULL, NULL);
    
    // CONSTRUCT
    fprintf(output(), "%s\n", "Running Perl5 -> C-Constructor...");
    if ((my_perl = perl_alloc()) == NULL) {
        fprintf(stderr, "no memory!");
        exit(1);
    }
    perl_construct(my_perl);

    // RUN
    char* perl_filename = halmalloc(strlen(filename)+4, "execute_perl5");
    strcpy(perl_filename, filename);
    strcat(perl_filename, ".pl");

    char* embedding[] = { "", perl_filename };
    fprintf(output(), "%s\n", perl_parse(my_perl, NULL, 2, embedding, (char **)NULL) ? "No success." : "Success.");
    reset_stdout();
    perl_run(my_perl);

    // DECONSTRUCT
    fprintf(output(), "%s\n", "Running Perl5 -> C-Destructor...");
    if ( my_perl ) {
        perl_destruct(my_perl);
        perl_free(my_perl);
    }
}
