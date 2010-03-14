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

#define HAL2009_DISTRIB_NO_SERVER 1
#define HAL2009_DISTRIB_NO_MAIN 1

#include "hal2009-distrib.cpp"

int main (int argc, char** argv) {
    
    if (argc < 2) {
        printf("usage: %s \"INPUT\"\n", argv[0]);
        return 1;
    }

    const char** servers = hal2009_distrib_search_servers();
    int i = 0;
    while (servers[i]) {
        char* param = (char*)calloc(50000, 1);
        {
            const char* copy_of_input = argv[1];
            const char* copy_of_language = "de";
            
            sprintf(param, "%s:%s", copy_of_language, copy_of_input);
        }
        
        hal2009_distrib_invoke_on(servers[i], distrib_port, "hal2009_answer__1", param);
        
        free((void*)servers[i]);
        ++i;
    }
    free((void*)servers);
    return 0;
}

