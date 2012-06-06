#ifdef _WIN32
#include "boinc/boinc_win.h"
#else
#include "boinc/str_util.h"
#endif
#include <cstdio>
#include <cctype>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <unistd.h>

#include "hal2012-grammar2012.h"
#include "hal2012-tagger2012.h"
#include "hal2012-parser2012.h"
#include "hal2012-util2012.h"

#include "boinc/boinc_api.h"
#include "boinc/util.h"
#include "boinc/filesys.h"

using std::string;

#define CHECKPOINT_FILE "state"
#define INPUT_FILENAME "in"
#define OUTPUT_FILENAME "out"

bool run_slow = false;

// do a billion floating-point ops
// (note: I needed to add an arg to this;
// otherwise the MS C++ compiler optimizes away
// all but the first call to it!)
//
static double do_a_giga_flop(int foo) {
    double x = 3.14159*foo;
    int i;
    for (i=0; i<500000000; i++) {
        x += 5.12313123;
        x *= 0.5398394834;
    }
    return x;
}

int do_checkpoint(MFILE& mf, int nchars) {
    int retval;
    string resolved_name;

    FILE* f = fopen("temp", "w");
    if (!f) return 1;
    fprintf(f, "%d", nchars);
    fclose(f);

    retval = mf.flush();
    if (retval) return retval;
    boinc_resolve_filename_s(CHECKPOINT_FILE, resolved_name);
    retval = boinc_rename("temp", resolved_name.c_str());
    if (retval) return retval;

    return 0;
}

int boinc_parse(string line, int* n_sentence, ofstream& out) {
    grammar2012::tagger* _t = new grammar2012::tagger();
    _t->set_verbose(true);
    _t->read_pos_file("brain.pos");
    _t->read_pos_file("memory.pos");
    _t->read_regex_pos_file("regex.pos");

    grammar2012::grammar* _g = new grammar2012::grammar();
    _g->read_grammar("grammar.txt");
    _g->set_verbose(true);
    _g->expand();

    grammar2012::parser* p = new grammar2012::parser();
    p->set_lang("de");
    p->set_path(".");
    p->set_tagger(_t);
    p->set_grammar(_g);
    p->set_verbose(true);

    p->parse(line);

    const vector<grammar2012::sentence*>& vs = p->get_sentences();
    int n_clause = 0;
    foreach (grammar2012::sentence* s, vs) {
        out << "---- " << ++(*n_sentence) << " " << ++n_clause << endl;
        out << grammar2012::grammar::print_graph(s->get_parsed()) << endl;
        out << "---- " << endl;
    }
}

int main(int argc, char **argv) {
    int i;
    int c, retval, n;
    double fsize, fd;
    char input_path[512], output_path[512], chkpt_path[512], buf[256];
    FILE* state;
    int n_sentence = 0;

    retval = boinc_init();
    if (retval) {
        fprintf(stderr, "%s boinc_init returned %d\n",
            boinc_msg_prefix(), retval
        );
        exit(retval);
    }

    // open the input file (resolve logical name first)
    //
    boinc_resolve_filename(INPUT_FILENAME, input_path, sizeof(input_path));
    ifstream infile(input_path);
    if (!infile.is_open()) {
        fprintf(stderr,
            "%s Couldn't find input file, resolved name %s.\n",
            boinc_msg_prefix(), input_path
        );
        exit(-1);
    }

    // get size of input file (used to compute fraction done)
    //
    //file_size(input_path, fsize);

    boinc_resolve_filename(OUTPUT_FILENAME, output_path, sizeof(output_path));

    boinc_resolve_filename(CHECKPOINT_FILE, chkpt_path, sizeof(chkpt_path));
    state = boinc_fopen(chkpt_path, "r");
    if (state) {
        n = fscanf(state, "%d", &n_sentence);
        fclose(state);
    }
    ofstream out;
    if (state && n==1) {
        out.open(output_path, std::ios::app);
    } else {
        out.open(output_path);
    }

    string line;
    while (std::getline(infile, line)) {
        boinc_parse(line, &n_sentence, out);
    }

    out.close();

    // See if there's a valid checkpoint file.
    // If so seek input file and truncate output file
    //
/*
    if (retval) {
        fprintf(stderr, "%s APP: upper_case output open failed:\n",
            boinc_msg_prefix
        );
        fprintf(stderr, "%s resolved name %s, retval %d\n",
            boinc_msg_prefix, output_path, retval
        );
        perror("open");
        exit(1);
    }
*/

    // main loop - read characters, convert to UC, write
    //
/*
    for (i=0; ; i++) {
        c = fgetc(infile);

        if (c == EOF) break;
        c = toupper(c);
        out._putchar(c);
        nchars++;
        if (run_slow) {
            boinc_sleep(1.);
        }

        if (early_exit && i>30) {
            exit(-10);
        }

        if (early_crash && i>30) {
            boinc_crash();
        }
        if (early_sleep && i>30) {
            g_sleep = true;
            while (1) boinc_sleep(1);
        }

        if (boinc_time_to_checkpoint()) {
            retval = do_checkpoint(out, nchars);
            if (retval) {
                fprintf(stderr, "%s APP: upper_case checkpoint failed %d\n",
                    boinc_msg_prefix, retval
                );
                exit(retval);
            }
            boinc_checkpoint_completed();
        }

        fd = nchars/fsize;
        if (cpu_time) fd /= 2;
        boinc_fraction_done(fd);
    }

    retval = out.flush();
    if (retval) {
        fprintf(stderr, "%s APP: upper_case flush failed %d\n",
            boinc_msg_prefix, retval
        );
        exit(1);
    }

    // burn up some CPU time if needed
    //
    if (cpu_time) {
        double start = dtime();
        for (i=0; ; i++) {
            double e = dtime()-start;
            if (e > cpu_time) break;
            fd = .5 + .5*(e/cpu_time);
            boinc_fraction_done(fd);

            if (boinc_time_to_checkpoint()) {
                retval = do_checkpoint(out, nchars);
                if (retval) {
                    fprintf(stderr, "%s APP: upper_case checkpoint failed %d\n",
                        boinc_msg_prefix, retval
                    );
                    exit(1);
                }
                boinc_checkpoint_completed();
            }
            comp_result = do_a_giga_flop(i);
        }
    }
*/

    boinc_fraction_done(1);
    boinc_finish(0);
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR Args, int WinMode) {
    LPSTR command_line;
    char* argv[100];
    int argc;

    argc = 0;
    return main(argc, argv);
}
#endif

void grammar2012::tagger::ask_user(const string word, grammar2012::tags* tags) {
}

EXTERN_C char* check_config(const char* name, const char* _default) {
    return strdup("1");
}
