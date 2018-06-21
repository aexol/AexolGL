//
//  UniformStruct.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/UniformStruct.hpp>
#include <aex/ShaderDrw.hpp>
namespace aex
{
Uniform*
UniformStruct::copy()
{
    return new UniformStruct(*this);
}
UniformStruct::UniformStruct(const aex::string& name):
    Uniform(name),
    uniforms {},
    m_empty {true}
{
}
UniformStruct::~UniformStruct() {}
void
UniformStruct::setUniform(const ShaderDrw& shader)
{
    setUniform(shader.GetProgram());
}

void
UniformStruct::setUniform(const AexShader& programHandler)
{
    for(auto& uniform : uniforms)
    {
        uniform->setUniform(programHandler);
    }
}

void 
UniformStruct::setContent(std::vector<Uniform*> uniforms)
{
    for(size_t i = 0 ;i< uniforms.size() ;++i)
    {
        addContent(uniforms[i]);
    }
}

void
UniformStruct::addContent(Uniform* u)
{
    if(m_name.empty()) throw UnnamedUniform{};

    m_empty = false;

    u->setName(m_name + "." + u->getBaseName());

    uniforms.push_back(u);
}
void
UniformStruct::setName(const aex::string& name)
{
    for(auto& u : uniforms)
    {
        u->setName(name + "." + u->getBaseName());
    }

    m_name = name;
}


}
