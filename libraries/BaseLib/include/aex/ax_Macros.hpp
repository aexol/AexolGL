//
//  ax_Macros.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 07.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#ifndef aexolGL_ax_Macros_h
#define aexolGL_ax_Macros_h
#include <aex/Common.hpp>
extern "C" AEX_PUBLIC_API void CheckOpenGLError(const char* stmt, const char* fname, int line);

#ifndef NDEBUG
#define GL_CHECK(stmt) do { stmt; CheckOpenGLError(#stmt, __FILE__, __LINE__); } while (0)//
#else
#define GL_CHECK(stmt)  stmt
#endif


#endif
