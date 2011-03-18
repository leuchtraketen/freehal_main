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
#include "hal2009-ipc.h"
BEGIN_EXTERN_C
#include "hal2009-universal.h"
END_EXTERN_C

string download_from_url(const string& url);

int wiki_begin() {
    return 1;
}

string remove_between(const string& orig, char start, char stop) {
    if (orig.size() == 0) {
        return string();
    }

    string ret;

    int i;
    int in_gap = 0;
    for (i = 0; i < orig.size(); ++i) {
        if (orig[i] == start) {
            ++in_gap;
        }

        if (0 == in_gap) {
            ret += orig[i];
        }

        if (orig[i] == stop) {
            --in_gap;
        }
    }

    return ret;
}

string replace_spaces(const string& _s) {
    string s(_s);
    std::replace_if(s.begin(), s.end(), boost::is_any_of(" \t\r\n"), '_');
    trim_if(s, boost::is_any_of(" \t\r\n_"));
    return s;
}

string delete_bad_chars(const string& _s, short with_star, short with_underscore) {
    string s(_s);
    if (with_star) {
        erase_all(s, "*");
    }
    if (with_underscore) {
        std::replace_if(s.begin(), s.end(), boost::is_any_of("_"), ' ');
    }
    return s;
}

const string define_general_verb(const string& sentence, const string& entity) {

    int position_ist = sentence.find(" ist ");
    int position_sind = sentence.find(" sind ");
    int position_war = sentence.find(" war ");
    int position_waren = sentence.find(" waren ");
    int position_dot = sentence.find(".");
    int position_komma = sentence.find(",");
    int position_article_indef = 0;
    if (position_article_indef == 0) position_article_indef = sentence.find("Ein ");
    if (position_article_indef == 0) position_article_indef = sentence.find("Eine ");
    if (position_article_indef == 0) position_article_indef = sentence.find("ein ");
    if (position_article_indef == 0) position_article_indef = sentence.find("eine ");
    int position_article_def = 0;
    if (position_article_def == 0) position_article_def = sentence.find("Der ");
    if (position_article_def == 0) position_article_def = sentence.find("Die ");
    if (position_article_def == 0) position_article_def = sentence.find("Das ");
    if (position_article_def == 0) position_article_def = sentence.find("der ");
    if (position_article_def == 0) position_article_def = sentence.find("die ");
    if (position_article_def == 0) position_article_def = sentence.find("das ");


    if (sentence.find(" gehoeren") != string::npos && sentence.find(" gehoeren") < position_dot) {
        return "equal-pl-def";
    }

    if (position_article_def != string::npos && position_article_def < position_ist && position_ist < position_dot && position_article_def < position_komma) {
        return "equal-sg-def";
    }
    if (position_article_indef != string::npos && position_article_indef < position_ist && position_ist < position_dot && position_article_indef < position_komma) {
        return "equal-sg-indef";
    }
    if (position_article_def != string::npos && position_article_def < position_sind && position_sind < position_dot && position_article_def < position_komma) {
        return "equal-pl-def";
    }
    if (position_article_indef != string::npos && position_article_indef < position_sind && position_sind < position_dot && position_article_indef < position_komma) {
        return "equal-pl-indef";
    }
    if (position_ist < position_dot) {
        return "equal-sg";
    }
    if (position_sind < position_dot) {
        return "equal-pl";
    }

    if (position_war < position_dot) {
        return "equal-sg";
    }
    if (position_waren < position_dot) {
        return "equal-pl";
    }


    if (sentence.find(" entspri") != string::npos) {
        return "equal-sg";
    }
    if (sentence.find(" entspre") != string::npos) {
        return "equal-pl";
    }
    if (sentence.find(" nennt man ") != string::npos) {
        return "equal-pl";
    }
    if (sentence.find("bezeichnen") != string::npos) {
        return "equal-pl";
    }
    if (sentence.find("beschreibt") != string::npos) {
        return "equal";
    }
    if (sentence.find("beschreiben") != string::npos) {
        return "equal";
    }
    if (sentence.find(" und ") != string::npos) {
        return "equal-pl";
    }
    return "equal-sg";
}

string transform_sentence(const string& _sentence, const string& entity) {

    string sentence(_sentence);
    cout << "sentence: " << sentence << endl;

    vector<string> verbs;
    verbs.push_back("ist");
    verbs.push_back("sind");
    verbs.push_back("war");
    verbs.push_back("waren");
    verbs.push_back("heissen");
    verbs.push_back("versteht man");
    verbs.push_back("bezeichnet man");
    verbs.push_back("nennt man");
    verbs.push_back("entspricht");
    verbs.push_back("entsprechen");
    verbs.push_back("gehoeren zu den");
    verbs.push_back("gehoeren zu der");
    verbs.push_back("gehoeren zu");
    verbs.push_back("gehoert zu den");
    verbs.push_back("gehoert zu der");
    verbs.push_back("gehoert zu");
    verbs.push_back("entsprach");
    verbs.push_back("beschreibt");
    verbs.push_back("beschreiben");
    verbs.push_back("bezeichnet");
    
    string verb_str;

    int i;
    for (i = 0; i < verbs.size(); ++i) {
        if (verb_str.size() > 0) break;
        
        string verb = " " + verbs[i] + " ";
        
        int verb_pos = sentence.find(verb);
        if (verb_pos != string::npos && verb_pos < sentence.find(".")) {
            verb_str = sentence.substr(verb_pos + verb.size());
        }
    }
    for (i = 0; i < verbs.size(); ++i) {
        if (verb_str.size() > 0) break;
        
        string verb = " " + verbs[i] + " ";
        
        int verb_pos = sentence.find(verb);
        if (verb_pos != string::npos) {
            verb_str = sentence.substr(verb_pos + verb.size());
        }
    }
    if (verb_str.size() == 0) {
        verb_str = sentence;
    }
    
    vector<string> dot_allowed;
    dot_allowed.push_back("bspw");
    dot_allowed.push_back("mio");
    dot_allowed.push_back("mrd");
    dot_allowed.push_back("mr");
    dot_allowed.push_back("mrs");
    dot_allowed.push_back("dr");
    dot_allowed.push_back("dres");
    dot_allowed.push_back("0");
    dot_allowed.push_back("1");
    dot_allowed.push_back("2");
    dot_allowed.push_back("3");
    dot_allowed.push_back("4");
    dot_allowed.push_back("5");
    dot_allowed.push_back("6");
    dot_allowed.push_back("7");
    dot_allowed.push_back("8");
    dot_allowed.push_back("9");
    for (i = 0; i < dot_allowed.size(); ++i) {
        verb_str = replace(verb_str, dot_allowed[i] + ".", dot_allowed[i]);
        verb_str = replace(verb_str, "." + dot_allowed[i], dot_allowed[i]);
    }

    int object_end = 0;
    int first_upper = verb_str.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (first_upper != string::npos) {
        int end = verb_str.find_first_of(" ", first_upper+1);
        if (end != string::npos) {
            object_end = end;
        }
    }
    // verb_str.substr(0, end)
    
    string object = verb_str;
    int dot_pos = verb_str.find_first_of(".!?", object_end);
    if (dot_pos != string::npos) {
        object = object.substr(0, dot_pos);
    }

    cout << "object: " << object << endl;

    return object;
}

string upper (const string& _s) {
    string n = _s;

    int i;
    for (i = 0; i < n.size(); ++i) {
        if (i == 0 || (i && (n[i-1] == ' ' || n[i-1] == '\t' || n[i-1] == '\n' || n[i-1] == '\r'))) {
            n[i] = toupper(n[i]);
        }
    }

    return n;
}

string delete_articles(const string& _s) {
    string s(_s);
    
    vector<string> articles;
    articles.push_back("der");
    articles.push_back("die");
    articles.push_back("das");
    articles.push_back("den");
    articles.push_back("dem");
    articles.push_back("einer");
    articles.push_back("einen");
    articles.push_back("einem");
    articles.push_back("eine");
    articles.push_back("ein");
    
    int i;
    for (i = 0; i < articles.size(); ++i) {
        if (s.find(articles[i] + " ") < 5) {
            replace_first(s, articles[i] + " ", "");
        }
    }

    return s;
}

vector<string> search_results;
int search_results_line;
string search_results_entity;

int is_no_wiki_word(const string& word) {
    return (
        (
            word == "ich"
         || word == "du"
         || word == "er"
         || word == "sie"
         || word == "es"
         || word == "wir"
         || word == "ihr"
         || word == "sie"
         || word == "i"
         || word == "you"
         || word == "he"
         || word == "she"
         || word == "it"
         || word == "we"
         || word == "us"
         || word == "they"
         || word == "them"
        )
         ?  1
         :  0
    );
}

struct fact** search_facts_wiki_page(const string&, const string&);

EXTERN_C struct fact** search_facts_wiki(const char* _entity, short todo) {
    string entity(_entity);

    if (is_a_trivial_word(entity.c_str())) {
        return 0;
    }
    if (is_no_wiki_word(entity)) {
        return 0;
    }
    char* config_online = check_config("online", "1");
    if (strcmp("1", config_online)) {
        cout << "online == 0" << endl;
        free(config_online);
        return 0;
    }
    free(config_online);

    string entity_upper = upper(delete_articles(delete_bad_chars(entity, 1, 1)));
    string entity_to_save_upper = upper(delete_articles(delete_bad_chars(entity, 1, 0)));
    string url;
    vector<string> lines;

    /// if NEW
    if (todo == NEW) {
        string url = hal2009_get_text_language() + string(".wikipedia.org/w/index.php?title=Spezial%3ASuche&fulltext=Volltext&search=") + replace_spaces(entity_upper);
        cout << "url: " << url << endl;
        string file = download_from_url(url);

        if (file.size() == 0) {
            cout << "file.size() == 0" << endl;
            return 0;
        }
        if (search_results.size() > 0) {
            search_results.clear();
        }

        search_results_entity = entity_to_save_upper;

        boost::split(lines, file, boost::is_any_of("\n\r"));
        search_results = lines;
    }
    /// if CONTINUE
    else if (search_results.size() > 0 && search_results_entity.size() > 0) {
        lines = search_results;
    }
    else {
        cout << "return 0;" << endl;
        return 0;
    }
    
    struct fact** facts = 0;

    int i = (todo == NEW) ? 0 : search_results_line;
    
    cout << "(" << lines.size() << " lines / search_results_line = " << search_results_line << " / current_line = " << i << endl;
    for (++i; i < lines.size(); ++i) {
        string line(lines[i]);
    
        cout << "- " << line << endl;
        if (line.size() <= 1) {
            continue;
        }

        if (line.find("<li>") != string::npos && line.find("<a href=\"/wiki/") != string::npos) {
            int start = line.find("\"");
            if (start == string::npos) continue;
            ++start;

            int stop = line.find("\"", start);
            if (stop == string::npos) continue;
            line = line.substr(start, stop - start);

            cout << "page: \"" << line << "\"" << endl;

            facts = search_facts_wiki_page(line, search_results_entity);
            if (facts) {
                search_results_line = i;
                break;
            }
        }

        if (line.find("searchdidyoumean") != string::npos && line.size() > 55) {
            int start = line.find("search=", 50);
            if (start != string::npos && line.size()-start > 20) {
                start += 7;

                int stop = line.find("&", start);
                if (stop == string::npos) continue;
                string page = line.substr(start, stop - start);
                string path = "/wiki/%s" + page;

                cout << "page: \"" << page << "\"" << endl;

                facts = search_facts_wiki_page(path, search_results_entity);
                if (facts) {
                    search_results_line = i;
                    break;
                }
            }
        }
    }
    
    return facts;
}

struct fact** search_facts_wiki_page(const string& __url, const string& entity_upper) {
    if (entity_upper.size() == 0 || entity_upper.size() < 3) {
        return 0;
    }

    string url = replace_spaces(hal2009_get_text_language() + string(".wikipedia.org") + __url);
    cout << "page: url: " << url << endl;
    string file = download_from_url(url);
    
    if (file.size() == 0) {
        printf("page: Was not successful: download_from_url\n");
        return 0;
    }

    struct fact** facts = (struct fact**)calloc(sizeof(struct fact*), 2);

    vector<string> lines;
    boost::split(lines, file, boost::is_any_of("\n\r"));
    cout << "page: (" << lines.size() << " lines)" << endl;

    int in_header = 1;
    int in_table = 0;
    int in_script = 0;
    int in_ul = 0;

    int i = 0;
    for (i = 0; i < lines.size(); ++i) {
        string line = lines[i];
        
        if (line.size() <= 1) {
            continue;
        }

        if (line.find("class=\"noprint\"") != string::npos || line.find("Wechseln zu") != string::npos) {
            in_header = 0;
            continue;
        }
        if (line.find("zu diesem Stichwort gibt es nicht") != string::npos) {
            in_header = 0;
            in_script = 0;
            in_table  = 0;
            in_ul     = 1;
        }
        if (line.find("<ul") != string::npos) {
            ++in_ul;
        }
        if (line.find("</ul>") != string::npos) {
            --in_ul;
        }

        if (0 == in_header) {
            if (line.find("<table") != string::npos) {
                in_table += 1;
            }
            if (line.find("</table") != string::npos) {
                in_table -= 1;
            }
            if (line.find("<script") != string::npos) {
                in_script += 1;
            }
            if (line.find("</script") != string::npos) {
                in_script -= 1;
            }

            if (0 < in_script || 0 < in_table) {
                continue;
            }
            if (in_ul && line.find("<a href=\"/wiki/") != string::npos) {
                int start = line.find("\"");
                if (start == string::npos) continue;
                ++start;

                int stop = line.find("\"", start);
                if (stop == string::npos) continue;
                string page = line.substr(start, stop - start);

                cout << "page: \"" << page << "\"" << endl;

                struct fact** temp = search_facts_wiki_page(page, entity_upper);
                if (temp) {
                    if (facts) free(facts);
                    facts = temp;
                    break;
                }
            }
            if (line.find("action=edit") != string::npos && line.find("redlink") == string::npos) {
                continue;
            }
            if (line.find("bezeichnet") != string::npos && line.find(". ") == string::npos) {
                continue;
            }
            if (line.find("bezeichnet:") != string::npos || line.find("aus Wikipedia") != string::npos || line.find("benannt:") != string::npos || line.find("Koordinaten") != string::npos || line.find("gesichtete Version") != string::npos || line.find("latitude") != string::npos || line.find("</div") != string::npos || line.find("<li") != string::npos || line.find("Vorlage:Infobox") != string::npos || line.find("Siehe auch:") != string::npos || line.find("In diesem Artikel") != string::npos || line.find("iehe:") != string::npos || line.find("der Seite zur internen Wikipedia-Qualit") != string::npos) {
                continue;
            }

            line = remove_between(line, '(', ')');
            line = remove_between(line, '[', ']');
            line = remove_between(line, '<', '>');
            line = remove_between(line, '&', ';');

            if (line.size() == 0) {
                continue;
            }

            string ascii_text = ascii(line);
            if (ascii_text.size() == 0) {
                line = ascii_text;
            }
            boost::trim_if(line, boost::is_any_of(" \t\r\n"));

            if (line.size() < 30) {
                continue;
            }

            string general_verb = define_general_verb(line, entity_upper);
            string object = transform_sentence(line, entity_upper);
            if (object.find(":") > object.size()-5 && object.find(":") < object.size()+1) {
                break;
            }

            struct fact* fact  = (struct fact*)calloc(sizeof(struct fact), 1);
            fact->pk           = 0;
            fact->verbs        = divide_words(general_verb.c_str());
            fact->subjects     = divide_words(entity_upper.c_str());
            fact->objects      = divide_words(object.c_str());
            fact->adverbs      = divide_words("...");
            fact->extra        = divide_words("");
            fact->questionword = strdup("");
            fact->filename     = strdup("");
            fact->line         = strdup("");
            fact->truth        = 1.0;

            /// write to prot file
            {
                string file = "lang_" + string(hal2009_get_text_language()) + "/wiki/" + entity_upper[0] + "-wiki.prot";
                
                string line;
                string object_working = object;
                int end;
                while ((end = object_working.find(",", end+1)) != string::npos) {

                    int i;
                    short found_big_letter = 0;
                    for (i = 0; i < object_working.size(); ++i) {
                        if (object_working[i] >= 'A' && object_working[i] <= 'Z' && (object_working[i] == ' ' || object_working.find(" ", i) > end)) {
                            ++found_big_letter;
                        }
                    }

                    if (found_big_letter) {
                        object_working = object_working.substr(0, end);
                        break;
                    }
                    else {
                        object_working[end] = ' ';
                    }
                }
                int i;
                for (i = 0; i < object.size(); ++i) {
                    if ((object.find(" und", i) == i || object.find(" oder", i) == i) && i > 3 && object[i-1] != '-') {
                        object[i] = '\0';
                    }
                }
                cout << entity_upper << " ~~~ " << object << endl;
                if (entity_upper.size() >= 3 && entity_upper[0] != '0') {
                    ofstream prot_out(file.c_str());
                    if (prot_out.is_open()) {
                        prot_out << line;
                        prot_out.close();
                    }
                }
            }

            facts[0] = fact;

            if (object.size() > 0 && object[0] != ' ' && object[0] != '\t') {
                break;
            }
        }
    }
    
    return facts;
}


