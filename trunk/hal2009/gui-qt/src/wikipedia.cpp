#ifndef JELIZA_WIKIPEDIA
#define JELIZA_WIKIPEDIA 1

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

#include "defs.h"

using namespace std;
using freehal::log;

bool offline_mode = false;
bool www_surf_mode = true;

freehal::strings wikipedia_words;
freehal::string allWikiWords;
freehal::string firstLink;

freehal::strings listenLinkUrls;


freehal::string addNeuWikiWord (freehal::string tempStr) {
    if (!((freehal::string("+" + allWikiWords + "+")).contains(tempStr))) {
        wikipedia_words.push_back(tempStr);
        allWikiWords += "+";
        allWikiWords += tempStr;
        allWikiWords += "+";
    }
    return allWikiWords;
}

freehal::string ohneEckKlammer (freehal::string satz, bool withPipeInBrackets, freehal::string currentWort = freehal::string()) {
    satz.set(satz.toASCII());
    int inKlammer2 = 0;
    freehal::string tempStr;
    freehal::string satz2 = satz + " ";
    satz.set("");

    bool schon_gesucht = false;
    int y = 0;
    while (y < (signed) satz2.size()) {
        char t1[2];
        t1[0] = satz2[y];
        t1[1] = '\0';
        freehal::string ch(t1);
        freehal::string nch("");
        if (y + 1 < (signed) satz2.size()) {
            char t2[2];
            t2[0] = satz2[y+1];
            t2[1] = '\0';
            nch.set(t2);
        }

        if (ch == "$" && nch == "(") {
            inKlammer2++;
            y++;
        }

        if (inKlammer2 == 0 && ch.ref() != "$") {
            satz += ch;
        }

        if (ch == "|") {
            freehal::string tempStrLower = tempStr;
            tempStrLower.lower();
            if (!allWikiWords.contains(freehal::string("+" + tempStrLower + "+")) // fix lower!!!
                    && !is_similar(currentWort, tempStr)) {
                wikipedia_words.push_back(tempStr);
                allWikiWords += "+";
                allWikiWords += tempStrLower;
                allWikiWords += "+";
            }

            schon_gesucht = true;
            if (firstLink.size() < 1) {
                firstLink = tempStr;
            }
            tempStr.set("");
        }

        // cout << 5 << flush;
        if (inKlammer2 > 0 && ch.ref() != "$" && ch.ref() != string("|") && ch.ref() != string("(") && ch.ref() != string(")")) {
            tempStr += ch;
        }

        // cout << 6 << flush;
        if (ch == string(")") && nch == string("$")) {
            // cout << 6.5 << flush;
            if (!schon_gesucht) {
                // cout << 6.7 << tempStr << flush;
                freehal::string tempStrLower = tempStr;
                tempStrLower.lower();
                if (!allWikiWords.contains("+" + tempStrLower + "+")) {
                    wikipedia_words.push_back(tempStr);
                    allWikiWords += "+";
                    allWikiWords += tempStrLower;
                    allWikiWords += "+";
                }
                // cout << 6.9 << flush;
            }
            // cout << 8 << flush;
            inKlammer2--;
            // cout << 9 << flush;
            if (withPipeInBrackets) {
                // cout << 10 << flush;
                satz += tempStr;
                // cout << 11 << flush;
            }
            // cout << 12 << flush;
            if (firstLink.size() < 1) {
                firstLink = tempStr;
            }
            tempStr.set("");
            // cout << 13 << flush;
            y++;
            schon_gesucht = false;
        }
        // cout << 7 << endl << flush;

        y++;
        // cout << "2. " << y << " " << satz2.size() << "  ch " << ch << "#" << endl;
    }

    satz = satz.replace("$", "");
    satz.strip();
    return (satz);
}


freehal::strings wikipedia (freehal::string wort, int count, bool rec = false, bool with_newlines = false, int redirect = 0) {
    wort.replace(freehal::string(" "), freehal::string("_"));

    log("- Suche in der deutschen Wikipedia nach: " + wort.ref() + " [freehal::strings wikipedia()]");

    listenLinkUrls << NULL;

    freehal::string all;


///    Temporaer deaktiviert, da momentan kein Internet

    {
        freehal::string url = "http://de.wikipedia.org/w/index.php?title=" + wort + "&action=edit";
        clogger << "Url: \"" << url.ref() << "\"" << endl;

        all.set(freehal::download(url.ref()));
    }

/**
    {
        ifstream i("download.php");
        string lline;
        while (i) {
            getline(i, lline);
            all += lline;
            all += "\n";
        }
    }
**/
    all = all.toASCII();
    all = all.replace("cols='80' >", "");
    all = all.replace("[[", "$(");
    all = all.replace("]]", ")$");
    all = all.replace("{|", "[");
    all = all.replace("|}", "]");
    all = all.replace("{", "[");
    all = all.replace("}", "]");
    all = all.replace("[[", "[");
    all = all.replace("]]", "]");
    all.strip();
    if (all.size() < 2) {
        return freehal::strings();
    }

    freehal::strings lines_;
    util::split(all, string("\n"), lines_);

    freehal::strings saetze;

    unsigned int x = 0;
    for (; x < lines_.size(); ++x) {
        freehal::string line(lines_[x]);
        line.strip();

        if (line.contains("<textarea")) {
            ++x;
            break;
        }
    }

    freehal::strings lines;
    for (; x < lines_.size(); ++x) {
        freehal::string line(lines_[x]);
        line.strip();

        if (line.contains("Bild:")) {
            continue;
        }
        if (line.contains("==")) {
            continue;
        }

        lines.push_back(line);
    }
    x = 0;

    unsigned long open = 0;

    char array[2];
    array[1] = '\0';
    freehal::strings lines_cleaned;
    for (unsigned int y = x; y < lines.size() && y < x + 80; ++y) {
        freehal::string line_clean;

        firstLink.set("");
        lines[y].set(ohneEckKlammer(lines[y], true, wort));
        firstLink.strip();
        if (firstLink.size() && lines[y].startswith("*")) {
            listenLinkUrls.push_back(firstLink);
        }

        for (unsigned z = 0; z < lines[y].size(); ++z) {
            if (lines[y][z] == '[') {
                ++open;
            }
            else if (lines[y][z] == ']') {
                --open;
            }
            else if (open == 0) {
                array[0] = lines[y][z];

                line_clean += array;
            }
        }

        if (line_clean.size() > 0) {
            if (line_clean.contains("</textarea>")) {
                break;
            }
            if (line_clean.contains("Bild:")) {
                continue;
            }
            if (line_clean.contains("==")) {
                continue;
            }

            lines_cleaned.push_back(line_clean);
        }
    }
    x = 0;

    for (; x < lines_cleaned.size(); ++x) {
        freehal::string line(lines_cleaned[x]);
        line.strip();

///        line.set(ohneEckKlammer(line, true, wort));
        saetze.push_back(line);
        ++x;
        break;
    }

    unsigned int is_list = 0;

    for (; x < lines_cleaned.size(); ++x) {
        cout << lines_cleaned[x].ref() << endl;
        if (lines_cleaned[x].get_strip().contains("*")) {
            ++is_list;
            freehal::string line(lines_cleaned[x]);
            line = line.replace("*", "");
            line.strip();

            stringstream sst;
            sst << is_list;
            string num;
            sst >> num;
            line = num + ". " + line;
            line.strip();

            if (saetze.size() && line.size() > 0) {
                saetze[0] += "\n";
                saetze[0] += line;
            }
        }
        else {
            break;
        }
    }

    if (is_list) {
        if (saetze.size()) {
            saetze[0] += "\n";
            saetze[0] += freehal::string("Wenn du mehr ueber eins dieser Dinge erfahren "
                    "moechtest, gib bitte jetzt die davor stehende Nummer ein!\n"
                    "(Aber auch bitte NUR die Nummer, sonst kommt mein Gehirn durcheinander!)");
        }
    }

    cout << endl << endl << endl;
    for (int x = 0; x < lines_cleaned.size(); x++) {
        cout << lines_cleaned[x].ref() << endl;
    }
    cout << endl << endl << endl;

    freehal::string rsatz;
    if (saetze.size() > 0) {
        rsatz = saetze[0];
    }


    if (rsatz.startswith("#REDIRECT")) {
        rsatz = rsatz.replace("#REDIRECT", "");
        rsatz.strip();
        if (rsatz.size()) {
            if (redirect < 3) {
                return (wikipedia(rsatz, count, rec, with_newlines, ++redirect));
            }
        }
    }

    return saetze;
}

freehal::string search_in_wikipedia_with_newlines(freehal::string wort) {
    if (offline_mode) {
        return freehal::string();
    }

    cout << "- Wort zum Nachschlagen: " << wort.ref() << endl;
    freehal::strings a;
    wort = wort.replace("#", ";");
    util::split(wort, ";", a);
    if (a.size() > 1) {
        wort = a[0];
    }

    wort.strip();
    freehal::string orig_wort = wort;
    wort.lower();
    freehal::string firstchar;
    (firstchar.set(wort.substr(0, 1))).upper();
    wort.set(wort.substr(1, wort.size()));
    wort.set(firstchar + wort);
    wort.strip();

    freehal::strings satz;

    if (wort.size() < 1) {
        return freehal::string();
    }

    clogger << "- Wort zum Nachschlagen: " << wort.ref() << endl;
    satz << NULL << wikipedia(wort, 1, true, true, 0);
    if (satz.size() < 1) {
        clogger << "- Wort zum Nachschlagen: " << orig_wort.ref() << endl;
        satz << NULL << wikipedia(orig_wort, 1, true, true, 0);
        if (satz.size() < 1) {
            clogger << "- Wort zum Nachschlagen: " << orig_wort.get_upper().ref() << endl;
            satz << NULL << wikipedia(orig_wort.get_upper(), 1, true, true, 0);
        }
    }

    freehal::string rsatz;
    if (satz.size() > 0) {
        rsatz = satz[0];
    }

    rsatz.replace(freehal::string("&lt;!"), freehal::string(""));
    rsatz.replace(freehal::string("&lt;"), freehal::string(""));
    rsatz.replace(freehal::string("&gt;"), freehal::string(""));
    rsatz.replace(freehal::string("&"), freehal::string(""));
    rsatz.replace(freehal::string("'"), freehal::string(""));
    rsatz.replace(freehal::string("amp;"), freehal::string("&"));
    rsatz.replace(freehal::string("nbsp;"), freehal::string(" "));

    rsatz.strip();

    return rsatz;
}

freehal::strings search_in_wikipedia_acticle(freehal::string wort) {
    if (offline_mode) {
        return freehal::strings();
    }

    freehal::strings a;
    wort = wort.replace("#", ";");
    util::split(wort, ";", a);
    if (a.size() > 1) {
        wort = a[0];
    }

    wort.strip();
    freehal::string orig_wort = wort;
    wort.lower();
    freehal::string firstchar;
    (firstchar.set(wort.substr(0, 1))).upper();
    wort.set(wort.substr(1, wort.size()));
    wort.set(firstchar + wort);
    wort.strip();

    freehal::strings satz;

    if (wort.size() < 1) {
        return freehal::strings();
    }

    clogger << "- Wort zum Nachschlagen: " << wort.ref() << endl;
    satz << NULL << wikipedia(wort, 1, true, true, 0);
    if (satz.size() < 1) {
        clogger << "- Wort zum Nachschlagen: " << orig_wort.ref() << endl;
        satz << NULL << wikipedia(orig_wort, 1, true, true, 0);
        if (satz.size() < 1) {
            clogger << "- Wort zum Nachschlagen: " << orig_wort.get_upper().ref() << endl;
            satz << NULL << wikipedia(orig_wort.get_upper(), 1, true, true, 0);
        }
    }

    for (int x = 0; x < satz.size(); ++x) {
        freehal::string& rsatz = satz[x];

        rsatz.replace(freehal::string("&lt;!"), freehal::string(""));
        rsatz.replace(freehal::string("&lt;"), freehal::string(""));
        rsatz.replace(freehal::string("&gt;"), freehal::string(""));
        rsatz.replace(freehal::string("&"), freehal::string(""));
        rsatz.replace(freehal::string("'"), freehal::string(""));
        rsatz.replace(freehal::string("amp;"), freehal::string("&"));
        rsatz.replace(freehal::string("nbsp;"), freehal::string(" "));

        rsatz.strip();
    }

    return satz;
}


#endif
