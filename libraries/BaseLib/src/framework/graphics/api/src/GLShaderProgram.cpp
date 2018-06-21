#include "../include/GLContext.hpp"
#include "../include/GLShaderProgram.hpp"
#include "../../include/ShaderProgramInternal.hpp"
#include "../include/GLUniforms.hpp"
#include "../include/GLVertexAttribArrays.hpp"
#include "../include/GLBuffers.hpp"
#include "../../include/ShaderProgramInternal.hpp"
#include <aex/Common.hpp>
#include <aex/GPUApi.hpp>
#include <aex/Logger.hpp>
#include <aex/utils.hpp>
#include <aex/ax_Macros.hpp>
#include <mutex>
#include <unordered_map>
#include <map>

namespace aex
{
    struct InternalGLShaderProgram final : public ContextObject
    {
        typedef std::unique_ptr<UniformInfo> UniformInfo_ptr;
        typedef std::unique_ptr<UniformInfo_ptr> map_type;
        typedef std::unordered_map<aex::string, map_type> uniform_map_t;
        void draw() const;
        void drawPart(size_t count , size_t offset) const;
        uint32_t m_progId;
        uniform_map_t m_uniformMap;
        std::map<aex::string, std::pair<aex::string, uint32_t>> m_attributeMap;
        std::mutex m_lock;
        void initInternalShader();
        void onContextDirty(); 
    };

    void
    InternalGLShaderProgram::draw() const
    {
        context_guard ctxGuard {};
        return ctxGuard([&](aex_context & ctx)
        {
            commitChanges(ctx);
            IndexBuffer ib = getIndexBuffer(ctx);
            GL_CHECK(glDrawElements(ib.type, ib.count, GL_UNSIGNED_SHORT, 0));
        });
    }
    
    void
    InternalGLShaderProgram::drawPart(size_t count , size_t offset) const
    {
        context_guard ctxGuard {};
        return ctxGuard([&](aex_context & ctx)
        {
            commitChanges(ctx);
            IndexBuffer ib = getIndexBuffer(ctx);
            //LOG_DEBUG(count," ", offset);
            GL_CHECK(glDrawElements(ib.type, count, GL_UNSIGNED_SHORT,   (const GLvoid *)(offset * sizeof(GLushort))));
        });
    }

    void
    InternalGLShaderProgram::initInternalShader()
    {
        int prog;
        GL_CHECK(prog = glCreateProgram());
        m_attributeMap["gl_Vertex"] = std::make_pair("_gl_Vertex", 0);
        m_attributeMap["gl_TexCoord"] = std::make_pair("_gl_TexCoord", 1);
        m_attributeMap["gl_Normal"] = std::make_pair("_gl_Normal", 2);
        m_attributeMap["gl_nextFrame"] = std::make_pair("_gl_nextFrame", 3);
        m_progId = prog;
    }

    void
    InternalGLShaderProgram::onContextDirty()
    {
        m_progId = 0;
    }

    void* makeInternalShader()
    {
        InternalGLShaderProgram* p = new InternalGLShaderProgram;
        p->initInternalShader();
        return p;
    }

    void freeInternalShader(void* s)
    {
        InternalGLShaderProgram* prog = static_cast<InternalGLShaderProgram*>(s);
        int32_t pn = prog->m_progId;
        GL_CHECK(glDeleteProgram(pn));
        delete prog;
    }

    void* findParameter(const aex::string& name, void* shader)
    {
        typedef InternalGLShaderProgram::UniformInfo_ptr uniform_ptr_t;
        typedef InternalGLShaderProgram::map_type map_type;
        typedef InternalGLShaderProgram::uniform_map_t::value_type uniform_map_value_t;
        InternalGLShaderProgram* prog = static_cast<InternalGLShaderProgram*>(shader);
        uniform_ptr_t* uniformInfoP;

        try
        {
            uniformInfoP = prog->m_uniformMap.at(name).get();
        } catch(std::out_of_range& e) {
            //uniformInfoP = makeUniformInfo(prog->m_progId, name);
            prog->m_uniformMap.insert(uniform_map_value_t{name, map_type{new uniform_ptr_t{new UnsetUniformInfo{prog->m_progId, name}}}});
            uniformInfoP = prog->m_uniformMap.at(name).get();
        }

        return uniformInfoP;
    }

    void enableProgram(void* s)
    {
        InternalGLShaderProgram* _s = static_cast<InternalGLShaderProgram*>(s);
        uint32_t progId = _s->m_progId;

        if(progId != 0)
        {
            context_guard ctxGuard {};
            ctxGuard([progId, _s](aex_context & ctx)
            {
                if(ctx.ShaderProgramState != progId)
                {
                    ctx.ShaderProgramState = progId;
                    GL_CHECK(glUseProgram(progId));
                    for(auto& u_info : _s->m_uniformMap)
                    {
                        (*u_info.second)->set();
                    }
                }
            });
        }
    }

    int32_t
    compileSource(int32_t type, const aex::string& source)
    {
        uint32_t shader;
        GL_CHECK(shader = glCreateShader(type));
        const char* c_str = source.c_str();
        GL_CHECK(glShaderSource(shader, 1, &c_str, NULL));
        GL_CHECK(glCompileShader(shader));
        int32_t compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if(compiled == GL_FALSE)
        {
            LOG_ERR("Shader compilation error, compilation log:", aex::getShaderLog(shader));
            LOG_ERR("GLSL Version = ", glGetString(GL_SHADING_LANGUAGE_VERSION));
            LOG_ERR("GL Version = ", glGetString(GL_VERSION));
            LOG_ERR("Shader Source: ", source);
#ifndef NDEBUG
            abort();
#endif
        }
        return shader;
    }

    void* makeVertexShader(const aex::string& vsource)
    {
        int32_t* s = new int32_t;
        *s = compileSource(GL_VERTEX_SHADER, vsource);
        return s;
    }

    void* makeFragmentShader(const aex::string& vsource)
    {
        int32_t* s = new int32_t;
        *s = compileSource(GL_FRAGMENT_SHADER, vsource);
        return s;
    }

    void makeShader(void* prog, void* vprog, void* fprog)
    {
        const InternalGLShaderProgram& glProg = *static_cast<const InternalGLShaderProgram*>(prog);
        uint32_t progId = glProg.m_progId;

        if(prog == 0)
        {
            LOG_DEBUG("Program ID = 0!");
            return;
        }

        int32_t shaderV = *static_cast<int32_t*>(vprog);
        int32_t shaderF = *static_cast<int32_t*>(fprog);
        GL_CHECK(glAttachShader(progId, shaderV));
        GL_CHECK(glAttachShader(progId, shaderF));

        for(auto& attrib : glProg.m_attributeMap)
        {
            GL_CHECK(glBindAttribLocation(progId, attrib.second.second, attrib.second.first.c_str()));
        }

        GL_CHECK(glLinkProgram(progId));
        int32_t linkStatus;
        GL_CHECK(glGetProgramiv(progId, GL_LINK_STATUS, &linkStatus));

        if(linkStatus == GL_FALSE)
        {
            LOG_ERR("Shader compilation error, compilation log:", aex::getShaderProgramLog(progId));
            LOG_ERR("GLSL Version = ", glGetString(GL_SHADING_LANGUAGE_VERSION));
            LOG_ERR("GL Version = ", glGetString(GL_VERSION));
            std::runtime_error {"Shader compilation failed."};
        }

        GL_CHECK(glDetachShader(progId, shaderV));
        GL_CHECK(glDetachShader(progId, shaderF));
    }

    void freeShaderObject(void* s)
    {
        int32_t* impl = static_cast<int32_t*>(s);
        int32_t program = *impl;
        GL_CHECK(glDeleteShader(program));
        delete impl;
    }

    void setAttribImpl(void* prog, const aex::string& name, const api_detail::DataFormatCommon& attrType)
    {
        InternalGLShaderProgram& glProg = *static_cast<InternalGLShaderProgram*>(prog);
        auto mapIndex = glProg.m_attributeMap.insert(std::make_pair(name, std::make_pair(name, 0)));
        int32_t location = -1;

        if(mapIndex.second)
        {
            GL_CHECK(location = glGetAttribLocation(glProg.m_progId, name.c_str()));

            if(location == -1)
                std::runtime_error {"Invalid vertex data layout."};

            mapIndex.first->second = std::make_pair(name, location);
        }

        location = mapIndex.first->second.second;
        setArray(location, attrType);
    }

    void setAttribImpl(void* prog, const api_detail::AttribList& attribs)
    {
        for(auto& att : attribs)
        {
            const aex::string& name = att.first;
            const api_detail::DataFormatCommon& attrType = att.second;
            InternalGLShaderProgram& glProg = *static_cast<InternalGLShaderProgram*>(prog);
            auto mapIndex = glProg.m_attributeMap.insert(std::make_pair(name, std::make_pair(name, 0)));
            int32_t location = -1;

            if(mapIndex.second)
            {
                GL_CHECK(location = glGetAttribLocation(glProg.m_progId, name.c_str()));

                if(location == -1)
                    std::runtime_error {"Invalid vertex data layout."};

                mapIndex.first->second = std::make_pair(name, location);
            }

            location = mapIndex.first->second.second;
            setArray(location, attrType);
        }
    }

    void unsetAttribImpl(void* prog, const aex::string& name)
    {

        const InternalGLShaderProgram& glProg = *static_cast<InternalGLShaderProgram*>(prog);
        const auto mapIndex = glProg.m_attributeMap.find(name);

        if(mapIndex != glProg.m_attributeMap.end())
        {
            const int32_t location = mapIndex->second.second;
            unsetArray(location);
        }
    }

    void unsetAttribImpl(void* prog, const api_detail::AttribList& attribs)
    {
        for(auto& att : attribs)
        {
            const aex::string& name = att.first;
            unsetAttribImpl(prog, name);
        }
    }

    void drawInternal(void* prog)
    {
        static_cast<InternalGLShaderProgram*>(prog)->draw();
    }
    
    void drawInternalPart(void* prog, size_t count, size_t offset)
    {
        static_cast<InternalGLShaderProgram*>(prog)->drawPart(count, offset);
    }
}
