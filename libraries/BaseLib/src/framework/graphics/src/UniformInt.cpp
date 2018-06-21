#include <aex/UniformInt.hpp>
#include "../include/UniformInternal.hpp"

namespace aex
{
//////////////////////////////////////////Float

    UniformInt::UniformInt() : UniformInt(0, "") {}
    UniformInt::UniformInt(int value) : UniformInt(value, "") {}
    UniformInt::UniformInt(const aex::string& name) : UniformInt(0, name) {}
    UniformInt::UniformInt(int value, const aex::string& name) :
        UniformPrimitive(name),
        m_value(value)
    {

        // valueChanged();
    }

    Uniform*
    UniformInt::copy()
    {
        return new UniformInt(*this);
    }
    void
    UniformInt::checkUniform(void* uInfo)
    {
        UniformLib::setIntShaderParam(uInfo, m_value);
    }

    UniformInt::~UniformInt()
    {

    }

    void UniformInt::setValue(int value)
    {
        m_value = value;
    }

    int UniformInt::getValue() const
    {
        return m_value;
    }



}
