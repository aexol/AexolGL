
/** @file ThreadLocalSingleton.hpp
 *  @brief Definition of ThreadLocalSingleton.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__ThreadLocalSingleton__
#define __aexolGL__ThreadLocalSingleton__

#include <aex/platform_definitions.hpp>
//#if __TARGET_PLATFORM__==__APPLE_IOS__
#include <aex/TLS.hpp>
//#endif
#include <iostream>
#include <atomic>
#include <mutex>
#include <memory>

namespace aex
{

    template<class T>
    class ThreadLocalSingleton
    {
    protected:
        ThreadLocalSingleton() = default;
    public:
        ~ThreadLocalSingleton();
        ThreadLocalSingleton(const ThreadLocalSingleton&) = delete;
        ThreadLocalSingleton(ThreadLocalSingleton&&) = delete;
        ThreadLocalSingleton& operator=(const ThreadLocalSingleton&) = delete;
        ThreadLocalSingleton& operator=(ThreadLocalSingleton&&) = delete;
        static T& GetInstanceForThread();
    private:
        //#if __TARGET_PLATFORM__==__APPLE_IOS__
        //static ThreadLocalStorage<T> m_instance;
        //static ThreadLocalStorage<T>& __instance();
        static aex::unique_ptr<aex::ThreadLocalStorage<T>> m_instance;
        //#else
        //            static thread_local std::unique_ptr<T> m_instance;
        //#endif
    };

//template<class T>

//#if __TARGET_PLATFORM__==__APPLE_IOS__
// ThreadLocalStorage<T> ThreadLocalSingleton<T>::m_instance {
//};
//#else
//    thread_local std::unique_ptr<T> ThreadLocalSingleton<T>::m_instance{};
//#endif
    template<class T>
    aex::unique_ptr<aex::ThreadLocalStorage<T>>
            ThreadLocalSingleton<T>::m_instance = nullptr;

    template<class T>
    ThreadLocalSingleton<T>::~ThreadLocalSingleton()
    {
        m_instance.reset();
        m_instance = nullptr;
    }

    /*template<class T>
    ThreadLocalStorage<T>&
    ThreadLocalSingleton<T>::__instance() {
        static ThreadLocalStorage<T> instance;
        return instance;
    }*/

    template<class T>
    T&
    ThreadLocalSingleton<T>::GetInstanceForThread()
    {
        //#if __TARGET_PLATFORM__!=__APPLE_IOS__
        if(m_instance == nullptr)
        {
            m_instance.reset(new aex::ThreadLocalStorage<T>());
        }

        //#endif
        return **m_instance;
    }
}

#endif /* defined(__aexolGL__ThreadLocalSingleton__) */
