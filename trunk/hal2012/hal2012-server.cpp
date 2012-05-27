/*
 * This file is part of FreeHAL 2012.
 *
 * Copyright(c) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Tobias Schulz and contributors.
 * http://www.freehal.org
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

#include "hal2012.h"
#include "hal2012-server.h"
#include "hal2012-util.h"
#include "hal2012-startup.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
using namespace boost::posix_time;
#include <boost/process.hpp>
namespace bp = ::boost::process;
#include <pthread.h>

#ifdef WINDOWS
#include <windows.h>
int win32_bind (SOCKET s, const struct sockaddr* addr, int);
namespace boost {
}
#endif

debugging_options* current_debugging_options;
extern instance* current_instance;

vector<tcp::iostream*> hal2012_server_clients;
vector<boost::thread*> hal2012_server_clients_threads;

void hal2012_server_start();
char* hal2012_server_get_value_from_socket(const char* s1, const char* s2);
int set_nonblocking(int);

boost::asio::io_service io_service;
tcp::acceptor* hal2012_server_acceptor;


void hal2012_server_start() {
    int ntry = 5;
    while (!hal2012_server_acceptor) {
        try {
            tcp::endpoint endpoint(tcp::v4(), current_instance->server_port);
            hal2012_server_acceptor = new tcp::acceptor(io_service, endpoint);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            halusleep(1000);
            if (strstr(e.what(), "already in use")) {
                --ntry;
                if (ntry <= 0) {
                    exit(0);
                }
            }
        }
    }
}

void stream_process_to_std(int num, bool& do_exit, bp::pistream* i, std::ostream* o, bp::postream* o_stdin) {
    time_facet *facet = new time_facet("%d-%m-%Y %H:%M:%S");
    o->imbue(locale(o->getloc(), facet));
    string line;
    string oldline;
    int times_oldline = 0;
    while (!do_exit && *i && *o) {
	if (getline(*i, line)) {
            if (current_debugging_options->g_gdb) {
                if (strstr(line.c_str(), "Program received signal")) {
                    (*o_stdin) << "bt" << endl;
                }
                if (strstr(line.c_str(), "Backtrace:")) {
                    (*o_stdin) << "bt" << endl;
                }
            }
            if (line == oldline) {
                ++times_oldline;
                if (times_oldline % 50 == 0) {
                    (*o) << "#" << num << " ";
                    (*o) << second_clock::local_time() << "  ";
                    (*o) << oldline << " (" << times_oldline << " times)" << endl;
                }
            }
            else {
                if (times_oldline > 0) {
                    (*o) << "#" << num << " ";
                    (*o) << second_clock::local_time() << "  ";
                    (*o) << oldline << " (" << times_oldline << " times)" << endl;
                    times_oldline = 0;
                }
                (*o) << "#" << num << " ";
                (*o) << second_clock::local_time() << "  ";
                (*o) << line << endl;
                oldline = line;
            }
        }
    }
}

bool lock_db = false;

bool is_locked(int num, string lock_request) {
    if (strstr(lock_request.c_str(), "LOCK:DB")) {
        cout << "#" << num << " ";
        cout << "is_locked: lock_db = " << lock_db << endl;
        return lock_db;
    }
}
bool set_locked(int num, string lock_request, bool l) {
    if (strstr(lock_request.c_str(), "LOCK:DB")) {
        cout << "#" << num << " ";
        cout << "set_locked: lock_db = " << l << endl;
        lock_db = l;
    }
    return l;
}

void stream_process_to_network(int num, bool& do_exit, bp::pistream* i, tcp::iostream* o, string& lock_request, string& unlock_request, bp::postream* o_lock) {
    string line;
    while (!do_exit && *i && *o && o->rdbuf()->is_open()) {
	if (!do_exit && getline(*i, line)) {

            if (do_exit) return;

            if (line.c_str() == strstr(line.c_str(), "LOCK:")) {
                lock_request = line;
                cout << "#" << num << " ";
                cout << "process requested a lock: " << lock_request << endl;
                while (is_locked(num, lock_request)) {
                    halusleep(250);
                }
                set_locked(num, lock_request, true);
                (*o_lock) << lock_request << endl;
                lock_request = "";
            }
            if (do_exit) return;
            if (line.c_str() == strstr(line.c_str(), "UNLOCK:")) {
                unlock_request = line;
                cout << "#" << num << " ";
                cout << "process released a lock: " << unlock_request << endl;
                set_locked(num, unlock_request, false);
                (*o_lock) << unlock_request << endl;
                unlock_request = "";
            }

            if (do_exit) return;

            cout << "#" << num << " ";
            cout << "-------- p -> n: " << line << endl;
            if (!o->rdbuf()->is_open()) { cout << "#" << num << " "; cout << "-------- stream closed (p -> n)" << endl; return; }
            (*o) << line << endl;
        }
    }
}

void stream_network_to_process(int num, bool& do_exit, tcp::iostream* i, bp::postream* o, string& lock_request, string& unlock_request) {
    string line;
    while (!do_exit && *i && *o) {
        if (lock_request.size() > 0) {
            while (is_locked(num, lock_request)) {
               if (!i->rdbuf()->is_open()) { cout << "#" << num << " "; cout << "-------- stream closed (n -> p)" << endl; return; }
                halusleep(100);
            }
        }
        if (!i->rdbuf()->is_open()) { cout << "#" << num << " "; cout << "-------- stream closed (n -> p)" << endl; return; }
	if (getline(*i, line) && !strstr(line.c_str(), "HOLD:") && line.size() > 0) {
            if (line.c_str() == strstr(line.c_str(), "EXIT:")) {
                do_exit = true;
            }
            cout << "#" << num << " ";
            cout << "-------- n -> p: " << line << endl;
            (*o) << line << endl;
        }
    }
}


/*void stream_process_to_network(bp::pistream* i, tcp::iostream* o, int* lock_i, int* lock_o) {
    string line;
    while (1) {
        while (*lock_i) {} *lock_i = 1;
        while (getline(*i, line)) {
            (*i) >> line;

        }
        *lock_i = 0;
    }
}

void stream_network_to_process(tcp::iostream* i, bp::postream* o, int* lock_i, int* lock_o) {
    string line;
    while (1) {
        while (*lock_i) {} *lock_i = 1;
        while (getline(*i, line)) {
            (*i) >> line;
        }
        *lock_i = 0;
    }
}*/


void hal2012_server_client_connection(tcp::iostream* stream, int num) {
    ofstream booted_file("booted");
    booted_file.close();

#if defined(MINGW) || defined(WIN32)
    string exec_fh = "runner-thread.exe";
#else
    string exec_fh = "./hal2012-server-thread";
#endif

    std::vector<std::string> args;


    string exec;
#if defined(MINGW) || defined(WIN32)
    exec = exec_fh;
    args.push_back(exec);
#else
    if (current_debugging_options->g_valgrind) {
        exec = "/usr/bin/valgrind";
        args.push_back(exec);
        if (current_debugging_options->g_valgrind_tool.size() > 0) {
            args.push_back(string("--tool=")+current_debugging_options->g_valgrind_tool);
        }
        else {
            args.push_back("--leak-check=full");
        }
        args.push_back(exec_fh);
    }
    else if (current_debugging_options->g_gdb) {
        exec = "/usr/bin/gdb";
        args.push_back(exec);
        args.push_back(exec_fh);
    }
    else {
        exec = exec_fh;
        args.push_back(exec);
    }
#endif

    bp::context ctx;
    bp::postream* child_stdin = 0;
    bp::pistream* child_stdout = 0;
    bp::pistream* child_stderr = 0;

    ctx.environment = bp::self::get_environment();
    ctx.stdout_behavior = bp::capture_stream();
    ctx.stderr_behavior = bp::capture_stream();
//    ctx.stderr_behavior = bp::redirect_stream_to_stdout();
    ctx.stdin_behavior = bp::capture_stream();
    bp::child c = bp::launch(exec, args, ctx);
    child_stdout = &c.get_stdout();
    child_stderr = &c.get_stderr();
    child_stdin = &c.get_stdin();

    if (current_debugging_options->g_gdb) {
        (*child_stdin) << "run" << endl;
    }

    (*child_stdin) << "--language" << endl << current_instance->tlanguage << endl;
    (*child_stdin) << "--sql" << endl << current_instance->sql_engine << endl;
    (*child_stdin) << "--interpreter" << endl << current_instance->planguage << endl;
    (*child_stdin) << "." << endl;

    (*child_stdin) << "init_thread_ended=" << (num>1?1:0) << endl;
    cout << "#" << num << ": " << "init_thread_ended=" << (num>1?1:0) << endl;

    string lock_request = "";
    string unlock_request = "";
    bool do_exit = false;

    boost::thread t1(boost::bind(&stream_process_to_std, num, do_exit, child_stdout, &cout, child_stdin));
    boost::thread t2(boost::bind(&stream_process_to_network, num, do_exit, child_stderr, stream, lock_request, unlock_request, child_stdin));
    boost::thread t3(boost::bind(&stream_network_to_process, num, do_exit, stream, child_stdin, lock_request, unlock_request));

/*    while (*stream && !do_exit) {
        halusleep(100);
    }*/

    t1.join();
    t2.join();
    t3.join();

    bp::status s = c.wait();
    boost::thread::id this_id = boost::this_thread::get_id();

    {
        vector<tcp::iostream*>::iterator iter = hal2012_server_clients.begin();
        for(; iter != hal2012_server_clients.end();) {
            if (*iter == stream) {
                delete(*iter);
                iter = hal2012_server_clients.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }

    {
        vector<boost::thread*>::iterator iter = hal2012_server_clients_threads.begin();
        for(; iter != hal2012_server_clients_threads.end();) {
            if ((*iter)->get_id() == this_id) {
                delete(*iter);
                iter = hal2012_server_clients_threads.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }
}

void hal2012_server_run() {
    ofstream booted_file("booted");
    booted_file.close();

    for (;;) {
        tcp::iostream* stream;
        try {
            stream = new tcp::iostream();
        }
        catch (std::exception& e)
        {
            std::cerr << "1: " << e.what() << std::endl;
            if ( stream ) delete stream;
            continue;
        }
        try {
            if (!hal2012_server_acceptor) {
                hal2012_server_start();
            }
            if (!hal2012_server_acceptor) {
                delete stream;
                continue;
            }
            hal2012_server_acceptor->accept(*stream->rdbuf());
        }
        catch (std::exception& e)
        {
            std::cerr << "2: " << e.what() << std::endl;
            if ( stream ) delete stream;
            continue;
        }
        try {
            hal2012_server_clients.push_back(stream);


            static int num = 0;
            ++num;

            hal2012_server_clients_threads.push_back(new boost::thread(boost::bind(&hal2012_server_client_connection, stream, num)));
            halusleep(2000);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void hal2012_server_stop() {
    if ( hal2012_server_acceptor ) delete hal2012_server_acceptor;
    for (vector<tcp::iostream*>::iterator iter = hal2012_server_clients.begin(); iter != hal2012_server_clients.end(); iter++) {
        delete(*iter);
    }
    for (vector<boost::thread*>::iterator iter = hal2012_server_clients_threads.begin(); iter != hal2012_server_clients_threads.end(); iter++) {
        delete(*iter);
    }
    hal2012_server_clients = vector<tcp::iostream*>();
    hal2012_server_clients_threads = vector<boost::thread*>();
}

char* hal2012_server_get_value_from_socket(const char* s1, const char* s2) {
    cout << "Get stream." << endl;
    while (0 == hal2012_server_clients.size()) {
        halusleep(1000);
    }
    tcp::iostream* stream = hal2012_server_clients[hal2012_server_clients.size()-1];

    cout << "Print into stream." << endl;
    (*stream) << "GET_" << s1 << ":" << s2 << endl;

    cout << "Wait for stream." << endl;
    std::vector<std::string>* result;
    string line;
    while (*stream && stream->rdbuf() != 0 && getline(*stream, line)) {
        if ( line.size() != 0 ) {
            cout << line << endl;
            result = simple_split(line, ":" );
            if ( result->at(0) == string("HERE_IS_") + s1 && result->size() >= 2 ) {
                break;
            }
        }
    }
    cout << "End of waiting for stream." << endl;
    int i;
    char* value = (char*)calloc(1000, 1);
    strcpy(value, "");
    for (i = 1; i < result->size(); ++i) {
        strncat(value, result->at(i).c_str(), 999);
        if (i+1 < result->size()) {
            strncat(value, ":", 999);
        }
    }
    return value;
}

int set_nonblocking(int fd)
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined (__MINGW) || defined(__MINGW32__)
#else
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
#endif
}

int main(int argc, char** argv) {
    current_instance = new instance();
    current_instance->sql_engine = "disk";
    current_instance->tlanguage = "de";
    current_instance->planguage = "perl5";
    current_debugging_options = new debugging_options();

    vector<string>* params = params_argv(argc, argv);

    for (int i = 0; i < params->size(); i += 2) {
        if (i+1 == params->size()) {
            fprintf(output(), "No value: %s.\n", params->at(i).c_str());
        }
        const string key = params->at(i);
        const string value = params->at(i+1);

        if (key == "--language") {
            current_instance->tlanguage = value;
        }
        if (key == "--sql") {
            current_instance->sql_engine = value;
        }
        if (key == "--interpreter") {
            current_instance->planguage = value;
        }
        if (key == "--valgrind") {
            if (value == "false" || value == "off" || value == "0") {
                current_debugging_options->g_valgrind = 0;
            }
            if (value != "true" && value != "on" && value.size() > 1) {
                current_debugging_options->g_valgrind = 1;
                current_debugging_options->g_valgrind_tool = value;
            }
            else {
                current_debugging_options->g_valgrind = 1;
                current_debugging_options->g_valgrind_tool = string("");
            }
        }
        if (key == "--gdb") {
            if (value == "false" || value == "off" || value == "0") {
                current_debugging_options->g_gdb = 0;
            }
            else {
                current_debugging_options->g_gdb = 1;
            }
        }
    }

    fprintf(output(), "Language is %s.\n", current_instance->tlanguage.c_str());
    fprintf(output(), "Interpreter is %s.\n", current_instance->planguage.c_str());
    fprintf(output(), "Database Tool is %s.\n", current_instance->sql_engine.c_str());
    current_instance->base_dir = ".";

    hal2012_server_start();

    hal2012_server_run();

    hal2012_server_stop();
}
