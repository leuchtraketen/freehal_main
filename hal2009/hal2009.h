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

#ifndef HAL2009
#define HAL2009 1

#define FULL_NAME "FreeHAL"
#define FULL_VERSION 00000

// C or C++? Windows or POSIX?
#if defined (USE_CXX) || defined(__cplusplus)
#   define __cplusplus 1
#   define USE_CXX 1
#   define CXX 1
#endif
#if defined (__MINGW) || defined(__MINGW32__)
#   define WINDOWS 1
#endif

#ifdef CXX
#   define EXTERN_C extern "C" 
#   define BEGIN_EXTERN_C extern "C" {
#   define END_EXTERN_C }
#else
#   define EXTERN_C 
#   define BEGIN_EXTERN_C 
#   define END_EXTERN_C 
#endif

// include Freehal headers
#ifdef CXX
#   include "hal2009-cxx.h"
    extern "C" {
#       include "hal2009-c.h"
    }
#else
#   include "hal2009-c.h"
#endif

#include "hal2009-as.h"

#endif /* HAL2009 */

