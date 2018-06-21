#ifndef _AEX_BOX_HPP
#define _AEX_BOX_HPP

#include <aex/Common.hpp>
#include <aex/Logger.hpp>
#include <aex/Point.hpp>
#include <array>

namespace aex
{
template<class T, size_t DIM>
class AEX_PUBLIC_API Box;
template<class T, size_t S>
bool operator==(const Box<T, S>& lhs, const Box<T, S>& rhs);
template<class T, size_t S>
bool operator!=(const Box<T, S>& lhs, const Box<T, S>& rhs);
template<class T, size_t DIM>
class AEX_PUBLIC_API Box
{
public:
    static CONSTEXPR size_t dimensons = DIM;
    typedef T value_type;
    template<class... Args>
    Box(Args... args) : m_data {{aex::forward<Args>(args)...}} {}
    Point<T, DIM>& operator[](size_t idx)
    {
        _AEX_ASSERT_LOG(idx < 2);
        return m_data[idx];
    }
    const Point<T, DIM>& operator[](size_t idx) const
    {
        _AEX_ASSERT_LOG(idx < 2);
        return m_data[idx];
    }
    const Point<T, DIM>& at(size_t idx) const
    {
        _AEX_ASSERT_LOG(idx < 2);
        return m_data[idx];
    }
protected:
    std::array<Point<T, DIM>, 2> m_data;
    friend bool operator==<>(const Box<T, DIM>& lhs, const Box<T, DIM>& rhs);
    friend bool operator!=<>(const Box<T, DIM>& lhs, const Box<T, DIM>& rhs);
};
template<class T> using Box2D = Box<T, 2>;
using IBox2D = Box<int, 2>;
using UIBox2D = Box<uint32_t, 2>;
using FBox2D = Box<float, 2>;


template<class T> using Box3D = Box<T, 3>;
using IBox3D = Box<int, 3>;
using UIBox3D = Box<uint32_t, 3>;
using FBox3D = Box<float, 3>;

template<class T, size_t S>
bool operator==(const Box<T, S>& lhs, const Box<T, S>& rhs) //CONSTEXPR?
{
    for(std::size_t ii = 0; ii < 2; ii++)
        for(std::size_t jj = 0; jj < S; jj++)
            if(lhs[ii][jj] != rhs[ii][jj])
            {
                return false;
			}

    return true;
}
template<class T, size_t S>
bool operator!=(const Box<T, S>& lhs, const Box<T, S>& rhs)  //CONSTEXPR?
{
    return !(lhs == rhs);
}
}

#endif
