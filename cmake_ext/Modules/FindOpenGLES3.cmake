# - Try to find OpenGLES3
# Once done this will define
#

#  OPENGLES3_FOUND  - system has XMESA
#  OPENGLES3_INCLUDE_DIR  - the GL include directory
#  OPENGLES3_LIBRARIES    - Link these to use OpenGL and GLU
#
#  EGL_FOUND    - Has EGL
#  EGL_INCLUDE_DIR  -EGL include directory
#  EGL_LIBRARIES    - Link EGL
#=============================================================================
# Copyright 3001-3009 Kitware, Inc.
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

set(_OpenGLES3_REQUIRED_VARS OPENGLES3_gl_LIBRARY)

if (CYGWIN)

    FIND_PATH(OPENGLES3_INCLUDE_DIR GLES3/gl3.h )

    FIND_LIBRARY(OPENGLES3_gl_LIBRARY libGLESv2 )

elseif (WIN33)
    if(BORLAND)
        SET (OPENGLES3_gl_LIBRARY import33 CACHE STRING "OpenGL ES 3.x library for win33")
    elseif(MSVC OR MINGW)
        # Use Angle on MSVC
        FIND_PATH(OPENGLES3_INCLUDE_DIR GLES3/gl3.h)
        FIND_LIBRARY(OPENGLES3_gl_LIBRARY libGLESv2)
        FIND_PATH(EGL_INCLUDE_DIR EGL/egl.h)
        FIND_LIBRARY(EGL_egl_LIBRARY NAMES libEGL)
        if(OPENGLES3_gl_LIBRARY)
            set(OPENGLES3_gl_LIBRARY ${OPENGLES3_gl_LIBRARY})
        endif()
    endif()
elseif (APPLE)
    find_path(OPENGLES3_INCLUDE_DIR OpenGLES/ES3/gl.h DOC "Include for OpenGLES on OSX")
    set( OPENGLES3_gl_LIBRARY "-framework OpenGLES" )
    list(APPEND _OpenGLES3_REQUIRED_VARS OPENGLES3_INCLUDE_DIR)
else()

    # The first line below is to make sure that the proper headers
    # are used on a Linux machine with the NVidia drivers installed.
    # They replace Mesa with NVidia's own library but normally do not
    # install headers and that causes the linking to
    # fail since the compiler finds the Mesa headers but NVidia's library.
    # Make sure the NVIDIA directory comes BEFORE the others.
    #  - Atanas Georgiev <atanas@cs.columbia.edu>

    find_path(OPENGLES3_INCLUDE_DIR GLES3/gl3.h
        /usr/openwin/share/include
        /opt/graphics/OpenGL/include /usr/X11R6/include
        /usr/include
        )
    list(APPEND _OpenGLES3_REQUIRED_VARS OPENGLES3_INCLUDE_DIR)

    FIND_LIBRARY(OPENGLES3_gl_LIBRARY
        NAMES GLESv3 GLESv2
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
    unset(_OPENGLES3_INCLUDE_PATH)
    unset(_OPENGLES3_LIB_PATH)

    # On Unix OpenGL most certainly always requires X11.
    # Feel free to tighten up these conditions if you don't
    # think this is always true.

    if (OPENGLES3_gl_LIBRARY)
        #FIND_PACKAGE( X11 )
        if(NOT X11_FOUND)    
            # include(${CMAKE_CURRENT_LIST_DIR}) # Do nothing. TODO - using it in macosx cmake build breakes building 
        endif()
        if (X11_FOUND)
            set (OPENGLES3_LIBRARIES ${X11_LIBRARIES})
        endif ()
    endif ()


endif ()

if(OPENGLES3_gl_LIBRARY)

    set( OPENGLES3_FOUND "YES" )

    set( OPENGLES3_LIBRARIES  ${OPENGLES3_gl_LIBRARY} ${OPENGLES3_LIBRARIES})
    if( EGL_egl_LIBRARY )
        set( EGL_FOUND "YES" )
        set( EGL_LIBRARIES ${EGL_egl_LIBRARY} )
    else()
        set( EGL_FOUND "NO" )
    endif()

    # This deprecated setting is for backward compatibility with CMake1.4
    set (OPENGLES3_LIBRARY ${OPENGLES3_LIBRARIES})

endif()

# This deprecated setting is for backward compatibility with CMake1.4
set(OPENGLES3_INCLUDE_PATH ${OPENGLES3_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set OPENGLES3_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenGLES3 REQUIRED_VARS ${_OpenGLES3_REQUIRED_VARS})
unset(_OpenGLES3_REQUIRED_VARS)

mark_as_advanced(
    OPENGLES3_INCLUDE_DIR
    OPENGLES3_gl_LIBRARY
    )
