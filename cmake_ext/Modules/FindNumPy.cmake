# Locate NumPy library
# This module defines
# NUMPY_LIBRARIES Optionaly numpy lb for linking (IOS/Android)
# NUMPY_MODULE_DIR Numpy sie path.
# NUMPY_FOUND 
# NUMPY_INCLUDE_DIR, where to find SDL.h
#
# This module searches for NumPy Python module based on Python information found by pkg-config

# Find PkgConfig
if(NOT ANDROID)
    find_package(PkgConfig)
endif()

set(__numpy_subroot "site-packages/numpy")
set(__numpy_subpaths "core" "fft" "linalg" "lib" "random")
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_PYTHON python)
    if(PC_PYTHON_FOUND)
        # If PkgConfig found python.pc file, use prefix and version from it.
        set(__numpy_hints 
            ${__numpy_hints}
            "${PC_PYTHON_PREFIX}/"
            "${PC_PYTHON_PREFIX}/local/"
            "${PC_PYTHON_PREFIX}/usr/local/"
            )
        set(__numpy_include_suffixes
            ${__numpy_include_suffixes}
            "lib/python${PC_PYTHON_VERSION}/${__numpy_subroot}/core/include"
            )
 
        foreach(s ${__numpy_subpaths})
            set(__numpy_library_suffixes
                ${__numpy_library_suffixes}
                "lib/python${PC_PYTHON_VERSION}/${__numpy_subroot}/${s}"
                )
        endforeach()
        set(__numpy_module_suffixes
            ${__numpy_module_suffixes}
            "lib/python${PC_PYTHON_VERSION}/${__numpy_subroot}"
            )
    endif(PC_PYTHON_FOUND)
endif(PKG_CONFIG_FOUND)

# Try to figure out python version based on interpreter version if PkgConfig search failed
find_package(PythonInterp)
if(PYTHONINTERP_FOUND)
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
                        OUTPUT_VARIABLE __include_hint
                        ERROR_QUIET
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        )
    execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "import numpy; from os.path import dirname, realpath; print(dirname(realpath(numpy.__file__)))"
                        OUTPUT_VARIABLE __lib_hint
                        ERROR_QUIET
                        OUTPUT_STRIP_TRAILING_WHITESPACE
                        )
    foreach(s ${__numpy_subpaths})
        set(__lib_hints
            ${__lib_hints}
            "${__lib_hint}/${s}"
            )
    endforeach()
    set(__numpy_hints
        ${__include_hint} ${__lib_hints} ${__numpy_hints}
        )
    set(__numpy_include_suffixes
        ${__numpy_include_suffixes}
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__numpy_subroot}/core/include"
        )
    set(__numpy_library_suffixes
        ${__numpy_library_suffixes}
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__numpy_subroot}/core"
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__numpy_subroot}/fft"
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__numpy_subroot}/lib"
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__numpy_subroot}/linalg"
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__numpy_subroot}/random"
        )
    set(__numpy_module_suffixes
        ${__numpy_module_suffixes}
        "lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/${__numpy_subroot}"
        )
endif(PYTHONINTERP_FOUND)

# Find include path for building native libraries with numpy
if(NOT NUMPY_INCLUDE_DIR)
    find_path(NUMPY_INCLUDE_DIR numpy/arrayobject.h 
        HINTS ${__numpy_hints}
        PATH_SUFFIXES ${__numpy_include_suffixes}
        )
endif()

# Find numpy module path
find_path(NUMPY_MODULE_DIR "__init__.py" "matlib.py"
    HINTS ${__numpy_hints}
    PATH_SUFFIXES ${__numpy_module_suffixes}
    )
set(__numpy_libraries_temp 
            "_dummy"
            "multiarray"
            "multiarray_tests"
            "operand_flag_tests"
            "scalarmath"
            "struct_ufunc_test"
            "test_rational"
            "umath"
            "umath_tests"
            "fftpack_lite"
            "_compiled_base"
            "_umath_linalg"
            "lapack_lite"
            "mtrand"
            )
# TODO: Unneeded?
if(ANDROID)
    # 
    foreach(__numpy_lib_name ${__numpy_libraries_temp})
        find_library(__numpy_library_${__numpy_lib_name} NAMES ${__numpy_lib_name}${CMAKE_SHARED_LIBRARY_SUFFIX}
                    HINTS ${__numpy_hints}
                    PATH_SUFFIXES ${__numpy_library_suffixes} lib
                    )
        if(NOT __numpy_library_${__numpy_lib_name})
            message(STATUS "Could NOT find NumPy ${__numpy_lib_name}")
        else(NOT __numpy_library_${__numpy_lib_name})
            set(__numpy_libraries ${__numpy_libraries} ${__numpy_library_${__numpy_lib_name}})
        endif(NOT __numpy_library_${__numpy_lib_name})
    endforeach(__numpy_lib_name ${__numpy_libraries_temp})
    set(NUMPY_LIBRARIES ${__numpy_libraries})
else()
    # For IOS use statically linked numpy.
    find_library(NUMPY_LIBRARIES NAMES numpy
        HINTS ${__numpy_hints} ${PC_PYTHON_LIBDIR}
        PATH_SUFFIXES ${__numpy_library_suffixes}
        )
endif()
# PkgConfig search failed. Try using host interpreter to find numpy.
if(NOT NUMPY_INCLUDE_DIR)
    find_package ( PythonInterp )

    if ( PYTHONINTERP_FOUND )
      execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
                        OUTPUT_VARIABLE NUMPY_INCLUDE_DIR
                        ERROR_QUIET
                        OUTPUT_STRIP_TRAILING_WHITESPACE )
    endif ()

    # set NUMPY_INCLUDE_DIRS
    set ( NUMPY_INCLUDE_DIRS ${NUMPY_INCLUDE_DIR} )

    # version
    if ( PYTHONINTERP_FOUND )
      execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "import numpy; print(numpy.__version__)"
                        OUTPUT_VARIABLE NUMPY_VERSION_STRING
                        OUTPUT_STRIP_TRAILING_WHITESPACE )

      if ( NUMPY_VERSION_STRING )
        string ( REGEX REPLACE "([0-9]+)\\..*" "\\1" NUMPY_MAJOR_VERSION ${NUMPY_VERSION_STRING} )
        string ( REGEX REPLACE "[0-9]+\\.([0-9]+).*" "\\1" NUMPY_MINOR_VERSION ${NUMPY_VERSION_STRING} )
        string ( REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" NUMPY_PATCH_VERSION ${NUMPY_VERSION_STRING} )
      endif ()

    endif ()

    # check version
    set ( _NUMPY_VERSION_MATCH TRUE )
    if ( Numpy_FIND_VERSION AND NUMPY_VERSION )
      if ( Numpy_FIND_VERSION_EXACT )
        if ( Numpy_FIND_VERSION VERSION_EQUAL NUMPY_VERSION_STRING )
        else()
          set ( _NUMPY_VERSION_MATCH FALSE)
        endif ()
      else ()
        if ( Numpy_FIND_VERSION VERSION_GREATER NUMPY_VERSION_STRING )
          set ( _NUMPY_VERSION_MATCH FALSE )
        endif ()
      endif ()
    endif ()

    # handle REQUIRED and QUIET options
    include ( FindPackageHandleStandardArgs )
    find_package_handle_standard_args ( Numpy DEFAULT_MSG
      NUMPY_VERSION_STRING
      _NUMPY_VERSION_MATCH
      NUMPY_INCLUDE_DIR
      NUMPY_INCLUDE_DIRS
    )

    mark_as_advanced (
      NUMPY_VERSION_STRING
      NUMPY_MAJOR_VERSION
      NUMPY_MINOR_VERSION
      NUMPY_PATCH_VERSION
      NUMPY_INCLUDE_DIR
      NUMPY_INCLUDE_DIRS
    )
else(NOT NUMPY_INCLUDE_DIR)
    include(FindPackageHandleStandardArgs)
    if(ANDROID OR IOS)
    find_package_handle_standard_args(NUMPY 
        REQUIRED_VARS NUMPY_MODULE_DIR NUMPY_INCLUDE_DIR NUMPY_LIBRARIES
        )
    else()
        find_package_handle_standard_args(NUMPY 
            REQUIRED_VARS NUMPY_MODULE_DIR NUMPY_INCLUDE_DIR
            )
    endif()
endif(NOT NUMPY_INCLUDE_DIR)
