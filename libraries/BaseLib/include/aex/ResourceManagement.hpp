#ifndef __RESOURCE_MANAGMENT_HPP
#define __RESOURCE_MANAGMENT_HPP

#include "UniversalManager.hpp"
#include <typeinfo>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <utility>
#include <functional>

namespace aex
{
    using TypeInfoRef = std::reference_wrapper<const std::type_info>;
    struct AEX_PUBLIC_API Hasher
    {
        size_t operator()(TypeInfoRef code) const
        {
            return code.get().hash_code();
        }
    };
    struct AEX_PUBLIC_API EqualTo
    {
        bool operator()(TypeInfoRef lhs, TypeInfoRef rhs) const
        {
            return lhs.get() == rhs.get();
        }
    };
    class AEX_PUBLIC_API ResourceManagement
    {
    public:
        typedef std::unordered_map<TypeInfoRef, std::unique_ptr<IUniversalManager>, Hasher, EqualTo> ManagersMap;
        ResourceManagement(const ResourceManagement&) = delete;
        ResourceManagement& operator=(const ResourceManagement&) = delete;

        template<class DATA>
        UniversalManager<DATA>& GetManager()
        {
            typedef UniversalManager<DATA> ManagerType;
            auto iter = m_Managers.find(typeid(ManagerType));

            if(iter == m_Managers.end())
            {
                ManagerType* mgr = new ManagerType;
                m_Managers[typeid(ManagerType)].reset(mgr);
            }

            return *static_cast<ManagerType*>(m_Managers[typeid(ManagerType)].get());
        }

        template<class DATA>
        UniversalManager<DATA>& SetDefaultLoadingPolicy(IDataLoader<DATA>* loader)
        {
            typedef UniversalManager<DATA> ManagerType;
            auto iter = m_Managers.find(typeid(ManagerType));

            if(iter == m_Managers.end())
            {
                ManagerType* mgr = new ManagerType(loader);
                m_Managers[typeid(ManagerType)].reset(mgr);
            }

            auto mgr = *static_cast<ManagerType*>(m_Managers[typeid(ManagerType)].get());
            return mgr;
        }

        template<class DATA>
        size_t RemoveManager()
        {
            typedef UniversalManager<DATA> ManagerType;
            return m_Managers.erase(typeid(ManagerType));
        }

        template<class DATA>
        auto Dereference(typename UniversalManager<DATA>::handle_type handle) ->
        decltype(std::declval<UniversalManager<DATA>>().Dereference(handle))
        {
            typedef UniversalManager<DATA> ManagerType;
            auto iter = m_Managers.find(typeid(ManagerType));

            if(iter != m_Managers.end())
            {
                return static_cast<ManagerType*>(iter->second.get())->Dereference(handle);
            }

            /* TO-DO Log warning in debug mode */
            return nullptr;
        }
        template<class DATA>
        auto Release(typename UniversalManager<DATA>::handle_type handle) ->
        decltype(std::declval<UniversalManager<DATA>>().Release(handle))
        {
            typedef UniversalManager<DATA> ManagerType;
            auto iter = m_Managers.find(typeid(ManagerType));

            if(iter != m_Managers.end())
            {
                static_cast<ManagerType*>(iter->second.get())->Release(handle);
            }

            /* TO-DO Log warning in debug mode */
        }

        template<class DATA>
        auto GetHandle(const aex::string& name, typename UniversalManager<DATA>::loader_type::type* loader) ->
        decltype(std::declval<UniversalManager<DATA>>().GetData(name, loader))
        {
            typedef UniversalManager<DATA> ManagerType;
            auto iter = m_Managers.find(typeid(ManagerType));

            if(iter == m_Managers.end())
            {
                return typename ManagerType::handle_type();
            }

            return static_cast<ManagerType*>(iter->second.get())->GetHandle(name, loader);
        }

        template<class DATA>
        auto Delete(const aex::string& name) ->
        decltype(std::declval<UniversalManager<DATA>>().Delete(name))
        {
            typedef UniversalManager<DATA> ManagerType;
            auto iter = m_Managers.find(typeid(ManagerType));

            if(iter == m_Managers.end())
            {
                return;
            }

            auto _Manager = static_cast<ManagerType*>(iter->second.get());

            if(_Manager)
            {
                _Manager->Delete();
            }
        }

        virtual ~ResourceManagement();
        static ResourceManagement& GetInstance();
    private:
        ResourceManagement() = default;
        static std::unique_ptr<ResourceManagement> m_instance;
        static std::once_flag m_onceFlag;
        ManagersMap m_Managers;

    };
}

#endif
