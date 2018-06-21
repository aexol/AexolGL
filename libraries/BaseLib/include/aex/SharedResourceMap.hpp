/**************************************************************
 *                                                            *
 *      Filename:   SharedResrouceMap.hpp                     *
 *                                                            *
 *   Description:                                             *
 *                                                            *
 *       Version:   1.0                                       *
 *       Created:   2017 maj 16 16:38:11                      *
 *                                                            *
 *        Author:   Kamil Matysiewicz                         *
 *          Mail:   kamil@aexol.com                           *
 *  Organization:   Aexol                                     *
 *                                                            *
 **************************************************************/
#ifndef SHAREDRESOURCEMAP_HPP
#define SHAREDRESOURCEMAP_HPP
#include <aex/Common.hpp>
#include <aex/functional.hpp>
#include <memory>
#include <iostream>
#include <thread>
#include <map>
#include <type_traits>
#include <iostream>
#include <tuple>
#include <functional>
#include <cassert>
#include <cds/container/michael_list_hp.h>
#include <cds/container/split_list_map.h>
namespace aex {
    namespace map {
        namespace cc = cds::container;
        using __functional_detail::sequence;
        using __functional_detail::gen_seq;
        using __functional_detail::gen_idx_seq;
        template<class ValueType>
        struct handle_type {
            using key_type = typename ValueType::first_type;
            using data_type = typename ValueType::second_type;
            using deleter_type = std::function<void(data_type*)>;
            using mapped_type = std::unique_ptr<data_type, deleter_type>;
            static constexpr int_fast32_t NEEDS_CREATING = -2;
            static constexpr int_fast32_t IN_CREATION = -3;
            static constexpr int_fast32_t MARKED_FOR_ERASE = -1;
            static constexpr int_fast32_t READY = 0;
            std::atomic_int_fast32_t counter;
            mapped_type m_data;
            const key_type* m_key;
            template<class RealValueType, class Alloc, class... Args>
            void create(RealValueType& v, Alloc& alloc, Args&&... args) {
                if(counter.load(std::memory_order_acquire) == NEEDS_CREATING) {
                    if(mark_in_creation()) {
                        m_key = &v.first;
                        data_type* newData = std::allocator_traits<Alloc>::allocate(alloc, 1);
                        std::allocator_traits<Alloc>::construct(alloc, newData, aex::forward<Args>(args)...);
                        m_data = mapped_type(newData, [alloc](data_type *p) mutable {
                                std::allocator_traits<Alloc>::destroy(alloc, p);
                                std::allocator_traits<Alloc>::deallocate(alloc, p, 1);
                        });
                        mark_created();
                    }
                }
            }
            bool increment() {
                auto t = counter.load(std::memory_order_acquire);
                while(t >= READY && !counter.compare_exchange_weak(t, t+1, std::memory_order_release))
                    t = counter.load(std::memory_order_acquire);
                return t + 1 > READY;
            }
            int_fast32_t decrement() {
                return --counter;
            }
            bool mark_for_erase() {
                auto expected = READY;
                return counter.compare_exchange_strong(expected, MARKED_FOR_ERASE, std::memory_order_acq_rel);
            }
            bool mark_in_creation() {
                auto expected = NEEDS_CREATING;
                return counter.compare_exchange_strong(expected, IN_CREATION, std::memory_order_release);
            }
            bool mark_created() {
                auto expected = IN_CREATION;
                return counter.compare_exchange_strong(expected, READY, std::memory_order_release);
            }
            handle_type() : counter(NEEDS_CREATING), m_data(nullptr), m_key(nullptr){}
            handle_type(handle_type&& other) = default;
            handle_type& operator=(handle_type&& other) = default;
            // This is temporary workaround because make_pair invokes a copy during creation, user should _never_ copy a handle.
            handle_type(const handle_type& other) :
                counter(other.counter.load())
            {
                // Assert so that any copy attempt on created/during creation of an object is actually
                // an error.
                assert(counter.load() == NEEDS_CREATING);
            }
        };

        template<bool Cond, template<class> class rewrite_rule, class T>
        struct rewrite_cond {
            using type = T;
        };

        template<template<class> class rewrite_rule, class T>
        struct rewrite_cond<true, rewrite_rule, T> {
            using type = typename rewrite_rule<T>::type;
        };

        template<template<class> class rewrite_rule, size_t N, class Seq, class Tuple, class ResultTuple>
        struct rewrite_type;

        template<template<class> class rewrite_rule, size_t N, size_t s, size_t... seq, class T, class... Types, class... ResultTypes>
        struct rewrite_type<rewrite_rule, N, sequence<s, seq...>, std::tuple<T, Types...>, std::tuple<ResultTypes...>> {
            using type = typename rewrite_type<rewrite_rule,
                  N,
                  sequence<seq...>,
                  std::tuple<Types...>,
                  std::tuple<ResultTypes..., typename rewrite_cond<N==s, rewrite_rule, T>::type>>::type;
        };

        template<template<class> class rewrite_rule, size_t N, class... ResultTypes>
        struct rewrite_type<rewrite_rule, N, sequence<>, std::tuple<>, std::tuple<ResultTypes...>> {
            using type = std::tuple<ResultTypes...>;
        };

        template<class Map> struct rewriteMappedType{
        };

        template<class Type, class Tuple>
        struct from_tuple_to_type;

        template<template <class...> class TType, class... Args, class... TupleArgs>
        struct from_tuple_to_type<TType<Args...>, std::tuple<TupleArgs...>> {
            using type = TType<TupleArgs...>;
        };

        template<class Map>
        struct Rewrite;

        template<template<class...> class TMap, class... Args>
        struct Rewrite<TMap<Args...>>{
            template<class T>
            struct rule {
                using type = handle_type<typename TMap<Args...>::value_type>;
            };
            using type = typename from_tuple_to_type<TMap<Args...>,
                    typename rewrite_type<rule, 1, typename gen_idx_seq<sizeof...(Args)>::type, std::tuple<Args...>, std::tuple<>>::type
                >::type;
        };

        template<class... Args>
        struct Rewrite<cc::SplitListMap<Args...>>{
            template<class T>
            struct rule {
                using type = handle_type<typename cc::SplitListMap<Args...>::value_type>;
            };
            using type = typename from_tuple_to_type<cc::SplitListMap<Args...>,
                    typename rewrite_type<rule, 2, typename gen_idx_seq<sizeof...(Args)>::type, std::tuple<Args...>, std::tuple<>>::type
                >::type;
        };

        template<class T>
        struct AdaptMapTraits
        {
            using map_type = typename Rewrite<T>::type;
            using key_type = typename T::key_type;
            using mapped_type = typename T::mapped_type;
            using value_type = typename T::value_type;
            using real_type = typename map_type::mapped_type;
            using real_value_type = typename map_type::value_type;
            using allocator_type = std::allocator<mapped_type>;
            struct accessor {
                real_type* get() {
                    return &(*m_iter);
                }
                const real_type* get() const {
                    return &(*m_iter);
                }
                real_type* operator->() {
                    return get();
                }
                const real_type* operator->() const {
                    return get();
                }
                operator bool() const {
                    return m_iter != m_map.end();
                }
                map_type& m_map;
                typename map_type::iterator m_iter;
            };
            template<class Func, class... Args>
            static bool insert(map_type& map,
                const key_type& k,
                Func&& f)
            {
                real_value_type i;
                i.first = k;
                auto insertion = map.insert(i);
                if(insertion.second) {
                    f(*insertion);
                }
                return insertion.second;
            }

            static accessor get_item(map_type& map,
                const key_type& k)
            {
                return accessor{map, map.find(k)};
            }
            static void erase(map_type& map, const key_type& k) {
                map.erase(k);
            }
        };

        template<class... Args>
        struct AdaptMapTraits<cc::SplitListMap<Args...>>
        {
            using base_map_type = cc::SplitListMap<Args...>;
            using RewriteRule = Rewrite<base_map_type>;
            using map_type = typename RewriteRule::type;
            using key_type = typename base_map_type::key_type;
            using mapped_type = typename base_map_type::mapped_type;
            using value_type = typename base_map_type::value_type;
            using real_type = typename map_type::mapped_type;
            using real_value_type = typename map_type::value_type;
            using allocator_type = std::allocator<mapped_type>;
            struct accessor {
                real_type* get() {
                    return &(ptr->second);
                }
                real_type* operator->() {
                    return get();
                }
                operator bool() const {
                    return bool(ptr);
                }
                typename map_type::guarded_ptr ptr;
            };
            template<class Func>
            static bool insert(map_type& map,
                const key_type& k,
                Func&& f)
            {
                return map.insert_with(k, f);
            }
            static accessor get_item(map_type& map, const key_type& key) {
                return accessor{map.get(key)};
            }
            static void erase(map_type& map, const key_type& k) {
                map.erase(k);
            }
        };

        template<class TMap, class Alloc>
        struct _add_allocator : public TMap
        {
            using allocator_type = Alloc;
        };

        template<class TMap, class Alloc>
        struct AdaptMapTraits<_add_allocator<TMap, Alloc>> : public AdaptMapTraits<TMap>
        {
            using allocator_type = Alloc;
        };
    }
    template<class TMap, class Traits = ::aex::map::AdaptMapTraits<TMap>>
    class SharedResourceMapAdaptor {
        private:
            using real_type = typename Traits::real_type;
            using real_value_type = typename Traits::real_value_type;
        public:
            using map_type = typename Traits::map_type;
            using key_type = typename Traits::key_type;
            using mapped_type = typename Traits::mapped_type;
            using value_type = typename Traits::value_type;
            using allocator_type = typename Traits::allocator_type;

            class ScopedMapAccessor{
                public:
                    using mapped_type = SharedResourceMapAdaptor::mapped_type;
                    using pointer_type = mapped_type*;
                    using const_pointer_type = const pointer_type;
                    using reference_type = mapped_type&;
                    using const_reference_type = const mapped_type&;
                    using key_type = SharedResourceMapAdaptor::key_type;
                    ScopedMapAccessor() : ScopedMapAccessor(nullptr, nullptr){}
                    ScopedMapAccessor(const ScopedMapAccessor& other) = delete;
                    ScopedMapAccessor& operator=(const ScopedMapAccessor& other) = delete;
                    ScopedMapAccessor(ScopedMapAccessor&& other)
                    {
                        *this = std::move(other);
                    }
                    ScopedMapAccessor& operator=(ScopedMapAccessor&& other)
                    {
                        m_buf = other.m_buf;
                        other.m_buf = nullptr;
                        m_parent = other.m_parent;
                        other.m_parent = nullptr;
                        return *this;
                    }
                    pointer_type get() {
                        return m_buf->m_data.get();
                    }
                    const_pointer_type get() const {
                        return m_buf->m_data.get();
                    }
                    pointer_type operator->() {
                        return get();
                    }
                    const_pointer_type operator->() const {
                        return get();
                    }
                    operator bool() const { return bool(m_buf); }
                    ~ScopedMapAccessor() {
                        if(m_parent) {
                            m_parent->ReleaseResource(m_buf);
                        }
                    }
                    friend class SharedResourceMapAdaptor;

                private:
                    ScopedMapAccessor(real_type* buf, SharedResourceMapAdaptor* parent)
                    {
                        m_buf = buf;
                        m_parent = parent;
                    }
                    real_type* m_buf;
                    SharedResourceMapAdaptor* m_parent;
            };
            using accessor_type = ScopedMapAccessor;

            bool insert(const key_type& k,
                    ScopedMapAccessor& v)
            {
                return insert(k, v, allocator_type());
            }

            template<class... Args,
                typename std::enable_if<!std::is_same<
                typename std::remove_reference<
                    typename std::tuple_element<0, std::tuple<Args...>>::type
                >::type,
                allocator_type>::value, int>::type* = nullptr>
            bool insert(const key_type& k,
                    ScopedMapAccessor& v,
                    Args&&... args)
            {
                auto alloc = allocator_type();
                return _insert(k, v, alloc, aex::forward<Args>(args)...);
            }

            template<class... Args,
                typename std::enable_if<std::is_same<
                    typename std::remove_reference<
                        typename std::tuple_element<0, std::tuple<Args...>>::type
                    >::type,
                    allocator_type>::value, int>::type* = nullptr>
            bool insert(const key_type& k,
                    ScopedMapAccessor& v,
                    Args&&... args)
            {
                return _insert(k, v, aex::forward<Args>(args)...);
            }

            ScopedMapAccessor get(const key_type& k) {
                auto h = Traits::get_item(m_map, k);
                if(!h || !h->increment()) {
                    return ScopedMapAccessor();
                }
                return ScopedMapAccessor(h.get(), this);
            }
        private:
            template<class Alloc, class Tuple, size_t... S>
            void delayed_create(real_value_type& r, Alloc&& alloc, Tuple& t, map::sequence<S...>*) {
                r.second.create(r, alloc, std::get<S>(t)...);
            }
            template<class Alloc, class... Args>
            bool _insert(const key_type& k,
                    ScopedMapAccessor& v,
                    Alloc&& alloc,
                    Args&&... args)
            {
                bool newInsertion = false;
                auto h = Traits::get_item(m_map, k);
                auto t = std::make_tuple(aex::forward<Args>(args)...);
                auto f = [&alloc, &t, this](real_value_type& r) {
                    this->delayed_create(r, alloc, t, (typename map::gen_idx_seq<sizeof...(Args)>::type*)(nullptr));
                };
                while(!h || !h->increment())
                {
                    newInsertion = Traits::insert(m_map, k, f);
                    h = Traits::get_item(m_map, k);
                }
                v.m_buf = h.get();
                v.m_parent = this;
                return newInsertion;
            }
            void ReleaseResource(real_type* buf) {
                if(buf){
                    if(buf->decrement() == 0) {
                        if(buf->mark_for_erase()) {
                            key_type k = *buf->m_key;
                            Traits::erase(m_map, k);
                        }
                    }
                }
            }
            map_type m_map;
    };
}

#endif /* SHAREDRESOURCEMAP_HPP */
