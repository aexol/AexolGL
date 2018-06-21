//
//  Common.h
//  AexolGL
//
//  Created by Kamil Matysiewicz on 22.10.2013.
//
//

#ifndef AexolGL_Common_h
#define AexolGL_Common_h
#include "aex-config.h"
#include <aex/platform_definitions.hpp>
//#include <aex/api_definitions.hpp>
#include <memory>
#include <locale>

#include <cmath>
#if defined(_MSC_VER)
#include <time.h>
#endif // defined(_MSC_VER)
#include <ctime>
#include <vector>
#include <exception>
#include <stdexcept>
#include <atomic>
#include <sstream>
#include <functional>
#include <string> 

#ifdef AEX_HAS_CXX11_CODECVT
#include <codecvt>
#endif

#ifdef USING_BOOST
#   include <boost/enable_shared_from_this.hpp>
#   include <boost/shared_ptr.hpp>
#   include <boost/weak_ptr.hpp>
#   include <boost/make_shared.hpp>
#   include <boost/memory_order.hpp>
#   include <boost/function.hpp>
#   include <boost/regex.hpp>
#else // USING_BOOST
#   include <regex>
#endif // USING_BOOST

#ifdef BUILD_FOR_ANDROID
#   include <stdio.h>
#endif // BUILD_FOR_ANDROID

namespace aex
{
    using string = std::string;
    using wstring = std::wstring;
    
#ifndef BUILD_FOR_ANDROID 
    using std::to_string;
#else // not defined(BUILD_FOR_ANDROID)
    template<class NUMBER, bool = std::is_floating_point<NUMBER>::value>
        struct StringFormat
        {
        };
    template<class NUMBER>
        struct StringFormat<NUMBER, true>
        {
            static CONSTEXPR const char* fmt = "%f";
        };
    template<class NUMBER>
        struct StringFormat<NUMBER, false>
        {
            static CONSTEXPR const char* fmt = "%d";
        }; //TODO: Unsigned, longs etc etc

    template<class NUMBER, typename = typename std::enable_if<std::is_arithmetic<NUMBER>::value>::type>
        aex::string to_string(NUMBER n)
        {
            //std::ostringstream s;
            //s << n;
            //return s.str();
            typedef typename aex::string::size_type size_type;
            aex::string resultString;
            resultString.resize(static_cast<size_type>(1));
            size_type stringCapacity = resultString.size();

            while(1)
            {
                size_type needed = static_cast<size_type>(snprintf(&resultString[0], stringCapacity + 1, StringFormat<NUMBER>::fmt, n));

                if(needed > stringCapacity)
                {
                    stringCapacity = stringCapacity * 2 + 1;
                } else {
                    break;
                }

                resultString.resize(stringCapacity);
            }
           
            return  resultString.substr(0, resultString.find_first_of('\0'));
        }
#endif // not BUILD_FOR_ANDROID

    typedef float mesh_vectors_type;
    typedef uint16_t mesh_indices_type;
    typedef double aexTimeUnit;

#define what_developer_needs 0xcafecafe
#define  TIME_RESOLUTION 1000

    using std::unique_ptr;
#ifdef USING_BOOST
    using boost::match_flag_type;
    using boost::match_default;
    using boost::regex;
    using boost::regex_replace;
    using boost::smatch;
    using boost::regex_search;
    using boost::shared_ptr;
    using boost::weak_ptr;
    using boost::make_shared;
    using boost::memory_order;
    using boost::enable_shared_from_this;
    using boost::function;

    template<class T> inline bool atomic_is_lock_free(shared_ptr<T> const* /*p*/)
    {
        return boost::atomic_is_lock_free(static_cast<const aex::shared_ptr<T>*>(0));
    }

    template<class T> shared_ptr<T> atomic_load(shared_ptr<T> const* p)
    {
        return boost::atomic_load(p);
    }

    template<class T> inline shared_ptr<T> atomic_load_explicit(shared_ptr<T> const* p, aex::memory_order mo)
    {
        return boost::atomic_load(p, mo);
    }

    template<class T> void atomic_store(shared_ptr<T>* p, const shared_ptr<T>& r)
    {
        boost::atomic_store(p, r);
    }

    template<class T> inline void atomic_store_explicit(shared_ptr<T>* p, const shared_ptr<T>& r, aex::memory_order mo)
    {
        boost::atomic_store_explicit(p, r, mo);
    }

    template<class T> shared_ptr<T> atomic_exchange(shared_ptr<T>* p, const shared_ptr<T>& r)
    {
        return boost::atomic_exchange(p, r);
    }

    template<class T> shared_ptr<T> atomic_exchange_explicit(shared_ptr<T>* p, const shared_ptr<T>& r, aex::memory_order mo)
    {
        return boost::atomic_exchange_explicit(p, r, mo);
    }

    template<class T> bool atomic_compare_exchange_weak(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w)
    {
        return boost::atomic_compare_exchange(p, v, w);
    }

    template<class T> inline bool atomic_compare_exchange_weak_explicit(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w, aex::memory_order success, aex::memory_order failure)
    {
        return boost::atomic_compare_exchange_explicit(p, v, w, success, failure);
    }

    template<class T> bool atomic_compare_exchange_strong(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w)
    {
        return boost::atomic_compare_exchange(p, v, w);
    }

    template<class T> inline bool atomic_compare_exchange_strong_explicit(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w, aex::memory_order success, aex::memory_order failure)
    {
        return boost::atomic_compare_exchange_explicit(p, v, w, success, failure);
    }

    template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const& r)
    {
        return boost::static_pointer_cast<T>(r);
    }

    template<class T, class U> shared_ptr<T> const_pointer_cast(shared_ptr<U> const& r)
    {
        return boost::const_pointer_cast<T>(r);
    }

    template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const& r)
    {
        return boost::dynamic_pointer_cast<T>(r);
    }
#else // defined(USING_BOOST)
    using std::shared_ptr;
    using std::weak_ptr;
    using std::make_shared;
    using std::regex;
    using std::regex_replace;
    using std::smatch;
    using std::regex_constants::match_flag_type;
    using std::regex_constants::match_default;
    using std::regex_search;
    using std::memory_order;
    using std::enable_shared_from_this;
    using std::function;

    template<class T> inline bool atomic_is_lock_free(shared_ptr<T> const* /*p*/)
    {
        return std::atomic_is_lock_free(static_cast<const aex::shared_ptr<T>*>(0));
    }

    template<class T> shared_ptr<T> atomic_load(shared_ptr<T> const* p)
    {
        return std::atomic_load(p);
    }

    template<class T> inline shared_ptr<T> atomic_load_explicit(shared_ptr<T> const* p, memory_order mo)
    {
        return std::atomic_load(p, mo);
    }

    template<class T> void atomic_store(shared_ptr<T>* p, const shared_ptr<T>& r)
    {
        std::atomic_store(p, r);
    }

    template<class T> inline void atomic_store_explicit(shared_ptr<T>* p, const shared_ptr<T>& r, memory_order mo)
    {
        std::atomic_store_explicit(p, r, mo);
    }

    template<class T> shared_ptr<T> atomic_exchange(shared_ptr<T>* p, const shared_ptr<T>& r)
    {
        return std::atomic_exchange(p, r);
    }

    template<class T> shared_ptr<T> atomic_exchange_explicit(shared_ptr<T>* p, const shared_ptr<T>& r, memory_order mo)
    {
        return std::atomic_exchange_explicit(p, r, mo);
    }

    template<class T> bool atomic_compare_exchange_weak(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w)
    {
        return std::atomic_compare_exchange_weak(p, v, w);
    }

    template<class T> inline bool atomic_compare_exchange_weak_explicit(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w, memory_order success, memory_order failure)
    {
        return std::atomic_compare_exchange_weak_explicit(p, v, w, success, failure);
    }

    template<class T> bool atomic_compare_exchange_strong(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w)
    {
        return std::atomic_compare_exchange_strong(p, v, w);
    }

    template<class T> inline bool atomic_compare_exchange_strong_explicit(shared_ptr<T>* p, shared_ptr<T>* v, const shared_ptr<T>& w, memory_order success, memory_order failure)
    {
        return std::atomic_compare_exchange_strong_explicit(p, v, w, success, failure);
    }

    template<class T, class U> shared_ptr<T> static_pointer_cast(shared_ptr<U> const& r)
    {
        return std::static_pointer_cast<T>(r);
    }

    template<class T, class U> shared_ptr<T> const_pointer_cast(shared_ptr<U> const& r)
    {
        return std::const_pointer_cast<T>(r);
    }

    template<class T, class U> shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const& r)
    {
        return std::dynamic_pointer_cast<T>(r);
    }
#endif // defined(USING_BOOST)

    template <class Base>
        class sharedCreator : public Base
    {
        private:
        public:

            template<typename... Arguments>
                explicit sharedCreator(Arguments... params) : Base(params...)
            {
            }
            template<typename... Arguments>
                static shared_ptr<Base> makeSharedOfBase(Arguments... params);
    };

    template<class Base>
        template<typename... Arguments>
        shared_ptr<Base>
        sharedCreator<Base>::makeSharedOfBase(Arguments... params)
        {
            return aex::static_pointer_cast<Base>(make_shared <sharedCreator <Base>> (params...));
        }
    namespace __has_detail
    {

        template <typename T, typename NameGetter>
            struct has_member_impl
            {
                typedef char matched_return_type;
                typedef long unmatched_return_type;

                template <typename C>
                    static matched_return_type f(typename NameGetter::template get<C>*);

                template <typename C>
                    static unmatched_return_type f(...);

                public:
                static const bool value = (sizeof(f<T>(0)) == sizeof(matched_return_type));
            };

    }

    template <typename T, typename NameGetter>
        struct has_member :
            std::integral_constant<bool, __has_detail::has_member_impl<T, NameGetter>::value>
    {
    };

    /*
     *        USAGE EXAMPLE:
     *        struct check_has_foo
     *        {
     *                template<typename T, return_type (T::*)( param_types ) = &T::foo >
     *                struct get { };
     *
     *        };
     *        template<lass T> struct check_T : has_member<T, check_has_foo> {};
     *        struct check_has_foo_and_bar
     *        {
     *                template<typename T, return_type (T::*)( param_types ) = &T::foo, return_type ( T::*)( param_types ) = &T::bar >
     *                struct get { };
     *
     *        };
     *        template<lass T> struct check_T : has_member<T, check_has_foo_and_bar> {};
     *        check_T<Type>::value;
     */

    template <class T>
        CONSTEXPR T&& forward(typename std::remove_reference<T>::type& t) NOEXCEPT
        {
            return static_cast < T && >(t);
        }

    template <class T>
        CONSTEXPR T&& forward(typename std::remove_reference<T>::type&&  t) NOEXCEPT
        {
            static_assert(!std::is_lvalue_reference<T>::value,
                    "Cannot forward rvalue as lvalue.");
            return static_cast < T && >(t);
        }

    template<size_t N, typename FType, size_t I>
        struct Unroll_Loop
        {

            Unroll_Loop(FType f) : func(f)
            {
            }

            _FORCE_INLINE_VISIBILITY
                FType operator()()
                {
                    func(I);
                    return Unroll_Loop<N, FType, I>(func)();
                }
            private:
            FType func;
        };

    template<size_t N, typename FType>
        struct Unroll_Loop<N, FType, N>
        {

            Unroll_Loop(FType f) : func(f)
            {
            }

            _FORCE_INLINE_VISIBILITY
                FType operator()()
                {
                    return func;
                }
            private:
            FType func;
        };

    template<size_t N, typename FType>
        Unroll_Loop<N, FType, 0> unroll(FType f)
        {
            return Unroll_Loop<N, FType, 0>(f);
        }

    template<size_t N, size_t M, typename FType, size_t I, size_t J>
        struct Unroll_LoopNM
        {

            Unroll_LoopNM(FType f) : func(f)
            {
            }

            _FORCE_INLINE_VISIBILITY
                FType operator()()
                {
                    func(I, J, N, M);
                    return Unroll_LoopNM < N, M, FType, I, J + 1 > (func)();
                }
            private:
            FType func;
        };

    template<size_t N, size_t M, typename FType, size_t I>
        struct Unroll_LoopNM<N, M, FType, I, M>
        {

            Unroll_LoopNM(FType f) : func(f)
            {
            }

            _FORCE_INLINE_VISIBILITY
                FType operator()()
                {
                    return Unroll_LoopNM < N, M, FType, I + 1, 0 > (func)();
                }
            private:
            FType func;
        };

    template<size_t N, size_t M, typename FType>
        struct Unroll_LoopNM<N, M, FType, N, 0>
        {

            Unroll_LoopNM(FType f) : func(f)
            {
            }

            _FORCE_INLINE_VISIBILITY
                FType operator()()
                {
                    return func;
                }
            private:
            FType func;
        };

    template<size_t N, size_t M, typename FType>
        Unroll_LoopNM<N, M, FType, 0, 0> unroll_nm(FType f)
        {
            return Unroll_LoopNM<N, M, FType, 0, 0>(f);
        }
    AEX_PUBLIC_API const char* getSDLError();
    AEX_PUBLIC_API void throw_sdl_error();
    template<class Func, class... Params>
        auto safe_sdl_call(Func&& f, Params&& ... params) -> decltype(f(aex::forward<Params>(params)...))
        {
            auto ret = f(aex::forward<Params>(params)...);
#ifdef DEBUG
            const char* error_msg = getSDLError();

            if(error_msg && error_msg[0] != '\0')
            {
                std::stringstream sstr {"SDL error occured."};
                sstr << "Error msg: " << error_msg;
                throw_sdl_error();
            }

#endif // DEBUG
            return ret;
        }

    template<class NumberType>
        CONSTEXPR bool is_pow_2(NumberType x)
        {
            static_assert(std::is_integral<NumberType>::value, "Only integral types allowed.");
            return ((x > 0) && ((x & (x - 1)) == 0));
        }
    class AEX_PUBLIC_API ShaderProgram;
    typedef aex::shared_ptr<ShaderProgram> AexShader;
    class AEX_PUBLIC_API ShaderParameterMapping;
    class AEX_PUBLIC_API ShaderObject;
    //    typedef aex::shared_ptr<ShaderParameterMapping> AexShaderParameterLocation;
    template <typename T, typename U>
        CONSTEXPR inline bool NonSelf()
        {
            using namespace std;
            using DecayedT = typename decay<T>::type;
            return !is_same<DecayedT, U>::value
                && !is_base_of<U, DecayedT>::value;
        }

    CONSTEXPR std::size_t shift_and_xor(const std::size_t size, const std::size_t shift)
    {
        return size | (size >> shift);
    }
    CONSTEXPR std::size_t eval_padding_rec(const std::size_t size, const std::size_t step)
    {
        return step == 1 ?
            shift_and_xor(size - 1, 1) : /*v--; v |= v>>1;*/
            shift_and_xor(eval_padding_rec(size, step / 2), step);
    }
    CONSTEXPR std::size_t eval_padding(const std::size_t size)
    {
        return eval_padding_rec(size, sizeof(std::size_t) * 4) + 1 != 0 ? eval_padding_rec(size, sizeof(std::size_t) * 4) + 1 - size
            : 0;
    }

    template<size_t padding>
        struct __padding
        {
            uint8_t _padding[padding];
        };
    template<>
        class __padding<0>
        {
        };
    struct AEX_PUBLIC_API EnumClassHash
    {
        template <typename T>
            std::size_t operator()(T t) const
            {
                return static_cast<std::size_t>(t);
            }
    };


    template<class T>
        struct NonConstLvalueReference
        {
            static CONSTEXPR const bool value = std::is_lvalue_reference<T>::value &&
                !std::is_const<typename std::remove_reference<T>::type>::value;
        };
    template<int ...>
        struct seq { };

    template<int N, int ...S>
        struct gens : gens<N-1, N-1, S...> { };

    template<int ...S>
        struct gens<0, S...> {
            typedef seq<S...> type;
        };

    template<bool B,typename T = void> using disable_if = std::enable_if<!B, T>;
    template<class... Args>
        struct assign_for_each_tuple
        {
            CONSTEXPR assign_for_each_tuple(){};

            template<class... CArgs>
                void operator()(std::tuple<Args...>& t, CArgs&&... cargs)
                {

                    assign_for_each_tuple<Args...>{}.assign<0>(t, aex::forward<CArgs>(cargs)...);
                }

            template<std::size_t idx, class T, class... CArgs, typename std::enable_if<(idx < sizeof...(Args))>::type* = nullptr>
                void assign(std::tuple<Args...>& t, T&& val, CArgs&&... cargs)
                {

                    std::get<idx>(t) = aex::forward<T>(val);
                    assign_for_each_tuple<Args...>{}.template assign<idx+1>(t, aex::forward<CArgs>(cargs)...);
                }

            template<std::size_t idx, class T, class... CArgs, typename std::enable_if<(idx >= sizeof...(Args))>::type* = nullptr>
                void assign(std::tuple<Args...>& t, T&& val, CArgs&&... cargs)
                {}

            template<std::size_t idx>
                void assign(std::tuple<Args...>& t)
                {}
        };
        
    AEX_PUBLIC_API std::string wstring_to_string(const std::wstring& wstr);
    AEX_PUBLIC_API std::tm localtime(const std::time_t* t);
}

#if __TARGET_PLATFORM__ == __AEX_WIN32__
#   define CPP_STRING(...) ::aex::wstring_to_string(__VA_ARGS__)
#else
#   define CPP_STRING(...) __VA_ARGS__
#endif
#define UNUSED(var) (void)var
#endif
