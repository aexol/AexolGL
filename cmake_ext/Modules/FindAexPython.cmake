# Locate Aex Python module
# This module defines
# AEXPYTHON_LIBRARIES, the name of the library to link against
# AEXPYTHON_MODULE_DIR, the name of the library to link against
# AEXPYTHON_FOUND, if false, do not try to link to AexLib
# AEXPYTHON_INCLUDE_DIR, where to find SDL.h
#
# This module searches for AexLib Python module based on Python information found by pkg-config

# Find PkgConfig
find_package(PkgConfig)

set(__aexlib_subroot "site-packages/AexLib")
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_PYTHON python)
    if(PC_PYTHON_FOUND)
        # If PkgConfig found python.pc file, use prefix and version from it.
        set(__aexlib_hints 
            ${__aexlib_hints}
            "${PC_PYTHON_PREFIX}/"
            "${PC_PYTHON_PREFIX}/local/"
            "${PC_PYTHON_PREFIX}/usr/local/"
            )

        set(__aexlib_library_suffixes
            ${__aexlib_library_suffixes}
            "lib/python${PC_PYTHON_VERSION}/${__aexlib_subroot}"
            )
        set(__aexlib_module_suffixes
            ${__aexlib_module_suffixes}
            "lib/python${PC_PYTHON_VERSION}/${__aexlib_subroot}"
            )
    endif(PC_PYTHON_FOUND)
endif(PKG_CONFIG_FOUND)

if(NOT PKG_CONFIG_FOUND OR NOT PC_PYTHON_FOUND)
    # Try to figure out python version based on interpreter version if PkgConfig search failed
    find_package(PythonInterp)
    if(PYTHONINTERP_FOUND)
        set(__aexlib_library_suffixes
            ${__aexlib_library_suffixes}
            "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__aexlib_subroot}"
            )
        set(__aexlib_module_suffixes
            ${__aexlib_module_suffixes}
            "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__aexlib_subroot}"
            )
    endif(PYTHONINTERP_FOUND)
endif(NOT PKG_CONFIG_FOUND OR NOT PC_PYTHON_FOUND)

# Find aexlib module path
find_path(AEXPYTHON_MODULE_DIR "__init__.py" 
    HINTS ${__aexlib_hints}
    PATH_SUFFIXES ${__aexlib_module_suffixes}
    )
SET(__aexlib_libraries_temp 
    "Aex"
    )

# Find native aexlib libraries
if(NOT IOS)
    # For most libraries aexlib will contatin dynamicly loaded libraries
    foreach(__aexlib_lib_name ${__aexlib_libraries_temp})
        find_library(__aexlib_library_${__aexlib_lib_name} NAMES ${__aexlib_lib_name}${CMAKE_SHARED_LIBRARY_SUFFIX}
                    HINTS ${__aexlib_hints}
                    PATH_SUFFIXES ${__aexlib_library_suffixes}
                    )
        if(NOT __aexlib_library_${__aexlib_lib_name})
            message(STATUS "Could NOT find AexLib (missing: ${__aexlib_lib_name})")
            unset(__aexlib_libraries)
            break()
        else(NOT __aexlib_library_${__aexlib_lib_name})
            set(__aexlib_libraries ${__aexlib_libraries} ${__aexlib_library_${__aexlib_lib_name}})
        endif(NOT __aexlib_library_${__aexlib_lib_name})
    endforeach(__aexlib_lib_name ${__aexlib_libraries_temp})
    set(AEXPYTHON_LIBRARIES ${__aexlib_libraries})
else(NOT IOS)
    # For IOS use static modules.
    find_library(AEXPYTHON_LIBRARIES NAMES aex_python
        HINTS ${__aexlib_hints} ${PC_PYTHON_LIBDIR}
        PATH_SUFFIXES ${__aexlib_library_suffixes} lib
        )
endif(NOT IOS)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AEXPYTHON REQUIRED_VARS AEXPYTHON_MODULE_DIR AEXPYTHON_LIBRARIES)
