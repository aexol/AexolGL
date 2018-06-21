
# Temporary fix for .tbd stubs compatibility, until officially fixed upstream.
include("${CMAKE_ROOT}/Modules/Platform/Darwin.cmake")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".tbd" ".dylib" ".so" ".a")
