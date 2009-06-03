#include "defs.h"

#include "communicate.h"
#include "unistd.h"

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

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

typedef int COMMUNICATOR_TYPE;
COMMUNICATOR_TYPE SERVER = 1;
COMMUNICATOR_TYPE CLIENT = 2;

COMMUNICATOR_TYPE communicator_type;

string host = "127.0.0.1";
unsigned short port = 5173;

vector<freehal::string> to_send;
vector<boost::shared_ptr<tcp::iostream> > streams;
bool ok_to_write = false;
bool ready_to_write = false;

const int max_length = 1024;
typedef boost::shared_ptr<tcp::socket> socket_ptr;
asio::io_service io_service;



void freehal::msleep (unsigned long msecs) {
    asio::deadline_timer timer(io_service, boost::posix_time::milliseconds(msecs));
    timer.wait();
}

void session_stream_read(boost::shared_ptr<tcp::iostream> stream) {
    freehal::string line;
    freehal::string cline;
    unsigned long got_nothing_for_milliseconds = 0;
    unsigned long timeout = 300000;
    for (;;) {
        asio::deadline_timer timer(io_service, boost::posix_time::milliseconds(50));
        try {
            getline(*stream, line.ref());
            
            if (line.size() == 0) {
                got_nothing_for_milliseconds += 50;
            }
            if (got_nothing_for_milliseconds > timeout) {
                stream->close();
                return;
            }

            while (line.size() > 0) {
                cout << 1 << line.ref() << endl;
                if (line.contains("MULTILINE:BEGIN")) {
                    int lines = 0;
                    (*stream) >> lines;
                    getline(*stream, line.ref());
                    cout << 2 << line.ref() << endl;
                    cline.set("\n");
                    time_t t = time(NULL);
                    while (!cline.contains("MULTILINE:END") && time(NULL) < t+2 && lines > 0) {
                        line += cline + "\n";
                        cout << 3 << line.ref() << endl;
                        getline(*stream, cline.ref());
                        --lines;
                    }
                    if (line.size())
                        line[line.size()-1] = '\0';
                }
                boost::thread t(boost::bind(freehal::comm_new, line));

                getline(*stream, line.ref());
            }

            timer.wait();
        }
        catch (std::exception& e) {
//            std::cerr << 1 << "Exception in thread: " << e.what() << "\n";
        }
    }
}

unsigned long in_schlange = 0;

void comm_write(std::string str) {
    for (unsigned int x = 0; x < streams.size(); x++) {
        try {
            ++in_schlange;
            do {
                ready_to_write = false;
                (*(streams[x])) << str << endl;
                ready_to_write = true;
            } while (!ready_to_write);
            --in_schlange;
        }
        catch (std::exception& e) {
            std::cerr << 3 << "Exception in thread: " << e.what() << "\n";
        }
    }
}
void flush_streams() {
    unsigned int y = 0;
    
    while (true) {
        y += 1;
        for (unsigned int x = 0; x < streams.size(); x++) {
            try {
                (*(streams[x])).flush();
                if ( y > 3 ) {
                    (*(streams[x])) << "HOLD:" << endl;
                }
            }
            catch (std::exception& e) {
                (*(streams[x])).close();
                return;
                std::cerr << 3 << "Exception in thread: " << e.what() << "\n";
            }
        }
        
        if ( y > 3 ) {
            y = 0;
        }


        asio::deadline_timer timer(io_service, boost::posix_time::milliseconds(500));
        timer.wait();
    }
}

void io_service_run() {
    io_service.run();
}

void send_hello() {
    asio::io_service io_service;
    asio::deadline_timer timer(io_service, boost::posix_time::milliseconds(2000));
    timer.wait();

    freehal::comm_send("HELLO:" + KI_NAME);
    freehal::send_name();
}

void server() {
    boost::thread t_ios(io_service_run);

    try {
        tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
        for (;;) {
            try {
                boost::shared_ptr<tcp::iostream> stream(new tcp::iostream);
                a.accept(*stream->rdbuf());
                asio::ip::tcp::no_delay option(true);
                stream->rdbuf()->set_option(option);

                cout << ":: Neuer Client" << endl;

                boost::thread t_r(boost::bind(session_stream_read, stream));
                boost::thread t_f(flush_streams);
                cout << ":: Threads started (" << streams.size() << ")" << endl;
                if (streams.size() > 50) {
                    boost::shared_ptr<tcp::iostream> stream1 = streams[0];
                    vector<boost::shared_ptr<tcp::iostream> > streams2;
                    streams2.push_back(stream1);
                    streams << NULL << streams2;
                    cout << "CLEANED STREAMS ARRAY" << endl;
                }
                streams.push_back(stream);
                ok_to_write = true;
                ready_to_write = true;

                boost::thread t_h(send_hello);
            }
            catch (std::exception& e) {
                std::cerr << 5.5 << "Exception in thread: " << e.what() << "\n";
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << 5 << "Exception in thread: " << e.what() << "\n";
    }
}

void client(std::string host, bool verbose = true, bool wait = false) {
    if (wait) {
        while ( 1 ) {
            ifstream i("booted");
            if ( i ) {
                freehal::display_sentence(freehal::string("FreeHAL wurde heruntergeladen. Bitte warten..."));
                freehal::msleep(3000);
                break;
            }

            ifstream i_bootlog("../boot.log");
            freehal::string all;
            freehal::string temp;
            while (i_bootlog) {
                getline(i_bootlog, temp.ref());
                temp.strip();

                all += temp;
                all += "<br>";
            }
            freehal::display_sentence(all);
            //emit answerHelper->signalTalkingScrollEnd(QString("anch"));
            
            freehal::msleep(300);
        }
    }

    boost::thread t_ios(io_service_run);

    
    while ( 1 ) {
        tcp::socket socket(io_service);
        boost::system::error_code error = asio::error::host_not_found;
        do
        {
            try {
                tcp::endpoint endp(asio::ip::address_v4::from_string(host.c_str()), (unsigned short)(port));
                boost::shared_ptr<tcp::iostream> stream(new tcp::iostream());
                stream->close();

                if (stream->rdbuf()->connect(endp)) {
                    if (verbose) {
                        cout << ":: Connected" << endl;
                    }

                    (*stream) << "Mensch" << endl;

                    boost::thread t_r(boost::bind(session_stream_read, stream));
                    boost::thread t_f(flush_streams);
                    if (verbose) {
                        cout << ":: Threads started" << endl;
                    }
                    streams.push_back(stream);
                    ok_to_write = true;
                    ready_to_write = true;
                    
                    while ( stream->rdbuf()->is_open() ) {
                        freehal::msleep(200);
                    }
                }
                else {
                    static int d = 0;
                    ++d;
                    if (d > 50) {
                        d = 0;
                        cout << "trying to connect..." << endl;
                        cout << "failed." << endl;
                    }
                }
            }
            catch (std::exception& e) {
                std::cerr << 4 << "Exception in thread: " << e.what() << "\n";
                freehal::msleep(3000);
            }
        freehal::msleep(300);
        } while (error);
    }
}



void freehal::comm_init_server() {
    communicator_type = SERVER;

    boost::thread t(server);
}


void freehal::comm_init_client(std::string host, bool verbose, bool wait = false) {
    communicator_type = CLIENT;

    boost::thread t(boost::bind(client, host, verbose, wait));
}


void freehal::comm_send(freehal::string s) {
    if (ok_to_write) {
        cout << "--" << s.ref() << endl;
        comm_write(s.ref());
    }
}

void freehal::comm_send(std::string s) {
    comm_send(freehal::string(s));
}

void freehal::send_name() {
    freehal::comm_send("NAME:" + KI_NAME);
    freehal::comm_send("JELIZA_FULL_VERSION:" + JELIZA_FULL_VERSION);
}
