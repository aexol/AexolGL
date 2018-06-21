# Locate FACEBOOK library
# This module defines
# FACEBOOK_FOUND, if false, do not try to link to FACEBOOK
# FACEBOOK_INCLUDE_DIR, where to find SDL.h

FIND_PATH(FacebookSDK_INCLUDE_DIR FBSDKCoreKit.framework
  HINTS
  /Frameworks/
  PATH_SUFFIXES FacebookSDK include/FacebookSDK include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( FacebookSDK DEFAULT_MSG FacebookSDK_INCLUDE_DIR )
