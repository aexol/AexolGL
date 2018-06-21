set(libdir lib)
set(includedir include)
set(configdir lib/cmake)

# Default lib type for targets.
# Except for iOS it is always shared.
set(DEFAULT_LIB_TYPE SHARED)
if(IOS)
    set(DEFAULT_LIB_TYPE STATIC)
endif()

# Set dummy import prefix for interface libraries.
# This is needed because libraries dependant on AexolGL
# will have invalid LINK_LIBRARY_INTERFACE.
set(_IMPORT_PREFIX ${CMAKE_INSTALL_PREFIX})

if(APPLE)
    set(apple_os_name macOS)
    if(IOS)
      set(apple_os_name iOS)
    endif()
endif()

ADD_SUBDIRECTORY( libraries/BaseLib )

IF(ANDROID OR IOS)
    ADD_SUBDIRECTORY( libraries/AdsModule )
ENDIF(ANDROID OR IOS)

if(BUILD_FBModule)
    ADD_SUBDIRECTORY( libraries/FBModule )
endif()

if(BUILD_HDWModule)
    ADD_SUBDIRECTORY( libraries/HDWModule )
endif()

if(BUILD_InAppsModule)
#    ADD_SUBDIRECTORY( libraries/InAppModule )
endif()
