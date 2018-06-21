/*
 * File:   GetRawData.hpp
 * Author: mritke
 *
 * Created on November 25, 2013, 2:06 PM
 */

#ifndef GETRAWDATA_HPP
#define	GETRAWDATA_HPP

namespace aex
{


    template <typename T> using STLVector     = std::vector<T>;
    template <typename T> using STLVectorPtr  = aex::shared_ptr<std::vector<T>>;

    template<typename> struct void_
    {
        typedef void type;
    };

    template<typename T = void>
    struct AEX_PUBLIC_API has_dataType_method
    {
    private:
        template <typename C>
        static CONSTEXPR auto has_dataType() -> decltype(C::dataType() , bool())
        {
            return true;
        }

        static CONSTEXPR bool has_dataType(...)
        {
            return false;
        }

    public:
        static CONSTEXPR bool value = has_dataType<T>();
    };

    template<typename T, typename = void>
    struct AEX_PUBLIC_API has_basetype
    {
        static CONSTEXPR bool value = false;
    };

//! Enable if has BaseType like IMath library elements
    template<typename T>
    struct AEX_PUBLIC_API has_basetype<T, typename void_<typename T::BaseType>::type>
    {
        static CONSTEXPR bool value = true;
    };



    template<class T, class U = typename std::enable_if<has_basetype<typename T::value_type>::value, typename T::value_type>::type::BaseType>
    typename std::enable_if<has_dataType_method<T>::value, STLVectorPtr<U>>::type
            GetRawData(typename T::value_type const* array, size_t size)
    {
        STLVectorPtr<U> ret(new STLVector<U>());
        size_t extent = T::dataType().getExtent();
        ret->reserve(extent * size);

        for(size_t i = 0; i < size; i++)
        {
            for(size_t j = 0; j < extent; j++)
            {
                ret->push_back((*array)[j]);
            }

            array++;
        }

        return ret;
    }

    template<class T>
    typename std::enable_if<std::is_fundamental<typename T::value_type>::value, STLVectorPtr<typename T::value_type>>::type GetRawData(typename T::value_type const* array, size_t size)
    {
        STLVectorPtr<typename T::value_type> ret(new STLVector<typename T::value_type>());
        ret->resize(size);
        std::copy(array, array + size,  ret->begin());
        return ret;
    }






}

#endif	/* GETRAWDATA_HPP */

