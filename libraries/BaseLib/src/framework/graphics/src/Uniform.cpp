//
//  Uniform.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/Uniform.hpp>
#include <aex/ShaderProgram.hpp>
#include <exception>
namespace aex
{
    DEFINE_AEX_EXCEPTION(UniformError)
    const char*
    UnnamedUniform::what() const NOEXCEPT
    {
        return "Uniform has no name";
    }
    aex::string
    Uniform::getName()
    {
        return m_name;
    }
    aex::string
    Uniform::getBaseName()
    {
        return m_base_name;
    }
    Uniform::Uniform(const aex::string& name) :
        m_name(name),
        m_base_name(name)
    {
    }

    Uniform::Uniform(aex::string&& name) :
        m_name(std::move(name)),
        m_base_name(m_name)
    {
    }

    Uniform::Uniform() :
        m_name("")
    {
    }
}
