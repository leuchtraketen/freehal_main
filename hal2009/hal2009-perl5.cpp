/*
 * This file is part of FreeHAL 2010.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010 Tobias Schulz and contributors.
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

#include "hal2009-perl5.h"

bp::postream* child_stdin = 0;
bp::pistream* child_stdout = 0;


/* PERL 5 convert functions */
static inline void perl5_convert_structure (string& hals, int just_compile) {
    hals = replace(hals, "\r", "");
    if (string::npos != hals.find("compile source " )) {
        hals = replace(hals, "compile source ", "");
        perl5_convert_file(hals);
        hals = "";
        return;
    }
    if (just_compile) {
        return;
    }

    if (string::npos != hals.find("require source " )) {
        hals = replace(hals, "require source ", "require(\"");
        hals += ".pl\");";
    }

    if (string::npos != hals.find("print into " )) {
        hals = replace(hals, "print into ", "print {");
        hals = replace(hals, " data ", "} ");
        hals += (";");
    }
    else if (string::npos != hals.find("print " )) {
        hals = replace(hals, "print ", "do print with ");
    }

    hals = replace(hals, "item", "element");

    hals = replace(hals, "# TEXT", "#---------------------------------------");
    hals = replace(hals, "# CODE", "#---------------------------------------");
    hals = replace(hals, "(end array)", "}");
    hals = replace(hals, "(end hash)", "}");
    hals = replace(hals, "(end)", "}");
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
    if (string::npos != hals.find("set global ")) {
        hals = replace(hals, "set global ", "use vars '");
        hals += ("';");
    }
    hals = replace(hals, "global hash ", "hash ::");
    hals = replace(hals, "global array ", "array ::");
    hals = replace(hals, "global var ", "var ::");
    hals = replace(hals, "size of", "elements of");
    hals = replace(hals, " is true", " ");
    hals = replace(hals, " is wrong", " == 0 ");
    hals = replace(hals, " is false", " == 0 ");
    hals = replace(hals, " the hash ", " from hash ");
    hals = replace(hals, " has element ", " element ");
    hals = replace(hals, " contains element ", " element ");
    hals = replace(hals, " has the element ", " element ");
    hals = replace(hals, " contains the element ", " element ");
    hals = replace(hals, " is not ", " not is ");
    hals = replace(hals, " an empty string", " empty string");
    hals = replace(hals, " a empty string", " empty string");
    hals = replace(hals, " is empty string", " matches empty string");
    hals = replace(hals, " empty string", " '' ");
    hals = replace(hals, "that array is empty: ", "0 == elements of that array: ");
    hals = replace(hals, "that array is empty ", "0 == elements of that array: ");
    hals = replace(hals, "that array not is empty: ", "elements of that array: ");
    hals = replace(hals, "that array not is empty ", "elements of that array: ");
    hals = replace(hals, "that array has elements: ", "elements of that array: ");
    hals = replace(hals, "that array has elements ", "elements of that array: ");
    hals = replace(hals, "first element", "element [ 0 ]");
    hals = replace(hals, "second element", "element [ 1 ]");
    hals = replace(hals, "third element", "element [ 2 ]");
    hals = replace(hals, "last element", "element [ -1 ]");
    {
        char _search_for[99];
        char* search_for = _search_for;
        sprintf(search_for+4, " element");
        char replace_with[99];
        sprintf(replace_with, " element [    - 1 ]");

        char _no_1, no_1, no_2;
        for (_no_1 = '0'; _no_1 <= '9'; ++_no_1) {
            no_1 = _no_1;
            if (_no_1 == '0') {
                no_1 =  ' ';
            }
            for (no_2 = '0'; no_2 <= '9'; ++no_2) {
                replace_with[11] = no_1;
                replace_with[12] = no_2;

                search_for[0] = no_1;
                search_for[1] = no_2;

                search_for[2] = 't';
                search_for[3] = 'h';
                if (no_2 == '1' && no_1 != '1') {
                    search_for[2] = 's';
                    search_for[3] = 't';
                }
                if (no_2 == '2' && no_1 != '1') {
                    search_for[2] = 'n';
                    search_for[3] = 'd';
                }
                if (no_2 == '3' && no_1 != '1') {
                    search_for[2] = 'r';
                    search_for[3] = 'd';
                }

                if (no_1 == ' ') {
                    ++search_for;
                }

                hals = replace(hals, search_for, replace_with);

                if (no_1 == ' ') {
                    --search_for;
                }
            }
        }
    }
    hals = replace(hals, "is an empty array ", "0 == @$");
    hals = replace(hals, "is an empty hash ", "0 == %$");
    hals = replace(hals, "that array is empty: ", "0 == @{ ");
    hals = replace(hals, "that hash is empty: ", "0 == %{ ");
    hals = replace(hals, "that array not is empty: ", "0 != @{ ");
    hals = replace(hals, "that hash not is empty: ", "0 != %{ ");
    hals = replace(hals, "end check", " }");
    hals = replace(hals, "do push into", "push into");
    if (string::npos != hals.find("push into that" )) {
        hals = replace(hals, "push into that", "do push with elements of that");
        hals = replace(hals, ", ", " end of elements, ");
    }
    hals = replace(hals, "push into", "do push with into");
    hals = replace(hals, "shift from that", "do shift with elements of that");
    hals = replace(hals, "shift from", "do shift with into");
    hals = replace(hals, "into array ", "@$");
    hals = replace(hals, "into hash ", "%$");
    hals = replace(hals, "elements of array ", "@$");
    hals = replace(hals, "elements of hash ", "%$");
    hals = replace(hals, "of array ", "@$");
    hals = replace(hals, "of hash ", "%$");
    hals = replace(hals, "elements of that array: ", "@{");
    hals = replace(hals, "elements of that hash: ", "%{");
    hals = replace(hals, "elements of that array ", "@{");
    hals = replace(hals, "elements of that hash ", "%{");
    hals = replace(hals, "end-elements-of", "}");
    hals = replace(hals, "end elements of", "}");
    hals = replace(hals, "end-of-elements", "}");
    hals = replace(hals, "end of elements", "}");
    hals = replace(hals, "end-elements", "}");
    hals = replace(hals, "end elements", "}");
    hals = replace(hals, " of that ", " of ");
    hals = replace(hals, "elements of hash: ", "%{");
    hals = replace(hals, "elements of array: ", "@{");
    hals = replace(hals, "end-array", "}");
    hals = replace(hals, "end-hash", "}");
    hals = replace(hals, "end array", "}");
    hals = replace(hals, "end hash", "}");
    hals = replace(hals, "end of array", "}");
    hals = replace(hals, "end of hash", "}");
    hals = replace(hals, "if not then", "||");
    hals = replace(hals, " is rw", " ");
    hals = replace(hals, " is copy", " ");
    hals = replace(hals, "var ", "$");
    hals = replace(hals, "const ", "$");
    hals = replace(hals, " flag ", " $");
    hals = replace(hals, "from array ", "$");
    hals = replace(hals, "an empty hash", "{}");
    hals = replace(hals, "an empty array", "[]");
    hals = replace(hals, "'array ", "'$");
    hals = replace(hals, " array ", " $");
    hals = replace(hals, "from hash ", "$");
    hals = replace(hals, " hash ", " $");
    hals = replace(hals, "'hash ", "'$");
    hals = replace(hals, " element ", "->");
    if (string::npos != hals.find("exists:" )) {
        hals = replace(hals, " exists: ", " -e ");
        hals = replace(hals, ", end test", "");
    }
    hals = replace(hals, "A-Z", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    hals = replace(hals, "a-z", "abcdefghijklmnopqrstuvwxyz");
    hals = replace(hals, "0-9", "0123456789");
    if (string::npos != hals.find("DEBUG VAR: " )) {
        hals = replace(hals, "DEBUG VAR: ", "eval 'use Data::Dumper; print Dumper( ");
        hals = replace(hals, "STOP DEBUG", ");'; print new line ; ");
    }
    if (string::npos != hals.find("file name " )) {
        hals = replace(hals, "file name ", " ");
    }
    if (string::npos != hals.find(" concat " )) {
        hals = replace(hals, " concat ", " . ");
    }
    if (string::npos != hals.find(" file handle for " )) {
        hals = replace(hals, " file handle for ", " open with ");
    }
    if (string::npos != hals.find(" handle for " )) {
        hals = replace(hals, " handle for ", " open with ");
    }
    if (string::npos != hals.find("do regex using " )) {
        hals = replace(hals, "do regex using ", "do regex ");
    }
    if (string::npos != hals.find("do regex with " )) {
        hals = replace(hals, "do regex with ", "do regex ");
    }
    if (string::npos != hals.find("do regex " )) {
        hals = replace(hals, "do regex ", "regex ");
    }
    if (string::npos != hals.find("regex " )) {
        hals = replace(hals, "regex ", "");
        hals = replace(hals, ": ", " =~ s");
        hals = replace(hals, "/ -> \"", "/");
        hals = replace(hals, "/, \"", "/");
        hals = replace(hals, "\"", "/");
        hals = replace(hals, "/, :", "/:");
        hals = replace(hals, "/ :", "/:");
        hals = replace(hals, ":global", "g");
        hals = replace(hals, ":i", "i");
        //hals = realloc(hals, strlen(hals) + 1 + 2);
        hals += (";");
    }
    if (string::npos != hals.find("define action " )) {
        hals = replace(hals, "define action ", "sub ");
        hals = replace(hals, " without arguments", " { \n    my (@_dummy");
        hals = replace(hals, " with ", " { \n    my (");
        hals = replace(hals, " do", ":");
        hals = replace(hals, ":", ") = @_;");
    }
    if (string::npos != hals.find("do wait" )) {
        hals = replace(hals, "do wait", "select undef, undef, undef,");
        hals = replace(hals, "seconds", ";");
        hals = replace(hals, "second", ";");
    }
    if (string::npos != hals.find("done" )) {
        hals = replace(hals, "done", "}");
    }
    if (string::npos != hals.find("else do" )) {
        hals = replace(hals, "else do", "else {");
    }
    if (string::npos != hals.find("else if " )) {
        hals = replace(hals, "else if ", "elsif ");
    }
    if (string::npos != hals.find("end if" )) {
        hals = replace(hals, "end if", "}");
    }
    hals = replace(hals, " not matches $", " ne $");
    hals = replace(hals, " not match $", " ne $");
    hals = replace(hals, " not matches '", " ne '");
    hals = replace(hals, " not match '", " ne '");
    hals = replace(hals, " not matches \"", " ne \"");
    hals = replace(hals, " not match \"", " ne \"");
    hals = replace(hals, " not matches ", " !~ ");
    hals = replace(hals, " not match ", " !~ ");
    hals = replace(hals, " not is ", " != ");
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
    if (string::npos != hals.find("if " )) {
        hals = replace(hals, " do", ") {");
        hals = replace(hals, "if ", "if (");
    }
    if (string::npos != hals.find("end for" )) {
        hals = replace(hals, "end for", "}");
    }
    if (string::npos != hals.find("while " )) {
        hals = replace(hals, "while ", "while (");
        hals = replace(hals, " do", ") {");
    }
    if (string::npos != hals.find("end while" )) {
        hals = replace(hals, "end while", "}");
    }
    if (string::npos != hals.find("loop " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, "loop ", "for (");
        hals = replace(hals, ":", ") {");
    }
    if (string::npos != hals.find("end loop" )) {
        hals = replace(hals, "end loop", "}");
    }
    if (string::npos != hals.find("new line" )) {
        hals = replace(hals, "new line", "qq{\\n}");
    }
    if (string::npos != hals.find("open " )) {
        hals = replace(hals, "do ", " ");
        if (string::npos != hals.find(" read" )) {
            hals = replace(hals, ", read", " ");
            hals = replace(hals, " read", " ");
            hals = replace(hals, "open", ", '<'");
        }
        if (string::npos != hals.find(" write" )) {
            hals = replace(hals, ", write", " ");
            hals = replace(hals, " write", " ");
            hals = replace(hals, "open", ", '>'");
        }
        if (string::npos != hals.find(" append" )) {
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
        string::size_type spaces_before_begin_of_line = hals.find_first_not_of(' ');
        if (spaces_before_begin_of_line != string::npos) {
            hals.insert(spaces_before_begin_of_line, "open(");
        }
        hals += ");";
    }
    else if ( (string::npos != hals.find("do " ) || string::npos != hals.find(" to " )) && (string::npos != hals.find(" with" ) || string::npos != hals.find(" using" )) ) {
        hals = replace(hals, "do ", "");
        hals = replace(hals, " without arguments", "(");
        hals = replace(hals, " with ", "(");
        hals = replace(hals, " by using ", "(");
        hals = replace(hals, " using ", "(");
        hals = replace(hals, " -> ", "(");
        hals += (")");
        if (string::npos != hals.find("split" )) {
            hals = replace(hals, " to ", " to [");
            hals += ("]");
        }
        if (string::npos != hals.find("to array-returning " )) {
            hals = replace(hals, "to array-returning ", "to @{");
            hals += ("}");
        }
        if (string::npos != hals.find("to elements-returning " )) {
            hals = replace(hals, "to elements-returning ", "to [");
            hals += ("]");
        }
        hals += (";");
    }
    hals = replace(hals, "new $::", "our $");
    hals = replace(hals, "new %::", "our %");
    hals = replace(hals, "new @::", "our @");
    hals = replace(hals, "new $", "my $");
    hals = replace(hals, "new %", "my %");
    hals = replace(hals, "new @", "my @");
    if (string::npos != hals.find(" the next line from " )) {
        hals = replace(hals, " the next line from ", " <");
        hals += (">");
    }
    if (string::npos != hals.find(" all lines from " )) {
        hals = replace(hals, " all lines from ", " <");
        hals += (">");
    }
    if (string::npos != hals.find("set " ) && string::npos != hals.find(" to" )) {
        hals = replace(hals, "set ", "");
        hals = replace(hals, " set to ", " = ");
        hals = replace(hals, " to ", " = ");
        if (string::npos != hals.find("multi-line" )) {
            hals = replace(hals, "multi-line", "");
        }
        else {
            hals += (";");
        }
    }
    if (string::npos != hals.find("go to next" ) || string::npos != hals.find("go to last" )) {
        hals = replace(hals, "go to ", "");
        hals += (";");
    }
    if (string::npos != hals.find("for each " ) && (string::npos != hals.find(" from " ))) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, " :", ":");
        hals = replace(hals, ":", ">)) {");
        hals = replace(hals, "for each ", "while (chomp(");
        hals = replace(hals, " file ", " ");
        hals = replace(hals, " from ", " = <");
    }
    if (string::npos != hals.find("for each " ) && (string::npos != hals.find(" in " ))) {
        hals = replace(hals, ":", " do");
        hals = replace(hals, " do", ") {");
        hals = replace(hals, ") {) {", "::");
        hals = replace(hals, "for each ", "foreach ");
        hals = replace(hals, " file ", " ");
        hals = replace(hals, " in ", " (@");
    }
    if (string::npos != hals.find("for " )) {
        hals = replace(hals, " do", ":");
        hals = replace(hals, "for", "for (");
        hals = replace(hals, ":", ") {");
        hals = replace(hals, ") {) {", "::");
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
    return;
}

string perl5_convert_code(string halcode, int just_compile) {
    std::vector<std::string> lines;
    boost::split(lines, halcode, boost::is_any_of("\n\r"));

    string newcode;
    newcode += "use strict;\n";
    newcode += "local $| = 1;\n";
    newcode += "\n";

    int i;
    for (i = 0; i < lines.size(); ++i) {
        if (lines[i].size() <= 1) {
            continue;
        }

        perl5_convert_structure(lines[i], just_compile);

        newcode += lines[i] + "\n";
    }
    
    newcode += "\n1\n";

    return newcode;
}

int perl5_convert_file(string filename) {
    static string last_filename = "";
    if (last_filename == filename) {
        fprintf(output(), "compiler: abort, unnecessary!\n");
        return 0;
    }
    
    string code;
    std::ifstream code_i(filename.c_str());
    if (!code_i.is_open()) {
        fprintf(output(), "compiler: file not found: %s (at: P1, lang: perl5)\n", filename.c_str());
        return 1;
    }
    else {
        std::stringstream ss;
        ss << code_i.rdbuf();
        code = ss.str();
    }
    
    int just_compile = 0;
    /// Look whether nothing has changed
    {
        std::ifstream code_tmp_i((filename + ".tmp").c_str());
        if (!code_tmp_i.is_open()) {
            fprintf(output(), "\ncompiler: file not found: %s (at: P1, lang: perl5)\n", (filename + ".tmp").c_str());
        }
        else {
            std::stringstream ss;
            ss << code_tmp_i.rdbuf();
            string code_tmp = ss.str();
            
            if (code == code_tmp && -1 == filename.find("temp")) {
                just_compile = 1;
            }
        }
    }
    {
        std::ofstream code_tmp_o((filename + ".tmp").c_str());
        code_tmp_o << code;
        code_tmp_o.close();
    }

    fprintf(output(), "compiler: compile %s (size: %i bytes, lang: perl5)\n", filename.c_str(), code.size());
    string targetcode = perl5_convert_code(code, just_compile);
    if (!just_compile) {
        std::ofstream code_target_o((filename + ".pl").c_str());
        code_target_o << targetcode;
        code_target_o.close();
    }

    last_filename = filename;
    return 0;
}

void perl5_execute(string filename) {

    // COMPILE
    static string last_filename = "";
    if (last_filename == filename) {
        cout << "compiler: " << filename << ": do not compile, unnecessary!" << endl;
    }
    else {
        cout << "compiler: " << filename << ": compile." << endl;
        perl5_convert_file(filename);
    }

    // INIT
    fprintf(output(), "%s\n", "compiler: module init (lang: perl5)");

    // CONSTRUCT
    fprintf(output(), "%s\n", "compiler: module constructor (lang: perl5)");

    // RUN
    hal2009_send_signal_func = &perl5_hal2009_send_signal;

#if defined(MINGW) || defined(WIN32)
    string exec = "runtime-perl5-win32\\bin\\perl.exe";
#else
    string exec = bp::find_executable_in_path("perl");
#endif

    std::vector<std::string> args;
    string arg = string(filename) + ".pl";
    args.push_back(exec);
    args.push_back(arg);

    bp::context ctx;
    ctx.environment = bp::self::get_environment();
    ctx.stdout_behavior = bp::capture_stream();
    ctx.stderr_behavior = bp::redirect_stream_to_stdout();
    ctx.stdin_behavior = bp::capture_stream();
    bp::child c = bp::launch(exec, args, ctx);
    child_stdout = &c.get_stdout();
    child_stdin = &c.get_stdin();
    hal2009_send_signals();
    std::string line;

    vector<pthread_t> threads;
    while (std::getline(*child_stdout, line)) {
        if (line.find("IPC:") == 0) {
            cout << "get (hal->c): IPC:" << endl;
            std::string vfile;
            std::getline(*child_stdout, vfile);
            trim(vfile);
            cout << vfile << endl;
            
            std::string data;
            std::string data_line;
            while (data_line != ".") {
                data += data.size()?"\n":"";
                data += data_line;
                std::getline(*child_stdout, data_line);
                trim(data_line);
                cout << data_line << endl;
            }
            
            char* parameters[3] = {strdup(vfile.c_str()), strdup(data.c_str()), (char*)MULTI};
            pthread_t thread_no_1;
            pthread_create (&thread_no_1, NULL, hal2009_handle_signal, (void*)parameters);
            threads.push_back(thread_no_1);
            
            if (threads.size() > 20) {
                int k;
                for (k = 0; k < threads.size(); ++k) {
                    pthread_join(threads[k], NULL);
                }
                threads.clear();
            }
        }
        else {
            std::cout << line << std::endl;
        }
    }
    int k;
    for (k = 0; k < threads.size(); ++k) {
        pthread_join(threads[k], NULL);
    }
    threads.clear();

    bp::status s = c.wait();
    child_stdout = 0;
    child_stdin = 0;
    hal2009_send_signal_func = 0;

    // DECONSTRUCT
    fprintf(output(), "%s\n", "compiler: module destructor (lang: perl5)");
    fprintf(output(), "%s\n", "");

    last_filename = filename;
}

void perl5_hal2009_send_signal(string vfile, string data) {
    if (child_stdin) {
        cout << "send (c->hal): IPC:" << endl;
        cout << vfile << endl;
        cout << data << endl;
        cout << "." << endl;
        *child_stdin << "IPC:" << endl;
        *child_stdin << vfile << endl;
        *child_stdin << data << endl;
        *child_stdin << "." << endl;
    }
}


