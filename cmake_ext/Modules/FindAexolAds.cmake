
# Locate AexolAds
# This module defines AexolAds_FOUND, AexolAds_INCLUDE_DIRS and AexolAds_LIBRARIES standard variables
#
# $AEXOLGLDIR - enviromental variable

FIND_PATH(AexolAds_INCLUDE_DIR NAMES "aex/ads/AdInterface.hpp"
    HINTS
    $ENV{AEXOLGLDIR}
    $ENV{AEXOLGL_PATH}
    PATH_SUFFIXES include
)

FIND_LIBRARY(AexolAds_LIBRARY
    NAMES AexolAds
    HINTS
    $ENV{AEXOLGLDIR}
    $ENV{AEXOLGL_PATH}
    PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64 Release
)

if(ANDROID)
    FIND_PATH(AEXLIB_ANDROID_DIR NAMES "com/aexol/aexlib/maven-metadata.xml"
        HINTS
        $ENV{AEXOLGLDIR}
        $ENV{AEXOLGL_PATH}
        PATH_SUFFIXES ".m2"
    )
endif()

SET(AexolAds_LIBRARIES "${AexolAds_LIBRARY}") 

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AexolAds DEFAULT_MSG AexolAds_LIBRARIES AexolAds_INCLUDE_DIR)
