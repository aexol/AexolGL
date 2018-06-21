IF(NOT Assimp_NO_SYSTEM_PATHS)
    IF(NOT ANDROID)
        FIND_PATH(
            assimp_INCLUDE_DIRS
            NAMES assimp/postprocess.h assimp/scene.h assimp/version.h assimp/config.h assimp/cimport.h
            PATH_SUFFIXES include
            PATHS /usr/local/
            /usr/

            )

        FIND_LIBRARY(
            assimp_LIBRARIES
            NAMES assimp
            PATH_SUFFIXES lib
            PATHS /usr/local/
            /usr/
            )

    ELSE(NOT ANDROID)
        FIND_PATH(
            assimp_INCLUDE_DIRS
            NAMES assimp/postprocess.h assimp/scene.h assimp/version.h assimp/config.h assimp/cimport.h
            $ENV{ASSIMP_DIR}
            /include/
            CMAKE_NO_SYSTEM_PATH
            )

        FIND_LIBRARY(
            assimp_LIBRARIES
            NAMES assimp
            HINTS
            $ENV{ASSIMP_DIR}
            /lib/
            CMAKE_NO_SYSTEM_PATH
            )

    ENDIF(NOT ANDROID)
ELSE(NOT Assimp_NO_SYSTEM_PATHS)
    FIND_PATH(
        assimp_INCLUDE_DIRS
        NAMES assimp/postprocess.h assimp/scene.h assimp/version.h assimp/config.h assimp/cimport.h
        HINTS $ENV{ASSIMP_DIR}/include
        NO_CMAKE_SYSTEM_PATH
        )

    FIND_LIBRARY(
        assimp_LIBRARIES
        NAMES assimp 
        HINTS $ENV{ASSIMP_DIR}/lib

        NO_CMAKE_SYSTEM_PATH

        )
ENDIF(NOT Assimp_NO_SYSTEM_PATHS)
FIND_LIBRARY(
    assimp_LIBRARIES_debug
    NAMES assimpd
    PATH_SUFFIXES lib
    PATHS /usr/local/
    /usr/
    )
if(assimp_LIBRARIES)
    if(assimp_LIBRARIES_debug)
        set(assimp_LIBRARIES optimized "${assimp_LIBRARIES}" debug "${assimp_LIBRARIES_debug}")
    endif()
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(assimp DEFAULT_MSG assimp_LIBRARIES assimp_INCLUDE_DIRS)
