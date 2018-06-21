#ifndef __AEX_ANDROID_API
#define __AEX_ANDROID_API

#include <aex/Common.hpp>
#include <type_traits>
#include <string>
#include <exception>

#if __TARGET_PLATFORM__ == __ANDROID_OS__
#include <jni.h>
namespace aex
{
/****Test for JNI types****/
//template<typename T> struct is_jni
//{
//static_assert(true, "Not a JNI type.");
//typedef void type;
//};
//template<> struct is_jni<void>
//{
//typedef void type;
//};
//template<> struct is_jni<jboolean>
//{
//typedef jboolean type;
//};
//template<> struct is_jni<jbyte>
//{
//typedef jbyte type;
//};
//template<> struct is_jni<jchar>
//{
//typedef jchar type;
//};
//template<> struct is_jni<jshort>
//{
//typedef jshort type;
//};
//template<> struct is_jni<jint>
//{
//typedef jint type;
//};
//template<> struct is_jni<jlong>
//{
//typedef jlong type;
//};
//template<> struct is_jni<jfloat>
//{
//typedef jfloat type;
//};
//template<> struct is_jni<jdouble>
//{
//typedef jdouble type;
//};
//template<> struct is_jni<jobject>
//{
//typedef jobject type;
//};
//template<> struct is_jni<jclass>
//{
//typedef jclass type;
//};
/**************************/
class AEX_PUBLIC_API JavaExceptionOccured : public std::exception
{
    const aex::string m_msg;
public:
    JavaExceptionOccured(const aex::string& msg) : m_msg(msg) {}
    JavaExceptionOccured(aex::string&& msg) : m_msg(std::move(msg)) {}
    virtual const char* what() const NOEXCEPT
    {
        return m_msg.c_str();
    }
};
class AEX_PUBLIC_API JNIObjectError : public std::exception
{
    const aex::string m_error;
public:
    JNIObjectError(const aex::string& object_name, const char* file, int line);
    virtual const char* what() const NOEXCEPT;
};
class AEX_PUBLIC_API JNIFrameGuard
{
public:
    JNIFrameGuard(JNIEnv& e, size_t frame_size = 16);
    JNIFrameGuard(const JNIFrameGuard&) = delete;
    JNIFrameGuard& operator=(const JNIFrameGuard&) = delete;
    JNIFrameGuard(JNIFrameGuard&& guard);
    JNIFrameGuard& operator=(JNIFrameGuard&& guard);
    ~JNIFrameGuard();
private:
    JNIEnv& m_env;
    bool m_created;
};
class AEX_PUBLIC_API JniSafeEnv
{
private:
    JNIEnv& m_env;
    JNIFrameGuard m_guard;
    size_t m_refCount;
    size_t m_refLimit;
    void investigateException()
    {
        jthrowable exc;
        exc = m_env.ExceptionOccurred();

        if(exc)   //Exception occured here. Crap...
        {
            if(m_refCount >= m_refLimit - 5)   //Do we have enough references left to investigate 'cause of error?
            {
                throw JavaExceptionOccured("Java exception occured, not enough references left to investigate.");
            }

            m_env.ExceptionClear();

#ifdef DEBUG
            m_env.ExceptionDescribe();
#endif
            jclass throwable = m_env.GetObjectClass(exc);
            jmethodID getMessageMethodID = m_env.GetMethodID(throwable, "getMessage", "()Ljava/lang/String;");
            jstring j_exc_msg = static_cast<jstring>(m_env.CallObjectMethod(exc, getMessageMethodID));
            const char* err_msg = m_env.GetStringUTFChars(j_exc_msg, 0);
            aex::string err = "Java exception occured with message: ";
            err.append(err_msg);
            JavaExceptionOccured cxx_exc(std::move(err));
            m_env.ReleaseStringUTFChars(j_exc_msg, err_msg);
            throw cxx_exc;
        }
    }
    template<class RType>
    struct ret
    {
        template<class Func>
        ret(Func&& f, JNIEnv& env)
        {
            r = f(env);
        }
        RType operator()()
        {
            if(!r)
            {
                return 0;
            }

            return r;
        }
        RType r;
    };
public:
    JniSafeEnv(JNIEnv* env) : m_env(*env), m_guard {*env}, m_refCount(0), m_refLimit(16) {}
    JniSafeEnv(JNIEnv* env, size_t frame_size) : m_env(*env), m_guard {*env, frame_size}, m_refCount(0), m_refLimit(frame_size) {}
    JniSafeEnv(const JniSafeEnv&) = delete;
    JniSafeEnv& operator=(const JniSafeEnv&) = delete;
    JniSafeEnv(JniSafeEnv&& safe_env);
    JniSafeEnv& operator=(JniSafeEnv&& safe_env);
    template<class Func>
    auto operator()(Func f) -> decltype(f(m_env))
    {
        typedef decltype(f(m_env)) RType;

        if(m_refCount >= m_refLimit - 1)   //We always need to keep one reference in case exception occurs.
        {
            throw JavaExceptionOccured("Not enough references in this frame. Check if you allocated enough.");
        }

        auto retVal = ret<RType> {f, m_env };
        m_refCount++;
        investigateException();
        return retVal();
    }
};
template<>
struct JniSafeEnv::ret<void>
{
    template<class Func>
    ret(Func&& f, JNIEnv& env)
    {
        f(env);
    }
    void operator()() {}
};
aex::string AndroidGetResourceString(const aex::string& field_id);
}
#endif

#endif
