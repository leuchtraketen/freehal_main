cmake_minimum_required (VERSION 2.6)
project (hal2012)


include_directories (.) 

add_executable (test test.cpp hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp hal2012-xml2012.cpp hal2012-diskdb2012.cpp hal2012-filterfacts2012.cpp)
target_link_libraries (test m dl boost_regex-mt boost_serialization-mt boost_filesystem-mt boost_system-mt)

add_executable (shell shell.cpp hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp hal2012-xml2012.cpp hal2012-diskdb2012.cpp hal2012-filterfacts2012.cpp)
target_link_libraries (shell m dl boost_regex-mt boost_serialization-mt boost_filesystem-mt boost_system-mt readline)

add_executable (graph graph.cpp hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp hal2012-xml2012.cpp hal2012-diskdb2012.cpp hal2012-filterfacts2012.cpp)
target_link_libraries (graph m dl boost_regex-mt boost_serialization-mt boost_filesystem-mt boost_system-mt)

set_target_properties("test" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("test" PROPERTIES LINK_FLAGS "-pg -g")

set_target_properties("shell" PROPERTIES COMPILE_FLAGS "-g -DREADLINE -O2 -O3")
set_target_properties("shell" PROPERTIES LINK_FLAGS "-g -O2 -O3")

set_target_properties("graph" PROPERTIES COMPILE_FLAGS " -g")
set_target_properties("graph" PROPERTIES LINK_FLAGS " -g")


install(PROGRAMS graph DESTINATION bin)
install(PROGRAMS shell DESTINATION bin)
install(DIRECTORY lang_de DESTINATION share/freehal
	FILE_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE
	DIRECTORY_PERMISSIONS WORLD_WRITE GROUP_WRITE OWNER_WRITE)
