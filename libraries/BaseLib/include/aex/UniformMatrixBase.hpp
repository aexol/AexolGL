#ifndef __AEX_UNIFORMMATRIXBASE_HPP
#define __AEX_UNIFORMMATRIXBASE_HPP
#include <aex/UniformPrimitive.hpp>
#include <aex/Math.hpp>
#include <array>
#include <vector>

namespace aex
{
    template<class T, std::size_t SIZE>
    class UniformMatrixBase : public UniformPrimitive
    {
    public:
        typedef std::array<T, SIZE> value_type;
        UniformMatrixBase();
        explicit UniformMatrixBase(const UniformMatrixBase&);
        explicit UniformMatrixBase(UniformMatrixBase&&);
        explicit UniformMatrixBase(const aex::string& name);
        explicit UniformMatrixBase(const value_type& value);
        explicit UniformMatrixBase(const std::vector<T>& value);
        UniformMatrixBase(const value_type& value, const aex::string& name);
        UniformMatrixBase(const std::vector<T>& value, const aex::string& name);
        UniformMatrixBase& operator=(const UniformMatrixBase&);
        UniformMatrixBase& operator=(UniformMatrixBase&&);
        void setValue(const value_type& value);
        void setValue(const std::vector<T>& value);
        value_type getValueA() const;
        std::vector<T> getValue() const;
    protected:
        value_type m_value;
    };
    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase() : UniformMatrixBase(aex::math::makeArrayIdentity<T, SIZE>(), "") {}

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase(const UniformMatrixBase& rhs) :
        UniformMatrixBase(rhs.m_value, rhs.m_name){}

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase(UniformMatrixBase&& rhs) :
        UniformPrimitive(std::move(rhs.m_name)),
        m_value(std::move(rhs.m_value)){}

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase(const aex::string& name) : UniformPrimitive(name) {}

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase(const value_type& value) : UniformMatrixBase(value, "") {}

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase(const std::vector<T>& value) : UniformMatrixBase(value, "") {}

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase(const value_type& value, const aex::string& name) : UniformPrimitive(name)
    {
        setValue(value);
    }

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>::UniformMatrixBase(const std::vector<T>& value, const aex::string& name) : UniformPrimitive(name)
    {
        setValue(value);
    }

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>&
    UniformMatrixBase<T, SIZE>::operator=(const UniformMatrixBase& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    template<class T, std::size_t SIZE>
    UniformMatrixBase<T, SIZE>&
    UniformMatrixBase<T, SIZE>::operator=(UniformMatrixBase&& rhs)
    {
        this->m_name = rhs.m_name;
        this->m_base_name = rhs.m_base_name;
        this->m_value = rhs.m_value;
        return *this;
    }

    template<class T, std::size_t SIZE>
    void
    UniformMatrixBase<T, SIZE>::setValue(const value_type& value)
    {
        if(&m_value != &value)
        {
            m_value = aex::math::transpose(value);
        }
    }

    template<class T, std::size_t SIZE>
    void
    UniformMatrixBase<T, SIZE>::setValue(const std::vector<T>& value)
    {
        value_type arr = aex::math::makeArrayIdentity<T, SIZE>();
        aex::math::vectorToArray(value, arr);
        setValue(arr);
    }

    template<class T, std::size_t SIZE>
    typename UniformMatrixBase<T, SIZE>::value_type
    UniformMatrixBase<T, SIZE>::getValueA() const
    {
        return aex::math::transpose(m_value);
    }

    template<class T, std::size_t SIZE>
    std::vector<T>
    UniformMatrixBase<T, SIZE>::getValue() const
    {
        std::vector<T> ret;
        aex::math::arrayToVector(getValueA(), ret);
        return ret;
    }
}

#endif
