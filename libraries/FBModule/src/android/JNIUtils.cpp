#include "JNIUtils.hpp"

namespace aex
{
    namespace fb
    {
        JniObject getJniClass(const std::string& classDesc){
            Jni& jni = Jni::get();
            JniClass& cls = jni.getClass(classDesc);
            return JniObject(&cls, nullptr);
        }

        std::string getCPPString(JNIEnv* env, jstring js)
        {
            const char* bytes = env->GetStringUTFChars(js, 0);
            std::string msg = std::string(bytes);
            env->ReleaseStringUTFChars(js, bytes);
            return msg;
        }
    }
}
