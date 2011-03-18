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

#include "hal2009.h"

#include <pthread.h>

asio::io_service iose;

string download_from_url(const string& url) {
    if (url.size() == 0) {
        return 0;
    }

    try {
        string host;
        string path;

        int divisor = url.find("/");
        if (divisor != string::npos) {
            host = url.substr(0, divisor);
            path = url.substr(divisor);
        }

        cout << endl
        << "Host: " << host << endl
        << "Path: " << path << endl
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
            cout << error << endl;
            return string();
        }

        cout << "=" << flush;

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        asio::streambuf request;
        std::ostream request_stream(&request);
        request_stream << "GET " << path << " HTTP/1.0\r\n";
        request_stream << "Host: " << host << "\r\n";
        request_stream << "User-Agent: Mozilla/5.0 (X11; U; Linux i686; de; rv:1.8.1.10) Gecko/20071213 Fedora/2.0.0.10-3.fc8 Firefox/2.0.0.10\r\n";
        request_stream << "Connection: close\r\n\r\n";

        cout << "GET " << path << " HTTP/1.0\r\n";
        cout << "Host: " << host << "\r\n";
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
            cout << "Invalid response" << endl;
            return string();
        }
        if (status_code != 200) {
            std::cout << "Response returned with status code " << status_code << endl;
            return string();
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
        string content;
        std::stringstream sst;

        // Write whatever content we already have to output.
        if (response.size() > 0) {
            sst << &response;
        std::cout << "&response1: " << &response << endl;
        }

        unsigned int aa = 5;
        // Read until EOF, writing data to output as we go.
        while (asio::read(socket, response, asio::transfer_at_least(1), error)) {
            sst << &response;
            ++aa;
        }
        if (error != asio::error::eof) {
            cout << error << endl;
        }

        content = sst.str();
        //cout << "content: " << content << " (size: " << content.size() << ")" << endl;
        return content;
    }
    catch (boost::system::system_error e) {
        std::cout << e.what() << std::endl;
        return string();
    }
}


