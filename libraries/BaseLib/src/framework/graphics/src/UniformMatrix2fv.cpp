//
//  UniformMatrix2f.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/UniformMatrix2fv.hpp>
#include "../include/UniformInternal.hpp"
namespace aex
{
    UniformMatrix2fv::UniformMatrix2fv() : Base{} {}

    UniformMatrix2fv::UniformMatrix2fv(const aex::string& name) : Base{name} {}

    UniformMatrix2fv::UniformMatrix2fv(const std::vector<float>& value) : Base{value, ""} {}

    UniformMatrix2fv::UniformMatrix2fv(const value_type& value) : Base{value, ""} {}

    UniformMatrix2fv::UniformMatrix2fv(const std::vector<float>& value, const aex::string& name) :
        Base(value, name) {}

    UniformMatrix2fv::UniformMatrix2fv(const value_type& value, const aex::string& name) : Base{value, name}
    {
    }

    Uniform*
    UniformMatrix2fv::copy()
    {
        return new UniformMatrix2fv(*this);
    }
    void
    UniformMatrix2fv::checkUniform(void* uInfo)
    {
        std::vector<mat2> value;
        value.push_back(mat2(this->m_value));
        UniformLib::setMat2fShaderParam(uInfo, value);
    }
}
