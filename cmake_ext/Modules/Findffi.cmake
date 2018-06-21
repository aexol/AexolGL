# - Locate FFI library
# This module defines
#  FFI_LIBRARIES, the library to link against
#  FFI_FOUND, if false, do not try to link to FFI
#  FFI_INCLUDE_DIR, where to find headers.
#
# $FFI_DIR is an environment variable that would
# correspond to the ./configure --prefix=$FFI_DIR
# used in building FFI.

if(NOT ANDROID)
    find_package(PkgConfig)
endif()
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_LIBFFI libffi)
    set(FFI_LIBRARIES ${PC_LIBFFI_LIBRARIES})
    set(FFI_INCLUDE_DIR ${PC_LIBFFI_INCLUDEDIR})
else(PKG_CONFIG_FOUND)
    find_path(FFI_INCLUDE_DIR ffi.h ffitarget.h
      HINTS
        ENV FFI_DIR
      PATH_SUFFIXES 
      include
      lib/libffi-3.2.1/include
      PATHS
        /usr/X11R6
        /usr/local/X11R6
        /usr/local/X11
        /usr/freeware
        ENV GTKMM_BASEPATH
        [HKEY_CURRENT_USER\\SOFTWARE\\gtkmm\\2.4;Path]
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\gtkmm\\2.4;Path]
    )

    find_library(FFI_LIBRARY
      NAMES ffi libffi
      HINTS
        ENV FFI_DIR
      PATH_SUFFIXES lib
      PATHS
        /usr/X11R6
        /usr/local/X11R6
        /usr/local/X11
        /usr/freeware
        ENV GTKMM_BASEPATH
        [HKEY_CURRENT_USER\\SOFTWARE\\gtkmm\\2.4;Path]
        [HKEY_LOCAL_MACHINE\\SOFTWARE\\gtkmm\\2.4;Path]
    )
endif(PKG_CONFIG_FOUND)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FFI
                                  REQUIRED_VARS FFI_LIBRARY FFI_INCLUDE_DIR
                                  VERSION_VAR FFI_VERSION_STRING)
