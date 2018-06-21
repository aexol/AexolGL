# Locate the SDL2_image library. This CMake module is a modified version
# of the original FindSDL_image.cmake file
# ###########################################################################
# Locate SDL_image library
# This module defines
# SDL2TTF_LIBRARY, the name of the library to link against
# SDLTTF_FOUND, if false, do not try to link to SDL
# SDL2TTF_INCLUDE_DIR, where to find SDL/SDL.h
#
# $SDLDIR is an environment variable that would
# correspond to the ./configure --prefix=$SDLDIR
# used in building SDL.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake 
# module, but with modifications to recognize OS X frameworks and 
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(SDL2TTF_INCLUDE_DIR SDL_ttf.h
  HINTS
  $ENV{SDL2TTFINCLUDEDIR}
  $ENV{SDL2TTFDIR}
  $ENV{SDL2DIR}
  PATH_SUFFIXES include include/SDL2 SDL2
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include
  /usr/include
  /sw/include # Fink
  /opt/local/include # DarwinPorts
  /opt/csw/include # Blastwave
  /opt/include
)

FIND_LIBRARY(SDL2TTF_LIBRARY 
  NAMES SDL2_ttf
  HINTS
  $ENV{SDL2TTFDIR}
  $ENV{SDL2DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)
if(MINGW)
    find_library(_FT_LIB freetype)
    find_library(_ZLIB_LIB z)
    set(SDL2TTF_LIBRARY ${SDL2TTF_LIBRARY} ${_FT_LIB} ${_ZLIB_LIB})
endif()

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2TTF
                                  REQUIRED_VARS SDL2TTF_LIBRARY SDL2TTF_INCLUDE_DIR)
