#ifndef JNIUTILS_AEXOL_HEADER_FILE
#define JNIUTILS_AEXOL_HEADER_FILE

#include <aex/JniObject.hpp>
#include <jni.h>
#include <string>

namespace aex
{
    namespace hdw
    {
        JniObject getJniClass(const std::string& classDesc);
        std::string getCPPString(JNIEnv* env, jstring js);
    }
}


#endif
