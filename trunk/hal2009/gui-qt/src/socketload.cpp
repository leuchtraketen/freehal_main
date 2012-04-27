#include "defs.h"

using namespace freehal;

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

#include <stdexcept> // runtime_error
#include <sstream>
#ifdef linux
# include <sys/socket.h> // socket(), connect()
# include <arpa/inet.h> // sockaddr_in
# include <netdb.h> // gethostbyname(), hostent
# include <errno.h> // errno
#else
# if defined(__APPLE__)
#  include <sys/socket.h> // socket(), connect()
#  include <arpa/inet.h> // sockaddr_in
#  include <netdb.h> // gethostbyname(), hostent
#  include <errno.h> // errno
# else
#  include <winsock2.h>
# endif
#endif

using namespace std;

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include "communicate.h"

using namespace std;
using asio::ip::tcp;


std::string freehal::proxy;
unsigned int freehal::proxy_port;


// Entfernt das http:// vor dem URL
void RemoveHttp(std::string& URL)
{
	size_t pos = URL.find("http://");
	if(pos != std::string::npos)
	{
		URL.erase(0, 7);
	}
}

// Gibt die Dateiendung im URL zurück
std::string GetFileEnding(std::string& URL)
{
	using namespace std;
	size_t pos = URL.rfind(".");
	if(pos == std::string::npos)
	{
		return "";
	}
	URL.erase(0, pos);
	std::string ending = ".";
	// Algorithmus um Sachen wie ?index=home nicht zuzulassen
	for(std::string::iterator it = URL.begin() + 1; it != URL.end(); ++it)
	{
		if(isalpha(*it))
		{
			ending += *it;
		}
		else
		{
			break;
		}
	}

	if (ending == ".org") {
	    ending = ".php";
	}

	return ending;
}

// Gibt den Hostnamen zurück und entfernt ihn aus der URL, sodass nur noch der Pfad übrigbleibt
std::string RemoveHostname(std::string& URL)
{
	size_t pos = URL.find("/");
	if(pos == std::string::npos)
	{
		std::string temp = URL;
		URL = "/";
		return temp;
	}
	std::string temp = URL.substr(0, pos);
	URL.erase(0, pos);
	return temp;
}

void freehal::async_download (std::string url) {
    boost::thread t1(boost::bind(freehal::download, url));
}

asio::io_service iose;

std::string freehal::download (std::string url) {
//std::string freehal_download_does_not_work (std::string url) {
    try {
        freehal::string URL;
        URL.ref() = url;
        URL.strip();

        URL = URL.replace("https://", "");
        URL = URL.replace("http://", "");
        std::string host = URL.substr(0, URL.find("/")).ref();
        std::string path = URL.replace(host, std::string("")).ref();
        cout << endl
             << "Host: " << host.c_str() << endl
             << "Path: " << path.c_str() << endl
             << endl;

        // Get a list of endpoints corresponding to the server name.
        cout << "," << flush;
        boost::thread t(boost::bind(&asio::io_service::run, &iose));
        tcp::resolver resolver(iose);
        cout << "," << flush;
        std::string prox_p;
        stringstream _sst;
        _sst << freehal::proxy_port;
        _sst >> prox_p;
        tcp::resolver::query query((freehal::proxy.size() == 0) ? host : ((freehal::proxy_port == 80) ? freehal::proxy : freehal::proxy + prox_p ), "http");
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
        if (error)
            cerr << error << endl;

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
        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            cerr << "Invalid response\n";
            return std::string();
        }
        if (status_code != 200)
        {
            std::cout << "Response returned with status code " << status_code << "\n";
            return std::string();
        }

        // Read the response headers, which are terminated by a blank line.
        asio::read_until(socket, response, "\r\n\r\n");

        // Process the response headers.
        std::string header;
        while (std::getline(response_stream, header) && header != "\r")
            std::cout << header << "\n";
        std::cout << "\n";

        // Content:
        std::string content;
        std::stringstream sst;

        // Write whatever content we already have to output.
        if (response.size() > 0)
            sst << &response;

        unsigned int aa = 5;
        // Read until EOF, writing data to output as we go.
        while (asio::read(socket, response,
                asio::transfer_at_least(1), error)) {
            sst << &response;
            ++aa;
        }
        if (error != asio::error::eof)
            cerr << error << endl;

        content = sst.str();

        return content;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return std::string();

/*
        std::string URL = url;

        RemoveHttp(URL);

        std::string filename;

        std::string hostname = RemoveHostname(URL);

        cout << endl
             << "Host: " << hostname << endl
             << "Path: " << URL << endl
             << "Url:  " << url << endl
             << endl;

        cout << ":: Connecting..." << endl;

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(hostname.c_str(), "http");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        // Try each endpoint until we successfully establish a connection.
        tcp::socket socket(io_service);
        asio::error_code error = asio::error::host_not_found;
        while (error && endpoint_iterator != end) {
            cout << "." << endl;
            socket.close();
            cout << "." << endl;
            socket.connect(*endpoint_iterator++, error);
            cout << "." << endl;
        }
        if (error) {
            cout << ":: Server nicht gefunden (host not found)" << endl;
            log(":: Server nicht gefunden (host not found)");
        }


        cout << ":: Ready" << endl;

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        asio::streambuf request_buf;
        std::ostream request_stream(&request_buf);
        request_stream << "GET " << URL << " HTTP/1.0\r\n";
        request_stream << "Host: " << hostname << "\r\n";
        request_stream << "User-Agent: Mozilla/5.0 (X11; U; Linux i686; de; rv:1.8.1.2) Gecko/20060601 Firefox/2.0.0.2 (Ubuntu-edgy)\r\n";
        request_stream << "Connection: close\r\n\r\n";

        // Send the request.
        asio::write(socket, request_buf);

        unsigned long size;
        while (true) {
            asio::streambuf sstream_buf;
            asio::read_until(socket, sstream_buf, "\n");
            istream sstream(&sstream_buf);

            freehal::string left; // Das was links steht
            sstream >> left.ref();
            left.strip();
            if (left.size() == 0) {
                break;
            }

            if (left == "Content-Length:") {
                sstream >> size;
            }
            if (left == "Location:") {
                freehal::string location;
                sstream >> location.ref();
                return freehal::download(location.ref());
            }
            if (left == "Transfer-Encoding:") {
                std::string transferEncoding;
                sstream >> transferEncoding;
            }
        }

        freehal::string content;

        asio::streambuf response_buf;
        istream response(&response_buf);
        while (asio::read(socket, response_buf, asio::transfer_at_least(1), error)) {
            //content += &response_buf;
            response >> content.ref();
        }
        if (error != asio::error::eof) {
            cerr << "ASIO Error: " << error << endl;
        }

        return content.ref();
    }
    catch (std::exception& e) {
        std::cerr << "Exception in freehal::download: " << e.what() << "\n";
    }
    return std::string();
*/
}

/*
std::runtime_error CreateSocketError()
{
	std::stringstream temp;
#ifdef linux
	temp << "Socket-Fehler #" << errno << ": " << strerror(errno);
#else
	int error = WSAGetLastError();
	char* msg;
//	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//				  NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//				  reinterpret_cast<char*>(&msg), 0, NULL);
	temp << "Socket-Fehler #" << error; // << ": "; // << msg;
#endif
	return std::runtime_error(temp.str());
}

void SendAll(int socket, const char* const buf, const size_t size)
{
	size_t bytesSent = 0; // Anzahl Bytes die wir bereits vom Buffer gesendet haben
	do
	{
		bytesSent += send(socket, buf + bytesSent, size, 0);
		if(bytesSent <= 0) // Wenn send einen Wert <= 0 zurück gibt deutet dies auf einen Fehler hin.
		{
			throw CreateSocketError();
		}
	} while(bytesSent < size);
}

// Liest eine Zeile des Sockets in einen std::stringstream
void GetLine(int socket, std::stringstream& line)
{
	for(char c; recv(socket, &c, 1, 0) > 0; line << c)
	{
		if(c == '\n')
		{
			return;
		}
	}
	throw CreateSocketError();
}


static std::string replace_aufz (std::string& in, const std::string rep) {
	int wit = 1;
	int pos;
	while (true) {
		pos = in.find(rep);
		if (pos == -1) {
			break;
		} else {
			in.erase(pos, rep.length());
			std::stringstream ss;
			ss << wit;
			std::string tmp;
			ss >> tmp;
			in.insert(pos, " " + tmp + ". ");
			wit++;
		}
	}
	return in;
}

std::string ohneHtml(std::string all3) {
	std::string allAscii = "";
	freehal::string all2(all3.c_str());
	std::string& all = all2.ref();
	all2 = all2.replace(std::string("<br>"), std::string("\n"));
	all2 = all2.replace(std::string("<br/>"), std::string("\n"));
	all2 = all2.replace(std::string("<br />"), std::string("\n"));
	int aufz = 1;
	bool inHtml = false;
	for (int x = 0; x < all.size(); x++) {
		char array[2];
		array[0] = all[x];
		array[1] = '\0';
		std::string y(array);

		if (y == std::string("<")) {
			inHtml = true;
		}
		if (!inHtml) {
			allAscii += y;
		}
		if (y == std::string(">")) {
			inHtml = false;
		}
	}

	allAscii = replace_aufz(allAscii, std::string(" - "));

	return allAscii;
}

std::string freehal::download (std::string url) {
	using namespace std;

	std::string URL = url;

#ifndef linux
	WSADATA w;
	if(WSAStartup(MAKEWORD(2,2), &w) != 0)
	{
		cerr << "Winsock 2 konnte nicht gestartet werden! Error #" << WSAGetLastError() << endl;
		return "";
	}
#endif

	RemoveHttp(URL);

	std::string filename;

	std::string hostname = RemoveHostname(URL);

	hostent* phe = gethostbyname(hostname.c_str());

	if(phe == NULL)
	{
		cerr << "Host konnte nicht aufgeloest werden!" << endl;
		return "";
	}

	if(phe->h_addrtype != AF_INET)
	{
		cerr << "Ungueltiger Adresstyp!" << endl;
		return "";
	}

	if(phe->h_length != 4)
	{
		cerr << "Ungueltiger IP-Typ!" << endl;
		return "";
	}

	int Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(Socket == -1)
	{
		cerr << "Socket konnte nicht erstellt werden!" << endl;
		return "";
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(80); // Das HTTP-Protokoll benutzt Port 80

	char** p = phe->h_addr_list; // p mit erstem Listenelement initialisieren
	int result; // Ergebnis von connect
	do
	{
		if(*p == NULL) // Ende der Liste
		{
			cerr << "Verbindung fehlgschlagen!" << endl;
			return "";
		}

//		char xyz[4];
//		xyz[0] = 66;
//		xyz[1] = 230;
//		xyz[2] = 200;
//		xyz[3] = 100;

//		service.sin_addr.s_addr = *reinterpret_cast<unsigned long*>(xyz);
		service.sin_addr.s_addr = *reinterpret_cast<unsigned long*>(*p);
		++p;
		result = connect(Socket, reinterpret_cast<sockaddr*>(&service), sizeof(service));
	}
	while(result == -1);

	cerr << "Verbindung erfolgreich!" << endl;

	std::string request = "GET ";
	request += URL;	// z.B. /faq/index.html
	request += " HTTP/1.1\n";
	request += "Host: " + hostname + "\n";
	request += "User-Agent: Mozilla/5.0 (X11; U; Linux i686; de; rv:1.8.1.2) Gecko/20060601 Firefox/2.0.0.2 (Ubuntu-edgy)\n";
	request += "Connection: close\n\n";


	try
	{
		SendAll(Socket, request.c_str(), request.size());

		int code = 100; // 100 = Continue
		std::string Protokoll;
		std::stringstream firstLine; // Die erste Linie ist anders aufgebaut als der Rest
		while(code == 100)
		{
			GetLine(Socket, firstLine);
			firstLine >> Protokoll;
			firstLine >> code;
			if(code == 100)
			{
				GetLine(Socket, firstLine); // Leere Zeile nach Continue ignorieren
			}
		}
		cerr << "Protokoll: " << Protokoll << endl;

		if(code != 200 && code != 302)
		{
			firstLine.ignore(); // Leerzeichen nach dem Statuscode ignorieren
			std::string msg;
			getline(firstLine, msg);
			cerr << "Error #" << code << " - " << msg << endl;
			return "";
		}

		bool chunked = false;
		const int noSizeGiven = -1;
		int size = noSizeGiven;

        std::string location = "";

		while(true)
		{
			std::stringstream sstream;
			GetLine(Socket, sstream);
			if(sstream.str() == "\r") // Header zu Ende?
			{
				break;
			}
			std::string left; // Das was links steht
			sstream >> left;
			sstream.ignore(); // ignoriert Leerzeichen
			if(left == "Content-Length:")
			{
				sstream >> size;
			}
			if(left == "Location:")
			{
				sstream >> location;
			}
			cout << left << "|" << location << endl;
			if(left == "Transfer-Encoding:")
			{
				std::string transferEncoding;
				sstream >> transferEncoding;
				if(transferEncoding == "chunked")
				{
					chunked = true;
				}
			}
		}

		if (location.size() > 0) {
		    cerr << "- Weiterleitung zu: " << location << endl;
		    cout << "- Weiterleitung zu: " << location << endl;
//		    log("- Weiterleitung zu: " + location);
		    return download(location);
		}

		filename = "download" + GetFileEnding(URL);
		cerr << "Filename: " << filename << endl;
		fstream fout(filename.c_str(), ios::binary | ios::out);
		if(!fout)
		{
			cerr << "Could Not Create File!" << endl;
			return "";
		}
		int recvSize = 0; // Empfangene Bytes insgesamt
		char buf[1024];
		int bytesRecv = -1; // Empfangene Bytes des letzten recv

        int a = 0;

		if(size != noSizeGiven) // Wenn die Größe über Content-length gegeben wurde
		{
			cerr << "0%";
			while(recvSize < size)
			{
				if((bytesRecv = recv(Socket, buf, sizeof(buf), 0)) <= 0)
				{
					throw CreateSocketError();
				}
				recvSize += bytesRecv;
				fout.write(buf, bytesRecv);
				cerr << "\r" << recvSize * 100 / size << "%" << flush; // Mit \r springen wir an den Anfang der Zeile

				if (recvSize * 100 / size > a) {
				    update_prozent(recvSize * 100 / size);
				    ++a;
				}
			}
		}
		else
		{
			if(!chunked)
			{
				cerr << "Downloading... (Unknown Filesize)" << endl;
				while(bytesRecv != 0) // Wenn recv 0 zurück gibt, wurde die Verbindung beendet
				{
					if((bytesRecv = recv(Socket, buf, sizeof(buf), 0)) < 0)
					{
						throw CreateSocketError();
					}
					fout.write(buf, bytesRecv);
				}
			}
			else
			{
				cerr << "Downloading... (Chunked)" << endl;
				while(true)
				{
					std::stringstream sstream;
					GetLine(Socket, sstream);
					int chunkSize = -1;
					sstream >> hex >> chunkSize; // Größe des nächsten Parts einlesen
					if(chunkSize <= 0)
					{
						break;
					}
					cerr << "Downloading Part (" << chunkSize << " Bytes)... " << endl;
					recvSize = 0; // Vor jeder Schleife wieder auf 0 setzen
					while(recvSize < chunkSize)
					{
						int bytesToRecv = chunkSize - recvSize;
						if((bytesRecv = recv(Socket, buf, bytesToRecv > sizeof(buf) ? sizeof(buf) : bytesToRecv, 0)) <= 0)
						{
							throw CreateSocketError();
						}
						recvSize += bytesRecv;
						fout.write(buf, bytesRecv);
						cerr << "\r" << recvSize * 100 / chunkSize << "%" << flush;

                        if (recvSize * 100 / chunkSize > a) {
                            update_prozent(recvSize * 100 / chunkSize);
                            ++a;
                        }
					}
					cerr << endl;
					for(int i = 0; i < 2; ++i)
					{
						char temp;
						recv(Socket, &temp, 1, 0);
					}
				}
			}
		}
		cerr << endl << "Finished!" << endl;

	}
	catch(exception& e)
	{
		cerr << endl;
		cerr << e.what() << endl;
	}

#ifdef linux
	close(Socket); // Verbindung beenden
#else
	closesocket(Socket); // Windows-Variante
#endif

    ifstream ifstr(filename.c_str());
    std::string all;
    std::string temp;
    while (ifstr) {
		getline(ifstr, temp);

		all += temp;
		all += "\n";
    }

    return all;
}

*/

//std::string freehal::download (std::string url) {

//}

