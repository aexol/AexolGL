//
//  UniformPrimitive.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/UniformPrimitive.hpp>
#include <aex/ShaderDrw.hpp>
#include <aex/ShaderProgram.hpp>
#include <aex/Logger.hpp>
namespace aex
{

void
UniformPrimitive::setUniform(const AexShader& programHandler)
{
    if(!m_name.empty() && programHandler)
    {
        void* uInfo = programHandler->getParameterLocation(m_name);
        checkUniform(uInfo);
    }
}
void
UniformPrimitive::setUniform(const ShaderDrw& shader)
{
    const auto& program = shader.GetProgram();
    if(!program)
    {
        throw UniformError("Could not set uniform, bad program.");
    }
    setUniform(program);
}

void
UniformPrimitive::addContent(std::unique_ptr<Uniform>&&)
{

}

void
UniformPrimitive::setName(const aex::string& name)
{
    m_name = name;
}

UniformPrimitive::UniformPrimitive(const aex::string& name) :
    Uniform(name){}

UniformPrimitive::UniformPrimitive(aex::string&& name) :
    Uniform(std::move(name)){}

UniformPrimitive::UniformPrimitive() :
    Uniform()
{

}

UniformPrimitive::~UniformPrimitive()
{
    
}
}
