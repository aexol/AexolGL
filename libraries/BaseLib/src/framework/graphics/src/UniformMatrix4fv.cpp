//
//  UniformMatrix4fv.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/UniformMatrix4fv.hpp>
#include <aex/Matrix.hpp>
#include "../include/UniformInternal.hpp"
namespace aex
{
UniformMatrix4fv::UniformMatrix4fv() : Base{} {}

UniformMatrix4fv::UniformMatrix4fv(const aex::string& name) : Base{name} {}

UniformMatrix4fv::UniformMatrix4fv(const std::vector<float>& value) : Base{value} {}

UniformMatrix4fv::UniformMatrix4fv(const value_type& value) : Base{value} {}

UniformMatrix4fv::UniformMatrix4fv(const std::vector<float>& value, const aex::string& name) : Base{value, name} {}

UniformMatrix4fv::UniformMatrix4fv(const value_type& value, const aex::string& name) : Base{value, name} {}

UniformMatrix4fv::UniformMatrix4fv(const Matrix& value) : UniformMatrix4fv(value.getM()) {}

UniformMatrix4fv::UniformMatrix4fv(const Matrix& value, const aex::string& name) : UniformMatrix4fv(value.getM(), name) {}

Uniform*
UniformMatrix4fv::copy()
{
    return new UniformMatrix4fv(*this);
}

void
UniformMatrix4fv::checkUniform(void* uInfo)
{
    std::vector<mat4> value;
    value.push_back(mat4(this->m_value));
    UniformLib::setMat4fShaderParam(uInfo, value);
}

void
UniformMatrix4fv::setValue(const Matrix& value)
{
    setValue(value.getM());
}
}
