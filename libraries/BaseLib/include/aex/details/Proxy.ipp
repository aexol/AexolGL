#ifndef __AEX_PROXY_IPP
#define __AEX_PROXY_IPP

namespace aex
{
    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::ProxyOwnerGuard() : ProxyOwnerGuard(nullptr)
    {}

    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::ProxyOwnerGuard(const owner_type& owner) : ProxyOwnerGuard(owner_pointer{owner})
    {
        tie();
    }

    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::ProxyOwnerGuard(const owner_pointer& owner) : m_owner(owner)
    {
        tie();
    }

    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::ProxyOwnerGuard(owner_pointer&& owner) : m_owner(std::move(owner))
    {
        tie();
    }

    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::ProxyOwnerGuard(const ProxyOwnerGuard& rhs) : m_owner{}
    {
        *this = rhs;
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::ProxyOwnerGuard&
    Proxy<RefCountT>::ProxyOwnerGuard::operator=(const owner_type& rhs)
    {
        m_owner = owner_pointer{rhs};
        return tie();
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::ProxyOwnerGuard&
    Proxy<RefCountT>::ProxyOwnerGuard::operator=(const owner_pointer& rhs)
    {
        m_owner = rhs;
        return tie();
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::ProxyOwnerGuard&
    Proxy<RefCountT>::ProxyOwnerGuard::operator=(owner_pointer&& rhs)
    {
        m_owner = std::move(rhs);
        return tie();
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::ProxyOwnerGuard&
    Proxy<RefCountT>::ProxyOwnerGuard::operator=(const ProxyOwnerGuard& rhs)
    {
        m_owner = rhs.m_owner;
        return tie();
    }

    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::ProxyOwnerGuard(ProxyOwnerGuard&& rhs) : m_owner{}
    {
        *this = std::move(rhs);
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::ProxyOwnerGuard&
    Proxy<RefCountT>::ProxyOwnerGuard::operator=(ProxyOwnerGuard&& rhs)
    {
        if(this != &rhs)
        {
            release();
            m_owner = std::move(rhs.m_owner);
            rhs.m_owner = owner_pointer{};
        }
        return *this;
    }

    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::~ProxyOwnerGuard()
    {
        release();
    }

    template<class RefCountT>
    const typename Proxy<RefCountT>::owner_pointer&
    Proxy<RefCountT>::ProxyOwnerGuard::operator->() const
    {
        return m_owner;
    }

    template<class RefCountT>
    Proxy<RefCountT>::ProxyOwnerGuard::operator bool() const
    {
        return bool(m_owner);
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::ProxyOwnerGuard&
    Proxy<RefCountT>::ProxyOwnerGuard::tie()
    {
        if(m_owner)
            m_owner->incrementRef();
        return *this;
    }

    template<class RefCountT>
    void
    Proxy<RefCountT>::ProxyOwnerGuard::release()
    {
        if(m_owner)
        {
            m_owner->decrementRef();
            m_owner = owner_pointer{};
        }
    }

    template<class RefCountT>
    Proxy<RefCountT>::Proxy() : Proxy(owner_pointer{}){}

    template<class RefCountT>
    Proxy<RefCountT>::Proxy(const owner_pointer& owner) : m_owner(owner){}

    template<class RefCountT>
    Proxy<RefCountT>::Proxy(owner_pointer&& owner) : m_owner(std::move(owner)){}

    template<class RefCountT>
    void
    Proxy<RefCountT>::reset(const owner_pointer& owner)
    {
        m_owner = ProxyOwnerGuard(owner);
    }

    template<class RefCountT>
    void
    Proxy<RefCountT>::reset(owner_pointer&& owner)
    {
        m_owner = std::move(owner);
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::impl_pointer
    Proxy<RefCountT>::operator->()
    {
        return get();
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::impl_type&
    Proxy<RefCountT>::operator*()
    {
        return *get();
    }

    template<class RefCountT>
    typename Proxy<RefCountT>::impl_pointer
    Proxy<RefCountT>::get()
    {
        impl_pointer impl = nullptr;
        if(m_owner)
            impl = m_owner->get();
        return impl;
    }

    template<class RefCountT>
    Proxy<RefCountT>::operator bool() const
    {
        return bool(m_owner);
    }

    namespace _proxy_details
    {
        template<class T, class... Args>
        InstanceCreator<T, Args...>::InstanceCreator() : m_args(){}

        template<class T, class... Args>
        template<class... CArgs>
        void
        InstanceCreator<T, Args...>::setArgs(CArgs&&... cargs)
        {
            assign_for_each_tuple<Args...>{}(m_args, aex::forward<CArgs>(cargs)...);
        }

        template<class T, class... Args>
        std::unique_ptr<T>
        InstanceCreator<T, Args...>::createInstance()
        {
            return delayedConstruction(typename gens<sizeof...(Args)>::type());
        }

        template<class T, class... Args>
        template<int ...S>
        std::unique_ptr<T>
        InstanceCreator<T, Args...>::delayedConstruction(seq<S...>)
        {
            return std::unique_ptr<T>(new T(std::get<S>(m_args) ...));
        }

        template<class TImpl, class CreatorType>
        _ProxyBuilderCommon<TImpl, CreatorType>::_ProxyBuilderCommon():
            m_instance{nullptr},
            m_creator{},
            m_instanceRefCount{0},
            m_instanceLock{}
        {
        }

        template<class TImpl, class CreatorType>
        template<class... CArgs>
        void
        _ProxyBuilderCommon<TImpl, CreatorType>::setArgs(CArgs&&... args)
        {
#if defined(_MSC_VER) && !defined(__clang__)
            CreatorType _t;
            m_creator = std::move(_t);
            m_creator.setArgs(aex::forward<CArgs>(args)...);
#else
            m_creator = CreatorType(aex::forward<CArgs>(args)...);
#endif
        }

        template<class TImpl, class CreatorType>
        void
        _ProxyBuilderCommon<TImpl, CreatorType>::incrementRef()
        {
            std::lock_guard<std::mutex> lock{m_instanceLock};
            m_instanceRefCount++;
        }

        template<class TImpl, class CreatorType>
        void
        _ProxyBuilderCommon<TImpl, CreatorType>::decrementRef()
        {
            std::lock_guard<std::mutex> lock{m_instanceLock};
            if(!(--m_instanceRefCount))
            {
                m_instance.reset(nullptr);
            }
        }

        template<class TImpl, class CreatorType>
        TImpl*
        _ProxyBuilderCommon<TImpl, CreatorType>::get()
        {
            if(!m_instanceRefCount)
                return nullptr;
            if(!m_instance)
                m_instance = m_creator.createInstance();
            return m_instance.get();
        }
    }
    template<class TImpl, class CreatorType>
    ProxyBuilderBase<TImpl, CreatorType>::ProxyBuilderBase(): m_base(){}

    template<class TImpl, class CreatorType>
    template<class... CArgs>
    void
    ProxyBuilderBase<TImpl, CreatorType>::setArgs(CArgs&&... args)
    {
        m_base.setArgs(args...);
    }

    template<class TImpl, class CreatorType>
    void
    ProxyBuilderBase<TImpl, CreatorType>::incrementRef()
    {
        m_base.incrementRef();
    }

    template<class TImpl, class CreatorType>
    void
    ProxyBuilderBase<TImpl, CreatorType>::decrementRef()
    {
        m_base.decrementRef();
    }

    template<class TImpl, class CreatorType>
    TImpl*
    ProxyBuilderBase<TImpl, CreatorType>::get()
    {
        return m_base.get();
    }

    template<class TImpl, class CreatorType>
    std::size_t
    ProxyBuilderBase<TImpl, CreatorType>::unsafeRefCount()
    {
        return m_base.m_instanceRefCount;
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::BiDirectionalSelfRef(BiDirectionalProxyBuilderBase<TImpl, CreatorType>* self) : m_self(self)
    {
        registerForSelf();
    }

    template<class TImpl, class CreatorType>
    typename BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef&
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::operator=(BiDirectionalProxyBuilderBase<TImpl, CreatorType>* self)
    {
        unregisterForSelf();
        m_self = self;
        registerForSelf();
        return *this;
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::BiDirectionalSelfRef(const BiDirectionalSelfRef& rhs) : m_self(nullptr)
    {
        *this = rhs;
    }

    template<class TImpl, class CreatorType>
    typename BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef&
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::operator=(const BiDirectionalSelfRef& rhs)
    {
        if(this != &rhs)
        {
            unregisterForSelf();
            m_self = rhs.m_self;
            registerForSelf();
        }
        return *this;
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::BiDirectionalSelfRef(BiDirectionalSelfRef&& rhs) : m_self(nullptr)
    {
        *this = std::move(rhs);
    }

    template<class TImpl, class CreatorType>
    typename BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef&
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::operator=(BiDirectionalSelfRef&& rhs)
    {
        if(this != &rhs)
        {
            auto self = m_self;
            auto rhs_self = rhs.m_self;
            unregisterForSelf();
            rhs.unregisterForSelf();
            m_self = rhs_self;
            rhs.m_self = self;
            registerForSelf();
            rhs.registerForSelf();
        }
        return *this;
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::~BiDirectionalSelfRef()
    {
        unregisterForSelf();
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>*
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::operator->() const
    {
        return m_self;
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>&
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::operator*() const
    {
        return *m_self;
    }

    template<class TImpl, class CreatorType>
    void
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::registerForSelf()
    {
        if(m_self)
        {
            m_self->registerPointer(this);
        }
    }

    template<class TImpl, class CreatorType>
    void
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::unregisterForSelf()
    {
        if(m_self)
        {
            m_self->unregisterPointer(this);
            m_self = nullptr;
        }
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalSelfRef::operator bool() const
    {
        return m_self != nullptr;
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::BiDirectionalProxyBuilderBase(): m_base(){}

    template<class TImpl, class CreatorType>
    template<class... CArgs>
    void
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::setArgs(CArgs&&... args)
    {
        m_base.setArgs(args...);
    }

    template<class TImpl, class CreatorType>
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::~BiDirectionalProxyBuilderBase()
    {
        for(auto& pointer : m_notifyOnDeath)
        {
            pointer->m_self = nullptr;
        }
    }

    template<class TImpl, class CreatorType>
    void
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::incrementRef()
    {
        m_base.incrementRef();
    }

    template<class TImpl, class CreatorType>
    void
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::decrementRef()
    {
        m_base.decrementRef();
    }

    template<class TImpl, class CreatorType>
    void
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::registerPointer(pointer_type* ptr)
    {
        m_notifyOnDeath.push_back(ptr);
    }

    template<class TImpl, class CreatorType>
    void
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::unregisterPointer(pointer_type* ptr)
    {
        m_notifyOnDeath.erase(std::remove(m_notifyOnDeath.begin(), m_notifyOnDeath.end(), ptr), m_notifyOnDeath.end());
    }

    template<class TImpl, class CreatorType>
    TImpl*
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::get()
    {
        return m_base.get();
    }

    template<class TImpl, class CreatorType>
    std::size_t
    BiDirectionalProxyBuilderBase<TImpl, CreatorType>::unsafeRefCount()
    {
        return m_base.m_instanceRefCount;
    }
}

#endif // __AEX_PROXY_IPP
