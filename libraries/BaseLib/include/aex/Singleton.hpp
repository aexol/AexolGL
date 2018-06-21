/** @file Singleton.hpp
 *  @brief Definition of Singleton.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __aexolGL__Singleton__
#define __aexolGL__Singleton__

#include <iostream>
#include <atomic>
#include <mutex>

namespace aex
{
template<class T>
class Singleton
{
protected:
    Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;
    static T& GetInstance();
private:
    struct U : public T
    {
        static T* create()
        {
            return static_cast<T*>(new U);
        }
    };
    static std::unique_ptr<T> m_instance;
    static std::once_flag m_flag;
};
template<typename T>
std::unique_ptr<T> Singleton<T>::m_instance {nullptr};
template<typename T>
std::once_flag Singleton<T>::m_flag {};
template<class T>
T&
Singleton<T>::GetInstance()
{
    std::call_once(m_flag, []
    {
        m_instance.reset(U::create());
    }
                  );
    return *m_instance.get();
}
}

#endif /* defined(__aexolGL__Singleton__) */
