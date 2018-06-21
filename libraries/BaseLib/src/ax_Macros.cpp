//
//  ax_Macros.c
//  aexolGL
//
//  Created by Kamil Matysiewicz on 07.08.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/ax_Macros.hpp>
#include <aex/AexTypes.hpp>
#include <aex/api_definitions.hpp>
#include <aex/Logger.hpp>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
#if __TARGET_AEX_API__ == __GLES__ || __TARGET_AEX_API__ == __GL__
    AexEnum err = glGetError();

    if(err != GL_NO_ERROR)
    {
        std::string err_msg;
        switch(err)
        {
        case 0x500:
        {
            err_msg = "GL_INVALID_ENUM";
        }
        break;

        case 0x501:
        {
            err_msg = "GL_INVALID_VALUE";
        }
        break;

        case 0x502:
        {
            err_msg = "GL_INVALID_OPERATION";
        }
        break;

        case 0x503:
        {
            err_msg = "GL_STACK_OVERFLOW";
        }
        break;

        case 0x504:
        {
            err_msg = "GL_STACK_UNDERFLOW";
        }
        break;

        case 0x505:
        {
            err_msg = "GL_OUT_OF_MEMORY";
        }
        break;

        case 0x506:
        {
            err_msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
        }
        break;

        case 0x8031:
        {
            err_msg = "GL_TABLE_TOO_LARGE";
        }
        break;
        }

        LOG_ERR("OpenGL error", err, " - ", err_msg, " at ", fname, ":", line, " - for ", stmt);
        LOG_ERR("GLSL Version = ", glGetString(GL_SHADING_LANGUAGE_VERSION));
        LOG_ERR("GL Version = ", glGetString(GL_VERSION));
#ifndef __NO__ABORT__
        //abort();
#endif
#ifdef AEX_GL_EXCEPTIONS
        std::stringstream ss;
        ss << "OpenGL error " << err << " - " << errMsg << " at " << fname << ":" << line << " - for " << stmt << std::endl;
        throw std::runtime_error {ss.str()};
#endif
    }
    else
    {
        //LOG_DEBUG( "OpenGL trace: call ", stmt, " at ", fname, ":", line );
    }

#endif
}
