# - Try to find OpenGLES2
# Once done this will define
#

#  OPENGLES2_FOUND  - system has XMESA
#  OPENGLES2_INCLUDE_DIR  - the GL include directory
#  OPENGLES2_LIBRARIES    - Link these to use OpenGL and GLU
#
#  EGL_FOUND    - Has EGL
#  EGL_INCLUDE_DIR  -EGL include directory
#  EGL_LIBRARIES    - Link EGL
#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

set(_OpenGLES2_REQUIRED_VARS OPENGLES2_gl_LIBRARY)

if (CYGWIN)

    FIND_PATH(OPENGLES2_INCLUDE_DIR GLES2/gl2.h )

    FIND_LIBRARY(OPENGLES2_gl_LIBRARY libGLESv2 )

elseif (WIN32)
    if(BORLAND)
        SET (OPENGLES2_gl_LIBRARY import32 CACHE STRING "OpenGL ES 2.x library for win32")
    elseif(MSVC OR MINGW)
        # Use Angle on MSVC
        FIND_PATH(OPENGLES2_INCLUDE_DIR GLES2/gl2.h)
        FIND_LIBRARY(OPENGLES2_gl_LIBRARY libGLESv2)
        FIND_PATH(EGL_INCLUDE_DIR EGL/egl.h)
        FIND_LIBRARY(EGL_egl_LIBRARY NAMES libEGL)
        if(OPENGLES2_gl_LIBRARY)
            set(OPENGLES2_gl_LIBRARY ${OPENGLES2_gl_LIBRARY})
        endif()
    endif()
elseif (APPLE)
    find_path(OPENGLES2_INCLUDE_DIR OpenGLES/ES2/gl.h DOC "Include for OpenGLES on OSX")
    set( OPENGLES2_gl_LIBRARY "-framework OpenGLES" )
    list(APPEND _OpenGLES2_REQUIRED_VARS OPENGLES2_INCLUDE_DIR)
else()

    # The first line below is to make sure that the proper headers
    # are used on a Linux machine with the NVidia drivers installed.
    # They replace Mesa with NVidia's own library but normally do not
    # install headers and that causes the linking to
    # fail since the compiler finds the Mesa headers but NVidia's library.
    # Make sure the NVIDIA directory comes BEFORE the others.
    #  - Atanas Georgiev <atanas@cs.columbia.edu>

    find_path(OPENGLES2_INCLUDE_DIR GLES2/gl2.h
        /usr/openwin/share/include
        /opt/graphics/OpenGL/include /usr/X11R6/include
        /usr/include
        )
    list(APPEND _OpenGLES2_REQUIRED_VARS OPENGLES2_INCLUDE_DIR)

    FIND_LIBRARY(OPENGLES2_gl_LIBRARY
        NAMES GLESv2
        PATHS /opt/graphics/OpenGL/lib
        /usr/openwin/lib
        /usr/shlib /usr/X11R6/lib
        /usr/lib
        )

    FIND_PATH(EGL_INCLUDE_DIR EGL/egl.h
        /usr/openwin/share/include
        /opt/graphics/OpenGL/include /usr/X11R6/include
        /usr/include
        )

    FIND_LIBRARY(EGL_egl_LIBRARY
        NAMES EGL
        PATHS /opt/graphics/OpenGL/lib
        /usr/openwin/lib
        /usr/shlib /usr/X11R6/lib
        /usr/lib
        )
    unset(_OPENGLES2_INCLUDE_PATH)
    unset(_OPENGLES2_LIB_PATH)

    # On Unix OpenGL most certainly always requires X11.
    # Feel free to tighten up these conditions if you don't
    # think this is always true.

    if (OPENGLES2_gl_LIBRARY)
        #FIND_PACKAGE( X11 )
        if(NOT X11_FOUND)    
            # include(${CMAKE_CURRENT_LIST_DIR}) # Do nothing. TODO - using it in macosx cmake build breakes building 
        endif()
        if (X11_FOUND)
            set (OPENGLES2_LIBRARIES ${X11_LIBRARIES})
        endif ()
    endif ()


endif ()

if(OPENGLES2_gl_LIBRARY)

    set( OPENGLES2_FOUND "YES" )

    set( OPENGLES2_LIBRARIES  ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_LIBRARIES})
    if( EGL_egl_LIBRARY )
        set( EGL_FOUND "YES" )
        set( EGL_LIBRARIES ${EGL_egl_LIBRARY} )
    else()
        set( EGL_FOUND "NO" )
    endif()

    # This deprecated setting is for backward compatibility with CMake1.4
    set (OPENGLES2_LIBRARY ${OPENGLES2_LIBRARIES})

endif()

# This deprecated setting is for backward compatibility with CMake1.4
set(OPENGLES2_INCLUDE_PATH ${OPENGLES2_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set OPENGLES2_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenGLES2 REQUIRED_VARS ${_OpenGLES2_REQUIRED_VARS})
unset(_OpenGLES2_REQUIRED_VARS)

mark_as_advanced(
    OPENGLES2_INCLUDE_DIR
    OPENGLES2_gl_LIBRARY
    )
