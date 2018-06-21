#include <aex/TLS.hpp>
#include <aex/platform_definitions.hpp>
#ifdef AEX_HAVE_PTHREAD_H
#   include <pthread.h>
#endif // HAVE_PTHREAD_H
#include <cstdlib>
#include <map>
#include <array>
#include <boost/lockfree/queue.hpp>
#include <atomic>
#include <functional>


#if __TARGET_PLATFORM__==__APPLE_IOS__ || !defined(AEX_HAS_CXX11_THREAD_LOCAL)
#   define AEX_PTHREAD_TLS
#else // if __TARGET_PLATFORM__==__APPLE_IOS__ || !defined(AEX_HAS_CXX11_THREAD_LOCAL)
#   define AEX_NATIVE_TLS
#endif // if __TARGET_PLATFORM__==__APPLE_IOS__ || !defined(AEX_HAS_CXX11_THREAD_LOCAL)


template<class T>
struct DeleteTemplate
{
    void operator()(void* p)
    {
        delete static_cast<T>(p);
    }
};
#define TLS_ARRAY_SIZE 1024
typedef std::array<aex::TLS_entry, TLS_ARRAY_SIZE> TLS_ArrayType;
#ifdef AEX_PTHREAD_TLS
static pthread_key_t pKey;
static pthread_once_t once_control = PTHREAD_ONCE_INIT;
#elif defined(AEX_NATIVE_TLS)
TLS_ArrayType& getTLS_array()
{
    static thread_local TLS_ArrayType TLS_array;
    return TLS_array;
}
#endif
boost::lockfree::queue<uint32_t, boost::lockfree::fixed_sized<false>>& getFreedSlots()
{
    static boost::lockfree::queue<uint32_t, boost::lockfree::fixed_sized<false>> FreeSlots(0);
    return FreeSlots;
}
static std::atomic<uint32_t> NextIndex {0};

namespace aex
{
template<>
void TLS_entry::delete_func<std::nullptr_t>(void* p)
{
    UNUSED(p);
}

struct tls_key
{
    uint32_t mIndex;
};

#ifdef AEX_PTHREAD_TLS
static void array_destructor(void* arr)
{
    static_cast<TLS_ArrayType*>(arr)->~TLS_ArrayType();
}

static void
make_key()
{
    pthread_key_create(&pKey, array_destructor);
}
#endif

TLS_entry* getEntryForThread(const tls_key* k)
{
#ifdef AEX_PTHREAD_TLS
    void* ptr;
    pthread_once(&once_control, make_key);

    if((ptr = pthread_getspecific(pKey)) == NULL)
    {
        ptr = malloc(sizeof(TLS_ArrayType));
        new(static_cast<TLS_ArrayType*>(ptr)) TLS_ArrayType();
        int status = pthread_setspecific(pKey, ptr);

        if(status)
        {
            if(status == EAGAIN)
            {
                throw std::runtime_error("Failed to create thread-specific data due to lack of necessary resources.");
            }

            if(status == ENOMEM)
            {
                throw std::runtime_error("Failed to create thread-specific data, insufficient memory.");
            }
        }
    }

    TLS_ArrayType& arr = *static_cast<TLS_ArrayType*>(ptr);
#elif defined(AEX_NATIVE_TLS)
    TLS_ArrayType& arr = getTLS_array();
#endif
    return &arr[k->mIndex];
}

tls_key* make_tls_key()
{
    uint32_t index = 0;

    if(!getFreedSlots().pop(index))
    {
        index = NextIndex.fetch_add(1, std::memory_order_acq_rel);

        if(index > TLS_ARRAY_SIZE)
        {
            NextIndex.fetch_sub(1, std::memory_order_relaxed);
            throw std::runtime_error("Failed to create thread-specific data due to lack of necessary resources.");
        }
    }

    return new tls_key {index};
}

void free_tls_key(tls_key* key)
{
    TLS_entry* e = getEntryForThread(key);
    e->template set<std::nullptr_t>(nullptr);
    getFreedSlots().push(key->mIndex);
    delete key;
}
}
