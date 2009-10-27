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

#ifndef HAL2009_DISTRIB
#define HAL2009_DISTRIB 1

/// C Part

#ifdef HAL2009_DISTRIB_NO_SERVER
extern "C" {
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>

    void* invoke (const char* func, void* data) {
        printf("Server not compiled in - Cannot execute function '%s' using data '%s'.\n", func, (const char*)data);
    }
}
#else // #ifdef HAL2009_DISTRIB_NO_SERVER
extern "C" {
    typedef void (*fp)(void*);

#   include "_distrib.h"

#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>

    void* invoke (const char* func, void* data) {
        const char* str_to_ptr[N_FUNC] = STR_TO_PTR;
        fp          ptr_to_str[N_FUNC] = PTR_TO_STR;

        int i;
        void* res = 0;
        for (i = 0; i < N_FUNC; ++i) {
            if (0 == strcmp( (const char*)(str_to_ptr[i]), func)) {
                res = ( (void* (*)(void*))(ptr_to_str[i]) )(data);
            }
        }

        return res;
    }
}
#endif // #ifdef HAL2009_DISTRIB_NO_SERVER

#ifdef __cplusplus

/// C++ Part

// Boost headers
#ifndef NO_ASIO_HERE
#include <boost/asio.hpp>
#endif
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
using namespace algorithm;
using boost::asio::ip::tcp;

boost::asio::io_service  io_service;
tcp::acceptor*           distrib_server_acceptor;
unsigned short           distrib_port              = 5171;

void hal2009_distrib_server_start() {
    while (!distrib_server_acceptor) {
        try {
            
            tcp::endpoint endpoint(tcp::v4(), distrib_port);
            distrib_server_acceptor = new tcp::acceptor(io_service, endpoint);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        
        usleep(2000);
    }
}

void hal2009_distrib_client_connection(tcp::iostream* stream) {
    std::string func;
    halgetline(*stream, func);
    while (func.size() && ( func[func.size()-1] == '\r' || func[func.size()-1] == '\n' )) {
        func[func.size()-1] = '\0';
    }

    long length = 0;
    (*stream) >> length;

    std::string data;
    std::string line;
    halgetline(*stream, line);
    printf("I'm going to get %i lines.\n", length);
    int line_num;
    for (line_num = 0; line_num < length; ++line_num) {
        halgetline(*stream, line);
        data += line;
    }
    printf("I got:\n----\n%s\n----\n", data.c_str());

    const char* result = (const char*)invoke(func.c_str(), (void*)(data.c_str()));
    (*stream) << result;
    stream->close();
    if (stream)
        delete stream;
}

void hal2009_distrib_server_run() {
    for (;;) {
        tcp::iostream* stream;
        try {
            stream = new tcp::iostream();
        }
        catch (std::exception& e)
        {
            std::cerr << "(1): " << e.what() << std::endl;
            if ( stream ) delete stream;
            usleep(2000);
            continue;
        }
        try {
            if (!distrib_server_acceptor) {
                hal2009_distrib_server_start();
                usleep(2000);
            }
            if (!distrib_server_acceptor) {
                delete stream;
                continue;
            }
            distrib_server_acceptor->accept(*stream->rdbuf());
        }
        catch (std::exception& e)
        {
            std::cerr << "(2): " << e.what() << std::endl;
            if ( stream ) delete stream;
            usleep(2000);
            continue;
        }
        try {
            hal2009_distrib_client_connection(stream);
        }
        catch (std::exception& e)
        {
            std::cerr << "(3): " << e.what() << std::endl;
            usleep(2000);
        }
    }
}

const char** hal2009_distrib_search_servers() {
    char** servers = (char**)calloc(sizeof(char*), 2);
    servers[0] = strdup("127.0.0.1");
    return (const char**)servers;
}

const char* hal2009_distrib_invoke_on(const char* host, unsigned short port, const char* func, const char* arg) {
    tcp::socket socket(io_service);
    boost::system::error_code error = asio::error::host_not_found;
    do
    {
        try {
            tcp::endpoint endp(asio::ip::address_v4::from_string(host), (unsigned short)(port));
            boost::shared_ptr<tcp::iostream> stream(new tcp::iostream());
            stream->close();

            if (stream->rdbuf()->connect(endp)) {
                cout << "[ distrib :: Connected ]." << endl;

                (*stream) << func << endl;

                int lines = 1;
                (*stream) << lines << endl;
                (*stream) << arg << endl;
                string line;
                string result;
                while (*stream && stream->rdbuf()->is_open()) {
                    halgetline(*stream, line);
                    result += line;
                    line = "";
                }

                cout << "[ distrib :: Result = \"" << result << "\" ]." << endl;

                return result.c_str();

                break;
            }
            else {
                cout << "[ distrib :: Connect failed ]." << endl;
            }
        }
        catch (std::exception& e) {
            std::cerr << 4 << "Exception in thread: " << e.what() << "\n";
            break;
        }
    } while (error);
}

#ifndef HAL2009_DISTRIB_NO_MAIN

int main (int argc, char** argv) {

    hal2009_distrib_server_start();
    hal2009_distrib_server_run();

    return 0;
}

#endif // #ifndef HAL2009_DISTRIB_NO_MAIN

#endif // #ifdef __cplusplus

#endif // #ifndef HAL2009_DISTRIB
