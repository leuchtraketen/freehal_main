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

include_directories (${FreeHAL_SOURCE_DIR}/perl5 ${FreeHAL_SOURCE_DIR}/parrot/include) 

add_executable (hal2009 hal2009-main.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c) 
target_link_libraries (hal2009 m crypt pthread perl dl z) 

add_executable (hal2009-3dnetz libaw_sdk.a 3dnetz.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c libaw_sdk.a) 
target_link_libraries (hal2009-3dnetz m crypt pthread perl dl z libaw_sdk.a) 

add_executable (hal2009-server hal2009-server.cpp hal2009-server.c hal2009-perl6-dummy.c hal2009-perl5.c sqlite3.c extract_with_unzipper.c unzip.c unzipper.c ioapi.c) 
target_link_libraries (hal2009-server m crypt pthread perl dl z boost_thread-mt boost_system-mt)

add_executable (hal2009-boinc hal2009-boinc.cpp hal2009-boinc.c hal2009-perl6-dummy.c /usr/lib/libperl.a hal2009-perl5.c sqlite3.c /usr/lib/boinc_graphics2.a /usr/lib/boinc_zip.a /usr/lib/boinc_api.a /usr/lib/boinc.a extract_with_unzipper.c unzipper.c unzip.c ioapi.c) 
target_link_libraries (hal2009-boinc m crypt pthread dl boinc_api boinc_zip boinc_graphics2 boinc z /usr/lib/libperl.a) 

install(PROGRAMS hal2009 DESTINATION bin)
install(FILES hal2009.hal hal2009-lang-de.hal hal2009-lang-indep.hal DESTINATION /etc/freehal)
install(DIRECTORY lang_de DESTINATION /etc/freehal
	FILE_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE
	DIRECTORY_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE)

