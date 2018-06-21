#ifndef _AEX_RESOURCE_HPP
#define _AEX_RESOURCE_HPP

//#include <aex/Logger.hpp>
#include<aex/Common.hpp>
#include<memory>
#include<string>
#include <cassert>
#include <iostream>
#include <mutex>
#include <type_traits>

namespace aex
{
template<typename T>
struct has_header_type
{
    template<class U>
    static std::true_type _has_typedef(typename U::header_type*);
    template<class U>
    static std::false_type _has_typedef(...);
    static CONSTEXPR bool value = decltype(_has_typedef<T>(0))::value;
};

template<class DATA_TYPE, class Allocator> class OrphansManager;
template<class DATA_TYPE, class Allocator, bool HasHeader = has_header_type<Allocator>::value> class ResourceInfo;
template<class DATA_TYPE, class Allocator> class Resource;

template<class DATA_TYPE, class Allocator>
class ResourceInfo<DATA_TYPE, Allocator, true>
{
public:
    typedef Allocator allocator;
    typedef DATA_TYPE value_type;;
    typedef typename Allocator::header_type header_type;
    static_assert(std::is_copy_constructible<header_type>::value, "Invalid header_type in allocator");
    ResourceInfo(DATA_TYPE* data, const header_type& header) :
        m_header(header),
        m_data(data, Deleter(header))
    {
    }
    ResourceInfo() :  //Null resource
        m_header(),
        m_data(nullptr)
    {
    }
    const header_type header() const
    {
        return m_header;
    }
    DATA_TYPE* data() const
    {
        return m_data.get();
    }
    bool isNull() const
    {
        return m_data == nullptr;
    }

protected:
    struct Deleter
    {
        Deleter(const header_type& header) : m_header(header) {}
        void operator()(DATA_TYPE* data)
        {
            Allocator alloc;

            if(data == nullptr)
            {
                return;
            }

            alloc.destroy(data, m_header);
            alloc.deallocate(data, m_header);
        }
        header_type m_header;
    };
    void destroy()
    {
        Allocator alloc;

        if(m_data == nullptr)
        {
            return;
        }

        alloc.destroy(m_data, m_header);
    }
    const header_type m_header;
    const aex::shared_ptr<DATA_TYPE> m_data;
    friend class Resource<DATA_TYPE, Allocator>;

};
template<class DATA_TYPE, class Allocator>
class ResourceInfo<DATA_TYPE, Allocator, false>
{
private:
    struct header_
    {
        size_t capacity() const
        {
            return 1;
        }
    };
public:
    typedef Allocator allocator;
    typedef DATA_TYPE value_type;;
    typedef void header_type;
    ResourceInfo(DATA_TYPE* data) :
        m_data(data, Deleter()),
        m_header()
    {
    }
    ResourceInfo() :  //Null resource
        m_data(nullptr),
        m_header()
    {
    }
    const header_* header() const
    {
        return &m_header;
    }
    DATA_TYPE* const data() const
    {
        return m_data.get();
    }
    bool isNull() const
    {
        return m_data == nullptr;
    }

protected:
    struct Deleter
    {
        void operator()(DATA_TYPE* data)
        {
            Allocator alloc;

            if(data == nullptr)
            {
                return;
            }

            alloc.destroy(data);
            alloc.deallocate(data, 1);
        }
    };
    void destroy()
    {
        Allocator alloc;

        if(m_data == nullptr)
        {
            return;
        }

        alloc.destroy(m_data);
    }
    const aex::shared_ptr<DATA_TYPE> m_data;
    const header_ m_header;
    friend class Resource<DATA_TYPE, Allocator>;

};

template<class DATA_TYPE, class Allocator> //Use this proxy to access resource information
class Resource
{
public:
    typedef Allocator allocator;
    typedef typename ResourceInfo<DATA_TYPE, Allocator>::header_type header_type;
    typedef ResourceInfo<DATA_TYPE, Allocator> value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef aex::shared_ptr<value_type> pointer;
    typedef aex::shared_ptr<const value_type> const_pointer;

    const_pointer ReadOnly() const  //ReadOnly access to data stored
    {
        return m_resource != nullptr ? m_resource : const_pointer(new value_type());
    }
    template < class U = header_type, typename = typename std::enable_if < !(std::is_same<U, void>::value) >::type >
    pointer Set(DATA_TYPE* data, U header)   //Swap with current owned and return previous.
    {
        aex::shared_ptr<value_type> sr { new value_type(data, header) };
        sr.swap(m_resource);
        return sr;
    }
    template<class U = header_type, typename = typename std::enable_if<(std::is_same<U, void>::value)>::type>
    pointer Set(DATA_TYPE* data)   //Swap with current owned and return previous.
    {
        aex::shared_ptr<value_type> sr { new value_type(data) };
        sr.swap(m_resource);
        return sr;
    }

    template < class U = header_type, typename = typename std::enable_if < !(std::is_same<U, void>::value) >::type >
    bool SwapIfFits(Resource& resource, size_t needed, header_type* h = 0)
    {
        auto expected = aex::atomic_load_explicit(&m_resource, aex::memory_order::memory_order_acquire);

        if(expected->header().capacity() >= needed)
        {
            if(aex::atomic_compare_exchange_strong_explicit(&m_resource, &expected, resource.m_resource, aex::memory_order::memory_order_release, aex::memory_order::memory_order_release))
            {
                resource.m_resource = expected;
                return true;
            }
        }

        return false;
    }
    template<class U = header_type, typename = typename std::enable_if<(std::is_same<U, void>::value)>::type>
    bool SwapIfFits(Resource& resource, size_t needed)
    {
        auto expected = aex::atomic_load_explicit(&m_resource, aex::memory_order::memory_order_acquire);

        if(aex::atomic_compare_exchange_strong_explicit(&m_resource, &expected, resource.m_resource, aex::memory_order::memory_order_release, aex::memory_order::memory_order_release))
        {
            resource.m_resource = expected;
            return true;
        }

        return false;
    }
    Resource<DATA_TYPE, Allocator> Release()
    {
        Resource<DATA_TYPE, Allocator> ret;
        ret.m_resource = aex::atomic_exchange(&m_resource, aex::make_shared<value_type>(nullptr));
        return ret;
    }

protected:
    void destroy()
    {
        m_resource.destroy();
    }
    aex::shared_ptr<value_type> m_resource;
    friend class OrphansManager<DATA_TYPE, Allocator>;
};

}
#endif
