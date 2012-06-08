
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
include_directories (/compiler/local/windows/include /compiler/local/all/include)
link_directories (/compiler/local/windows/lib)

add_executable (runner hal2012-startup.cpp hal2012-server.cpp extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2012-as-release.c hal2012-ipc.cpp hal2012-util.cpp  iowin32.c ioapi.c)
target_link_libraries (runner m pthread pthread z msvcrt crypt32 ws2_32 mswsock wsock32 ole32 oleaut32 winmm uuid gdi32 boost_iostreams-mt boost_regex-mt boost_system-mt boost_thread_win32-mt)
add_executable (runner-thread hal2012-startup.cpp hal2012-server-thread.cpp hal2012-download.cpp hal2012-wiki.cpp hal2012-perl6-dummy.cpp hal2012-perl5.cpp sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2012-as-release.c hal2012-xml.cpp hal2012-ipc.cpp  hal2012-util.cpp hal2012-talk.cpp hal2012-sql.cpp hal2012-sql-universal.cpp hal2012-universal.c hal2012-disk.c hal2012-ram.c hal2012-pro.cpp hal2012-universal-cxx.cpp  iowin32.c ioapi.c hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp)
target_link_libraries (runner-thread m pthread pthread z msvcrt crypt32 ws2_32 mswsock wsock32 ole32 oleaut32 winmm uuid gdi32 boost_iostreams-mt boost_regex-mt boost_system-mt boost_thread_win32-mt)


###################################################
##
##  Linux/Unix
##
###################################################

else(MINGW)

include_directories (.)
include_directories (/compiler/local/linux/include /compiler/local/all/include)
link_directories (/compiler/local/linux/lib)

add_executable (hal2012-server hal2012-startup.cpp hal2012-server.cpp extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2012-as-release.c hal2012-ipc.cpp hal2012-util.cpp)
target_link_libraries (hal2012-server m crypt pthread dl z boost_regex-mt boost_thread-mt boost_system-mt)

add_executable (hal2012-server-thread hal2012-startup.cpp hal2012-server-thread.cpp hal2012-download.cpp hal2012-wiki.cpp hal2012-perl6-dummy.cpp hal2012-perl5.cpp sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c hal2012-as-release.c hal2012-xml.cpp hal2012-ipc.cpp  hal2012-util.cpp hal2012-talk.cpp hal2012-sql.cpp hal2012-sql-universal.cpp hal2012-universal.c hal2012-disk.c hal2012-ram.c hal2012-pro.cpp hal2012-universal-cxx.cpp hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp)
target_link_libraries (hal2012-server-thread m crypt pthread dl z boost_regex-mt boost_thread-mt boost_system-mt)

set_target_properties("hal2012-server" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("hal2012-server" PROPERTIES LINK_FLAGS "-pg -g")
set_target_properties("hal2012-server-thread" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("hal2012-server-thread" PROPERTIES LINK_FLAGS "-pg -g")

endif(MINGW)



#install(PROGRAMS hal2012 DESTINATION bin)
#install(FILES hal2012.hal hal2012-lang-de.hal hal2012-lang-indep.hal DESTINATION /etc/freehal)
#install(DIRECTORY lang_de DESTINATION /etc/freehal
#	FILE_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE
#	DIRECTORY_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE)

