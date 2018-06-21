//  UniformVector2fv.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/UniformVector2fv.hpp>
#include "../include/UniformInternal.hpp"
namespace aex
{
UniformVector2fv::UniformVector2fv() : UniformVector2fv("") {}

UniformVector2fv::UniformVector2fv(const aex::string& name) : Base{value_type{{{0.0f, 0.0f}}}, name} {}

UniformVector2fv::UniformVector2fv(const value_type& value) : Base{value} {}

UniformVector2fv::UniformVector2fv(const value_type& value, const aex::string& name) : Base{value, name} {}

void
UniformVector2fv::setValue(const std::vector<aex::math::Vector2>& value)
{
    value_type t;
    t.reserve(value.size());
    for(const aex::math::Vector2& vec : value)
        t.push_back({{vec.x, vec.y}});
    setValue(t);
}

Uniform*
UniformVector2fv::copy()
{
    return new UniformVector2fv(*this);
}
void
UniformVector2fv::checkUniform(void* uInfo)
{
    UniformLib::setVec2fShaderParam(uInfo, this->m_value);
}
}
