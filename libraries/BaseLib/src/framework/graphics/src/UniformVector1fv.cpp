//
//  UniformVector1f.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/UniformVector1fv.hpp>
#include "../include/UniformInternal.hpp"
namespace aex
{
UniformVector1fv::UniformVector1fv() :
    Base{""} {}

UniformVector1fv::UniformVector1fv(const aex::string& name) :
    Base{name} {}

UniformVector1fv::UniformVector1fv(const value_type& value) :
    UniformVector1fv(value, "") {}

UniformVector1fv::UniformVector1fv(const value_type& value, const aex::string& name) :
    Base(value, name) {}

Uniform*
UniformVector1fv::copy()
{
    return new UniformVector1fv(*this);
}
void
UniformVector1fv::checkUniform(void* uInfo)
{
    UniformLib::setVec1fShaderParam(uInfo, this->m_value);
}
}
