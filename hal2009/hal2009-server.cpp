// C++ headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// Boost headers
#undef BOOST_ASIO_HAS_EPOLL
#undef BOOST_ASIO_HAS_KQUEUE
#undef BOOST_ASIO_HAS_DEV_POLL
#define BOOST_ASIO_DISABLE_EPOLL 1
#ifndef NO_ASIO_HERE
#include <boost/asio.hpp>
#endif
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#undef BOOST_ASIO_HAS_EPOLL
#undef BOOST_ASIO_HAS_KQUEUE
#undef BOOST_ASIO_HAS_DEV_POLL

#define NOT_HEADER_ONLY 1
#define DONT_DECLARE_STD 1
#define USE_CXX 1
#include "hal2009.h"
#include "hal2009-download.cpp"

#include <pthread.h>

using namespace std;
using namespace boost;
using namespace algorithm;
using boost::asio::ip::tcp;

unsigned short server_port = 5173;
unsigned short netcom_port = 5172;
vector<tcp::iostream*> hal2009_server_clients;

char* copy_for_init_planguage = 0;
char* copy_for_init_signal_handler_tlanguage = 0;
char* copy_for_init_base_dir = 0;
int init_thread_ended = 0;

#ifndef NO_PERL
#ifdef WINDOWS
#include <windows.h>
int win32_bind (SOCKET s, const struct sockaddr* addr, int);
namespace boost {
    #include <sys/socket.h>
}
#endif
#endif

extern "C" int cstat (char* c, struct stat* s);
extern "C" void extract();

void hal2009_server_start();
char* hal2009_server_get_value_from_socket(char* s1, const char* s2);
int set_nonblocking(int);
void* hal2009_init_thread (void*);

boost::asio::io_service io_service;
tcp::acceptor* hal2009_server_acceptor;
tcp::acceptor* hal2009_netcom_acceptor;
char* signal_handler_tlanguage;
char* base_dir;

std::vector<std::string>* simple_split (const string text, const string search) {
    std::vector<std::string>* result = new std::vector<std::string>();
    boost::algorithm::split(*result, text, is_any_of(search.c_str()));
    return result;
}

int locked_netcom = 0;
void hal2009_netcom_lock() {
    while (locked_netcom) {
        usleep(50);
        --locked_netcom;
    }
    locked_netcom = 15*1000/50;
}
void hal2009_netcom_unlock() {
    locked_netcom = false;
}

/// FreeHAL Network communication (NETCOM)

void hal2009_netcom_start() {
    try {
        
        tcp::endpoint endpoint(tcp::v4(), netcom_port);
        hal2009_netcom_acceptor = new tcp::acceptor(io_service, endpoint);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
#if defined(__linux__)
void hal2009_netcom_get_ips(char** ips, short* num_of_ips) {
    fprintf(output(), "Searching IP addresses...\n");
    
    /**
    Comment this out to test the functionality of FreeHAL NETCOM on the local subnet (127.0.0.1)
    
    {
        ips[*num_of_ips] = strdup("127.0.0.1");
        ++(*num_of_ips);
    }
    **/

    ifstream arp_file("/proc/net/arp");
    string buffer;
    halgetline(arp_file, buffer);
    while (arp_file) {
        halgetline(arp_file, buffer);
        if (0 == buffer.size()) {
            break;
        }
        
        if ((*num_of_ips) < 10) {
            ips[*num_of_ips] = (char*)(malloc(buffer.size()));
            for (int z = 0; z < buffer.size(); ++z) {
                if (!(buffer[z] == '0' || buffer[z] == '1' || buffer[z] == '2' || buffer[z] == '3' || buffer[z] == '4' || buffer[z] == '5' || buffer[z] == '6' || buffer[z] == '7' || buffer[z] == '8' || buffer[z] == '9' || buffer[z] == '.')) {
                    break;
                }

                ips[*num_of_ips][z] = buffer[z];
                ips[*num_of_ips][z+1] = 0;
            }

            fprintf(output(), "IP: %s\n", ips[*num_of_ips]);
            
            ++(*num_of_ips);
        }
    }
    
    fprintf(output(), "Searched IP addresses.\n");
    
    arp_file.close();
}

#define PTHREAD_IS_NOT_NULL(x) (x)
#define PTHREAD_SET_NULL(x) ((x)=0)

#else

#define NUM_ELEMENTS(x)  (sizeof((x)) / sizeof((x)[0])) // Für IP auslesen

void hal2009_netcom_get_ips(char** ips, short* num_of_ips) {
    fprintf(output(), "Searching IP addresses...\n");
    
    /**
    Comment this out to test the functionality of FreeHAL NETCOM on the local subnet (127.0.0.1)
    
    {
        ips[*num_of_ips] = strdup("127.0.0.1");
        ++(*num_of_ips);
    }
    **/

    struct    hostent* h;
    WSADATA   wsaData;
    UCHAR     ucAddress[4];
    CHAR      szHostName[MAX_PATH];
    int       x;  

    WSAStartup(MAKEWORD(1, 1), &wsaData);
    if(SOCKET_ERROR != gethostname(szHostName, NUM_ELEMENTS(szHostName))) {
        if(NULL != (h = gethostbyname(szHostName))) {
            for(x = 0; (h->h_addr_list[x]); x++) {
                ucAddress[0] = h->h_addr_list[x][0];
                ucAddress[1] = h->h_addr_list[x][1];
                ucAddress[2] = h->h_addr_list[x][2];
                ucAddress[3] = h->h_addr_list[x][3];

                ips[*num_of_ips] = (char*)(malloc(255));
                sprintf(ips[*num_of_ips], "%d.%d.%d.%d", ucAddress[0], ucAddress[1], ucAddress[2], ucAddress[3]);
                fprintf(output(), "IP: %s\n", ips[*num_of_ips]);
                ++(*num_of_ips);
            }
        }
    }
    WSACleanup();
    
    fprintf(output(), "Searched IP addresses.\n");
}

#define PTHREAD_IS_NOT_NULL(x) (x.p)
#define PTHREAD_SET_NULL(x) (1)

#endif

static bool i_am_giving = false;

void* hal2009_netcom_do_accept(void* arg) {
    fprintf(output(), "(hal2009_netcom_do_accept) Started NETCOM 'do accept' thread.\n");
    try {
        tcp::iostream* stream = (tcp::iostream*) arg;
        
        char* filename = (char*) malloc(200+strlen(signal_handler_tlanguage)+strlen(base_dir));
        strcpy(filename, "");
        strcat(filename, base_dir);
        strcat(filename, "/lang_");
        strcat(filename, signal_handler_tlanguage);
        strcat(filename, "/from-");
        strcat(filename, stream->rdbuf()->remote_endpoint().address().to_string().c_str());
        strcat(filename, ".pro");
        
        fprintf(output(), "(hal2009_netcom_do_accept) Open pro file %s. I will write into it whatever %s says (stream is %d).\n", filename, stream->rdbuf()->remote_endpoint().address().to_string().c_str(),  stream);
        ofstream pro_file(filename, ios::out | ios::app);
        string buffer;
        while (stream && *stream) {
            halgetline(*stream, buffer);
            if (buffer[0] == 'E' && buffer[1] == 'X' && buffer[2] == 'I' && buffer[3] == 'T') {
                break;
            }
            pro_file << buffer << "\n";
        }
        fprintf(output(), "(hal2009_netcom_do_accept) Close pro file %s...\n", filename);
        pro_file.close();
        fprintf(output(), "(hal2009_netcom_do_accept) Close socket for pro file %s...\n", filename);
        stream->close();
        free(filename);
        usleep(1500);
        if ( stream ) delete stream;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    fprintf(output(), "(hal2009_netcom_do_accept) Closed everything.\n");
}
void* hal2009_netcom_do_give(void* arg) {
    fprintf(output(), "(hal2009_netcom_do_give)   Started NETCOM 'do give' thread.\n");
    try {
        tcp::iostream* stream = (tcp::iostream*) arg;
        
        char* filename = (char*) malloc(200+strlen(signal_handler_tlanguage)+strlen(base_dir));
        strcpy(filename, "");
        strcat(filename, base_dir);
        strcat(filename, "/lang_");
        strcat(filename, signal_handler_tlanguage);
        strcat(filename, "/facts.pro");
        fprintf(output(), "(hal2009_netcom_do_give)   Giving file %s.\n", filename);
        
        fprintf(output(), "(hal2009_netcom_do_give)   I will send it to %s (stream is %d).\n", filename, stream->rdbuf()->remote_endpoint().address().to_string().c_str(),  stream);
        ifstream pro_file(filename);
        string buffer;
        hal2009_netcom_lock();
        while (stream && *stream && pro_file) {
            halgetline(pro_file, buffer);
            if (0 == buffer.size()) {
                break;
            }
            (*stream) << buffer << "\n";
        }
        (*stream) << "EXIT" << "\n";
        hal2009_netcom_unlock();
        fprintf(output(), "(hal2009_netcom_do_give)   Close pro file %s...\n", filename);
        pro_file.close();
        fprintf(output(), "(hal2009_netcom_do_give)   Close socket for pro file %s...\n", filename);
        stream->close();
        free(filename);
        usleep(2000);
        /*
        it is in the stack!!
        
        if ( stream ) {
            delete stream;
            stream = 0;
        }
        */
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    fprintf(output(), "(hal2009_netcom_do_give)   Closed everything.\n");
    
    i_am_giving = false;
}
void* hal2009_netcom_ip2net(int* sn1, int* sn2, int* sn3, const char* ip) {
    for (int k = 0, g = 1; k < strlen(ip); ++k) {
        if (ip[k] == '.') {
            ++g;
            continue;
        }
        
        if (g == 1)
            (*sn1) = ((*sn1)*10)+(ip[k]-48);
        if (g == 2)
            (*sn2) = ((*sn2)*10)+(ip[k]-48);
        if (g == 3)
            (*sn3) = ((*sn3)*10)+(ip[k]-48);
    }
}
void* hal2009_netcom_run_accept(void*) {
    fprintf(output(), "Started NETCOM 'accept' thread.\n");
    
    for (;;) {
        tcp::iostream* stream;
        try {
            stream = new tcp::iostream();
        }
        catch (std::exception& e)
        {
            std::cerr << "(netcom) 1: " << e.what() << std::endl;
            if ( stream ) delete stream;
            continue;
        }
        try {
            if (!hal2009_server_acceptor) {
                hal2009_server_start();
            }
            if (!hal2009_netcom_acceptor) {
                if ( stream ) delete stream;
                continue;
            }
            hal2009_netcom_acceptor->accept(*stream->rdbuf());
            fprintf(output(), "I got a request.\n");
        }
        catch (std::exception& e)
        {
            std::cerr << "(netcom) 2: " << e.what() << std::endl;
            if ( stream ) delete stream;
            continue;
        }
        try {
            usleep(1000);
            if (i_am_giving == false) {
                pthread_t t;
                pthread_create (&t, NULL, hal2009_netcom_do_accept, (void*)stream);
            }
            else {
                if ( stream ) delete stream;
            }
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}
int num_of_probethreads = 0;
void* hal2009_netcom_run_give_probethread(void* arg) {
    ++num_of_probethreads;
    
    char* remote_ip = (char*)arg;
    
    fprintf(output(), "Probing Remote IP: %s\n", remote_ip);

    tcp::iostream stream;
    try {    
//        stream = new tcp::iostream();
        tcp::endpoint endp(asio::ip::address_v4::from_string(remote_ip), (unsigned short)(netcom_port));
        
        stream.close();
        bool success = 0;
        
        if (stream.rdbuf()->connect(endp)) {
            i_am_giving = true;
            fprintf(output(), "Successful Probe!\nFound a valid FreeHAL on server %s.\n", remote_ip);

            //pthread_t t;
            //pthread_create (&t, NULL, hal2009_netcom_do_give, (void*)stream);
            hal2009_netcom_do_give((void*)(&stream));
        }
    }
    catch (boost::system::system_error e) {
        // ignore errors
    }
    free((void*)remote_ip);
    
    --num_of_probethreads;
    
    pthread_exit(0);
}
void* hal2009_netcom_run_give(void*) {
    fprintf(output(), "Started NETCOM 'give' thread.\n");
    return 0;
    while (1) {

        char* ips[11];
        short num_of_ips = 0;
        hal2009_netcom_get_ips(ips, &num_of_ips);
        
        for (int i = 0; i < 10 && i < num_of_ips; ++i) {
            
            fprintf(output(), "One of my IPs is %s.\n", ips[i]);
            
            int subnet_1 = 0;
            int subnet_2 = 0;
            int subnet_3 = 0;
            hal2009_netcom_ip2net(&subnet_1, &subnet_2, &subnet_3, ips[i]);
            fprintf(output(), "Subnet: %d.%d.%d.*\n", subnet_1, subnet_2, subnet_3);
            
            for (int subnet_4 = 1; subnet_4 < 254;) {
                pthread_t threadlist[12];
                int subnet_4_max = subnet_4 + 5;
            
                for (; subnet_4 < subnet_4_max && subnet_4 < 254; ++subnet_4) {
                    char* remote_ip = (char*)cxxhalmalloc(255, "hal2009_netcom_run_give");
                    snprintf(remote_ip, 254, "%d.%d.%d.%d", subnet_1, subnet_2, subnet_3, subnet_4);

                    pthread_t t;
                    pthread_create (&t, NULL, hal2009_netcom_run_give_probethread, (void*)(remote_ip));
                    
                    int s = 0;
                    while (s < 11 && PTHREAD_IS_NOT_NULL(threadlist[s])) {
                        ++s;
                    }
                    threadlist[s] = t;
                }

                int timeout = 30;
                while (num_of_probethreads != 0 && timeout >= 0) {
                    usleep(100);
                    timeout -= 0.1;
                }
                
                int e = 0;
                while (e < 11 && PTHREAD_IS_NOT_NULL(threadlist[e])) {
                    //pthread_detach(threadlist[e]);
                    // we use  pthread_exit()  in the threads' function now
                    PTHREAD_SET_NULL(threadlist[e]);
                    ++e;
                }
            }
            
            free(ips[i]);
        }
    }
}
void* hal2009_netcom_run(void* arg) {
    fprintf(output(), "Started NETCOM 'master' thread.\n");
    pthread_t t1;
    pthread_create (&t1, NULL, hal2009_netcom_run_accept, arg);
    pthread_t t2;
    pthread_create (&t2, NULL, hal2009_netcom_run_give, arg);
}

/// Normal FreeHAL communication with GUIs

void hal2009_server_start() {
    while (!hal2009_server_acceptor) {
        try {
            
            tcp::endpoint endpoint(tcp::v4(), server_port);
            hal2009_server_acceptor = new tcp::acceptor(io_service, endpoint);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void hal2009_server_statement(tcp::iostream* stream, const string s, string& username, char* language) {
    cout << "Begin of statement process." << endl;
    static string* answer;
    if (!answer) {
        answer = new string;
        time_t t;
        time(&t);
        char* time = ctime(&t);
        time[strlen(time)-1] = 0;
        (*answer) += "<i>" + string(time) + "</i><br /><br/>";
    }
    cout << "Initialize variables." << endl;

    if (s == "/del fact" || s == "/del fakt" || s == "/DEL FAKT" || s == "/DEL FACT" || s == "/df" || s == "/DF" || s == "/d f" || s == "/D F") {
        struct RECORD r;
        strcpy(r.pkey, "a");
        printf("pkey (in hal2009-server, 2): %s\n", r.pkey);
        sql_begin();
        char* source = sql_del_record(&r);
        if (source) {
            fact_delete_from_source(source);
            free(source);
        }
        sql_end();

        (*answer) += "<b>" + string(username) + "</b>: " + s + "<br />";
        (*answer) += "<b>FreeHAL</b>: Deleted.<br />";
        cout << "Got something to display from '" << username << "'." << endl;
        cout << "    " << (*answer) << endl;
        (*stream) << "DISPLAY:" << (*answer) << endl;
        unlink("_output2");
        hal2009_clean();
        return;
    }
    if (s == "de" || s == "deutsch" || s == "Deutsch" || s == "german" || s == "German" || s == "Deutsch!" || s == "deutsch!") {
        strcpy(language,             "de");

        (*answer) += "<b>" + string(username) + "</b>: " + s + "<br />";
        (*answer) += "<b>FreeHAL</b>: Language is " + string(language) + "<br />";
        cout << "Got something to display from '" << username << "'." << endl;
        cout << "    " << (*answer) << endl;
        (*stream) << "DISPLAY:" << (*answer) << endl;
        unlink("_output2");
        hal2009_clean();
        return;
    }
    if (s == "en" || s == "english" || s == "English" || s == "englisch" || s == "Englisch" || s == "English!" || s == "english!") {
        strcpy(language,             "en");

        (*answer) += "<b>" + string(username) + "</b>: " + s + "<br />";
        (*answer) += "<b>FreeHAL</b>: Language is " + string(language) + "<br />";
        cout << "Got something to display from '" << username << "'." << endl;
        cout << "    " << (*answer) << endl;
        (*stream) << "DISPLAY:" << (*answer) << endl;
        unlink("_output2");
        hal2009_clean();
        return;
    }
    strcpy(signal_handler_tlanguage, language);
    FILE* change_text_language = fopen("_change_text_language", "w");
    halwrite(language, 1, 2, change_text_language);
    halclose(change_text_language);
    
    char* programming_language = (char*)cxxhalmalloc(16, "hal2009_server_statement");
    strcpy(programming_language, "perl5");
    char* base_dir             = (char*)cxxhalmalloc(16, "hal2009_server_statement");
    strcpy(base_dir,             ".");
    char* language_copy        = (char*)cxxhalmalloc(16, "hal2009_server_statement");
    strcpy(language_copy,        language);
    char* input                = (char*)cxxhalmalloc(1025, "hal2009_server_statement");
    strncpy(input,               s.c_str(), 1024);

    bool only_learn = 0;
    if (s[0] == 'o' && s[1] == 'n' && s[2] == 'l' && s[3] == 'y') {
        only_learn = 1;
        strcpy(input, input+6);
    }

    const char* answer_from_c = "";
    int timeout = 3;
    static string last_input = string();
    static long last_input_time = 0;
    while (0 == strlen(answer_from_c) && timeout > 0) {
        
        
        time_t now;
        time(&now);
        cout << "input: now:  " << input      << endl;
        cout << "input: last: " << last_input << endl;
        if (input && (now - last_input_time) <= 3600 && last_input == string(input) ) {
            unlink("_output2");
            return;
        }
        last_input = string(input);
        last_input_time = now;
        
        cout << "Delete nonsense." << endl;
        unlink("_output2");
        hal2009_clean();

        (*answer) += "<b>" + string(username) + "</b>: " + string(input) + "<br />";
        hal2009_netcom_lock();
        (*stream) << "DISPLAY:" << (*answer) << endl;
        hal2009_netcom_unlock();

        cout << "Start threads (language: " << language_copy << ")." << endl;
        pthread_t answer_thread = hal2009_answer(strdup(input), strdup(programming_language), strdup(language_copy), strdup(base_dir), NOT_JOIN, MULTI);
        usleep(1000);
        //pthread_t signal_thread = hal2009_start_signal_handler(programming_language, language, MULTI);
        pthread_join(answer_thread, NULL);

        FILE* f;
        while (!(f = fopen("_output2", "r"))) {
            usleep(1000);
        }
        if (f) {
            fclose(f);
        }
        ifstream output_stream("_output2");
        usleep(500);
        string output;
        halgetline(output_stream, output);
        if (output.size() == 0) {
            ifstream output_stream("_output2");
            usleep(500);
            string output;//
            halgetline(output_stream, output);
        }
        unlink("_output2");
        answer_from_c = output.c_str();
        
        --timeout;
        hal2009_clean();
    }
    if (0 == strlen(answer_from_c)) {
        answer_from_c = "<i>FreeHAL sleeps... Unable to get an answer. Try again later.</i>";
    }
    else {
        last_input_time = 0;
    }
    (*answer) += "<b>FreeHAL</b>: " + string(answer_from_c) + "<br />";

    cout << "Got something to display from '" << username << "'." << endl;
    cout << "    " << (*answer) << endl;
    hal2009_netcom_lock();
    (*stream) << "DISPLAY:" << (*answer) << endl;
    hal2009_netcom_unlock();

    unlink("_output2");
    hal2009_clean();

    if (0) {
        long int last_line_number_from_log = -1;
        long int current_line_number_from_log = 0;
        char* code = (char*)cxxhalmalloc(100001, "hal2009_log_streamer");
        char* line = (char*)cxxhalmalloc(10000,  "hal2009_log_streamer");
        halclose(get_output_pipe());
        set_output_pipe(fopen("hal.log", "r"));
        if (get_output_pipe()) {
            set_nonblocking(fileno(get_output_pipe()));
            if (get_output_pipe()) {
                strcpy(code, "");
                strcpy(line, "");
                size_t nchars = 9999;
                do {
                    if (strlen(line) > 1)
                        strcat(code, line);
                    strcpy(line, "");
                    if (current_line_number_from_log >= last_line_number_from_log) {
                        last_line_number_from_log = current_line_number_from_log;
                        fgets(line, nchars, get_output_pipe());
                    }
                    ++current_line_number_from_log;
                } while (strlen(line));

                strcat(code, line);
                if (strlen(code)) {
                    int lines = 1;
                    for (int c = 0; c < strlen(code); ++c) {
                        if (code[c] == '\n')
                            ++lines;
                    }

                    hal2009_netcom_lock();
                    (*stream) << "MULTILINE:BEGIN" << endl;
                    (*stream) << lines << endl;
                    (*stream) << "LOG:" << code << "\n" << endl;
                    (*stream) << "MULTILINE:END" << endl;
                    hal2009_netcom_unlock();
                }
            }
            halclose(get_output_pipe());
        }
    }

    cout << "End of statement process." << endl;
}

int get_file_size(const char* filename) {
    struct stat* stbuf = (struct stat*)malloc(50000);
    stat(filename, stbuf);
    int file_size = stbuf ? stbuf->st_size : 0;
    free(stbuf);
    return file_size;
}

void hal2009_log_streamer(tcp::iostream* stream) {
/*
    static char* history;
    static int history_len;
    static int history_lines = 1;
    if (history) {
        (*stream) << "MULTILINE:BEGIN" << endl;
        (*stream) << history_lines << endl;
        (*stream) << "LOG:" << history << "\n" << endl;
        (*stream) << "MULTILINE:END" << endl;
    }
    else {
        history_len = 0;
        history = (char*)calloc(100000, 1);
    }

    long int last_line_number_from_log = -1;
    char* code = (char*)cxxhalmalloc(100001, "hal2009_log_streamer");
    char* line = (char*)cxxhalmalloc(10000,  "hal2009_log_streamer");
    while (stream > 0 && stdout > 0) {
        long int current_line_number_from_log = 0;
        usleep(10000);
    }
    cxxhalfree(code);
*/
}

void hal2009_server_client_connection(tcp::iostream* stream) {
    ofstream booted_file("booted");
    booted_file.close();
    vector<boost::thread*>* threads = new vector<boost::thread*>();
    
    hal2009_netcom_lock();
    (*stream) << "READY:EVERYTHING_INITIALIZED" << endl;
    (*stream) << "JELIZA_FULL_VERSION:" << FULL_VERSION << endl;
    (*stream) << "NAME:" << FULL_NAME << " SVN-Rev. " << FULL_VERSION << " (database " << (0 == strcmp(sql_engine, "ram") ? "in memory" : (0 == strcmp(sql_engine, "disk") ? "at disk" : "at disk (traditional)")) << ")" << endl;
    (*stream) << "Perl:." << endl;
    hal2009_netcom_unlock();
    
    string username;
    halgetline(*stream, username);
    replace_all(username, "USER:", "");
    trim(username);
    
    cout << "Got username." << endl;
    cout << "    " << username << endl;

    char* language             = (char*)cxxhalmalloc(16, "hal2009_server_client_connection");
    strcpy(language,             signal_handler_tlanguage);
    
    while (init_thread_ended <= 0) {
        usleep(1000);
    }

    string line;
    while (*stream && stream->rdbuf() != 0 && halgetline(*stream, line)) {
        ofstream booted_file("booted");
        booted_file.close();

        std::vector<std::string>* result = simple_split( line, ":" );
        
        std::vector<std::string>* result_underscore = simple_split( line, "_" );
        if ( result_underscore->at(0) == string("HERE") && result->size() >= 2 ) {
            cout << "Got a HERE_IS_* statement at the wrong place in code" << endl;
        }

        if ( result->at(0) == string("EXIT") ) {
            exit(0);
        }
        
        if ( result->at(0) == string("DELETE") && result->at(1) == string("FACT") && result->at(2) == string("PK") ) {
            struct RECORD r;
            strcpy(r.pkey, result->at(3).c_str());
            printf("pkey (in hal2009-server, 1): %s\n", r.pkey);
            sql_begin();
            char* source = sql_del_record(&r);
            if (source) {
                fact_delete_from_source(source);
                free(source);
            }
            sql_end();
            (*stream) << "DELETED:SUCCESS" << endl;
        }
        
        if ( init_thread_ended > 0 && result->at(0) == string("QUESTION") && result->at(1) != string("QUESTION") && result->size() >= 2 && result->at(1).size() > 0 && !(result->at(1).size() < 3 && ' ' == result->at(1)[0]) ) {
            
            string input;
            for (int i = 1; i < result->size(); ++i) {
                if (i != 1) {
                    input += ":";
                }
                input += result->at(i);
            }
            replace_all(input, "::", ":");
            hal2009_server_statement(stream, input, username, language);
        }
        
        delete result;
        delete result_underscore;
    }
}

void hal2009_server_run() {
    vector<boost::thread*>* threads = new vector<boost::thread*>();
    ofstream booted_file("booted");
    booted_file.close();

    boost::thread* log_streamer = 0;
    
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
            if (!hal2009_server_acceptor) {
                hal2009_server_start();
            }
            if (!hal2009_server_acceptor) {
                delete stream;
                continue;
            }
            hal2009_server_acceptor->accept(*stream->rdbuf());
        }
        catch (std::exception& e)
        {
            std::cerr << "2: " << e.what() << std::endl;
            if ( stream ) delete stream;
            continue;
        }
        try {
            hal2009_server_clients.push_back(stream);
            
            
            if (init_thread_ended == 0) {
                init_thread_ended = -1;
                
                pthread_t thread_init;
                pthread_create (&thread_init, NULL, hal2009_init_thread, (void*)NULL);

            }

            if (log_streamer)
                delete log_streamer;
            log_streamer = new boost::thread(boost::bind(&hal2009_log_streamer, stream));
            hal2009_server_client_connection(stream);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void hal2009_server_stop() {
    if ( hal2009_server_acceptor ) delete hal2009_server_acceptor;
    for (vector<tcp::iostream*>::iterator iter = hal2009_server_clients.begin(); iter != hal2009_server_clients.end(); iter++) {
        delete(*iter);
    }
    hal2009_server_clients = vector<tcp::iostream*>();
}

char* hal2009_server_get_value_from_socket(char* s1, const char* s2) {
    cout << "Get stream." << endl;
    while (0 == hal2009_server_clients.size()) {
        usleep(1000);
    }
    tcp::iostream* stream = hal2009_server_clients[hal2009_server_clients.size()-1];
    
    cout << "Print into stream." << endl;
    (*stream) << "GET_" << s1 << ":" << s2 << endl;
    
    cout << "Wait for stream." << endl;
    std::vector<std::string>* result;
    string line;
    while (*stream && stream->rdbuf() != 0 && halgetline(*stream, line)) {
        if ( line.size() != 0 ) {
            cout << line << endl;
            result = simple_split(line, ":" );
            if ( result->at(0) == string("HERE_IS_") + s1 && result->size() >= 2 ) {
                break;
            }
        }
    }
    cout << "End of waiting for stream." << endl;
    char* value = strdup(result->at(1).c_str());
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

void* hal2009_init_thread (void*) {
    hal2009_init(strdup(copy_for_init_planguage), strdup(copy_for_init_signal_handler_tlanguage), strdup(copy_for_init_base_dir));
    init_thread_ended = 1;
}

int main(int argc, char** argv) {
    sql_engine = (char*)calloc(64, 1);
    strcpy(sql_engine, "disk");
    if (argc >= 3 && argv[2]) {
        strcpy(sql_engine, argv[2]);
    }
    hal2009_clean();
    extract();

    char* planguage = "perl5";
    signal_handler_tlanguage = strdup("de");
    for (int g = 0; g < argc; ++g) {
        if (argv[g] && strlen(argv[g]) == 2) {
            strcpy(signal_handler_tlanguage, argv[g]);
        }
    }
    fprintf(output(), "Language is %s.\n", signal_handler_tlanguage);
    fprintf(output(), "Database Engine is %s.\n", sql_engine);
    base_dir = ".";

    {
        char* sqlite_filename = (char*)calloc(64+strlen(base_dir)+strlen(signal_handler_tlanguage), 1);
        strcat(sqlite_filename, base_dir);
        strcat(sqlite_filename, "/lang_");
        strcat(sqlite_filename, signal_handler_tlanguage);
        strcat(sqlite_filename, "/database.db");
        sql_sqlite_set_filename(sqlite_filename);
    }

    //halclose(stdout);
    int fd[2];
/*
#ifdef WIN32
	if(!CreatePipe(&fd[0], &fd[1], NULL, 0))
#else
	if(pipe(fd))
#endif
	{
        fprintf (output(), "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    set_output_pipe (fdopen (fd[0], "r"));
//  set_output      (fdopen (fd[1], "w"));
    freopen("x", "w", stdout);
    
//  set_output_fd   (fd[1]);
*/
    fprintf(output(), "Opened new stdout: stdout = %li\n", (long int)(get_output()));

    pthread_t signal_thread = hal2009_start_signal_handler(strdup(planguage), strdup(signal_handler_tlanguage), MULTI);
    copy_for_init_planguage = strdup(planguage);
    copy_for_init_base_dir = strdup(base_dir);
    copy_for_init_signal_handler_tlanguage = strdup(signal_handler_tlanguage);
    pthread_t thread_netcom;

    hal2009_server_start();
    hal2009_netcom_start();
    
    pthread_create (&thread_netcom, NULL, hal2009_netcom_run, (void*)NULL);
    hal2009_server_run();
    
    hal2009_server_stop();
}

void hal2009_handle_signal(void* arg) {
    char* type = (char*)((void**)arg)[0];
    char* text = (char*)((void**)arg)[1];

    if (0 == strcmp(type, "_output__pos")) {
        FILE* _doing = fopen("_doing__pos", "w+b");
        halclose(_doing);
        fprintf(output(), "\nUnknown part of speech:\n\n%s\n", text);
        text = hal2009_server_get_value_from_socket("WORD_TYPE", text);
        FILE* target = fopen("_input__pos", "w+b");
        halwrite(text, 1, strlen(text), target);
        halclose(target);
        unlink("_doing__pos");
    }
    else if (0 == strcmp(type, "_output__link")) {
        if (strlen(text) < 99) {
            char link[99] = {0};
            int f1 = 0;
            int f2 = 0;
            sscanf(text, "%98[a-zA-Z]%d\n%d", &link, &f1, &f2);
            fprintf(output(), "--%s--\n%s\n%i\n%i", text, link, f1, f2);
            hal2009_add_link(link, f1, f2);
        }
    }
    else if (0 == strcmp(type, "_output__add_pro_file")) {
        hal2009_add_pro_file(text);
        FILE* target = fopen("_input__add_pro_file", "w+b");
        halclose(target);
    }
    else if (0 == strcmp(type, "_output__get_csv")) {
        hal2009_set_text_language(signal_handler_tlanguage);
        struct DATASET set = hal2009_get_csv(text);
        const char* csv_data = hal2009_make_csv(&set);
        FILE* target = fopen("_input__get_csv", "w+b");
        halwrite(csv_data, 1, strlen(csv_data), target);
        halclose(target);
        fprintf(output(), "Release memory now.\n");
        free((void*)csv_data);
        fprintf(output(), "Memory is released.\n");
    }
    else if (0 == strcmp(type, "_output")) {
        /*fprintf(output(), "\nFreeHAL: %s\n", text);
        ofstream out("_output");
        out << text << endl;
        cout << "Printing into _input_server: " << text << endl;
        out.close();
        unlink("_output");
        ///pthread_exit(0);*/
        
        ofstream out("_output2");
        out << text << endl;
        out.close();
        unlink("_output");
    }
}

