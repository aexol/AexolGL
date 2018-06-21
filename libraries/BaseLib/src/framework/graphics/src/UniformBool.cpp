//
//  UniformBool.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include "../include/ApiInternal.hpp"
#include "../include/UniformInternal.hpp"
#include <aex/UniformBool.hpp>
namespace aex
{
    UniformBool::UniformBool() : UniformBool(false, "") {}
    UniformBool::UniformBool(bool value) : UniformBool(value, "") {}
    UniformBool::UniformBool(const aex::string& name) : UniformBool(false, name) {}
    UniformBool::UniformBool(bool value, const aex::string& name):
        UniformPrimitive(name),
        m_value(value)
    {
    }
    void
    UniformBool::checkUniform(void* uInfo)
    {
        UniformLib::setBoolShaderParam(uInfo, m_value);
    }
    Uniform*
    UniformBool::copy()
    {
        return new UniformBool(*this);
    }
    
    /*UniformBool&
    UniformBool::operator=(const UniformBool& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    UniformBool&
    UniformBool::operator=(UniformBool&& rhs)
    {
        this->m_name = std::move(rhs.m_name);
        this->m_base_name = std::move(rhs.m_base_name);
        this->m_value = std::move(rhs.m_value);
        return *this;
    }*/
}
