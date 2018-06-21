#ifndef _AEX_POINT_HPP
#define _AEX_POINT_HPP

#include <aex/Common.hpp>
#include <array>

namespace aex
{
    template<class T, size_t _DIM>
        class PointBase
        {
            public:
                static CONSTEXPR size_t DIM = _DIM;
                typedef T value_type;
                template<class... Args, typename std::enable_if<(sizeof...(Args)>1)>::type* = nullptr >
                    PointBase(Args... args) : m_data {{static_cast<value_type>(aex::forward<Args>(args))...}} {}
                template<class Arg, typename std::enable_if<NonSelf<Arg, PointBase<T, DIM>>()>::type* = nullptr>
                    explicit PointBase(Arg && arg) : m_data(aex::forward<Arg>(arg)) {}
                PointBase(){}
                T* data()
                {
                    return m_data.data();
                }

                std::array<T, DIM>& array()
                {
                    return m_data;
                }

                T& operator[](size_t idx)
                {
                    return m_data[idx];
                }
                const T& operator[](size_t idx) const
                {
                    return m_data[idx];
                }

            protected:
                std::array<T, DIM> m_data;
        };

    template<class T, size_t _DIM>
        class Point : public PointBase<T, _DIM>
    {
        public:
            using PointBase<T, _DIM>::DIM;
            using PointBase<T, _DIM>::value_type;
            template<class... Args>
                Point(Args... args) : PointBase<T, DIM>(aex::forward<Args>(args)...) {}
    };

    template<class T>
        class Point<T, 2> : public PointBase<T, 2>
        {
            public:
                using Base = PointBase<T, 2>;
                using Base::DIM;
                using typename Base::value_type;
                template<class... Args, typename std::enable_if<(sizeof...(Args)>1)>::type* = nullptr >
                    Point(Args... args) : Base(aex::forward<Args>(args)...) {}
                template<class Arg, typename std::enable_if<NonSelf<Arg, Point<T, 2>>()>::type* = nullptr>
                    Point(Arg && arg) : Base(aex::forward<Arg>(arg)) {}
                Point() : Base(){}
                T& x()
                {
                    return this->m_data[0];
                }
                const T& x() const
                {
                    return this->m_data[0];
                }

                T& y()
                {
                    return this->m_data[1];
                }
                const T& y() const
                {
                    return this->m_data[1];
                }
        };

    template<class T>
        class Point<T, 3> : public PointBase<T, 3>
        {
            public:
                using Base = PointBase<T, 3>;
                using Base::DIM;
                using typename Base::value_type;
                template<class... Args, typename std::enable_if<(sizeof...(Args)>1)>::type* = nullptr >
                    Point(Args... args) : Base(aex::forward<Args>(args)...) {}
                template<class Arg, typename std::enable_if<NonSelf<Arg, Point<T, 3>>()>::type* = nullptr>
                    Point(Arg && arg) : Base(aex::forward<Arg>(arg)) {}
                Point() : Base(){}
                T& x()
                {
                    return this->m_data[0];
                }
                const T& x() const
                {
                    return this->m_data[0];
                }

                T& y()
                {
                    return this->m_data[1];
                }
                const T& y() const
                {
                    return this->m_data[1];
                }

                T& z()
                {
                    return this->m_data[2];
                }
                const T& z() const
                {
                    return this->m_data[2];
                }
                
                friend std::ostream& operator<<(std::ostream& out, Point<T, 3>& p){
                    return out << "(" << p.x() << "," << p.y() << "," << p.z() << ")";
                }
        };

    template<class T>
        class Point<T, 4> : public PointBase<T, 4>
        {
            public:
                using Base = PointBase<T, 4>;
                using Base::DIM;
                using typename Base::value_type;
                template<class... Args, typename std::enable_if<(sizeof...(Args)>1)>::type* = nullptr >
                    Point(Args... args) : Base(aex::forward<Args>(args)...) {}
                template<class Arg, typename std::enable_if<NonSelf<Arg, Point<T, 4>>()>::type* = nullptr>
                    Point(Arg && arg) : Base(aex::forward<Arg>(arg)) {}
                Point() : Base(){}
                T& x()
                {
                    return this->m_data[0];
                }
                const T& x() const
                {
                    return this->m_data[0];
                }

                T& y()
                {
                    return this->m_data[1];
                }
                const T& y() const
                {
                    return this->m_data[1];
                }

                T& z()
                {
                    return this->m_data[2];
                }
                const T& z() const
                {
                    return this->m_data[2];
                }

                T& w()
                {
                    return this->m_data[3];
                }
                const T& w() const
                {
                    return this->m_data[3];
                }                
                   
                friend std::ostream& operator<<(std::ostream& out, Point<T, 4>& p){
                    return out << "(" << p.x() << "," << p.y() << "," << p.z() << "," << p.w() << ")";
                }
        };

    template<class T, size_t S>
        bool operator==(const Point<T, S>& lhs, const Point<T, S>& rhs)
        {
            for(std::size_t ii = 0; ii < S; ii++)
            {
                if(lhs[ii] != rhs[ii])
                {
                    return false;
                }
            }

            return true;
        }

    template<class T, size_t S>
        bool operator!=(const Point<T, S>& lhs, const Point<T, S>& rhs)
        {
            return !(lhs == rhs);
        }

    template<class T> using Point2D = Point<T, 2>;
    using IPoint2D  = Point2D<int>;
    using SPoint2D  = Point2D<int16_t>;
    using UIPoint2D = Point2D<uint32_t>;
    using FPoint2D  = Point2D<float>;


    template<class T> using Point3D = Point<T, 3>;
    using IPoint3D  = Point3D<int>;
    using SPoint3D  = Point3D<int16_t>;
    using UIPoint3D = Point3D<uint32_t>;
    using FPoint3D  = Point3D<float>;

    template<class T> using Point4D = Point<T, 4>;
    using IPoint4D  = Point4D<int>;
    using SPoint4D  = Point4D<int16_t>;
    using UIPoint4D = Point4D<uint32_t>;
    using FPoint4D  = Point4D<float>;
}

#endif
