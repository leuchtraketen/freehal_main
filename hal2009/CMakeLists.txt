#
# This file is part of FreeHAL 2009.
#
# Copyright(c) 2006, 2007, 2008, 2009 Tobias Schulz and contributors.
# http://freehal.org
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 3
# of the License, or any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

cmake_minimum_required (VERSION 2.6)
project (FreeHAL)


###################################################
##
##  Windows cross-compile (Compiler Server)
##
###################################################

if(MINGW)

include_directories (${FreeHAL_SOURCE_DIR}/perl5 ${FreeHAL_SOURCE_DIR}/parrot/include /compiler/boost) 
include_directories (/compiler/lib /compiler/include perl5/ parrot/include /add1/root/boinc_core_release_5_10/lib/ /add1/root/boinc_core_release_5_10/api/ /add1/root/boinc_core_release_5_10/)

add_executable (hal2009-test hal2009-test.c hal2009-perl6.c hal2009-perl5.c sqlite3.c hal2009-as-debug.c) 
target_link_libraries (hal2009-test parrot m crypt pthread dl)

add_executable (hal2009 hal2009-main.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c /compiler/lib/libperl510.a iowin32.c ioapi.c extract_with_unzipper.c unzipper.c unzip.c hal2009-as-release.c)
target_link_libraries (hal2009 m pthreadGC2 /compiler/lib/libperl510.a z) 

add_executable (runner hal2009-server.c hal2009-server.cpp /compiler/boost/libs/thread/src/tss_null.cpp hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c /compiler/lib/libperl510.a iowin32.c ioapi.c extract_with_unzipper.c unzipper.c unzip.c /compiler/boost/libs/thread/src/win32/exceptions.cpp /compiler/boost/libs/thread/src/win32/thread.cpp /compiler/boost/libs/thread/src/win32/tss_dll.cpp /compiler/boost/libs/thread/src/win32/tss_pe.cpp hal2009-crash.c precompiled/mingw/mingw_ntdll-a_RtlCaptureStackBackTrace.o hal2009-as-release.c)
target_link_libraries (runner m /compiler/lib/libboost_system-mgw-mt-1_37.a /compiler/lib/libboost_iostreams-mgw-mt-1_37.a pthread pthreadGC2 /compiler/lib/libperl510.a z msvcrt crypt32 ws2_32 mswsock wsock32 ole32 oleaut32 winmm uuid gdi32 /compiler/lib/libpthreadGC2.a /source/hal2009/mingw_ntdll.a)

add_executable (hal2009-boinc hal2009-boinc.cpp hal2009-boinc.c hal2009-perl6-dummy.c hal2009-perl5.c /compiler/lib/libpthread.a /compiler/boost/libs/thread/src/tss_null.cpp diagnostics_win.C sqlite3.c extract_with_unzipper.c unzipper.c unzip.c libcrypto.a libgdi32.a libsched.a libssl.a iowin32.c ioapi.c /compiler/lib/libperl510.a hal2009-wiki-dummy.c boinc_api.o hal2009-as-debug.c)
target_link_libraries (hal2009-boinc m pthread pthreadGC2 /compiler/lib/libperl510.a boinc boinc_api boinc_zip boinc_graphics2 z)

###################################################
##
##  Linux/Unix
##
###################################################

else(MINGW)

include_directories (${FreeHAL_SOURCE_DIR}/perl5 ${FreeHAL_SOURCE_DIR}/parrot/include) 
include_directories (/add1/root/boinc_core_release_5_10/lib/ /add1/root/boinc_core_release_5_10/api/ /add1/root/boinc_core_release_5_10/)

add_executable (hal2009 hal2009-main.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c) 
target_link_libraries (hal2009 m crypt pthread perl dl z) 

add_executable (hal2009-server hal2009-server.cpp hal2009-server.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c) 
target_link_libraries (hal2009-server m crypt pthread perl dl z boost_thread-mt boost_system-mt)

add_executable (hal2009-cgi hal2009-cgi.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-download.cpp hal2009-as-debug.c) 
target_link_libraries (hal2009-cgi m crypt pthread perl dl z boost_thread-mt boost_system-mt)

add_executable (hal2009-boinc hal2009-boinc.cpp hal2009-boinc.c hal2009-wiki-dummy.c hal2009-perl6-dummy.c /usr/lib/libperl.a hal2009-perl5.c sqlite3.c /usr/lib/libboinc_api.a /usr/lib/libboinc.a extract_with_unzipper.c unzipper.c unzip.c ioapi.c hal2009-as-debug.c) 
target_link_libraries (hal2009-boinc m crypt pthread dl boinc_api boinc z /usr/lib/libperl.a) 

add_executable (hal2009-debug hal2009-main.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-debug.c) 
target_link_libraries (hal2009-debug m crypt pthread perl dl z) 

add_executable (hal2009-server-debug hal2009-server.cpp hal2009-server.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-debug.c) 
target_link_libraries (hal2009-server-debug m crypt pthread perl dl z boost_thread-mt boost_system-mt)

set_target_properties("hal2009-debug" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("hal2009-debug" PROPERTIES LINK_FLAGS "-pg -g")
set_target_properties("hal2009-server-debug" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("hal2009-server-debug" PROPERTIES LINK_FLAGS "-pg -g")


add_executable (hal2009-intel-pentium4 hal2009-main.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c) 
target_link_libraries (hal2009-intel-pentium4 m crypt pthread perl dl z) 
add_executable (hal2009-server-intel-pentium4 hal2009-server.cpp hal2009-server.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c) 
target_link_libraries (hal2009-server-intel-pentium4 m crypt pthread perl dl z boost_thread-mt boost_system-mt)
set_target_properties("hal2009-intel-pentium4" PROPERTIES COMPILE_FLAGS "-march=pentium4 -O2 -O3")
set_target_properties("hal2009-server-intel-pentium4" PROPERTIES COMPILE_FLAGS "-march=pentium4 -O2 -O3")

add_executable (hal2009-amd-athlon-xp hal2009-main.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c) 
target_link_libraries (hal2009-amd-athlon-xp m crypt pthread perl dl z) 
add_executable (hal2009-server-amd-athlon-xp hal2009-server.cpp hal2009-server.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c) 
target_link_libraries (hal2009-server-amd-athlon-xp m crypt pthread perl dl z boost_thread-mt boost_system-mt)
set_target_properties("hal2009-amd-athlon-xp" PROPERTIES COMPILE_FLAGS "-march=athlon-xp -O2 -O3")
set_target_properties("hal2009-server-amd-athlon-xp" PROPERTIES COMPILE_FLAGS "-march=athlon-xp -O2 -O3")

endif(MINGW)




install(PROGRAMS hal2009 DESTINATION bin)
install(FILES hal2009.hal hal2009-lang-de.hal hal2009-lang-indep.hal DESTINATION /etc/freehal)
install(DIRECTORY lang_de DESTINATION /etc/freehal
	FILE_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE
	DIRECTORY_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE)

