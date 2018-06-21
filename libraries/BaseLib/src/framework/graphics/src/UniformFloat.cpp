//
//  UniformFloat.cpp
//  aexolGL
//
//  Created by Kamil Matysiewicz on 22.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/UniformFloat.hpp>
#include <aex/Logger.hpp>
#include "../include/UniformInternal.hpp"

namespace aex
{
    //////////////////////////////////////////Float

    UniformFloat::UniformFloat() : UniformFloat(0.0f, "") { }

    UniformFloat::UniformFloat(const UniformFloat& rhs) : UniformFloat(rhs.m_value, rhs.m_name) { }

    UniformFloat::UniformFloat(UniformFloat&& rhs) :
        UniformPrimitive(std::move(rhs.m_name)),
        m_value(rhs.m_value) { }

    UniformFloat::UniformFloat(float value) : UniformFloat(value, "") { }

    UniformFloat::UniformFloat(const aex::string& name) : UniformFloat(0.0f, name) { }

    UniformFloat::UniformFloat(float value, const aex::string& name) :
    UniformPrimitive(name),
    m_value(value) { }

    UniformFloat&
    UniformFloat::operator=(const UniformFloat& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    UniformFloat&
    UniformFloat::operator=(UniformFloat&& rhs)
    {
        this->m_name = std::move(rhs.m_name);
        this->m_base_name = std::move(rhs.m_base_name);
        this->m_value = std::move(rhs.m_value);
        return *this;
    }

    Uniform*
    UniformFloat::copy()
    {
        return new UniformFloat(*this);
    }

    void
    UniformFloat::checkUniform(void* uInfo)
    {
        UniformLib::setFloatShaderParam(uInfo, m_value);
    }

    UniformFloat::~UniformFloat() { }

    void UniformFloat::setValue(float value)
    {
        m_value = value;
    }

    float UniformFloat::getValue() const
    {
        return m_value;
    }



    //////////////////////////////////////////Float2

    UniformFloat2::UniformFloat2() : UniformFloat2(0.0f, 0.0f) { }

    UniformFloat2::UniformFloat2(const UniformFloat2& rhs) : UniformFloat2(rhs.m_value, rhs.m_name) { }

    UniformFloat2::UniformFloat2(UniformFloat2&& rhs) :
        UniformPrimitive(std::move(rhs.m_name)),
        m_value(std::move(rhs.m_value)) { }

    UniformFloat2::UniformFloat2(aex::math::Vector2 value) : UniformFloat2(value, "") { }

    UniformFloat2::UniformFloat2(float x, float y) : UniformFloat2(x, y, "") { }

    UniformFloat2::UniformFloat2(const aex::string& name) : UniformFloat2(0.0f, 0.0f, name) { }

    UniformFloat2::UniformFloat2(float x, float y, const aex::string& name) : UniformFloat2(
    aex::math::Vector2(x, y), name) { }

    UniformFloat2::UniformFloat2(aex::math::Vector2 value, const aex::string& name) :
    UniformPrimitive(name),
    m_value(value) { }

    UniformFloat2&
    UniformFloat2::operator=(const UniformFloat2& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    UniformFloat2&
    UniformFloat2::operator=(UniformFloat2&& rhs)
    {
        this->m_name = std::move(rhs.m_name);
        this->m_base_name = std::move(rhs.m_base_name);
        this->m_value = std::move(rhs.m_value);
        return *this;
    }

    Uniform*
    UniformFloat2::copy()
    {
        return new UniformFloat2(*this);
    }

    void
    UniformFloat2::checkUniform(void* uInfo)
    {
        UniformLib::setFloat2ShaderParam(uInfo, m_value);
    }

    UniformFloat2::~UniformFloat2() { }

    void UniformFloat2::setValue(aex::math::Vector2 value)
    {
        m_value = value;
    }

    void UniformFloat2::setValue(float x, float y)
    {
        m_value.x=x;
        m_value.y=y;
    }

    aex::math::Vector2 UniformFloat2::getValue() const
    {
        return m_value;
    }



    //////////////////////////////////////////Float3

    UniformFloat3::UniformFloat3() : UniformFloat3(0.0f, 0.0f, 0.0f) { }

    UniformFloat3::UniformFloat3(const UniformFloat3& rhs) : UniformFloat3(rhs.m_value, rhs.m_name) { }

    UniformFloat3::UniformFloat3(UniformFloat3&& rhs) :
        UniformPrimitive(std::move(rhs.m_name)),
        m_value(std::move(rhs.m_value)) { }

    UniformFloat3::UniformFloat3(aex::math::Vector3 value) : UniformFloat3(value, "") { }

    UniformFloat3::UniformFloat3(float x, float y, float z) : UniformFloat3(aex::math::Vector3(x, y, z)
    , "") { }

    UniformFloat3::UniformFloat3(const aex::string& name) : UniformFloat3(
    aex::math::Vector3(
    0.0f, 0.0f, 0.0f
    ), name) { }

    UniformFloat3::UniformFloat3(float x, float y, float z, const aex::string& name) : UniformFloat3(
    aex::math::Vector3(
    x, y, z
    ), name) { }

    UniformFloat3::UniformFloat3(aex::math::Vector3 value, const aex::string& name) :
    UniformPrimitive(name),
    m_value(value) { }

    UniformFloat3&
    UniformFloat3::operator=(const UniformFloat3& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    UniformFloat3&
    UniformFloat3::operator=(UniformFloat3&& rhs)
    {
        this->m_name = std::move(rhs.m_name);
        this->m_base_name = std::move(rhs.m_base_name);
        this->m_value = std::move(rhs.m_value);
        return *this;
    }

    Uniform*
    UniformFloat3::copy()
    {
        return new UniformFloat3(*this);
    }

    void
    UniformFloat3::checkUniform(void* uInfo)
    {
        UniformLib::setFloat3ShaderParam(uInfo, m_value);
    }

    UniformFloat3::~UniformFloat3() { }

    void UniformFloat3::setValue(aex::math::Vector3 value)
    {
        m_value = value;
    }

    void UniformFloat3::setValue(float x, float y, float z)
    {
        setValue(aex::math::Vector3(x, y, z));
    }

    void UniformFloat3::setValue(int r, int g, int b)
    {
        setValue(aex::math::Vector3(((float)r)/255.0f, ((float)g)/255.0f, ((float)b)/255.0f));
    }
    
    
    aex::math::Vector3 UniformFloat3::getValue() const
    {
        return m_value;
    }


    //////////////////////////////////////////Float4

    UniformFloat4::UniformFloat4() : UniformFloat4(0.0f, 0.0f, 0.0f, 0.0f) { }

    UniformFloat4::UniformFloat4(const UniformFloat4& rhs) : UniformFloat4(rhs.m_value, rhs.m_name) { }

    UniformFloat4::UniformFloat4(UniformFloat4&& rhs) :
        UniformPrimitive(std::move(rhs.m_name)),
        m_value(std::move(rhs.m_value)) { }

    UniformFloat4::UniformFloat4(aex::FPoint4D value) : UniformFloat4(value, "") { }

    UniformFloat4::UniformFloat4(float x, float y, float z, float w) : UniformFloat4(aex::FPoint4D(x, y, z, w), "") { }

    UniformFloat4::UniformFloat4(const aex::string& name) : UniformFloat4(aex::FPoint4D(0.0f, 0.0f, 0.0f, 0.0f), name) { }

    UniformFloat4::UniformFloat4(float x, float y, float z, float w, const aex::string& name) : UniformFloat4(aex::FPoint4D(x, y, z, w), name) { }

    UniformFloat4::UniformFloat4(FPoint4D value, const aex::string& name) :
    UniformPrimitive(name),
    m_value(value) { }

    UniformFloat4&
    UniformFloat4::operator=(const UniformFloat4& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    UniformFloat4&
    UniformFloat4::operator=(UniformFloat4&& rhs)
    {
        this->m_name = std::move(rhs.m_name);
        this->m_base_name = std::move(rhs.m_base_name);
        this->m_value = std::move(rhs.m_value);
        return *this;
    }

    UniformFloat4::~UniformFloat4() { }
    
    void UniformFloat4::checkUniform(void* uInfo) 
    {
        UniformLib::setFloat4ShaderParam(uInfo, m_value.array());
    }

    void UniformFloat4::setValue(aex::FPoint4D value) { 
        m_value = value;
    }

    void UniformFloat4::setValue(float x, float y, float z, float w)
    {
        m_value[0]=x;
        m_value[1]=y;
        m_value[2]=z;
        m_value[3]=w;
    }
    
    void UniformFloat4::setValue(int r, int g, int b, int a)
    {
        m_value[0]=((float)r)/255.0f;
        m_value[1]=((float)g)/255.0f;
        m_value[2]=((float)b)/255.0f;
        m_value[3]=((float)a)/255.0f;
    }
    //void setValue(Std::ar);

    Uniform*
    UniformFloat4::copy()
    {
        return new UniformFloat4(*this);
    }

    FPoint4D UniformFloat4::getValue() const
    {
        return m_value;
    }

}
