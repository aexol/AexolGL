//
//  UniformVector3fv.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/UniformVector3fv.hpp>
#include "../include/UniformInternal.hpp"
#include <aex/Logger.hpp>
namespace aex
{
UniformVector3fv::UniformVector3fv() : Base{} {}

UniformVector3fv::UniformVector3fv(const aex::string& name) : Base{name} {}

UniformVector3fv::UniformVector3fv(const value_type& value) : Base{value} {}

UniformVector3fv::UniformVector3fv(const aex::math::Vector3& value) : UniformVector3fv{value, ""} {}

UniformVector3fv::UniformVector3fv(const value_type& value, const aex::string& name) :
    Base{value, name} {}

UniformVector3fv::UniformVector3fv(const aex::math::Vector3& value, const aex::string& name) :
    Base{value_type{{{value.x, value.y, value.z}}}, name} {}

Uniform*
UniformVector3fv::copy()
{
    return new UniformVector3fv(*this);
}

void
UniformVector3fv::checkUniform(void* uInfo)
{
    UniformLib::setVec3fShaderParam(uInfo, this->m_value);
}

void UniformVector3fv::setValue(const std::vector<aex::math::Vector3>& value)
{
    value_type t;
    t.reserve(value.size());
    for(const aex::math::Vector3& vec : value)
        t.push_back({{vec.x, vec.y, vec.z}});
    setValue(t);
}
}
