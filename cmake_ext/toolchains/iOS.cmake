# This file is based off of the Platform/Darwin.cmake and Platform/UnixPaths.cmake
# files which are included with CMake 2.8.4
# It has been altered for iOS development

# Options:
#
# IOS_PLATFORM = OS (default) or SIMULATOR
#   This decides if SDKS will be selected from the iPhoneOS.platform or iPhoneSimulator.platform folders
#   OS - the default, used to build for iPhone and iPad physical devices, which have an arm arch.
#   SIMULATOR - used to build for the Simulator platforms, which have an x86 arch.
#
# CMAKE_IOS_DEVELOPER_ROOT = automatic(default) or /path/to/platform/Developer folder
#   By default this location is automatcially chosen based on the IOS_PLATFORM value above.
#   If set manually, it will override the default location and force the user of a particular Developer Platform
#
# CMAKE_IOS_SDK_ROOT = automatic(default) or /path/to/platform/Developer/SDKs/SDK folder
#   By default this location is automatcially chosen based on the CMAKE_IOS_DEVELOPER_ROOT value.
#   In this case it will always be the most up-to-date SDK found in the CMAKE_IOS_DEVELOPER_ROOT path.
#   If set manually, this will force the use of a specific SDK version

# Macros:
#
# set_xcode_property (TARGET XCODE_PROPERTY XCODE_VALUE)
#  A convenience macro for setting xcode specific properties on targets
#  example: set_xcode_property (myioslib IPHONEOS_DEPLOYMENT_TARGET "3.1")
#
# find_host_package (PROGRAM ARGS)
#  A macro used to find executable programs on the host system, not within the iOS environment.
#  Thanks to the android-cmake project for providing the command


# Standard settings
set (CMAKE_SYSTEM_NAME Ios)
set (CMAKE_SYSTEM_VERSION 1)
set (UNIX True)
set (APPLE True)
set (IOS True)
include(ExternalProject)
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
macro(SUBDIRLIST result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

# Required as of cmake 2.8.10
set (CMAKE_OSX_DEPLOYMENT_TARGET "" CACHE STRING "Force unset of the deployment target for iOS" FORCE)

# Determine the cmake host system version so we know where to find the iOS SDKs
find_program (CMAKE_UNAME uname /bin /usr/bin /usr/local/bin)
if (CMAKE_UNAME)
    exec_program(uname ARGS -r OUTPUT_VARIABLE CMAKE_HOST_SYSTEM_VERSION)
    string (REGEX REPLACE "^([0-9]+)\\.([0-9]+).*$" "\\1" DARWIN_MAJOR_VERSION "${CMAKE_HOST_SYSTEM_VERSION}")
endif (CMAKE_UNAME)

set (_xcompile_triplet "arm-apple-darwin11")

# Skip the platform compiler checks for cross compiling
set (CMAKE_CXX_COMPILER_WORKS TRUE)
set (CMAKE_C_COMPILER_WORKS TRUE)

# All iOS/Darwin specific settings - some may be redundant
set (CMAKE_SHARED_LIBRARY_PREFIX "lib")
set (CMAKE_SHARED_LIBRARY_SUFFIX ".dylib")
set (CMAKE_EXTRA_SHARED_LIBRARY_SUFFIXES ".tbd")
set (CMAKE_SHARED_MODULE_PREFIX "lib")
set (CMAKE_SHARED_MODULE_SUFFIX ".so")
set (CMAKE_MODULE_EXISTS 1)
set (CMAKE_DL_LIBS "")

set (CMAKE_C_OSX_COMPATIBILITY_VERSION_FLAG "-compatibility_version ")
set (CMAKE_C_OSX_CURRENT_VERSION_FLAG "-current_version ")
set (CMAKE_CXX_OSX_COMPATIBILITY_VERSION_FLAG "${CMAKE_C_OSX_COMPATIBILITY_VERSION_FLAG}")
set (CMAKE_CXX_OSX_CURRENT_VERSION_FLAG "${CMAKE_C_OSX_CURRENT_VERSION_FLAG}")

# Hidden visibilty is required for cxx on iOS
set (CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS} -Wall")
set (CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -Wall")

set (CMAKE_C_LINK_FLAGS "-Wl,-search_paths_first ${CMAKE_C_LINK_FLAGS}" CACHE STRING "" FORCE)
set (CMAKE_CXX_LINK_FLAGS "-Wl,-search_paths_first ${CMAKE_CXX_LINK_FLAGS}" CACHE STRING "" FORCE)

set (CMAKE_PLATFORM_HAS_INSTALLNAME 1)
set (CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "-dynamiclib -headerpad_max_install_names")
set (CMAKE_SHARED_MODULE_CREATE_C_FLAGS "-bundle -headerpad_max_install_names")
set (CMAKE_SHARED_MODULE_LOADER_C_FLAG "-Wl,-bundle_loader,")
set (CMAKE_SHARED_MODULE_LOADER_CXX_FLAG "-Wl,-bundle_loader,")
set (CMAKE_FIND_LIBRARY_SUFFIXES ".tbd" ".dylib" ".so" ".a")

# hack: if a new cmake (which uses CMAKE_INSTALL_NAME_TOOL) runs on an old build tree
# (where install_name_tool was hardcoded) and where CMAKE_INSTALL_NAME_TOOL isn't in the cache
# and still cmake didn't fail in CMakeFindBinUtils.cmake (because it isn't rerun)
# hardcode CMAKE_INSTALL_NAME_TOOL here to install_name_tool, so it behaves as it did before, Alex
if (NOT DEFINED CMAKE_INSTALL_NAME_TOOL)
    find_program(CMAKE_INSTALL_NAME_TOOL install_name_tool)
endif (NOT DEFINED CMAKE_INSTALL_NAME_TOOL)

# Setup iOS platform unless specified manually with IOS_PLATFORM
if (NOT DEFINED IOS_PLATFORM)
    set (IOS_PLATFORM "OS")
endif (NOT DEFINED IOS_PLATFORM)
set (IOS_PLATFORM ${IOS_PLATFORM} CACHE STRING "Type of iOS Platform")

# Check the platform selection and setup for developer root
if (${IOS_PLATFORM} STREQUAL "OS")
    set (IOS_PLATFORM_LOCATION "iPhoneOS.platform")

    # This causes the installers to properly locate the output libraries
    set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphonesimulator")
elseif (${IOS_PLATFORM} STREQUAL "SIMULATOR")
    set (IOS_PLATFORM_LOCATION "iPhoneSimulator.platform")

    # This causes the installers to properly locate the output libraries
    set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "-iphoneos")
else (${IOS_PLATFORM} STREQUAL "OS")
    message (FATAL_ERROR "Unsupported IOS_PLATFORM value selected. Please choose OS or SIMULATOR")
endif (${IOS_PLATFORM} STREQUAL "OS")

# Setup iOS developer location unless specified manually with CMAKE_IOS_DEVELOPER_ROOT
# Note Xcode 4.3 changed the installation location, choose the most recent one available
set (XCODE_POST_43_ROOT "/Applications/Xcode.app/Contents/Developer")
set (XCODE_PRE_43_ROOT "/Developer")
if (NOT DEFINED XCODE_DEVELOPER_ROOT)
    if (EXISTS ${XCODE_POST_43_ROOT})
        set (XCODE_DEVELOPER_ROOT ${XCODE_POST_43_ROOT})
    elseif(EXISTS ${XCODE_PRE_43_ROOT})
        set (XCODE_DEVELOPER_ROOT ${XCODE_PRE_43_ROOT})
    endif (EXISTS ${XCODE_POST_43_ROOT})
endif ()
set(CMAKE_IOS_DEVELOPER_ROOT "${XCODE_DEVELOPER_ROOT}/Platforms/${IOS_PLATFORM_LOCATION}/Developer")
set (CMAKE_IOS_DEVELOPER_ROOT ${CMAKE_IOS_DEVELOPER_ROOT} CACHE PATH "Location of iOS Platform")
set(CMAKE_LINKER
    "${XCODE_DEVELOPER_ROOT}/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
    CACHE STRING "Linker." FORCE)
set(CMAKE_C_COMPILER
    "${XCODE_DEVELOPER_ROOT}/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang"
    CACHE STRING "C Compiler." FORCE)
set(CMAKE_CXX_COMPILER
    "${XCODE_DEVELOPER_ROOT}/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++"
    CACHE STRING "CXX Compiler." FORCE)

# Find and use the most recent iOS sdk unless specified manually with CMAKE_IOS_SDK_ROOT
if (NOT DEFINED CMAKE_IOS_SDK_ROOT)
    file (GLOB _CMAKE_IOS_SDKS "${CMAKE_IOS_DEVELOPER_ROOT}/SDKs/*")
    if (_CMAKE_IOS_SDKS)
        list (SORT _CMAKE_IOS_SDKS)
        list (REVERSE _CMAKE_IOS_SDKS)
        list (GET _CMAKE_IOS_SDKS 0 CMAKE_IOS_SDK_ROOT)
    else (_CMAKE_IOS_SDKS)
        message (FATAL_ERROR "No iOS SDK's found in default search path ${CMAKE_IOS_DEVELOPER_ROOT}. Manually set CMAKE_IOS_SDK_ROOT or install the iOS SDK.")
    endif (_CMAKE_IOS_SDKS)
    message (STATUS "Toolchain using default iOS SDK: ${CMAKE_IOS_SDK_ROOT}")
endif (NOT DEFINED CMAKE_IOS_SDK_ROOT)
set (CMAKE_IOS_SDK_ROOT ${CMAKE_IOS_SDK_ROOT} CACHE PATH "Location of the selected iOS SDK")

# Set the sysroot default to the most recent SDK
set (CMAKE_OSX_SYSROOT ${CMAKE_IOS_SDK_ROOT} CACHE PATH "Sysroot used for iOS support")

# set the architecture for iOS
# NOTE: Currently both ARCHS_STANDARD_32_BIT and ARCHS_UNIVERSAL_IPHONE_OS set only, so set both manually
if (${IOS_PLATFORM} STREQUAL "OS")
    set (IOS_ARCH armv7 arm64)
else (${IOS_PLATFORM} STREQUAL "OS")
    set (IOS_ARCH i386)
endif (${IOS_PLATFORM} STREQUAL "OS")

set (CMAKE_OSX_ARCHITECTURES ${IOS_ARCH} CACHE string  "Build architecture for iOS")

#Some kind of bug on my mac, workaround
IF( NOT ( "${CMAKE_PREFIX_PATH}" STREQUAL "$ENV{CMAKE_PREFIX_PATH}" ) AND DEFINED ENV{CMAKE_PREFIX_PATH})
    SET( TEMP_PREFIX "" )
    STRING( REGEX REPLACE ":" ";" TEMP_PREFIX $ENV{CMAKE_PREFIX_PATH} )
    SET( CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${TEMP_PREFIX} )

ENDIF( NOT ( "${CMAKE_PREFIX_PATH}" STREQUAL "$ENV{CMAKE_PREFIX_PATH}" ) AND DEFINED ENV{CMAKE_PREFIX_PATH} )

# Set the find root to the iOS developer roots and to user defined paths
set (CMAKE_FIND_ROOT_PATH ${CMAKE_IOS_DEVELOPER_ROOT} ${CMAKE_IOS_SDK_ROOT} "${AEXOL_SDK}" CACHE string  "iOS find search path root")
foreach(r ${USER_ROOTS})
    list(APPEND CMAKE_FIND_ROOT_PATH "${r}")
endforeach()

# default to searching for frameworks first
set (CMAKE_FIND_FRAMEWORK FIRST)

# set up the default search directories for frameworks
set (CMAKE_SYSTEM_FRAMEWORK_PATH
    ${CMAKE_IOS_SDK_ROOT}/System/Library/Frameworks
    ${CMAKE_IOS_SDK_ROOT}/System/Library/PrivateFrameworks
    ${CMAKE_IOS_SDK_ROOT}/Developer/Library/Frameworks
    )
if(NOT "${CMAKE_HOST_UNIX}" STREQUAL "")
    if("${CMAKE_HOST_APPLE}" STREQUAL "")
        if(NOT DEFINED XCODE_TOOLCHAIN)
            set(post_43_toolchain "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/")
            set(pre_43_toolchain "/Developer/Toolchains/XcodeDefault.xctoolchain/")
            if(EXISTS ${post_43_toolchain})
                SET(XCODE_TOOLCHAIN "${post_43_toolchain}")
            elseif(EXISTS ${pre_43_toolchain})
                SET(XCODE_TOOLCHAIN "${pre_43_toolchain}")
            else()
                message(FATAL_ERRO "XcodeDefault.xctoolchain not found.")
            endif()
        endif()
        # Set toolchain prefix, suffix, and location for binutils.
        if(NOT DEFINED _CMAKE_TOOLCHAIN_PREFIX)
            set(_CMAKE_TOOLCHAIN_PREFIX "${_xcompile_triplet}-" CACHE STRING "" FORCE)
        endif()
        if(NOT DEFINED _CMAKE_TOOLCHAIN_SUFFIX)
            set(_CMAKE_TOOLCHAIN_SUFFIX "" CACHE STRING "" FORCE)
        endif()
        if(NOT DEFINED _CMAKE_TOOLCHAIN_LOCATION)
            set(_CMAKE_TOOLCHAIN_LOCATION "/usr/local/bin/" CACHE STRING "" FORCE)
        endif()
        # Try finding bin utils BEFORE setting search path to sysroot only.
        include(CMakeFindBinUtils)
        if("${CMAKE_IOS_MIN_VERSION}" STREQUAL "")
            find_program(FIND_MIN_IOS_VERSION find_min_ios_version)
            if(NOT FIND_MIN_IOS_VERSION)
                message(FATAL_ERROR "find_min_ios_version not found and CMAKE_IOS_MIN_VERSION not set.")
            endif()
            execute_process(COMMAND ${FIND_MIN_IOS_VERSION}
                RESULT_VARIABLE _ios_min_ver_result
                OUTPUT_VARIABLE _ios_min_ver
                )
            string(REGEX REPLACE "\n$" "" _ios_min_ver "${_ios_min_ver}")
            if(NOT "${_ios_min_ver_result}" STREQUAL "0")
                message(FATAL_ERROR "Could not find min ios version.")
            endif()
            set(CMAKE_IOS_MIN_VERSION "${_ios_min_ver}")
        endif()
        # CMAKE_REQUIRED_FLAGS needed for proper support tests.
        set (_ios_min_ver "-miphoneos-version-min=${CMAKE_IOS_MIN_VERSION}")
        set (_sysroot "--sysroot=${CMAKE_IOS_SDK_ROOT}")
        set (_target "-target ${_xcompile_triplet}")
        set (CMAKE_REQUIRED_FLAGS "${_sysroot} ${_ios_min_ver} ${_target}" CACHE STRING "" FORCE)
        set(_warnings "-Wmost -Wparentheses -Wswitch -Wunused-value -Wshorten-64-to-32 -Wdeprecated-declarations -Winvalid-offsetof ")
        set (_ios_isysroot "-isysroot ${CMAKE_IOS_SDK_ROOT}")
        set (_extra "-fmessage-length=198 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0 -fcolor-diagnostics -fpascal-strings -fstrict-aliasing")
        set (_common_flags "${CMAKE_REQUIRED_FLAGS} ${_warnings} ${_ios_isysroot} ${_extra}")
        # Find Darwin clang include dir.
        subdirlist(subdirs "${XCODE_TOOLCHAIN}/usr/lib/clang")
        list(GET subdirs 0 _clang_ver)
        set (_common_flags "${_common_flags} -isystem ${XCODE_TOOLCHAIN}/usr/lib/clang/${_clang_ver}/include")

        if(CMAKE_CXX11_ON)
            set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -std=c++11 -stdlib=libc++")
        endif(CMAKE_CXX11_ON)
        set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -cxx-isystem ${XCODE_TOOLCHAIN}/usr/include/c++/v1")
        set (CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS_INIT} ${_common_flags}")
        set (CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} ${_common_flags}")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS_INIT}" CACHE STRING "" FORCE)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT}" CACHE STRING "" FORCE)
        set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -g" CACHE STRING "" FORCE)
        set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g" CACHE STRING "" FORCE)
        set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -Os" CACHE STRING "" FORCE)
        set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Os" CACHE STRING "" FORCE)
        set (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} -g" CACHE STRING "" FORCE)
        set (CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELEASE} -g" CACHE STRING "" FORCE)
        set (CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "" FORCE)
        set (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    endif()
endif()

# only search the iOS sdks, not the remainder of the host filesystem
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_MACOSX_BUNDLE YES)
#set(CMAKE_OSX_SYSROOT "iphoneos")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.example")

set(CMAKE_SIZEOF_VOID_P 8)

# This little macro lets you set any XCode specific property
macro (set_xcode_property TARGET XCODE_PROPERTY XCODE_VALUE)
    set_property (TARGET ${TARGET} PROPERTY XCODE_ATTRIBUTE_${XCODE_PROPERTY} ${XCODE_VALUE})
endmacro (set_xcode_property)

# This macro lets you find executable programs on the host system
macro (find_host_package)
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    if( CMAKE_HOST_WIN32 )
        SET( WIN32 1 )
        SET( UNIX )
    elseif( CMAKE_HOST_APPLE )
        SET( APPLE 1 )
        SET( UNIX )
    endif()
    find_package(${ARGN})
    SET( WIN32 )
    SET( APPLE 1 )
    SET( UNIX 1 )
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro (find_host_package)

macro (find_host_external_project)
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    if( CMAKE_HOST_WIN32 )
        SET( WIN32 1 )
        SET( UNIX )
    elseif( CMAKE_HOST_APPLE )
        SET( APPLE 1 )
        SET( UNIX )
    endif()
    ExternalProject_Add(${ARGN})
    SET( WIN32 )
    SET( APPLE 1 )
    SET( UNIX 1 )
    set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro (find_host_external_project)

macro( find_host_program )
    set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
    set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER )
    set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER )
    if( CMAKE_HOST_WIN32 )
        SET( WIN32 1 )
        SET( UNIX )
    elseif( CMAKE_HOST_APPLE )
        SET( APPLE 1 )
        SET( UNIX )
    endif()
    find_program( ${ARGN} )
    SET( WIN32 )
    SET( APPLE 1 )
    SET( UNIX 1 )
    set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY )
    set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
    set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
endmacro()

macro(append_root)
    SET(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} ${ARGN})
endmacro(append_root)
