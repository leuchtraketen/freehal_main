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

#include "hal2012.h"

#ifndef HAL2009_BOINC
#define HAL2009_BOINC 1

#if defined(__cplusplus) /* GENERATED */

BEGIN_EXTERN_C

END_EXTERN_C
#endif /* GENERATED */

// functions begin
#if defined(__cplusplus) /* GENERATED */
double uniform_deviate ( int seed );
int boinc_sleep_if_slow(long t);
int is_slow();
int main (int argc, char** argv);
unsigned time_seed();
void* app_thread (void* p);
void* cpu_thread (void* p);
void* hal2012_handle_signal(void* arg);
#endif /* GENERATED */
// functions end
#endif /* HAL2009_BOINC */

