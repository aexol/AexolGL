#ifndef __SAFEINTEGRALCAST_HPP
#define __SAFEINTEGRALCAST_HPP

#include <aex/Common.hpp>
#include <aex/Logger.hpp>
#include <type_traits>
#include <limits>

namespace aex
{
//    template< typename T, typename S, S s, T MAX = std::numeric_limits<T>::max(), T MIN = std::numeric_limits<T>::min() >
//    struct IsCastSafe{
//        typedef char no;
//        struct yes{
//            char _[2];
//        };
//        template< S val,
//        bool LESS = val <= MAX,
//        bool MORE = val >= MIN,
//        typename U = std::enable_if<LESS && MORE, T>>
//        yes test( U* );
//        no test( ... );
//
//        enum { value = sizeof(test(0)) == 2 } ;
//    };
//    template< typename T >
//    struct SafeIntegralCast{
//        template <typename S>
//        T operator( S s )
//        {
//            static_assert( std::is_integral<T>(), "Result type not integral." );
//            static_assert( std::is_integral<S>(), "Source type not integral" );
//            if( !IsCastSafe<T, S>::value )
//            {
//                LOG_WARR( "Potentialy unsafe integer cast" );
//            }
//        }
//    };

    template<typename T>
    struct CheckedIntegralCast
    {
        template<typename S>
        static T cast(S s)
        {

            static_assert(std::is_integral<T>(), "Result type not integral.");
            static_assert(std::is_integral<S>(), "Source type not integral");

            if(s > std::numeric_limits<T>::max() || s < std::numeric_limits<T>::min())
            {
                LOG_WARN("Potential overflow in cast");
            }

            return (T)s;
        }
    };
    template<typename T>
    struct CheckedIntegralCast<T*>
    {
        template<typename S>
        static T cast(S* s)
        {

            static_assert(std::is_integral<T>(), "Result type not integral.");
            static_assert(std::is_integral<S>(), "Source type not integral");
            static_assert(sizeof(T) == sizeof(S), "Size array types mismatch.");

            if((*s) > std::numeric_limits<T>::max() || (*s) < std::numeric_limits<T>::min())
            {
                LOG_WARN("Potential overflow in cast");
            }

            return (T)s;
        }
    };
}

#endif
