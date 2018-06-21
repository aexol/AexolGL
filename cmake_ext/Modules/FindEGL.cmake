# FindEGL module for android, ios includes EAGL in OpenGLES framework so it's unneeded
# This module sets:
# EGL_FOUND
# EGL_INCLUDE_DIR
# EGL_LIBRARY
find_path( EGL_INCLUDE_DIR egl.h
        PATH_SUFFIXES EGL 
    )
find_library( EGL_LIBRARY EGL )
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( EGL DEFAULT_MSG EGL_LIBRARY EGL_INCLUDE_DIR )

