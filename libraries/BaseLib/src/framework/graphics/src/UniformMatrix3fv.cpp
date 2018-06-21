//
//  UniformMatrix3f.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/UniformMatrix3fv.hpp>
#include "../include/UniformInternal.hpp"
namespace aex
{
    UniformMatrix3fv::UniformMatrix3fv() : Base{} {}

    UniformMatrix3fv::UniformMatrix3fv(const aex::string& name) : Base{name} {}

    UniformMatrix3fv::UniformMatrix3fv(const std::vector<float>& value) : Base{value} {}

    UniformMatrix3fv::UniformMatrix3fv(const value_type& value) : Base{value} {}

    UniformMatrix3fv::UniformMatrix3fv(const std::vector<float>& value, const aex::string& name) : Base{value, name} {}

    UniformMatrix3fv::UniformMatrix3fv(const value_type& value, const aex::string& name) : Base{value, name} {}
    Uniform*
    UniformMatrix3fv::copy()
    {
        return new UniformMatrix3fv(*this);
    }
    void
    UniformMatrix3fv::checkUniform(void* uInfo)
    {
        std::vector<mat3> value;
        value.push_back(mat3(this->m_value));
        UniformLib::setMat3fShaderParam(uInfo, value);
    }
}
