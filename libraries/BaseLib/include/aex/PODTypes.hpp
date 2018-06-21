#ifndef __PODTYPES_HPP
#define __PODTYPES_HPP

#include <type_traits>
#include <stdexcept>
#include <array>
#include <initializer_list>

namespace aex
{
    namespace PODT
    {
        template<size_t... _SIZES> struct size_EVAL;

        template<size_t _SIZE, size_t... _SIZES> struct size_EVAL<_SIZE, _SIZES...>
        {
            enum { value = _SIZE * size_EVAL<_SIZES...>::value };
        };
        template<size_t _SIZE> struct size_EVAL<_SIZE>
        {
            enum { value = _SIZE };
        };

        template<size_t... _SIZES> struct dim_EVAL;

        template<size_t _SIZE, size_t... _SIZES> struct dim_EVAL<_SIZE, _SIZES...>
        {
            enum { value = dim_EVAL<_SIZES...>::value + 1 };
        };
        template<size_t _SIZE> struct dim_EVAL<_SIZE>
        {
            enum { value = 1 };
        };
        template<typename T, bool Enabled, size_t... _SIZES> struct PODArrayType;

        template <typename T, size_t _SIZE, size_t... _SIZES>
        struct PODArrayType<T, true, _SIZE, _SIZES...> : std::array<PODArrayType<T, true, _SIZES...>, _SIZE>
        {
            typedef PODArrayType<T, true, _SIZES...> ARRAY_TYPE;
            typedef T PODType;
            typedef PODArrayType<T, true, _SIZES...> CELL_TYPE;
            CONSTEXPR static size_t DIMS = dim_EVAL<_SIZE, _SIZES...>::value;
            CONSTEXPR static size_t DIM_SIZE = _SIZE;
            CONSTEXPR static size_t SIZE = size_EVAL<_SIZE, _SIZES...>::value;
            CONSTEXPR static size_t BYTE__SIZE = SIZE * sizeof(T);
        };

        template<typename T, typename... Params>
        struct variadic_is_same;

        template<typename T, typename U, typename... Params>
        struct variadic_is_same<T, U, Params...> : std::false_type {};

        template<typename T, typename... Params>
        struct variadic_is_same<T, T, Params...> : variadic_is_same<Params...> {};

        template <typename T, typename U>
        struct variadic_is_same<T, U> : std::false_type {};

        template<typename T>
        struct variadic_is_same<T, T> : std::true_type {};

        template <typename T, size_t _SIZE>
        struct PODArrayType<T, true, _SIZE> : std::array<T, _SIZE>
        {
            typedef T ARRAY_TYPE;
            typedef T PODType;
            typedef PODType CELL_TYPE;
            CONSTEXPR static size_t DIMS = dim_EVAL<_SIZE>::value;
            CONSTEXPR static size_t DIM_SIZE = _SIZE;
            CONSTEXPR static size_t SIZE = size_EVAL<_SIZE>::value;
            CONSTEXPR static size_t BYTE__SIZE = SIZE * sizeof(T);
        };
        template<class T, bool Enabled, size_t... _SIZES>
        struct MPODArray;
        template<class T, size_t _SIZE, size_t... _SIZES>
        struct MPODArray<T, true, _SIZE, _SIZES...>
        {
            typedef std::array<typename MPODArray<T, true, _SIZES...>::type, _SIZE> type;
        };
        template<class T, size_t _SIZE>
        struct MPODArray<T, true, _SIZE>
        {
            typedef std::array<T, _SIZE> type;
        };

        template<class T, bool Enabled, size_t... _SIZES>
        struct MPODArrayWrapper
        {
            typedef T BaseType;
            typedef typename PODT::MPODArray<T, std::is_fundamental<T>::value, _SIZES...>::type type;
            CONSTEXPR static size_t SIZE = size_EVAL<_SIZES...>::value;
            CONSTEXPR static size_t DIMS = dim_EVAL<_SIZES...>::value;
        };

    }

//template <typename T, size_t... _SIZES> using PODArrayType = PODT::PODArrayType<T, std::is_fundamental<T>::value, _SIZES...>;
//template <typename T, size_t _SIZE, true> using __APODType = std::array<T, _SIZE>;
//template <typename T, size_t _SIZE> using _APODType = __APODType<T, _SIZE, std::is_fundamental<T>::value>;
    template<typename T, size_t... _SIZES> using PODArrayType = typename PODT::MPODArrayWrapper<T, std::is_fundamental<T>::value, _SIZES...>;

    /* Float arrays */
    typedef PODArrayType<float, 1> AFloat1;
    typedef PODArrayType<float, 2> AFloat2;
    typedef PODArrayType<float, 3> AFloat3;
    typedef PODArrayType<float, 4> AFloat4;

    typedef PODArrayType<float, 2, 1> AFloat2x1;
    typedef PODArrayType<float, 2, 2> AFloat2x2;
    typedef PODArrayType<float, 2, 3> AFloat2x3;
    typedef PODArrayType<float, 2, 4> AFloat2x4;

    typedef PODArrayType<float, 3, 1> AFloat3x1;
    typedef PODArrayType<float, 3, 2> AFloat3x2;
    typedef PODArrayType<float, 3, 3> AFloat3x3;
    typedef PODArrayType<float, 3, 4> AFloat3x4;

    typedef PODArrayType<float, 4, 1> AFloat4x1;
    typedef PODArrayType<float, 4, 2> AFloat4x2;
    typedef PODArrayType<float, 4, 3> AFloat4x3;
    typedef PODArrayType<float, 4, 4> AFloat4x4;



    /* Double arrays */
    typedef PODArrayType<double, 1> ADouble1;
    typedef PODArrayType<double, 2> ADouble2;
    typedef PODArrayType<double, 3> ADouble3;
    typedef PODArrayType<double, 4> ADouble4;

    typedef PODArrayType<double, 2, 1> ADouble2x1;
    typedef PODArrayType<double, 2, 2> ADouble2x2;
    typedef PODArrayType<double, 2, 3> ADouble2x3;
    typedef PODArrayType<double, 2, 4> ADouble2x4;

    typedef PODArrayType<double, 3, 1> ADouble3x1;
    typedef PODArrayType<double, 3, 2> ADouble3x2;
    typedef PODArrayType<double, 3, 3> ADouble3x3;
    typedef PODArrayType<double, 3, 4> ADouble3x4;

    typedef PODArrayType<double, 4, 1> ADouble4x1;
    typedef PODArrayType<double, 4, 2> ADouble4x2;
    typedef PODArrayType<double, 4, 3> ADouble4x3;
    typedef PODArrayType<double, 4, 4> ADouble4x4;
}

#endif
