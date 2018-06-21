# Locate FACEBOOK library
# This module defines
# FACEBOOK_LIBRARY, the name of the library to link against
# FACEBOOK_FOUND, if false, do not try to link to FACEBOOK
# FACEBOOK_INCLUDE_DIR, where to find SDL.h

FIND_PATH(FirebaseAnalytics_INCLUDE_DIR FirebaseAnalytics.framework
  HINTS
  /Frameworks/
  PATH_SUFFIXES Firebase Firebase/Analytics
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FirebaseAnalytics DEFAULT_MSG FirebaseAnalytics_INCLUDE_DIR)
