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

#include <pthread.h>

using namespace std;
using namespace boost;
using namespace algorithm;
using boost::asio::ip::tcp;
asio::io_service iose;

halstring* cpp_download_from_url(const char* _url) {
    try {
        halstring url_st;
        url_st.s = strdup(_url);
        url_st.do_free = 1;
        
        halstring* url = &url_st;
        
        if (!url->s) {
            return 0;
        }
        
        string host;
        string path;
        
        int i;
        int assign_to = 1;
        for (i = 0; i < strlen(url->s); ++i) {
            if (assign_to == 1 && url->s[i] == '/') {
                assign_to = 2;
            }
            
            char c[2];
            c[0] = url->s[i];
            c[1] = '\0';

            if (assign_to == 1) {
                host += c;
            }
            if (assign_to == 2) {
                path += c;
            }
        }
        
        cout << endl
        << "Host: " << host.c_str() << endl
        << "Path: " << path.c_str() << endl
        << endl;

        // Get a list of endpoints corresponding to the server name.
        cout << "," << flush;
        boost::thread t(boost::bind(&asio::io_service::run, &iose));
        tcp::resolver resolver(iose);
        cout << "," << flush;
        tcp::resolver::query query(host, "http");
        cout << "," << flush;
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        cout << "," << flush;
        tcp::resolver::iterator end;
        cout << "," << flush;

        // Try each endpoint until we successfully establish a connection.
        tcp::socket socket(iose);
        cout << "," << flush;
        boost::system::error_code error = asio::error::host_not_found;
        cout << "," << flush;
        while (error && endpoint_iterator != end)
        {
            cout << "." << flush;
            socket.close();
            cout << "." << flush;
            socket.connect(*endpoint_iterator++, error);
        }
        if (error) {
            cerr << error << endl;
            return 0;
        }

        cout << "=" << flush;

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "GET " << path << " HTTP/1.0\r\n";
        request_stream << "Host: " << host.c_str() << "\r\n";
        request_stream << "User-Agent: Mozilla/5.0 (X11; U; Linux i686; de; rv:1.8.1.10) Gecko/20071213 Fedora/2.0.0.10-3.fc8 Firefox/2.0.0.10\r\n";
        request_stream << "Connection: close\r\n\r\n";

        cout << "GET " << path << " HTTP/1.0\r\n";
        cout << "Host: " << host.c_str() << "\r\n";
        cout << "User-Agent: Mozilla/5.0 (X11; U; Linux i686; de; rv:1.8.1.10) Gecko/20071213 Fedora/2.0.0.10-3.fc8 Firefox/2.0.0.10\r\n";
        //        cout << "Accept: */*\r\n";
        cout << "Connection: close\r\n\r\n";

        // Send the request.
        asio::write(socket, request);

        // Read the response status line.
        asio::streambuf response;
        asio::read_until(socket, response, "\r\n");

        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        unsigned int status_code;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            cerr << "Invalid response\n";
            return 0;
        }
        if (status_code != 200) {
            std::cout << "Response returned with status code " << status_code << "\n";
            return 0;
        }

        // Read the response headers, which are terminated by a blank line.
        asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r") {
            std::cout << header << "\n";
        }
        std::cout << "\n";

        // Content:
        halstring* content = (halstring*)calloc(sizeof(halstring), 1);
        content->do_free = 1;
        std::stringstream sst;

        // Write whatever content we already have to output.
        if (response.size() > 0) {
            sst << &response;
        }

        unsigned int aa = 5;
        // Read until EOF, writing data to output as we go.
        while (asio::read(socket, response, asio::transfer_at_least(1), error)) {
            sst << &response;
            ++aa;
        }
        if (error != asio::error::eof) {
            cerr << error << endl;
        }

        content->s = strdup(sst.str().c_str());
        return content;
    }
    catch (boost::system::system_error e) {
        return 0;
    }
}

extern "C" {
    halstring* download_from_url(const char* url) {
        return cpp_download_from_url(url);
    }
}



