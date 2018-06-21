#ifndef __AEX_UNIFORMVECTORBASE_HPP
#define __AEX_UNIFORMVECTORBASE_HPP
#include <aex/UniformPrimitive.hpp>
#include <aex/Math.hpp>
#include <string>
#include <vector>

namespace aex
{
template<class T, std::size_t SIZE>
class UniformVectorBase : public UniformPrimitive
{
public:
    typedef std::array<T, SIZE> element_type;
    typedef std::vector<element_type> value_type;

    UniformVectorBase();
    UniformVectorBase(const aex::string& name);
    UniformVectorBase(const value_type& value);
    UniformVectorBase(const element_type& value);
    UniformVectorBase(const value_type& value, const aex::string& name);
    UniformVectorBase(const element_type& value, const aex::string& name);
    template<class _InputIterator>
    UniformVectorBase(const _InputIterator& begin, const _InputIterator& end, const aex::string& name);

    void setValue(const value_type& value);
    void setValue(const element_type& value);
    template<class _InputIterator>
    void setValue(const _InputIterator& begin, const _InputIterator& end);
    const value_type& getValue() const;
protected:
    std::size_t getArraySize();
    value_type m_value;
};

template<class T, std::size_t SIZE>
UniformVectorBase<T, SIZE>::UniformVectorBase() :
    UniformVectorBase(value_type{}, "") {}

template<class T, std::size_t SIZE>
UniformVectorBase<T, SIZE>::UniformVectorBase(const aex::string& name) :
    UniformVectorBase(value_type{}, name) {}

template<class T, std::size_t SIZE>
UniformVectorBase<T, SIZE>::UniformVectorBase(const value_type& value) : UniformVectorBase(value, "") {}

template<class T, std::size_t SIZE>
UniformVectorBase<T, SIZE>::UniformVectorBase(const element_type& value) : UniformVectorBase(value, "") {}

template<class T, std::size_t SIZE>
UniformVectorBase<T, SIZE>::UniformVectorBase(const value_type& value, const aex::string& name) : UniformPrimitive(name)
{
    setValue(value);
}

template<class T, std::size_t SIZE>
UniformVectorBase<T, SIZE>::UniformVectorBase(const element_type& value, const aex::string& name) : UniformPrimitive(name)
{
    setValue(value);
}

template<class T, std::size_t SIZE>
template<class _InputIterator>
UniformVectorBase<T, SIZE>::UniformVectorBase(const _InputIterator& begin, const _InputIterator& end, const aex::string& name) 
: UniformPrimitive(name)
{
    setValue(begin, end);
}

template<class T, std::size_t SIZE>
void
UniformVectorBase<T, SIZE>::setValue(const value_type& value)
{
    m_value = value;
}

template<class T, std::size_t SIZE>
template<class _InputIterator>
void
UniformVectorBase<T, SIZE>::setValue(const _InputIterator& begin, const _InputIterator& end)
{
    std::size_t size = std::distance(begin, end);
    m_value.resize(size);
    std::copy(begin, end, m_value.begin());
}

template<class T, std::size_t SIZE>
const typename UniformVectorBase<T, SIZE>::value_type&
UniformVectorBase<T, SIZE>::getValue() const
{
    return m_value;
}
template<class T, std::size_t SIZE>
std::size_t
UniformVectorBase<T, SIZE>::getArraySize()
{
    return m_value.size()/SIZE;
}

template<class T, std::size_t SIZE>
void
UniformVectorBase<T, SIZE>::setValue(const element_type& value)
{
    setValue(value_type{value});
}
}

#endif
