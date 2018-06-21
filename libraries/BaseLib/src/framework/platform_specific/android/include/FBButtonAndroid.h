#ifndef __AEX_FBBUTTON_ANDROID_HPP
#define __AEX_FBBUTTON_ANDROID_HPP

#include <aex/AndroidAPI.hpp>
#include <jni.h>
#include <map>
#include <thread>

namespace aex
{
    class FBInitRequestEvent;
    namespace android_impl
    {
        class FBButtonAndroidImpl
        {
        public:
            FBButtonAndroidImpl();
            ~FBButtonAndroidImpl();
            void clicked(const FBInitRequestEvent* e);
        private:
            struct threadLocalJavaData
            {
                threadLocalJavaData(JNIEnv* env, size_t frameSize) : m_env(env, frameSize) {}
                threadLocalJavaData(threadLocalJavaData&& localJava);
                threadLocalJavaData& operator=(threadLocalJavaData&& localJava);
                JniSafeEnv m_env;
                jobject m_ActivityObject;
            };
            std::thread::id addThread();
            std::map<std::thread::id, threadLocalJavaData> m_jReferences;
        };
    }
}

#endif
