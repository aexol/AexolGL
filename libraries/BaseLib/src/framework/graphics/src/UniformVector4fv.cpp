//
//  UniformVector4fv.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/UniformVector4fv.hpp>
#include "../include/UniformInternal.hpp"
namespace aex
{
UniformVector4fv::UniformVector4fv() : Base{} {}

UniformVector4fv::UniformVector4fv(const aex::string& name) : Base{name} {}

UniformVector4fv::UniformVector4fv(const value_type& value) : Base{value} {}

UniformVector4fv::UniformVector4fv(const value_type& value, const aex::string& name) :
    Base{value, name} {}

Uniform*
UniformVector4fv::copy()
{
    return new UniformVector4fv(*this);
}
void
UniformVector4fv::checkUniform(void* uInfo)
{
    UniformLib::setVec4fShaderParam(uInfo, m_value);
}
}
