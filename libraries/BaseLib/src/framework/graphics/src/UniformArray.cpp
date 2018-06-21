//
//  UniformArray.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/UniformArray.hpp>
#include <aex/ShaderDrw.hpp>
namespace aex
{
UniformArray::UniformArray(const aex::string& name) :
    Uniform(name),
    uniforms(),
    m_size(0)
{}
UniformArray::UniformArray(const UniformArray& array) :
    Uniform(array.m_name),
    uniforms(),
    m_size(array.uniforms.size())
{
    uniforms.reserve(m_size);

    for(auto u : array.uniforms)
    {
        uniforms.push_back(u);
    }
}

void
UniformArray::setUniform(const ShaderDrw& shader)
{
    setUniform(shader.GetProgram());
}
void
UniformArray::setUniform(const AexShader& programHandler)
{
    for(auto& uniform : uniforms)
    {
        uniform->setUniform(programHandler);
    }
}
void
UniformArray::addContent(Uniform* uniform)
{
    if(m_name.empty()) throw UnnamedUniform{};

    uniform->setName(m_name + "[" + aex::to_string(uniforms.size()) + "]");

    uniforms.push_back(uniform);

    m_size++;
}
void
UniformArray::setName(const aex::string& name)
{
    for(size_t  u = 0 ; u < uniforms.size(); ++u)
    {
        uniforms[u]->setName(m_name + "[" + aex::to_string(u) + "]");
    }

    m_name = name;
}
Uniform*
UniformArray::copy()
{
    return new UniformArray {*this};
}
}
