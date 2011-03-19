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

#define NOT_HEADER_ONLY 1
#include <dirent.h>
#include "hal2009.h"

int unzipper(const char *zipfilename);


void __extract__() {
    FILE* do_not_extract = fopen(".do-not-extract", "r");
    if ( !do_not_extract ) {
        int i;
        for (i = 0; i < 10000; ++i ) {
            char* filename = malloc(5002);
            snprintf(filename, 5000, "fh-%d.zip", i);
            unzipper(filename);
            snprintf(filename, 5000, "../../projects/freehal.net_freehal_at_home/fh-%d.zip", i);
            unzipper(filename);
            snprintf(filename, 5000, "../../projects/www.freehal.net_freehal_at_home/fh-%d.zip", i);
            unzipper(filename);

            int j;
            for (j = 0; j < 50; ++j ) {
                snprintf(filename, 5000, "../../projects/freehal.net_freehal_at_home/fh-%d-%d.zip", j, i);
                unzipper(filename);
                snprintf(filename, 5000, "../../projects/www.freehal.net_freehal_at_home/fh-%d-%d.zip", j, i);
                unzipper(filename);
            }

            free(filename);
        }
    }
}



void extract() {
    FILE* do_not_extract = fopen(".do-not-extract", "r");
    if ( !do_not_extract ) {
        DIR *hdir;

#if defined (__MINGW) || defined(__MINGW32__)
//      struct direct *entry;
        struct dirent *entry;
#else
        struct dirent *entry;
#endif

        hdir = opendir(".");
        do {
            entry = readdir(hdir);
            if (entry) {
                unzipper(entry->d_name);
            }
        } while (entry);
        closedir(hdir);

        hdir = opendir("../../projects/freehal.net_freehal_at_home");
        if (hdir) {
        do {
            entry = readdir(hdir);
            if (entry) {
                char name[999];
                strcpy(name, "../../projects/freehal.net_freehal_at_home/");
                strcat(name, entry->d_name);
                unzipper(name);
            }
        } while (entry);
        }
        closedir(hdir);

        hdir = opendir("../../projects/www.freehal.net_freehal_at_home");
        if (hdir) {
        do {
            entry = readdir(hdir);
            if (entry) {
                char name[999];
                strcpy(name, "../../projects/www.freehal.net_freehal_at_home/");
                strcat(name, entry->d_name);
                unzipper(name);
            }
        } while (entry);
        }
        closedir(hdir);
    }
}



