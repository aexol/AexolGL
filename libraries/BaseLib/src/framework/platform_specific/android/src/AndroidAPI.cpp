#if __TARGET_PLATFORM__ == __ANDROID_OS__
#include <aex/AndroidAPI.hpp>
#include <android/log.h>
#include <aex/Common.hpp>
#include <SDL.h>
#include <algorithm>
#include <mutex>
#include <cstdio>
namespace aex
{
JNIObjectError::JNIObjectError(const aex::string& object_name, const char* file, int line) :
    m_error(aex::string("JNI error: object class=").append(object_name).append(" in file=").append(file).append(":").append(aex::to_string(line)))
{}
const char*
JNIObjectError::what() const NOEXCEPT
{
    return m_error.c_str();
}


JNIFrameGuard::JNIFrameGuard(JNIEnv& e, size_t frame_size) : m_env(e)
{
    if((m_created = (m_env.PushLocalFrame(frame_size) < 0)))
    {
        throw std::runtime_error("Failed to create JNI frame");
    }
}
JNIFrameGuard::JNIFrameGuard(JNIFrameGuard&& guard) :
    m_env(guard.m_env),
    m_created(guard.m_created)
{
    guard.m_created = false;
}

JNIFrameGuard& JNIFrameGuard::operator=(JNIFrameGuard&& guard)
{
    m_env = guard.m_env;
    m_created = guard.m_created;
    guard.m_created = false;
    return *this;
}
JNIFrameGuard::~JNIFrameGuard()
{
    if(m_created)
    {
        m_env.PopLocalFrame(NULL);
    }
}
JniSafeEnv::JniSafeEnv(JniSafeEnv&& safe_env) :
    m_env(safe_env.m_env),
    m_guard(std::move(safe_env.m_guard)),
    m_refCount(safe_env.m_refCount),
    m_refLimit(safe_env.m_refLimit)
{}
JniSafeEnv& JniSafeEnv::operator=(JniSafeEnv&& safe_env)
{
    m_env = safe_env.m_env;
    m_guard = std::move(safe_env.m_guard);
    m_refCount = safe_env.m_refCount;
    m_refLimit = safe_env.m_refLimit;
    return *this;
}

aex::string AndroidGetMainActivityPackage()
{
    static aex::string packageName;
    static std::once_flag flag;

    if(packageName.empty())
    {
        std::call_once(flag, [&]()
        {
            jobject context;
            jmethodID mid;
            jstring package_name;
            JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
            JniSafeEnv safe_env { env, 16 };
            context = static_cast<jobject>(SDL_AndroidGetActivity());
            mid = safe_env([&](JNIEnv & env)
            {
                return env.GetMethodID(env.GetObjectClass(context), "getPackageName", "()Ljava/lang/String;");
            });
            package_name = safe_env([&](JNIEnv & env)
            {
                return static_cast<jstring>(env.CallObjectMethod(context, mid));
            });
            const char* p_name = env->GetStringUTFChars(package_name, 0);
            packageName = p_name;
            env->ReleaseStringUTFChars(package_name, p_name);
        });
    }
    return packageName;
}

aex::string AndroidGetResourceString(const aex::string& field_id)
{
    jobject context;
    jmethodID mid;
    jclass resourceClass;
    jfieldID fid;
    jint field_int_val;
    jstring field_value;
    if (SDL_AndroidGetExternalStorageState()==0)
    {
        __android_log_write(ANDROID_LOG_INFO, "AEX", "dziala 0 ");
    }
   
    __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 0 ");

    JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
     __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 0.5");
    JniSafeEnv safe_env { env, 16 };
     __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 0.7");
    context = static_cast<jobject>(SDL_AndroidGetActivity());
    __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 1");
    if(!context)
        throw JNIObjectError { "context", __FILE__, __LINE__ };
        
    mid =
        safe_env([&](JNIEnv & env)
    {
        return env.GetMethodID(env.GetObjectClass(context), "getStringResource", "(I)Ljava/lang/String;");
    });
    __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 2");
    auto packageName = AndroidGetMainActivityPackage();
    std::replace(packageName.begin(), packageName.end(), '.', '/');
    auto split_pos = field_id.rfind(".");
    packageName.append("/R$").append(field_id.substr(0, split_pos));
    auto c_field_name = field_id.substr(split_pos + 1);
    
    __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 3");
    
    resourceClass =
        safe_env([&](JNIEnv & env)
    {
        return env.FindClass(packageName.c_str());
    });

    __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 4");
    
    if(!resourceClass)
        throw JNIObjectError { packageName, __FILE__, __LINE__ };

    fid =
        safe_env([&](JNIEnv & env)
    {
        return env.GetStaticFieldID(resourceClass, c_field_name.c_str(), "I");
    });
    
    __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 5");
    
    field_int_val =
        safe_env([&](JNIEnv & env)
    {
        return env.GetStaticIntField(resourceClass, fid);
    });

    if(!field_int_val)
        throw JNIObjectError { packageName + c_field_name, __FILE__, __LINE__ };
    
    __android_log_write(ANDROID_LOG_INFO, "AEX", "AndroidGetResourceString 6");
        
    field_value =
        safe_env([&](JNIEnv & env)
    {
        return static_cast<jstring>(env.CallObjectMethod(context, mid, field_int_val));
    });
    const char* f_val = env->GetStringUTFChars(field_value, 0);
    aex::string ret {f_val};
    env->ReleaseStringUTFChars(field_value, f_val);
    return ret;
    

    
}

}
#endif
