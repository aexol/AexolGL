FIND_PATH(JSONCPP_INCLUDE_DIR json/json.h
  HINTS
  $ENV{JSONCPPDIR}
  $ENV{JSONCPPINCLUDEDIR}
  PATH_SUFFIXES include include/jsoncpp
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(JSONCPP_LIBRARY
  NAMES jsoncpp Jsoncpp
  HINTS
  $ENV{JSONCPPDIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  /sw
  /opt/local
  /opt/csw
  /opt
)

FIND_LIBRARY(JSONCPP_LIBRARY_DEBUG
  NAMES jsoncppd Jsoncppd
  HINTS
  $ENV{JSONCPPDIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  /sw
  /opt/local
  /opt/csw
  /opt
)
if(JSONCPP_LIBRARY_DEBUG)
    set(JSONCPP_LIBRARY optimized "${JSONCPP_LIBRARY}" debug "${JSONCPP_LIBRARY_DEBUG}")
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JSONCPP DEFAULT_MSG JSONCPP_LIBRARY JSONCPP_INCLUDE_DIR)
