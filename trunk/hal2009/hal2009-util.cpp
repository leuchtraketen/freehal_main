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

#include "hal2009.h"

#include "hal2009-util.h"
#include "hal2009-startup.h"

EXTERN_C int halusleep(double seconds) {
    seconds /= 1000;
#ifdef _WIN32
    Sleep((int)(1000*seconds));
#else
    double end_time = time(NULL) + seconds - 0.01;
    while (1) {
        if (seconds >= 1) {
            sleep((unsigned int) seconds);
        } else {
            usleep((int)fmod(seconds*1000000, 1000000));
        }
        seconds = end_time - time(NULL);
        if (seconds <= 0) break;
    }
#endif
}
EXTERN_C int halsleep(double seconds) {
    halusleep(seconds*1000);
}

string& replace (string& in, const string& rep, const string& wit) {
  int pos;
  int last_pos = 0;
  while (true) {
    pos = in.find(rep, last_pos);
    if (pos == -1) {
      break;
    } else {
      in.erase(pos, rep.length());
      in.insert(pos, wit);
      last_pos = pos + wit.size();
    }
  }
  return in;
}

/* UTF8/Latin --> ASCII */
string ascii(const string& not_ascii) {
    string ascii(not_ascii);

    ascii = replace(ascii, "Ã¼", "ue");
	ascii = replace(ascii, "Ã", "ss");
	ascii = replace(ascii, "Ã¤", "ae");
	ascii = replace(ascii, "ü", "ue");
	ascii = replace(ascii, "ß", "ss");
	ascii = replace(ascii, "ä", "ae");
	ascii = replace(ascii, "ö", "oe");
	ascii = replace(ascii, "Ü", "Ue");
	ascii = replace(ascii, "Ä", "Ae");
	ascii = replace(ascii, "Ö", "Oe");
	ascii = replace(ascii, "Ã¢ÂÂ", "\"");
	ascii = replace(ascii, "Ã¢ÂÂ", "\"");
	ascii = replace(ascii, "&lt;/br&gt;", " ");

    string _ascii = ascii;
    ascii = "";
    int x;
    for (x = 0; x < _ascii.size(); x++) {
        short y = (short)(_ascii[x]);

        if (y == 228) {
            ascii += "ae";
        }
        else if (y == 196) {
            ascii += "Ae";
        }
        else if (y == 252) {
            ascii += "ue";
        }
        else if (y == 220) {
            ascii += "Ue";
        }
        else if (y == 246) {
            ascii += "oe";
        }
        else if (y == 214) {
            ascii += "Oe";
        }
        else if (y == 223) {
            ascii += "ss";
        }
        else if (y == 226) {
            x += 2;
        }
        else if (y > 129) {
            x += 1;
        }

        else {
            char _char[2];
            _char[0] = y;
            _char[1] = '\0';
            ascii += _char;
        }
    }

    return ascii;
}

EXTERN_C char* check_config (const char* name, const char* _default) {
    const char* config_file = "config.txt";
    ifstream i;
    if (!i.is_open()) {
        i.open("config.txt");
        if (i.is_open()) config_file = "config.txt";
    }
    if (!i.is_open()) {
        i.open("../config.txt");
        if (i.is_open()) config_file = "../config.txt";
    }
    if (!i.is_open()) {
        i.open("../../config.txt");
        if (i.is_open()) config_file = "../../config.txt";
    }

    if (i) {
        string temp;
        while (std::getline(i, temp)) {
            if (temp.find(name) != string::npos) {
                temp = replace(temp, " =", "=");
                temp = replace(temp, "= ", "=");
                temp = replace(temp, "\n", "=");
                temp = replace(temp, "\r", "=");
                temp = replace(temp, name, "");
                temp = replace(temp, "=", "");

                i.close();
                if (string(name).find("limit") != string::npos) {
                    cout << config_file << ": " << name << " = " << temp << ", default " << _default << endl;
                }

                return strdup(temp.c_str());
            }
        }
        i.close();
    }

    FILE* o = fopen(config_file, "a");
    if (o) {
        fprintf(o, "%s = %s\n", name, _default);
        fclose(o);
    }

    return strdup(_default);
}

EXTERN_C char* halgetline(FILE *handle) {
    char *line;
    char zeichen;
    int length;

    length = 0;
    line = NULL;
    zeichen = fgetc(handle);

    if(zeichen == '\n') {
        line = (char*)malloc(sizeof(char)*2);
        line[0] = zeichen;
        line[1] = '\0';
        return line;
    }

    if(zeichen == EOF) {
        return NULL;
    }

    line = (char*)malloc(LINE_SIZE+1);
    int index = 0;
    line[index] = zeichen;
    ++index;
    do {
        zeichen = fgetc(handle);
        if(zeichen == '\n') {
            line[index] = zeichen;
            ++index;
            break;
        }

        if (EOF == zeichen) { /* feof() verwenden? */
            break;
        }

        line[index] = zeichen;
        ++index;
    }
    while(1 && index < LINE_SIZE - 1);

    line[index] = 0;

    return line;
}

EXTERN_C int network_lock(const char* c) {
    string idstr = string("LOCK:")+c;
    cerr << idstr << endl;
    string temp;
    while (std::getline(cin, temp)) {
        if (strstr(temp.c_str(), idstr.c_str())) {
            return 1;
        }
    }
}

EXTERN_C int network_unlock(const char* c) {
    string idstr = string("UNLOCK:")+c;
    cerr << idstr << endl;
    string temp;
    while (std::getline(cin, temp)) {
        if (strstr(temp.c_str(), idstr.c_str())) {
            return 1;
        }
    }
}

std::vector<std::string>* simple_split (const string text, const string search) {
    std::vector<std::string>* result = new std::vector<std::string>();
    boost::algorithm::split(*result, text, is_any_of(search.c_str()));
    return result;
}

EXTERN_C int cstat (char* c, struct stat* s) {
    return stat(c, s);
}

