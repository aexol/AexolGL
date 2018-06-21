# Locate FACEBOOK library
# This module defines
# FACEBOOK_LIBRARY, the name of the library to link against
# FACEBOOK_FOUND, if false, do not try to link to FACEBOOK
# FACEBOOK_INCLUDE_DIR, where to find SDL.h

FIND_PATH(Fabric_INCLUDE_DIR Fabric.framework
  HINTS
  /Frameworks/
  PATH_SUFFIXES Fabric include/Fabric include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Fabric DEFAULT_MSG Fabric_INCLUDE_DIR)
