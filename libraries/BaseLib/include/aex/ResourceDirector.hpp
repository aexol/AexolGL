#ifndef _AEX_RESOURCE_DIRECTOR_HPP
#define _AEX_RESOURCE_DIRECTOR_HPP

#include "UniversalManager.hpp"
#include <boost/lockfree/queue.hpp>
#include <typeinfo>
#include <typeindex>
#include <atomic>

namespace aex
{
    class AEX_PUBLIC_API ResourceDirector
    {
    private:
        template<class DATA_TYPE, class Alloc>
        static void _destroy(IManager** mgr) {
            if(mgr && *mgr)
            {
                delete static_cast<Manager<DATA_TYPE, Alloc>*>(*mgr);
            }
        }
        struct cast_destroy : public std::allocator<IManager*> {
            typedef void (*destroy_fn)(IManager**);
            void destroy(IManager** p) {
                dest_fn(p);
            }
            destroy_fn dest_fn;            
        };
        using ManagersMapType = SharedResourceMapAdaptor<_manager_detail::_default_cc_splitlistmap<std::type_index, IManager*, cast_destroy>>;
        using handle_type = typename ManagersMapType::accessor_type;
        using HandleBufferType = boost::lockfree::queue<handle_type*, boost::lockfree::fixed_sized<false> >;
        ResourceDirector();
    public:
        ResourceDirector(const ResourceDirector&) = delete;
        ResourceDirector(ResourceDirector&&) = delete;
        ResourceDirector operator=(const ResourceDirector&) = delete;
        ResourceDirector operator=(ResourceDirector&&) = delete;
        template<class DATA_TYPE, class Alloc = std::allocator<DATA_TYPE>>
        Manager<DATA_TYPE, Alloc>& GetManager()
        {
            std::type_index idx(typeid(Manager<DATA_TYPE, Alloc>));
            auto mgr = m_managers.get(idx);
            while(!mgr) {
                // Try inserting new manager.
                auto alloc = cast_destroy{&(_destroy<DATA_TYPE, Alloc>)};
                if(m_managers.insert(idx, mgr, alloc)) {
                    (*mgr.get()) = new Manager<DATA_TYPE, Alloc>();
                    // Managers should live through the application life time
                    // so just move the handle to handle buffer to keep it alive
                    // through the life time of director.
                    auto mgr_ptr = new decltype(mgr)(std::move(mgr));
                    m_handlesBuffer.push(mgr_ptr);
                }
                mgr = m_managers.get(idx);
            }
            return *(static_cast<Manager<DATA_TYPE, Alloc>*>(*(mgr.get())));
        }
        static ResourceDirector& GetInstance();
        ~ResourceDirector();
    private:
        ManagersMapType m_managers;
        HandleBufferType m_handlesBuffer;
        static std::unique_ptr<ResourceDirector> m_instance;
        static std::once_flag m_flag;
    };
}

#endif
