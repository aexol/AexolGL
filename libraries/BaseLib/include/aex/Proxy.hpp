#ifndef __AEX_PROXY_HPP
#define __AEX_PROXY_HPP

#include <aex/Common.hpp>
#include <memory>
#include <mutex>
#include <tuple>
#include <type_traits>
#include <list>

namespace aex
{
    namespace _proxy_details
    {
        template<typename T>
        class has_pointer_type {
            template<typename C> static std::true_type test(typename C::pointer_type* = nullptr);
            template<typename C> static std::false_type test(...);
          public:
            static bool const value = decltype(test<T>(0))::value;
        };
        template<class T, bool hasPointerType = has_pointer_type<T>::value>
        struct eval_pointer_type
        {
            typedef typename T::pointer_type pointer_type;
        };
        template<class T>
        struct eval_pointer_type<T, false>
        {
            typedef T* pointer_type;
        };
    }
    /*
     * @brief Create proxy to an instance of an object.
     *
     * Creates an proxy to an instance of TImpl object. As long
     * as atleast one proxy exists, object will be valid, unless builder which created proxy is destroyed.
     *
     * Proxies do not own an instance, instance is owned by ProxyBuilder, which manages the lifetime of
     * an instance.
     *
     * When all proxies to an instance are destroyed, instance is automatically cleand up.
     */
    template<class RefCountingT>
    class Proxy
    {
        public:
            typedef RefCountingT owner_type;
            typedef 
                typename _proxy_details::eval_pointer_type<RefCountingT>::pointer_type owner_pointer;
        private:
            typedef decltype(std::declval<owner_type>().get()) impl_pointer;
            typedef decltype(*std::declval<impl_pointer>()) impl_type;
            /*
             * @brief Automatically manage life time of owner_pointer
             */
            class ProxyOwnerGuard
            {
                public:
                    ProxyOwnerGuard();
                    explicit ProxyOwnerGuard(const owner_type& owner);
                    explicit ProxyOwnerGuard(const owner_pointer& owner);
                    explicit ProxyOwnerGuard(owner_pointer&& owner);
                    explicit ProxyOwnerGuard(const ProxyOwnerGuard& rhs);
                    explicit ProxyOwnerGuard(ProxyOwnerGuard&& rhs);
                    ProxyOwnerGuard& operator=(const owner_type& owner);
                    ProxyOwnerGuard& operator=(const owner_pointer& owner);
                    ProxyOwnerGuard& operator=(owner_pointer&& owner);
                    ProxyOwnerGuard& operator=(const ProxyOwnerGuard& rhs);
                    ProxyOwnerGuard& operator=(ProxyOwnerGuard&& rhs);
                    ~ProxyOwnerGuard();

                    const owner_pointer& operator->() const;
                    operator bool() const;
                private:
                    ProxyOwnerGuard& tie();
                    void release();
                    owner_pointer m_owner;
            };
        public:
            /*
             * @brief Empty proxy.
             */
            Proxy();

            /*
             * @brief Make new proxy with builder.
             */
            explicit Proxy(const owner_pointer& owner);
            explicit Proxy(owner_pointer&& owner);
            void reset(const owner_pointer& owner = owner_pointer{});
            void reset(owner_pointer&& owner);

            /*
             * @brief Access object instance.
             */
            impl_pointer operator->();
            impl_type& operator*();
            impl_pointer get();

            /*
             * @brief Returns true if valid.
             */
            operator bool() const;

            virtual ~Proxy() = default;
        private:
            ProxyOwnerGuard m_owner;
    };

    /*
     * @brief Builder managers the lifetime of an instance object.
     *
     * Builder creates an instance of an object on request from proxy. Instance is not released as long
     * as there is a proxy connected to builder.
     */
    namespace _proxy_details
    {
        template<class T, class... Args>
        struct InstanceCreator
        {
            InstanceCreator();

            template<class U, class... CArgs, typename std::enable_if<NonSelf<InstanceCreator, U>()>::type* = nullptr>
            InstanceCreator(U&& u, CArgs&&... cargs)
            {
                setArgs(aex::forward<U>(u), aex::forward<CArgs>(cargs)...);
            }
            template<class... CArgs>
            void setArgs(CArgs&&... cargs);

            template<int ...S>
            std::unique_ptr<T> delayedConstruction(seq<S...>);

            std::unique_ptr<T> createInstance();

            std::tuple<Args...> m_args;
        };

        template<class TImpl, class CreatorType>
        struct _ProxyBuilderCommon
        {
                /*
                 * @brief Create new builder.
                 */
                _ProxyBuilderCommon();

                /*
                 * @brief Create new builder and copy parameters for delayed creation.
                 */
                template<class U, class... CArgs, typename std::enable_if<NonSelf<_ProxyBuilderCommon, U>()>::type* = nullptr>
                _ProxyBuilderCommon(U&& u, CArgs&&... cargs):
                    m_instance{nullptr},
                    m_creator{aex::forward<U>(u), aex::forward<CArgs>(cargs)...},
                    m_instanceRefCount{0},
                    m_instanceLock{}
                {
                }

                virtual ~_ProxyBuilderCommon() = default;

                /*
                 * @brief Set parameters for delayed creation.
                 */
                template<class... Args>
                void setArgs(Args&&... args);
                void incrementRef();
                void decrementRef();
                TImpl* get();

                std::unique_ptr<TImpl> m_instance;
                CreatorType m_creator;
                std::size_t m_instanceRefCount;
                std::mutex m_instanceLock;
        };

    }

    template<class TImpl, class CreatorType>
    class ProxyBuilderBase
    {
        public:
            typedef Proxy<ProxyBuilderBase<TImpl, CreatorType>> proxy_type;
            /*
             * @brief Create new builder.
             */
            ProxyBuilderBase();

            /*
             * @brief Create new builder and copy parameters for delayed creation.
             */
            template<class U, class... CArgs, typename std::enable_if<NonSelf<ProxyBuilderBase, U>()>::type* = nullptr>
            ProxyBuilderBase(U&& u, CArgs&&... args): m_base(aex::forward<U>(u), aex::forward<CArgs>(args)...){}

            /*
             * @brief Set parameters for delayed creation.
             */
            template<class... CArgs>
            void setArgs(CArgs&&... args);
        protected:
            typedef _proxy_details::_ProxyBuilderCommon<TImpl, CreatorType> base_builder;
            friend class Proxy<ProxyBuilderBase<TImpl, CreatorType>>;
            void incrementRef();
            void decrementRef();
            std::size_t unsafeRefCount();
            TImpl* get();
            base_builder m_base;
    };
    
    template<class TImpl, class CreatorType>
    class BiDirectionalProxyBuilderBase
    {
        private:
            struct BiDirectionalSelfRef
            {
                explicit BiDirectionalSelfRef(BiDirectionalProxyBuilderBase* self = nullptr);
                BiDirectionalSelfRef& operator=(BiDirectionalProxyBuilderBase* self);
                BiDirectionalSelfRef(const BiDirectionalSelfRef& rhs);
                BiDirectionalSelfRef& operator=(const BiDirectionalSelfRef& rhs);
                BiDirectionalSelfRef(BiDirectionalSelfRef&& rhs);
                BiDirectionalSelfRef& operator=(BiDirectionalSelfRef&& rhs);
                ~BiDirectionalSelfRef();
                BiDirectionalProxyBuilderBase* operator->() const;
                BiDirectionalProxyBuilderBase& operator*() const;
                void registerForSelf();
                void unregisterForSelf();
                operator bool() const;
                BiDirectionalProxyBuilderBase* m_self;
            };
        public:
            typedef BiDirectionalSelfRef pointer_type;
            typedef Proxy<BiDirectionalProxyBuilderBase<TImpl, CreatorType>> proxy_type;
            /*
             * @brief Create new builder.
             */
            BiDirectionalProxyBuilderBase();

            /*
             * @brief Create new builder and copy parameters for delayed creation.
             */
            template<class U, class... CArgs, typename std::enable_if<NonSelf<BiDirectionalProxyBuilderBase, U>()>::type* = nullptr>
            BiDirectionalProxyBuilderBase(U&& u, CArgs&... args): m_base(aex::forward<U>(u), aex::forward<CArgs>(args)...){}

            /*
             * @brief Set parameters for delayed creation.
             */
            template<class... CArgs>
            void setArgs(CArgs&&... args);

            ~BiDirectionalProxyBuilderBase();
        protected:
            typedef _proxy_details::_ProxyBuilderCommon<TImpl, CreatorType> base_builder;
            friend class Proxy<BiDirectionalProxyBuilderBase<TImpl, CreatorType>>;
            void incrementRef();
            void decrementRef();
            void registerPointer(pointer_type* ptr);
            void unregisterPointer(pointer_type* ptr);
            std::size_t unsafeRefCount();
            TImpl* get();

            base_builder m_base;
            std::mutex m_mutex;
            std::list<pointer_type*> m_notifyOnDeath;
    };

}

#include "details/Proxy.ipp"
namespace aex
{
    template<class T, class... Args>
    using ProxyBuilder = ProxyBuilderBase<T, _proxy_details::InstanceCreator<T, Args...>>;

    template<class T, class... Args>
    using BiDirectionalProxyBuilder = BiDirectionalProxyBuilderBase<T, _proxy_details::InstanceCreator<T, Args...>>;
}
#endif // __AEX_PROXY_HPP
