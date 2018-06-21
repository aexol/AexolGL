//
//  ios_api_definitions.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 27.09.2013.
//
//

#ifndef AexolGL_ios_api_definitions_h
#define AexolGL_ios_api_definitions_h

#include <aex/api_definitions.hpp>

#if __TARGET_AEX_API__==__GLES__

#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/EAGL.h>
#include <OpenGLES/EAGLDrawable.h>

#endif

#endif
