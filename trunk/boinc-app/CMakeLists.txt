cmake_minimum_required (VERSION 2.6)
project (hal2012-boincapp)

###################################################
##
##  Windows cross-compile (Compiler Server)
##
###################################################

if(MINGW)

include_directories (. /compiler/local/boinc-win32/include)
link_directories (. /compiler/local/boinc-win32/lib)

add_executable (freehalapp hal2012-boincapp.cpp hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp)
target_link_libraries (freehalapp m boost_regex-mt boost_serialization-mt boinc_api boinc)

set_target_properties("freehalapp" PROPERTIES COMPILE_FLAGS "-g")
set_target_properties("freehalapp" PROPERTIES LINK_FLAGS "-g -static-libgcc")

###################################################
##
##  Linux/Unix
##
###################################################

else(MINGW)

include_directories (. ~/boinc/include/)
link_directories (. ~/boinc/lib/)

add_executable (freehalapp hal2012-boincapp.cpp hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp)
target_link_libraries (freehalapp m dl boost_regex-mt boost_serialization-mt boinc_api boinc)

set_target_properties("freehalapp" PROPERTIES COMPILE_FLAGS "-g")
set_target_properties("freehalapp" PROPERTIES LINK_FLAGS "-g -static-libgcc")

endif(MINGW)

