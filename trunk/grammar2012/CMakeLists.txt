cmake_minimum_required (VERSION 2.6)
project (hal2012)


include_directories (.) 

add_executable (main main.cpp hal2012-grammar2012.cpp  hal2012-parser2012.cpp  hal2012-tagger2012-builtin.cpp  hal2012-tagger2012.cpp  hal2012-util2012.cpp)
target_link_libraries (main m dl boost_regex-mt)

add_executable (graph graph.cpp hal2012-grammar2012.cpp  hal2012-parser2012.cpp  hal2012-tagger2012-builtin.cpp  hal2012-tagger2012.cpp  hal2012-util2012.cpp)
target_link_libraries (graph m dl boost_regex-mt)

set_target_properties("main" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("main" PROPERTIES LINK_FLAGS "-pg -g")

set_target_properties("graph" PROPERTIES COMPILE_FLAGS "-pg -g")
set_target_properties("graph" PROPERTIES LINK_FLAGS "-pg -g")


install(PROGRAMS graph DESTINATION bin)
