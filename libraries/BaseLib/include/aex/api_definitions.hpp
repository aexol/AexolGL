//
//  api_definitions.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 07.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/platform_definitions.hpp>

#ifndef aexolGL_api_definitions_h
#define aexolGL_api_definitions_h

#define __GLES__  1
#define __GL__  2
#define __DX__  3
#define __UNSUPPORTED_API__ 0

#define __TARGET_AEX_API__ __UNSUPPORTED_API__

#ifdef BUILD_FOR_GLES2 
#undef __TARGET_AEX_API__
#define __TARGET_AEX_API__ __GLES__
#endif

#ifdef  BUILD_FOR_GLES3
#undef __TARGET_AEX_API__
#define __TARGET_AEX_API__ __GLES__
#endif

#ifdef  BUILD_FOR_GL
#undef __TARGET_AEX_API__
#define __TARGET_AEX_API__ __GL__
#endif
#ifdef  __COMPILE__FOR__DIRECTX__
#undef __TARGET_AEX_API__
#define __TARGET_AEX_API__ __DX__
#endif


#ifdef HAVE_OPENGL_GL3_H
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif
#ifdef HAVE_OPENGL_GL_H
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif
#ifdef HAVE_OPENGL_GLEXT_H
#include <OpenGL/glext.h>
#define GLEXT
#endif
#ifdef HAVE_GLEW_H
#include <GL/glew.h>
#define GLEW
#endif
#ifdef HAVE_GL_GL_H
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#define GLEXT
#endif

#ifdef HAVE_GL_H
#   define GL_GLEXT_PROTOTYPES
#   include <gl.h>
#   include <glext.h>
#   define GLEXT
#endif

#ifdef HAVE_OPENGLES_ES2_GL_H
#   define GL_GLEXT_PROTOTYPES
#   include <OpenGLES/ES2/gl.h>
#   include <OpenGLES/ES2/glext.h>
#endif

#ifdef HAVE_OPENGLES_ES3_GL_H
#   define GL_GLEXT_PROTOTYPES
#   include <OpenGLES/ES3/gl.h>
#   include <OpenGLES/ES3/glext.h>
#endif

#ifdef HAVE_GLES2_GL2_H
#   define GL_GLEXT_PROTOTYPES
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#   include <GLES2/gl2platform.h>
#endif

#ifdef HAVE_GLES3_GL3_H
#   define GL_GLEXT_PROTOTYPES
#   include <GLES3/gl3.h>
//  Workaround for Android API 18 to not include GLES2/gl2.h
//  from gl2ext.h
#   define __gl2_h_
#   include <GLES2/gl2ext.h>
#   include <GLES3/gl3platform.h>
#endif

#ifdef HAVE_GL_LOAD_GEN
#   define GL_GLEXT_PROTOTYPES
#   define WIN32_LEAN_AND_MEAN
#   define NOGDI
#   define NOMINMAX
#   include <aex/glLoadGen/gl_core_3_0.hpp>
#undef FAR
#undef  NEAR
#undef IN
#undef  OUT
#endif

#endif
