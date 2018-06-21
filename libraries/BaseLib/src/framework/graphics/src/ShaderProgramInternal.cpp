#include "../include/ShaderProgramInternal.hpp"
#include <aex/Logger.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/VertexData.hpp>
namespace aex
{
    DEFINE_AEX_EXCEPTION(ShaderProgramError);

    /***********SHADER_PROGRAM*************/
    AexShader
    ShaderProgram::makeProgram()
    {
        AexShader program {new ShaderProgram, [](ShaderProgram * s)
            {
                delete s;
            }
        };
        program->m_impl.reset(makeInternalShader());
        return program;
    }
    ShaderProgram::ShaderProgram() : m_impl {nullptr, [](void* p)
    {
        freeInternalShader(p);
    }
                                            } {}
    ShaderProgram::~ShaderProgram()
    {
    }
    void
    ShaderProgram::swap(aex::ShaderProgram& prog)
    {
        m_impl.swap(prog.m_impl);
    }

    ShaderProgram::operator bool() const
    {
        return m_impl != nullptr;
    }

    void*
    ShaderProgram::getParameterLocation(const aex::string& name) const
    {
        return findParameter(name, m_impl.get());;
    }

    void ShaderProgram::enable() const
    {
        enableProgram(m_impl.get());
    }

    void
    ShaderProgram::setVertexData(const VertexData& vd) const
    {
        if(vd)
        {
            vd.enable();
            setAttribImpl(m_impl.get(), vd.m_attribList);
        }
    }

    void
    ShaderProgram::setVertexData(const VertexData& vd, const aex::string& name) const
    {
        if(vd)
        {
            vd.enable();

            if(vd.m_attribList.size() != 1)
            {
                throw ShaderProgramError("Cannot bind vertex data structure to named attribute.");
            }

            auto& attrib = vd.m_attribList.front();
            const api_detail::DataFormatCommon& attrType = attrib.second;
            setAttrib(name, attrType);
        }
    }

    void
    ShaderProgram::setVertexData(const VertexData& vd, const AttribList& attribMap) const
    {
        if(vd)
        {
            vd.enable();

            if(vd.m_attribList.size() != attribMap.size())
            {
                throw ShaderProgramError("Cannot bind vertex data structure, invalid attrib map.");
            }

            setAttribImpl(m_impl.get(), attribMap);
        }
    }

    void
    ShaderProgram::setAttrib(const aex::string& name, const api_detail::DataFormatCommon& attrType) const
    {
        setAttribImpl(m_impl.get(), name, attrType);
    }
    void
    ShaderProgram::unsetAttrib(const aex::string& name) const
    {
        unsetAttribImpl(m_impl.get(), name);
    }

    void
    ShaderProgram::releaseVertexData(const aex::VertexData& vd) const
    {
        if(vd)
        {
            vd.disable();
            unsetAttribImpl(m_impl.get(), vd.m_attribList);
        }
    }
    void
    ShaderProgram::releaseVertexData(const VertexData& vd, const aex::string& name)const
    {
        if(vd)
        {
            vd.disable();
            unsetAttrib(name);
        }
    }

    void
    ShaderProgram::releaseVertexData(const VertexData& vd, const AttribList& attribMap) const
    {
        if(vd)
        {
            vd.disable();

            if(vd.m_attribList.size() != attribMap.size())
            {
                throw ShaderProgramError("Cannot release vertex data structure, invalid attrib map.");
            }

            unsetAttribImpl(m_impl.get(), attribMap);
        }
    }

    void
    ShaderProgram::setIndices(const aex::IndexData& id) const
    {
        if(id)
            id.enable();
    }
    void
    ShaderProgram::draw() const
    {
        drawInternal(m_impl.get());
    }
    void
    ShaderProgram::drawPart( size_t count, size_t offset) const
    {
        drawInternalPart(m_impl.get(),  count, offset);
    }
    
    
    void buildProgramObject(const AexShader& program, const std::unique_ptr<ShaderObject>& v, const std::unique_ptr<ShaderObject>& f)
    {
        if(!program)
        {
            LOG_DEBUG("Program 0");
            return;
        }

        makeShader(program->m_impl.get(), v->m_impl.get(), f->m_impl.get());
    }
    /***********SHADER_OBJECT***********/
    std::unique_ptr<ShaderObject>
    ShaderObject::makeVertexShader(const aex::string& vsource)
    {
        std::unique_ptr<ShaderObject> s {new ShaderObject};
        s->m_impl.reset(aex::makeVertexShader(vsource));
        return s;
    }
    std::unique_ptr<ShaderObject>
    ShaderObject::makeFragmentShader(const aex::string& vsource)
    {
        std::unique_ptr<ShaderObject> s {new ShaderObject};
        s->m_impl.reset(aex::makeFragmentShader(vsource));
        return s;
    }
    ShaderObject::ShaderObject() : m_impl {nullptr, [](void* p)
    {
        freeShaderObject(p);
    }
                                          } {};
    ShaderObject::ShaderObject(ShaderObject&& shader)
    {
        std::swap(m_impl, shader.m_impl);
    }
    ShaderObject::~ShaderObject()
    {
    }

    /***********SHADER_PARAMETER_MAPPING*******/
    ShaderParameterMapping::ShaderParameterMapping() : m_standAlone {false}, m_impl {nullptr,
                [](void* p)
    {
        freeParamInternal(p);
    }
                                                                                    }
    {}
    ShaderParameterMapping::operator bool() const
    {
        if(m_impl)
        {
            return testParameter(m_impl.get());
        }

        return false;
    }
}
