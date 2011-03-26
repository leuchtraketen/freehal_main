
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

include_directories (.) 

add_executable (hal2009 hal2009-main.cpp hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c iowin32.c ioapi.c extract_with_unzipper.c unzipper.c unzip.c hal2009-as-release.c hal2009-xml.cpp hal2009-ipc.cpp  hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp hal2009-wiki.cpp hal2009-download.cpp)
target_link_libraries (hal2009 m pthread z)

add_executable (runner hal2009-download.cpp hal2009-wiki.cpp hal2009-server.cpp hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c iowin32.c ioapi.c extract_with_unzipper.c unzipper.c unzip.c hal2009-as-release.c hal2009-xml.cpp hal2009-ipc.cpp  hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp)
target_link_libraries (runner m pthread pthread z msvcrt crypt32 ws2_32 mswsock wsock32 ole32 oleaut32 winmm uuid gdi32 boost_iostreams-mt boost_system-mt boost_thread_win32-mt)

#set_target_properties("runner" PROPERTIES COMPILE_FLAGS "-DBOOST_PROCESS_WINDOWS_USE_NAMED_PIPE=1 ")

#add_executable (hal2009-boinc hal2009-boinc.cpp hal2009-boinc.c hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c extract_with_unzipper.c unzipper.c unzip.c iowin32.c ioapi.c hal2009-wiki-dummy.c hal2009-as-debug.c hal2009-xml.cpp hal2009-ipc.cpp hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp hal2009-download.cpp)
#target_link_libraries (hal2009-boinc m pthread pthreadGC2 boinc boinc_api boinc_zip boinc_graphics2 z)

###################################################
##
##  Linux/Unix
##
###################################################

else(MINGW)

include_directories (.) 
include_directories (${FreeHAL_SOURCE_DIR}/perl5 ${FreeHAL_SOURCE_DIR}/parrot/include) 
include_directories (/add1/root/boinc_core_release_5_10/lib/ /add1/root/boinc_core_release_5_10/api/ /add1/root/boinc_core_release_5_10/)

add_executable (hal2009 hal2009-main.cpp hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c hal2009-xml.cpp hal2009-ipc.cpp hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp hal2009-wiki.cpp hal2009-download.cpp)
target_link_libraries (hal2009 m crypt pthread dl z boost_thread-mt boost_system-mt)

add_executable (hal2009-server hal2009-server.cpp hal2009-download.cpp hal2009-wiki.cpp hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-release.c hal2009-xml.cpp hal2009-ipc.cpp  hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp)
target_link_libraries (hal2009-server m crypt pthread dl z boost_thread-mt boost_system-mt)

add_executable (hal2009.cgi hal2009-cgi.c hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-download.cpp hal2009-as-debug.c hal2009-xml.cpp hal2009-ipc.cpp hal2009.cpp hal2009-sql.c hal2009.cpp hal2009-sql.c hal2009-universal.cpp)
target_link_libraries (hal2009.cgi m crypt pthread dl z boost_thread-mt boost_system-mt)

add_executable (hal2009-boinc hal2009-boinc.cpp hal2009-boinc.c hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c /usr/lib/libboinc_api.a /usr/lib/libboinc.a extract_with_unzipper.c unzipper.c unzip.c ioapi.c hal2009-as-debug.c hal2009-xml.cpp hal2009-ipc.cpp hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp hal2009-wiki.cpp hal2009-download.cpp)
target_link_libraries (hal2009-boinc m crypt pthread dl boinc_api boinc z boost_thread-mt boost_system-mt)

add_executable (hal2009-debug hal2009-main.cpp hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-debug.c hal2009-xml.cpp hal2009-ipc.cpp hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp hal2009-wiki.cpp hal2009-download.cpp)
target_link_libraries (hal2009-debug m crypt pthread dl z boost_thread-mt boost_system-mt)

add_executable (hal2009-server-debug hal2009-server.cpp hal2009-download.cpp hal2009-wiki.cpp hal2009-perl6-dummy.cpp hal2009-perl5.cpp sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2009-as-debug.c hal2009-xml.cpp hal2009-ipc.cpp hal2009.cpp hal2009-sql.c hal2009-pro.cpp hal2009-universal.cpp)
target_link_libraries (hal2009-server-debug m crypt pthread dl z boost_thread-mt boost_system-mt)

set_target_properties("hal2009-debug" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("hal2009-debug" PROPERTIES LINK_FLAGS "-pg -g")
set_target_properties("hal2009-server-debug" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("hal2009-server-debug" PROPERTIES LINK_FLAGS "-pg -g")


#set_target_properties("hal2009-intel-pentium4" PROPERTIES COMPILE_FLAGS "-march=pentium4 -O2 -O3")
#set_target_properties("hal2009-amd-athlon-xp" PROPERTIES COMPILE_FLAGS "-march=athlon-xp -O2 -O3")

endif(MINGW)




install(PROGRAMS hal2009 DESTINATION bin)
install(FILES hal2009.hal hal2009-lang-de.hal hal2009-lang-indep.hal DESTINATION /etc/freehal)
install(DIRECTORY lang_de DESTINATION /etc/freehal
	FILE_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE
	DIRECTORY_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE)

