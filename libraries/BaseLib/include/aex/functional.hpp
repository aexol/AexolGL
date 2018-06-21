/**************************************************************
 *                                                            * 
 *      Filename:   functional.hpp                            * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2017 cze 01 13:58:12                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP 
#include <aex/Common.hpp>
#include <type_traits>
#include <functional>
#include <utility>

namespace aex{
    namespace __functional_detail {
        template<class t>
        struct is_reference_wrapper : std::false_type {};
        template<class u>
        struct is_reference_wrapper<std::reference_wrapper<u>> : std::true_type {};

        template<bool>
        struct get_invoke_meth{};

        template<>
        struct get_invoke_meth<true>
        {
            template<bool>
            struct _get_invoke_meth {
                template<class T, class Type, class T1, class... Args>
                static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                    decltype((aex::forward<T1>(t1).*f)(aex::forward<Args>(args)...)) {
                        return (aex::forward<T1>(t1).*f)(aex::forward<Args>(args)...);
                    }
            };
            template<class T, class Type, class T1, class... Args>
            static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                decltype(_get_invoke_meth<std::is_base_of<T,
                        typename std::decay<T1>::type
                        >::value>::invoke(f, aex::forward<T1>(t1), aex::forward<Args>(args)...))
            {
                return _get_invoke_meth<std::is_base_of<T,
                        typename std::decay<T1>::type
                        >::value>::invoke(f, aex::forward<T1>(t1), aex::forward<Args>(args)...);
            }
        };
        template<>
        struct get_invoke_meth<true>::_get_invoke_meth<false> {
            template<bool>
            struct __get_invoke_meth {
                template<class T, class Type, class T1, class... Args>
                static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                    decltype((t1.get().*f)(aex::forward<Args>(args)...)) {
                        return (t1.get().*f)(aex::forward<Args>(args)...);
                    }
            };


            template<class T, class Type, class T1, class... Args>
            static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                decltype(__get_invoke_meth<is_reference_wrapper<
                        typename std::decay<T1>::type
                    >::value>::invoke(f,
                            aex::forward<T1>(t1),
                            aex::forward<Args>(args)...
                        )) {
                    return __get_invoke_meth<is_reference_wrapper<
                            typename std::decay<T1>::type
                        >::value>::invoke(f,
                                aex::forward<T1>(t1),
                                aex::forward<Args>(args)...
                            );
                }
        };

        template<>
        struct get_invoke_meth<true>::_get_invoke_meth<false>::__get_invoke_meth<false> {
            template<class T, class Type, class T1, class... Args>
            static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                decltype(((*aex::forward<T1>(t1)).*f)(aex::forward<Args>(args)...)){
                    return ((*aex::forward<T1>(t1)).*f)(aex::forward<Args>(args)...);
                }
        };

        template<>
        struct get_invoke_meth<false>
        {
            template<bool>
            struct _get_invoke_meth {
                template<class T, class Type, class T1, class... Args>
                static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                    decltype(aex::forward<T1>(t1).*f) {
                        return aex::forward<T1>(t1).*f;
                    }
            };
            template<class T, class Type, class T1, class... Args>
            static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                decltype(_get_invoke_meth<std::is_base_of<T,
                        typename std::decay<T1>::type
                        >::value>::invoke(f, aex::forward<T1>(t1), aex::forward<Args>(args)...))
            {
                static_assert(std::is_member_object_pointer<decltype(f)>::value, "not a member function or object pointer.");
                static_assert(sizeof...(args) == 0, "extra args for member object pointer.");
                return _get_invoke_meth<std::is_base_of<T,
                        typename std::decay<T1>::type
                        >::value>::invoke(f, aex::forward<T1>(t1), aex::forward<Args>(args)...);
            }
        };

        template<>
        struct get_invoke_meth<false>::_get_invoke_meth<false> {
            template<bool>
            struct __get_invoke_meth {
                template<class T, class Type, class T1, class... Args>
                static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                    decltype(t1.get().*f) {
                        return t1.get().*f;
                    }
            };

            template<class T, class Type, class T1, class... Args>
            static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                decltype(__get_invoke_meth<is_reference_wrapper<
                        typename std::decay<T1>::type
                    >::value>::invoke(f,
                            aex::forward<T1>(t1),
                            aex::forward<Args>(args)...
                        )) {
                    return __get_invoke_meth<is_reference_wrapper<
                            typename std::decay<T1>::type
                        >::value>::invoke(f,
                                aex::forward<T1>(t1),
                                aex::forward<Args>(args)...
                            );
                }
        };

        template<>
        struct get_invoke_meth<false>::_get_invoke_meth<false>::__get_invoke_meth<false> {
            template<class T, class Type, class T1, class... Args>
            static inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
                decltype((*aex::forward<T1>(t1)).*f){
                    return (*aex::forward<T1>(t1)).*f;
                }
        };


        template<class T, class Type, class T1, class... Args>
        inline auto invoke(Type T::* f, T1&& t1, Args&&... args) ->
            decltype(get_invoke_meth<std::is_member_function_pointer<decltype(f)>::value>::invoke(f, aex::forward<T1>(t1), aex::forward<Args>(args)...))
        {
            return get_invoke_meth<std::is_member_function_pointer<decltype(f)>::value>::invoke(f, aex::forward<T1>(t1), aex::forward<Args>(args)...);
        }

        template<class F, class... Args>
        inline auto invoke(F&& f, Args&&... args) ->
            decltype(aex::forward<F>(f)(aex::forward<Args>(args)...))
        {
            return aex::forward<F>(f)(aex::forward<Args>(args)...);
        }

        template<size_t... seq>
        struct sequence{};

        template<size_t... seq>
        struct gen_seq;

        template<size_t least, size_t... rest>
        struct gen_seq<least, rest...> {
            using type = typename gen_seq<least-1, least, rest...>::type;
        };

        template<size_t... seq>
        struct gen_seq<0, seq...> {
            using type = sequence<0, seq...>;
        };

        template<size_t idx>
        struct gen_idx_seq {
            using type = typename gen_seq<idx-1>::type;
        };

        template<>
        struct gen_idx_seq<0> {
            using type = sequence<>;
        };
    }

    template<class F, class... Args>
    inline auto invoke(F&& f, Args&&... args) ->
       decltype(__functional_detail::invoke(aex::forward<F>(f), aex::forward<Args>(args)...)) 
    {
        return __functional_detail::invoke(aex::forward<F>(f), aex::forward<Args>(args)...);
    }

    namespace __functional_detail {
        template<class F, class... Args, size_t... seq>
        inline auto invoke_tuple(F&& f, std::tuple<Args...>&& tuple, const sequence<seq...>& s) ->
            decltype(invoke(aex::forward<F>(f), std::move(std::get<seq>(tuple))...))
        {
            return invoke(aex::forward<F>(f), std::move(std::get<seq>(tuple))...);
        }

        template<class F, class... Args, size_t... seq>
        inline auto invoke_tuple(F&& f, const std::tuple<Args...>& tuple, const sequence<seq...>& s) ->
            decltype(invoke(aex::forward<F>(f), std::get<seq>(tuple)...))
        {
            return invoke(aex::forward<F>(f), std::get<seq>(tuple)...);
        }
    }

    template<class F, class... Args>
    inline auto invoke_tuple(F&& f, std::tuple<Args...>&& tuple) ->
        decltype(
            __functional_detail::invoke_tuple(aex::forward<F>(f),
            std::move(tuple),
            typename __functional_detail::gen_idx_seq<std::tuple_size<std::tuple<Args...>>::value>::type())
        )
    {
        using tuple_size = std::tuple_size<std::tuple<Args...>>;
        return __functional_detail::invoke_tuple(aex::forward<F>(f),
                std::move(tuple),
                typename __functional_detail::gen_idx_seq<tuple_size::value>::type());
    }
    template<class F, class... Args>
    inline auto invoke_tuple(F&& f, const std::tuple<Args...>& tuple) ->
        decltype(
        __functional_detail::invoke_tuple(aex::forward<F>(f),
                tuple,
                typename __functional_detail::gen_idx_seq<std::tuple_size<std::tuple<Args...>>::value>::type())
        )
    {
        using tuple_size = std::tuple_size<std::tuple<Args...>>;
        return __functional_detail::invoke_tuple(aex::forward<F>(f),
                tuple,
                typename __functional_detail::gen_idx_seq<tuple_size::value>::type());
    }
}

#endif /* FUNCTIONAL_HPP */
