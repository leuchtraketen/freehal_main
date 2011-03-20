# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER /opt/mingw32/bin/i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER /opt/mingw32/bin/i686-w64-mingw32-g++)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH /opt/mingw32/i686-w64-mingw32/ /opt/mingw32/ /compiler/local )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CMAKE_C_FLAGS "-O3")

FOREACH(type SHARED_LIBRARY SHARED_MODULE EXE)
  SET(CMAKE_${type}_C_FLAGS "-O3")
  SET(CMAKE_${type}_C_FLAGS_DEBUG "-O3")
  SET(CMAKE_${type}_C_FLAGS_RELEASE "-O3")
  SET(CMAKE_${type}_LINK_STATIC_C_FLAGS "-O3")
  SET(CMAKE_${type}_LINK_DYNAMIC_C_FLAGS "-O3")
ENDFOREACH(type)

include_directories (/opt/mingw32/i686-w64-mingw32/include /opt/mingw32/include /compiler/local/include )
link_directories (/opt/mingw32/i686-w64-mingw32/lib /opt/mingw32/lib /compiler/local/lib )
