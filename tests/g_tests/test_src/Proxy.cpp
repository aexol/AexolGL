#include <aex/Proxy.hpp>
#include "gmock/gmock.h"

class MockBuilder
{
    public:
        MOCK_METHOD0(incrementRef, void());
        MOCK_METHOD0(decrementRef, void());
        MOCK_METHOD0(get, int*());
};

TEST(Proxy, CanCreate)
{
    aex::Proxy<MockBuilder> proxy;
    EXPECT_FALSE(bool(proxy));
    MockBuilder builder;
    EXPECT_CALL(builder, incrementRef())
        .Times(1);
    EXPECT_CALL(builder, decrementRef())
        .Times(1);
    aex::Proxy<MockBuilder> proxy2(&builder);
    EXPECT_TRUE(bool(proxy2));
}

ACTION_P(IncrementPointee, p)
{
    (*p)++;
}

ACTION_P(DecrementPointee, p)
{
    (*p)--;
}

TEST(Proxy, CanReset)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    aex::Proxy<MockBuilder> proxy[SAMPLE];
    int refCount = 0;
    MockBuilder builder;
    EXPECT_CALL(builder, incrementRef())
        .Times(10)
        .WillRepeatedly(IncrementPointee(&refCount))
        ;
    EXPECT_CALL(builder, decrementRef())
        .Times(10)
        .WillRepeatedly(DecrementPointee(&refCount))
        ;
    for(auto& p : proxy)
    {
        EXPECT_FALSE(bool(p));
        p.reset(&builder);
        EXPECT_TRUE(bool(p));
    }
    EXPECT_EQ(refCount, 10);
    for(auto& p : proxy)
    {
        EXPECT_TRUE(bool(p));
        p.reset();
        EXPECT_FALSE(bool(p));
    }
    EXPECT_EQ(refCount, 0);
}

struct impl
{
    impl() : m_i(10), m_j(20){}
    explicit impl(int i) : m_i(i), m_j(i){}
    impl(int i, int j) : m_i(i), m_j(j){}
    int m_i, m_j;
};

struct mock_destroy_impl
{
    MOCK_METHOD0(Die, void());
    virtual ~mock_destroy_impl() { Die(); }
};

template<class Impl, class... Args>
class TestProxyBuilder : public aex::ProxyBuilder<Impl, Args...>
{
    public:
        typedef typename aex::ProxyBuilder<Impl, Args...>::proxy_type proxy_type;
        std::size_t getRefCount()
        {
            return this->unsafeRefCount();
        }

        Impl* get_impl()
        {
            return this->get();
        }
};

using TestProxyBuilderDCtor = TestProxyBuilder<impl>;
using TestProxyBuilderCtor1P = TestProxyBuilder<impl, int>;
using TestProxyBuilderCtor2P = TestProxyBuilder<impl, int, int>;

template<class Impl, class... Args>
class TestBiDirectionalProxyBuilder : public aex::BiDirectionalProxyBuilder<Impl, Args...>
{
    public:
        typedef aex::BiDirectionalProxyBuilder<Impl, Args...> base_type;
        typedef typename base_type::proxy_type proxy_type;
        using pointer_type = typename base_type::pointer_type;

        std::size_t getRefCount()
        {
            return this->unsafeRefCount();
        }

        Impl* get_impl()
        {
            return this->get();
        }

        pointer_type operator&()
        {
            return pointer_type{static_cast<base_type*>(this)};
        }
};

using TestBiDirectionalProxyBuilderDCtor = TestBiDirectionalProxyBuilder<impl>;
using TestBiDirectionalProxyBuilderCtor1P = TestBiDirectionalProxyBuilder<impl, int>;
using TestBiDirectionalProxyBuilderCtor2P = TestBiDirectionalProxyBuilder<impl, int, int>;

TEST(TestProxyBuilder, CanCreateNoProxies)
{
    TestProxyBuilderDCtor tpbdc;
    TestProxyBuilderCtor1P tpbc1p;
    TestProxyBuilderCtor2P tpbc2p;
    EXPECT_EQ(tpbdc.get_impl(), nullptr);
    EXPECT_EQ(tpbc1p.get_impl(), nullptr);
    EXPECT_EQ(tpbc2p.get_impl(), nullptr);
}

TEST(TestProxyBuilder, CanCreateWithArgs)
{
    TestProxyBuilderDCtor tpbdc;
    TestProxyBuilderCtor1P tpbc1p;
    TestProxyBuilderCtor2P tpbc2p;
    typename TestProxyBuilderDCtor::proxy_type proxyDCtor;
    typename TestProxyBuilderCtor1P::proxy_type proxyCtor1P;
    typename TestProxyBuilderCtor2P::proxy_type proxyCtor2P;
    proxyDCtor.reset(&tpbdc);
    proxyCtor1P.reset(&tpbc1p);
    proxyCtor2P.reset(&tpbc2p);
    EXPECT_EQ(proxyDCtor->m_i, 10);
    EXPECT_EQ(proxyDCtor->m_j, 20);

    /*
     * Creator value-initalized?
     */
    EXPECT_EQ(proxyCtor1P->m_i, 0);
    EXPECT_EQ(proxyCtor1P->m_j, 0);
    EXPECT_EQ(proxyCtor2P->m_i, 0);
    EXPECT_EQ(proxyCtor2P->m_j, 0);
    proxyCtor1P.reset();
    proxyCtor2P.reset();
    proxyCtor1P.reset(&tpbc1p);
    proxyCtor2P.reset(&tpbc2p);
    tpbc1p.setArgs(5);
    tpbc2p.setArgs(5, 10);
    EXPECT_EQ(proxyCtor1P->m_i, 5);
    EXPECT_EQ(proxyCtor1P->m_j, 5);
    EXPECT_EQ(proxyCtor2P->m_i, 5);
    EXPECT_EQ(proxyCtor2P->m_j, 10);
}

TEST(TestProxyBuilder, CanDestroy)
{
    using TestProxyBuilderDCtor = TestProxyBuilder<mock_destroy_impl>;
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestProxyBuilderDCtor tpbdc;
    typename TestProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    for(int i = 0; i < SAMPLE; i++)
    {
        proxyDCtor[i].reset(&tpbdc);
    }

    EXPECT_CALL(*proxyDCtor[0], Die())
        .Times(1)
        ;

    for(int i = 0; i < SAMPLE; i++)
    {
        proxyDCtor[i].reset();
    }
}

TEST(TestProxyBuilder, IsBeingIncremented)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestProxyBuilderDCtor tpbdc;
    TestProxyBuilderCtor1P tpbc1p;
    TestProxyBuilderCtor2P tpbc2p;

    typename TestProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    typename TestProxyBuilderCtor1P::proxy_type proxyCtor1P[SAMPLE];
    typename TestProxyBuilderCtor2P::proxy_type proxyCtor2P[SAMPLE];

    int i;
    for(i = 0; i < SAMPLE; i++)
    {
        EXPECT_EQ(tpbdc.getRefCount(), i);
        EXPECT_EQ(tpbc1p.getRefCount(), i);
        EXPECT_EQ(tpbc2p.getRefCount(), i);
        proxyDCtor[i].reset(&tpbdc);
        proxyCtor1P[i].reset(&tpbc1p);
        proxyCtor2P[i].reset(&tpbc2p);
    }
}

TEST(TestProxyBuilder, IsSameInstance)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestProxyBuilderDCtor tpbdc;
    TestProxyBuilderCtor1P tpbc1p;
    TestProxyBuilderCtor2P tpbc2p;

    typename TestProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    typename TestProxyBuilderCtor1P::proxy_type proxyCtor1P[SAMPLE];
    typename TestProxyBuilderCtor2P::proxy_type proxyCtor2P[SAMPLE];

    int i;
    for(i = 0; i < SAMPLE; i++)
    {
        EXPECT_EQ(tpbdc.getRefCount(), i);
        EXPECT_EQ(tpbc1p.getRefCount(), i);
        EXPECT_EQ(tpbc2p.getRefCount(), i);
        proxyDCtor[i].reset(&tpbdc);
        proxyCtor1P[i].reset(&tpbc1p);
        proxyCtor2P[i].reset(&tpbc2p);
    }

    for(i = 1; i < SAMPLE; i++)
    {
        EXPECT_EQ(proxyDCtor[i].get(), proxyDCtor[i - 1].get());
        EXPECT_EQ(proxyCtor1P[i].get(), proxyCtor1P[i - 1].get());
        EXPECT_EQ(proxyCtor2P[i].get(), proxyCtor2P[i - 1].get());
    }
}

TEST(TestProxyBuilder, CanDestroyAfterOrphaned)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestProxyBuilderDCtor tpbdc;
    TestProxyBuilderCtor1P tpbc1p;
    TestProxyBuilderCtor2P tpbc2p;

    typename TestProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    typename TestProxyBuilderCtor1P::proxy_type proxyCtor1P[SAMPLE];
    typename TestProxyBuilderCtor2P::proxy_type proxyCtor2P[SAMPLE];

    int i;
    for(i = 0; i < SAMPLE; i++)
    {
        EXPECT_EQ(tpbdc.getRefCount(), i);
        EXPECT_EQ(tpbc1p.getRefCount(), i);
        EXPECT_EQ(tpbc2p.getRefCount(), i);
        proxyDCtor[i].reset(&tpbdc);
        proxyCtor1P[i].reset(&tpbc1p);
        proxyCtor2P[i].reset(&tpbc2p);
    }

    for(i = 1; i < SAMPLE; i++)
    {
        EXPECT_EQ(proxyDCtor[i].get(), proxyDCtor[i - 1].get());
        EXPECT_EQ(proxyCtor1P[i].get(), proxyCtor1P[i - 1].get());
        EXPECT_EQ(proxyCtor2P[i].get(), proxyCtor2P[i - 1].get());
    }
}

TEST(TestBiDirectionalProxyBuilder, CanCreateNoProxies)
{
    TestBiDirectionalProxyBuilderDCtor tpbdc;
    TestBiDirectionalProxyBuilderCtor1P tpbc1p;
    TestBiDirectionalProxyBuilderCtor2P tpbc2p;
    EXPECT_EQ(tpbdc.get_impl(), nullptr);
    EXPECT_EQ(tpbc1p.get_impl(), nullptr);
    EXPECT_EQ(tpbc2p.get_impl(), nullptr);
}

TEST(TestBiDirectionalProxyBuilder, IsBeingIncremented)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestBiDirectionalProxyBuilderDCtor tpbdc;
    TestBiDirectionalProxyBuilderCtor1P tpbc1p;
    TestBiDirectionalProxyBuilderCtor2P tpbc2p;

    typename TestBiDirectionalProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor1P::proxy_type proxyCtor1P[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor2P::proxy_type proxyCtor2P[SAMPLE];

    int i;
    for(i = 0; i < SAMPLE; i++)
    {
        EXPECT_EQ(tpbdc.getRefCount(), i);
        EXPECT_EQ(tpbc1p.getRefCount(), i);
        EXPECT_EQ(tpbc2p.getRefCount(), i);
        proxyDCtor[i].reset(&tpbdc);
        proxyCtor1P[i].reset(&tpbc1p);
        proxyCtor2P[i].reset(&tpbc2p);
    }
}

TEST(TestBiDirectionalProxyBuilder, IsBiDirectional)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    typename TestBiDirectionalProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor1P::proxy_type proxyCtor1P[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor2P::proxy_type proxyCtor2P[SAMPLE];

    {
        TestBiDirectionalProxyBuilderDCtor tpbdc;
        TestBiDirectionalProxyBuilderCtor1P tpbc1p;
        TestBiDirectionalProxyBuilderCtor2P tpbc2p;

        int i;
        for(i = 0; i < SAMPLE; i++)
        {
            EXPECT_EQ(tpbdc.getRefCount(), i);
            EXPECT_EQ(tpbc1p.getRefCount(), i);
            EXPECT_EQ(tpbc2p.getRefCount(), i);
            proxyDCtor[i].reset(&tpbdc);
            proxyCtor1P[i].reset(&tpbc1p);
            proxyCtor2P[i].reset(&tpbc2p);
        }
    }
    for(int i = 0; i < SAMPLE; i++)
    {
        EXPECT_FALSE(bool(proxyDCtor[i]));
        EXPECT_FALSE(bool(proxyCtor1P[i]));
        EXPECT_FALSE(bool(proxyCtor2P[i]));
    }
}

TEST(TestBiDirectionalProxyBuilder, CanCreateWithArgs)
{
    TestBiDirectionalProxyBuilderDCtor tpbdc;
    TestBiDirectionalProxyBuilderCtor1P tpbc1p;
    TestBiDirectionalProxyBuilderCtor2P tpbc2p;
    typename TestBiDirectionalProxyBuilderDCtor::proxy_type proxyDCtor;
    typename TestBiDirectionalProxyBuilderCtor1P::proxy_type proxyCtor1P;
    typename TestBiDirectionalProxyBuilderCtor2P::proxy_type proxyCtor2P;
    proxyDCtor.reset(&tpbdc);
    proxyCtor1P.reset(&tpbc1p);
    proxyCtor2P.reset(&tpbc2p);
    EXPECT_EQ(proxyDCtor->m_i, 10);
    EXPECT_EQ(proxyDCtor->m_j, 20);

    /*
     * Creator value-initalized?
     */
    EXPECT_EQ(proxyCtor1P->m_i, 0);
    EXPECT_EQ(proxyCtor1P->m_j, 0);
    EXPECT_EQ(proxyCtor2P->m_i, 0);
    EXPECT_EQ(proxyCtor2P->m_j, 0);
    proxyCtor1P.reset();
    proxyCtor2P.reset();
    proxyCtor1P.reset(&tpbc1p);
    proxyCtor2P.reset(&tpbc2p);
    tpbc1p.setArgs(5);
    tpbc2p.setArgs(5, 10);
    EXPECT_EQ(proxyCtor1P->m_i, 5);
    EXPECT_EQ(proxyCtor1P->m_j, 5);
    EXPECT_EQ(proxyCtor2P->m_i, 5);
    EXPECT_EQ(proxyCtor2P->m_j, 10);
}

TEST(TestBiDirectionalProxyBuilder, CanDestroy)
{
    using TestBiDirectionalProxyBuilderDCtor = TestBiDirectionalProxyBuilder<mock_destroy_impl>;
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestBiDirectionalProxyBuilderDCtor tpbdc;
    typename TestBiDirectionalProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    for(int i = 0; i < SAMPLE; i++)
    {
        proxyDCtor[i].reset(&tpbdc);
    }

    EXPECT_CALL(*proxyDCtor[0], Die())
        .Times(1)
        ;

    for(int i = 0; i < SAMPLE; i++)
    {
        proxyDCtor[i].reset();
    }
}

TEST(TestBiDirectionalProxyBuilder, IsSameInstance)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestBiDirectionalProxyBuilderDCtor tpbdc;
    TestBiDirectionalProxyBuilderCtor1P tpbc1p;
    TestBiDirectionalProxyBuilderCtor2P tpbc2p;

    typename TestBiDirectionalProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor1P::proxy_type proxyCtor1P[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor2P::proxy_type proxyCtor2P[SAMPLE];

    int i;
    for(i = 0; i < SAMPLE; i++)
    {
        EXPECT_EQ(tpbdc.getRefCount(), i);
        EXPECT_EQ(tpbc1p.getRefCount(), i);
        EXPECT_EQ(tpbc2p.getRefCount(), i);
        proxyDCtor[i].reset(&tpbdc);
        proxyCtor1P[i].reset(&tpbc1p);
        proxyCtor2P[i].reset(&tpbc2p);
    }

    for(i = 1; i < SAMPLE; i++)
    {
        EXPECT_EQ(proxyDCtor[i].get(), proxyDCtor[i - 1].get());
        EXPECT_EQ(proxyCtor1P[i].get(), proxyCtor1P[i - 1].get());
        EXPECT_EQ(proxyCtor2P[i].get(), proxyCtor2P[i - 1].get());
    }
}

TEST(TestBiDirectionalProxyBuilder, CanDestroyAfterOrphaned)
{
    static CONSTEXPR std::size_t SAMPLE = 10;
    TestBiDirectionalProxyBuilderDCtor tpbdc;
    TestBiDirectionalProxyBuilderCtor1P tpbc1p;
    TestBiDirectionalProxyBuilderCtor2P tpbc2p;

    typename TestBiDirectionalProxyBuilderDCtor::proxy_type proxyDCtor[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor1P::proxy_type proxyCtor1P[SAMPLE];
    typename TestBiDirectionalProxyBuilderCtor2P::proxy_type proxyCtor2P[SAMPLE];

    int i;
    for(i = 0; i < SAMPLE; i++)
    {
        EXPECT_EQ(tpbdc.getRefCount(), i);
        EXPECT_EQ(tpbc1p.getRefCount(), i);
        EXPECT_EQ(tpbc2p.getRefCount(), i);
        proxyDCtor[i].reset(&tpbdc);
        proxyCtor1P[i].reset(&tpbc1p);
        proxyCtor2P[i].reset(&tpbc2p);
    }

    for(i = 1; i < SAMPLE; i++)
    {
        EXPECT_EQ(proxyDCtor[i].get(), proxyDCtor[i - 1].get());
        EXPECT_EQ(proxyCtor1P[i].get(), proxyCtor1P[i - 1].get());
        EXPECT_EQ(proxyCtor2P[i].get(), proxyCtor2P[i - 1].get());
    }
}
