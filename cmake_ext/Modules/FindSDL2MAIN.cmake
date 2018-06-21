FIND_PATH(SDL2_INCLUDE_DIR SDL.h
  HINTS
  $ENV{SDL2DIR}
  $ENV{SDL2INCLUDEDIR}
  PATH_SUFFIXES include/SDL2 include SDL2
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/SDL2
  /usr/include/SDL2
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

IF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework" )
    FIND_LIBRARY(SDL2MAIN_LIBRARY
      NAMES SDL2main
      HINTS
      $ENV{SDL2DIR}
      PATH_SUFFIXES lib64 lib
      PATHS
      /sw
      /opt/local
      /opt/csw
      /opt
    )
    IF(SDL2MAIN_LIBRARY)
        IF(ANDROID)
            SET(SDL2MAIN_LIBRARY "-Wl,--whole-archive" ${SDL2MAIN_LIBRARY} "-Wl,--no-whole-archive")
        ENDIF(ANDROID)
    ENDIF(SDL2MAIN_LIBRARY)
    INCLUDE(FindPackageHandleStandardArgs)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2MAIN
                                  REQUIRED_VARS SDL2MAIN_LIBRARY)
ELSE(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
    SET(SDL2MAIN_LIBRARY "" PARENT_SCOPE)
ENDIF(NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
