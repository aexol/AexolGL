#ifndef __AEX_TLS_HPP
#define __AEX_TLS_HPP
#include <aex/Common.hpp>
#include <aex/Logger.hpp>
#include <utility>
#include <tuple>
#include <functional>

namespace aex
{

class AEX_PUBLIC_API TLS_entry
{
private:

    struct Delete
    {

        void operator()(void* p)
        {
            if(self->mDelete)
            {
                self->mDelete(p);
            }
        }
        TLS_entry* self;
    };
    typedef std::unique_ptr<void, Delete> pointer;
    template<class T>
    static void delete_func(void* p);
    std::function<void(void*)> mDelete;
    pointer mData;

public:
    TLS_entry() :
        mDelete([](void* p)
    {
        UNUSED(p);
    })
    ,
    mData(nullptr, Delete {this})
    {
    }

    template<class T>
    void set(T* p)
    {
        mData.reset(p);
        mDelete = std::function<void(void*)>(TLS_entry::delete_func<T>);
    }

    const pointer& data() const
    {
        return mData;
    }
};

template<class T>
void TLS_entry::delete_func(void* p)
{
    delete static_cast<T*>(p);
}

template<>
void TLS_entry::delete_func<std::nullptr_t>(void* p);

struct AEX_PUBLIC_API tls_key;
AEX_PUBLIC_API tls_key* make_tls_key();
AEX_PUBLIC_API void free_tls_key(tls_key* key);
AEX_PUBLIC_API TLS_entry* getEntryForThread(const tls_key* key);

template<class T>
class ThreadLocalStorage
{
public:

    ThreadLocalStorage()
    {
        init();
    }

    ThreadLocalStorage(ThreadLocalStorage&& tls)
    {
        std::swap(mKey, tls.mKey);
    }

    ThreadLocalStorage& operator=(ThreadLocalStorage&& tls)
    {
        if(this != &tls)
        {
            std::swap(mKey, tls.mKey);
        }

        return *this;
    }

    ThreadLocalStorage(const ThreadLocalStorage& tls) = delete;
    ThreadLocalStorage& operator=(const ThreadLocalStorage& tls) = delete;

    template<class Arg, typename std::enable_if<NonSelf<Arg, ThreadLocalStorage<T>>()>::type* = nullptr>
    explicit ThreadLocalStorage(Arg && arg)
    {
        init(aex::forward<decltype(arg)>(arg));
    }

    /*template<class... Args>
    ThreadLocalStorage(Args&&... args) :
    ThreadLocalStorage() {
        init(std::forward<Args>(args)...);
    }*/
    template<class... Args>
    ThreadLocalStorage(Args&& ... args)
    {
        init(std::forward<Args>(args)...);
    }

    ~ThreadLocalStorage()
    {
        //LOG_DEBUG( "~ThreadLocalStorage()");
        free_tls_key(mKey);
    }

    const T* operator->() const
    {
        TLS_entry* t = getEntryForThread(mKey);
        return static_cast<T*>(t->data().get());
    }

    T* operator->()
    {
        const TLS_entry* t = getEntryForThread(mKey);
        return static_cast<T*>(t->data().get());
    }

    T& operator*()
    {
        return *this->operator->();
    }

    const T& operator*() const
    {
        const TLS_entry* t = getEntryForThread(mKey);
        return *static_cast<T*>(t->data().get());
    }

    bool operator==(const ThreadLocalStorage& rhs)
    {
        if(this == &rhs)
        {
            return true;
        }

        return this->mKey == rhs.mKey;
    }

    template<class... Args>
    void init(Args&& ... args)
    {
        mKey = make_tls_key();
        TLS_entry* entry = getEntryForThread(mKey);
        entry->set(new T(std::forward<Args>(args)...));
    }
private:
    tls_key* mKey;

};
}

#endif
