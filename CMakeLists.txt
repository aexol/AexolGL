cmake_minimum_required(VERSION 3.1)
project(AexolGL)
cmake_policy(SET CMP0022 NEW)
cmake_policy(SET CMP0054 NEW)
option(PYTHON_PROJECT "Python based project." ON)
option(BUILD_LIB_ONLY "Build only AexolGL library without apps." ON)
option(NO_PYTHON  "Don't build python lib." ON)
option(AEX_LOG_DEBUG_RELEASE  "Enable debug logger in Aexol release build." ON)
set(AEX_PYTHON_VERSION "3.5" CACHE STRING "version of python used to build aexol library.")
set(AEX_OPENGL_ES_PREFERED_VERSION "3" CACHE STRING "version of OpenGL ES used to build aexol library.")
option(AEX_USE_GLEW "Should aexol use glew for engine build." OFF)
option(AEX_USE_GL_LOAD_GEN "Should aexol use glLoadGen." OFF)

# For now Android and iPhone only support Python 2.7.
if(IOS OR ANDROID)
    set(AEX_PYTHON_VERSION "2.7")
endif()

if("${CMAKE_GENERATOR}" STREQUAL "Ninja")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
    endif()
endif()

#General debugng flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_GL -DDEBUG -DLOGGING_LEVEL=2")

# Generate compile_commands.json, useful
# with IDE/Editor tools like VSCode, NetBeans, Atom, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(AEX_LOG_DEBUG_RELEASE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Os -ffast-math -DNDEBUG -DLOGGING_LEVEL=2")
else(AEX_LOG_DEBUG_RELEASE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Os -ffast-math -DNDEBUG")
endif(AEX_LOG_DEBUG_RELEASE)

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -DNDEBUG")
if(ANDROID)
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DNDK_DEBUG=1 -ggdb")
endif(ANDROID)


#Library is build using new C++11 features that's why this line is required no matter the enviroment
#except for MSVC which has it enabled by default

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm200 -D_SCL_SECURE_NO_WARNINGS")
    # MSVC debug magic is annoying, get rid of it.
    set(CMAKE_CXX_FLAGS_DEBUG
        "${CMAKE_CXX_FLAGS_DEBUG} /D_ITERATOR_DEBUG_LEVEL=0 /MD"
      CACHE STRING "Debug compiler flags" FORCE
    )
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -stdlib=libc++")
endif(APPLE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GXX_VERSION)
    if(NOT GXX_VERSION VERSION_LESS 4.9)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto")
    endif()
endif()

#Custom modules search
set(CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake/Modules"
    "${CMAKE_SOURCE_DIR}/cmake/utils"
    "${CMAKE_SOURCE_DIR}/cmake_ext/Modules"
    "${CMAKE_SOURCE_DIR}/cmake_ext/utils"
    ${CMAKE_MODULE_PATH}
    )

set(ANDROID_ROOT_PACKAGE "com.aexol.android")
#set(ANDROID_ROOT_BUILD_GRADLE build.gradle)
INCLUDE(helper_macros)
if(ANDROID)
    if(ANDROID_TOOLS_PREBUILT)
        if(WIN32)
            set(__detect_prebuilt_make "${ANDROID_TOOLS_PREBUILT}/make.exe")
        else(WIN32)
            set(__detect_prebuilt_make "${ANDROID_TOOLS_PREBUILT}/make")
        endif(WIN32)
        if(EXISTS ${__detect_prebuilt_make})
            unset(CMAKE_MAKE_PROGRAM CACHE)
            set(CMAKE_MAKE_PROGRAM ${__detect_prebuilt_make} CACHE STRING "Android prebuild make")
        endif(EXISTS ${__detect_prebuilt_make})
    endif(ANDROID_TOOLS_PREBUILT)
    if(NOT ANDROID_SDK)
        if(DEFINED ENV{ANDROID_SDK})
            set(ANDROID_SDK $ENV{ANDROID_SDK})
        else() #Last attempt, let's try finding android
            find_host_program(ANDROID_SDK_BIN_PATH NAMES android)
            if(ANDROID_SDK_BIN_PATH)
               string(REGEX REPLACE "(.*)tools/android" "\\1" ANDROID_SDK ${ANDROID_SDK_BIN_PATH})
            endif(ANDROID_SDK_BIN_PATH)
        endif()
        if(NOT ANDROID_SDK)
            message(FATAL_ERROR "No path to android SDK, add -DANDROID_SDK=/sdk/path to your execution or define enviroment variable ANDROID_SDK")
        endif(NOT ANDROID_SDK)
    endif(NOT ANDROID_SDK)
    if(NOT IS_DIRECTORY ${ANDROID_SDK})
        message(FATAL_ERROR "${ANDROID_SDK} is not a directory.")
    endif(NOT IS_DIRECTORY ${ANDROID_SDK})
    set(ANDROID_SDK ${ANDROID_SDK} CACHE STRING "Path to android SDK")
endif(ANDROID)
include(common)

if(DEFINED AEX_ADD_ROOTS)
    append_root(${AEX_ADD_ROOTS})
endif(DEFINED AEX_ADD_ROOTS)

#CONFIGURATION
set(BUILD_FOR_IOS OFF)
set(USING_ASSIMP OFF)
set(BUILD_FOR_LINUX OFF)
set(BUILD_FOR_MAC OFF)
set(BUILD_FOR_WIN32 OFF)
set(BUILD_FOR_ANDROID OFF)
set(BUILD_FOR_MINGW OFF)

if(APPLE)
    if("${CMAKE_GENERATOR}" STREQUAL "Xcode")
        # Common dummy file for all Xcode
        # when creating library from object files.
        # Necessery 'cause XCode is known to not create
        # targets that are made only from object files.
        set(XCODE_DUMMY_SOURCE_WORKAROUND "${CMAKE_BINARY_DIR}/dummy.cxx")
        file(WRITE ${XCODE_DUMMY_SOURCE_WORKAROUND} "")
    endif()
    if(IOS)
        set(BUILD_FOR_IOS ON)
    else(IOS)
        set(BUILD_FOR_MAC ON)
    endif(IOS)
else(APPLE)
    set(XCODE_DUMMY_SOURCE_WORKAROUND)
    if(UNIX AND NOT ANDROID)
        set(BUILD_FOR_LINUX ON)
    endif(UNIX AND NOT ANDROID)
endif(APPLE)
if(WIN32)
    if(MINGW)
        set(BUILD_FOR_MINGW ON)
    else()
        set(BUILD_FOR_WIN32 ON)
    endif()
endif(WIN32)
if(ANDROID)
	set(BUILD_FOR_ANDROID ON)
endif(ANDROID)

#CXX features.
include(AexCompilerChecks)
CHECK_HAS_CONSTEXPR(AEX_HAS_CXX11_CONSTEXPR)
CHECK_HAS_NOEXCEPT(AEX_HAS_CXX11_NOEXCEPT)
CHECK_HAS_THREAD_LOCAL(AEX_HAS_CXX11_THREAD_LOCAL)
CHECK_HAS_CODECVT(AEX_HAS_CXX11_CODECVT)

#Some basic libs that our engine depends on
include(${AexolGL_SOURCE_DIR}/cmake/utils/ExecRequirements.cmake)
include_directories(${GL_INCLUDE})
include(CheckIncludeFiles)
set(HEADER_PREFIX "")
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${GL_INCLUDE})
if(BUILD_FOR_GL)
	if (GLEW_FOUND)
		check_include_files (GL/glew.h HAVE_GLEW_H)
	else(GLEW_FOUND)
        if(${GL_INCLUDE} MATCHES "OpenGL.framework")
            #Test for GL3 on OS X
            find_path(TEST_GL3_HEADER
                NAMES OpenGL/gl3.h
                HINTS ${GL3_ROOT}
                PATHS ENV GL3_ROOT
           )
            if(TEST_GL3_HEADER)
                set(HAVE_OPENGL_GL3_H ON)
            else(TEST_GL3_HEADER)
                set (HAVE_OPENGL_GL_H ON)
            endif(TEST_GL3_HEADER)
        else()
            check_include_files("GL/gl.h;GL/glext.h" HAVE_GL_GL_H)
            if(NOT HAVE_GL_GL_H)
                check_include_files("gl.h;glext.h" HAVE_GL_H)
            endif()
        endif()
	endif(GLEW_FOUND)
else(BUILD_FOR_GL)
	if(BUILD_FOR_GLES${AEX_OPENGL_ES_PREFERED_VERSION})
        if(${GL_INCLUDE} MATCHES "OpenGLES.framework")
		    set (HAVE_OPENGLES_ES${AEX_OPENGL_ES_PREFERED_VERSION}_GL_H ON)
        endif(${GL_INCLUDE} MATCHES "OpenGLES.framework")
		if(NOT HAVE_OPENGLES_ES${AEX_OPENGL_ES_PREFERED_VERSION}_GL_H)
			check_include_files (GLES${AEX_OPENGL_ES_PREFERED_VERSION}/gl${AEX_OPENGL_ES_PREFERED_VERSION}.h HAVE_GLES${AEX_OPENGL_ES_PREFERED_VERSION}_GL${AEX_OPENGL_ES_PREFERED_VERSION}_H)
		endif(NOT HAVE_OPENGLES_ES${AEX_OPENGL_ES_PREFERED_VERSION}_GL_H)
	endif(BUILD_FOR_GLES${AEX_OPENGL_ES_PREFERED_VERSION})
endif(BUILD_FOR_GL)
check_include_files (pthread.h AEX_HAVE_PTHREAD_H)

if(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Verbosity of logs for build.")
endif()

# INCLUDE_DIRECTORIES(${JSONCPP_INCLUDE_DIR})

if(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")
endif()
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(LOGGING_LEVEL "DEBUG_BUILD" CACHE STRING "Verbosity of logs for build.")
else()
    set(LOGGING_LEVEL "RELEASE_BUILD" CACHE STRING "Verbosity of logs for build.")
endif()

mark_as_advanced(LOGGING_LEVEL)

if(NOT IOS)
    check_include_files("stdio.h" HAVE_STDIO_H)
endif(NOT IOS)

configure_file(${PROJECT_SOURCE_DIR}/cmake/config/aex-config.h.in ${PROJECT_BINARY_DIR}/aex-config.h)
install(FILES ${PROJECT_BINARY_DIR}/aex-config.h
    DESTINATION "include/aex"
    COMPONENT Devel
	)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/cmake_ext/
    DESTINATION lib/cmake
    COMPONENT Devel
    REGEX "\\.git.*" EXCLUDE
	)

include_directories(${PROJECT_BINARY_DIR})
#END OF CONFIGURATION
set(HEADERS_GLOBAL
    ${PROJECT_BINARY_DIR}/aex-config.h
)

add_custom_target(GLOBAL_HEADERS SOURCES ${HEADERS_GLOBAL})
enable_testing()

if(UNIX AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    if(NOT "${CMAKE_INSTALL_RPATH}" STREQUAL "")
        set(CMAKE_INSTALL_RPATH ":${CMAKE_INSTALL_RPATH}")
    endif(NOT "${CMAKE_INSTALL_RPATH}" STREQUAL "")
    set(CMAKE_INSTALL_RPATH "$ORIGIN${CMAKE_INSTALL_RPATH}")
elseif(UNIX AND APPLE)
    if(NOT "${CMAKE_INSTALL_RPATH}" STREQUAL "")
        set(CMAKE_INSTALL_RPATH ":${CMAKE_INSTALL_RPATH}")
    endif(NOT "${CMAKE_INSTALL_RPATH}" STREQUAL "")
    set(CMAKE_INSTALL_RPATH "@rpath${CMAKE_INSTALL_RPATH}")
endif(UNIX AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")

#Include our libraries
include(libraries/CMakeLists.txt)
#Test binaries
if(NOT BUILD_LIB_ONLY)
    add_subdirectory(tests)
endif(NOT BUILD_LIB_ONLY)


if(NOT DEFINED AEXOL_INSTALL_DIR)
	set(AEXOL_INSTALL_DIR "/usr/local")
endif(NOT DEFINED AEXOL_INSTALL_DIR)
#set(CPACK_COMPONENT_AEXOLGL_INC_DISPLAY_NAME "Main library headers")
#set(CPACK_COMPONENT_AEXOLGL_LIB_DISPLAY_NAME "Main library")
set(CPACK_COMPONENTS_ALL Library Headers PythonModule )
set(CPACK_PACKAGE_NAME "AexolGL_LIB")
set(CPACK_PACKAGE_VENDOR "aexol.org")
set(CPACK_PACKAGE_CONTACT "kamil@aexol.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Aexol GL Engine omg yay build pack, fix this")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${AEXOL_INSTALL_DIR}")
set(CPACK_SET_DESTDIR "ON")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0,libsdl2-image-2.0-0,libsdl2-ttf-2.0-0,libjsoncpp1,libopenal1,libassimp3v5,libvorbis0a,libvorbisfile3,libvorbisenc2,libfreetype6")
include(CPack)
# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

option(BUILD_DOC "Build the docs" OFF)
if(BUILD_DOC)
    if(IOS OR ANDROID)
        find_host_package(Doxygen)
    else()
        find_package(Doxygen)
    endif()
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUT_DIR ${CMAKE_BINARY_DIR}/doc/DoxyGen)
        file(RELATIVE_PATH _doc_rel_path ${DOXYGEN_OUT_DIR} ${CMAKE_SOURCE_DIR})
        configure_file(${CMAKE_SOURCE_DIR}/doc/DoxyGen/Doxyfile.in ${DOXYGEN_OUT_DIR}/Doxyfile)
        configure_file(${CMAKE_SOURCE_DIR}/README.md
            ${CMAKE_BINARY_DIR}/CMakeFiles/README.md.base
            COPYONLY)
        file(READ ${CMAKE_BINARY_DIR}/CMakeFiles/README.md.base README_CONTENTS)
        configure_file(${CMAKE_SOURCE_DIR}/doc/DoxyGen/README.md.in
            ${DOXYGEN_OUT_DIR}/README.md)
        # Doxygen has a weird behaviour with
        # markdown files. If relative links to
        # other markdown files are not in doxygen 
        # workdir (or below) wrong link gets generated.
        # That's why for a workaround copy md dir.
        add_custom_target(Doc_DoxyGen ALL
            COMMAND ${CMAKE_COMMAND}
                -E
                copy_directory
                ${CMAKE_SOURCE_DIR}/doc/md
                ${DOXYGEN_OUT_DIR}/doc/md
            COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${DOXYGEN_OUT_DIR}
            COMMENT "Generating API Docs."
            DEPENDS ${CMAKE_SOURCE_DIR}/README.md
            VERBATIM)
    else()
        message("No doxygen found. Skipping docs generation.")
    endif()
endif()
