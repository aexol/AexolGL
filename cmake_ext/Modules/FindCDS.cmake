FIND_PATH(CDS_INCLUDE_DIR cds/version.h
    HINTS
    $ENV{CDSDIR}
    $ENV{CDSINCLUDEDIR}
    PATH_SUFFIXES include
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
    )

if(IOS)
    # On iOS always search static library first.
    set(CDS_NAMES cds-s cds)
else()
    # On all other platforms prefer dynamic library.
    set(CDS_NAMES cds cds-s)
endif()

FIND_LIBRARY(CDS_LIBRARY
    NAMES ${CDS_NAMES}
    HINTS
    $ENV{CDSDIR}
    PATH_SUFFIXES lib64 lib
    PATHS
    /sw
    /opt/local
    /opt/csw
    /opt
    )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CDS DEFAULT_MSG CDS_LIBRARY CDS_INCLUDE_DIR)
