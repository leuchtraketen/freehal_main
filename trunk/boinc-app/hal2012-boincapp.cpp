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

#include "hal2012-revision.h"

using std::string;

#define CHECKPOINT_FILE "state"
#define INPUT_FILENAME "in"
#define OUTPUT_FILENAME "out"

bool run_slow = false;

grammar2012::tagger* t = 0;
grammar2012::grammar* g = 0;

bool fexists(const char *filename)
{
  ifstream ifile(filename);
  return ifile;
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

grammar2012::parser* setup_parser() {

    string path = "../../projects/www.freehal.net_freehal_at_home/";

    if (t == 0) {
        t = new grammar2012::tagger();
        t->set_verbose(true);
        t->read_pos_file((path+"brain.pos").c_str());
        t->read_pos_file((path+"memory.pos").c_str());
        t->read_regex_pos_file((path+"regex.pos").c_str());
    }

    if (g == 0) {
        g = new grammar2012::grammar();
        g->read_grammar((path+"grammar.txt").c_str());
        g->set_verbose(false);
        g->expand();
    }

    grammar2012::parser* p = 0;
    if (p == 0) {
        p = new grammar2012::parser();
        p->set_lang("de");
        p->set_path(".");
        p->set_tagger(t);
        p->set_grammar(g);
        p->set_verbose(false);
    }

    return p;
}

int boinc_parse(string line, int* n_sentence, ofstream& out) {

    grammar2012::parser* p = setup_parser();
    p->parse(line);

    ++(*n_sentence);
    const vector<grammar2012::sentence*>& vs = p->get_sentences();
    int n_clause = 0;
    foreach (grammar2012::sentence* s, vs) {
        out << "---- " << REVISION_T << ";" << (*n_sentence) << ";" << ++n_clause << endl;
        out << "::input= " << line << endl;
        out << grammar2012::grammar::print_graph(s->get_parsed()) << endl;
        out << "---- " << endl;
    }

    delete p;
    delete t;
    t = 0;
    delete g;
    g = 0;
}

bool find_file( const fs::path & dir_path,         // in this directory,
                const std::string & file_name, // search for this name,
                fs::path & path_found,            // placing path here if found
                const std::string & only_if_dir_contains) // only if the filename contains
{
  if ( !exists( dir_path ) ) return false;
  fs::directory_iterator end_itr; // default construction yields past-the-end
  for ( fs::directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr )
  {
    if ( fs::is_directory(itr->status()) && (only_if_dir_contains.empty() || algo::contains(grammar2012::lc(itr->path().leaf().string()), only_if_dir_contains)) )
    {
      if ( find_file( itr->path(), file_name, path_found, "" ) ) return true;
    }
    else if ( itr->path().leaf() == file_name ) // see below
    {
      path_found = itr->path();
      return true;
    }
  }
  return false;
}

int main(int argc, char **argv) {

    std::cout.rdbuf(std::cerr.rdbuf());

    int i;
    int c, retval, n;
    double fsize, fd;
    char input_path[512], output_path[512], chkpt_path[512], buf[256];
    FILE* state;
    int n_sentence = 0;

    retval = boinc_init();
    if (retval) {
        fprintf(stderr, "freehalapp: boinc_init returned %d\n", retval
        );
        exit(retval);
    }

    // open the input file (resolve logical name first)
    //
    boinc_resolve_filename(INPUT_FILENAME, input_path, sizeof(input_path));
    ifstream infile(input_path);
    if (!infile.is_open()) {
        cout <<
            "freehalapp: Couldn't find input file, resolved name "
            << input_path << "." << endl;
        exit(-1);
    }
    int lines = 0;
    {
        string line;
        while (std::getline(infile, line)) {
            ++lines;
        }
    }
    infile.close();
    infile.open(input_path);
    cout << "lines: " << lines << endl;

    fs::path java_exe;
    fs::path dir;
    string only_if_dir_contains;
    bool found;

    dir = "/";
    only_if_dir_contains = "prog";
    found = find_file(fs::path(dir), "java.exe", java_exe, only_if_dir_contains);
    cout << "java found in "<<dir<<"? " << (found?"yes, here: " + java_exe.string() : "no") << endl;


    run_slow = fexists("SLOW");
    cout << "run slow? " << (run_slow?"yes":"no") << endl;

    // get size of input file (used to compute fraction done)
    //
    //file_size(input_path, fsize);

    boinc_resolve_filename(OUTPUT_FILENAME, output_path, sizeof(output_path));

    boinc_resolve_filename(CHECKPOINT_FILE, chkpt_path, sizeof(chkpt_path));
    state = boinc_fopen(chkpt_path, "r");
    if (state) {
        n = fscanf(state, "%d", &n_sentence);
        fclose(state);
	stringstream ss;
	ss << n_sentence << "/" << lines;
	cout << "checkpoint state? yes, progress=" << ss.str() << endl;
    }
    else {
        cout << "checkpoint state? none" << endl;
    }

    ofstream out;
    if (state && n==1) {
        out.open(output_path, std::ios::app);
    } else {
        out.open(output_path);
    }

    int checkpoint_skip = n_sentence;
    while (checkpoint_skip > 0) {
        string line;
        std::getline(infile, line);
        --checkpoint_skip;
    }

    string line;
    int l = 0;
    while (std::getline(infile, line)) {
        ++l;
        boinc_parse(line, &n_sentence, out);

        boinc_fraction_done((double)l/(double)lines);
        if (run_slow) boinc_sleep(run_slow?90:1);

        ofstream chk;
        chk.open(chkpt_path);
        if (chk.is_open()) {
            chk << n_sentence;
            chk.close();
        }
    }

    for (int h = 0; h < 60; ++h) {
        out << "second " << h << endl;
        boinc_sleep(1);
    }

    out.close();

    delete t;
    delete g;

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
