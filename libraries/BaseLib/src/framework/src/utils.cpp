//
//  utils.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 09.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/utils.hpp>
#include <aex/api_definitions.hpp>
#include "../include/internal_sdl.hpp"

namespace aex
{
    void
    fatalError(const char* string)
    {
        printf("%s: %s\n", string, SDL_GetError());
        exit(1);
    }
    aex::string getShaderLog(uint32_t obj)
    {
#if __TARGET_AEX_API__ == __GLES__ || __TARGET_AEX_API__ == __GL__
        int infologLength = 0;

        int charsWritten  = 0;
        char* infoLog;

        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

        if(infologLength > 0)
        {
            infoLog = (char*)malloc(infologLength);
            glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);

            aex::string log = infoLog;

            free(infoLog);

            return log;
        }

#endif

        return "<Blank Log>";
    }
    aex::string getShaderProgramLog(uint32_t obj)
    {
#if __TARGET_AEX_API__ == __GLES__ || __TARGET_AEX_API__ == __GL__
        int infologLength = 0;

        int charsWritten  = 0;
        char* infoLog;

        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

        if(infologLength > 0)
        {
            infoLog = (char*)malloc(infologLength);
            glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);

            aex::string log = infoLog;

            free(infoLog);

            return log;
        }

#endif
        return "<Blank Log>";
    }

    aex::math::Vector3 vectorToClipSpace(const aex::math::Vector3& vec, const aex::math::Matrix& Projection, const aex::math::Matrix& ModelView)
    {
        return (Projection * ModelView).transformPoint(vec);
    }
}
