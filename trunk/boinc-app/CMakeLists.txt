cmake_minimum_required (VERSION 2.6)
project (hal2012-boincapp)

###################################################
##
##  Windows cross-compile (Compiler Server)
##
###################################################

if(MINGW)

SET(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

include_directories (.)
include_directories (/compiler/local/windows/include)
link_directories (/compiler/local/windows/lib)

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

    set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
    set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
    set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(CMAKE_USE_RELATIVE_PATHS true)
ADD_DEFINITIONS(-static-libgcc)
SET(CMAKE_CXX_LINK_EXECUTABLE" ${CMAKE_CXX_LINK_EXECUTABLE} -static-libgcc")
SET(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})

include_directories (.)
include_directories (/compiler/local/linux/include)
link_directories (/compiler/local/linux/lib)

#set(Boost_USE_STATIC_LIBS   ON)
#set(Boost_USE_MULTITHREADED ON)
#find_package( Boost 1.36.0 COMPONENTS regex serialization system REQUIRED )
#find_package( Threads )

add_executable (freehalapp hal2012-boincapp.cpp hal2012-grammar2012.cpp hal2012-parser2012.cpp hal2012-tagger2012-builtin.cpp hal2012-tagger2012.cpp hal2012-util2012.cpp)
target_link_libraries( freehalapp m dl boinc_api boinc ${CMAKE_THREAD_LIBS_INIT} boost_regex-mt )

set_target_properties("freehalapp" PROPERTIES COMPILE_FLAGS "-g")
set_target_properties("freehalapp" PROPERTIES LINK_FLAGS "-g -static-libgcc -static -pthread")


endif(MINGW)

