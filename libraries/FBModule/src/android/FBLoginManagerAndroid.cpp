#include "aex/fb/FBLoginManager.hpp"
#include "FBBridge.hpp"
#include "JNIUtils.hpp"
#include <queue>
#include <functional>
#include <aex/Logger.hpp>

JNIEXPORT void JNICALL Java_com_aexolgl_android_fb_FBBridge_fbLoginResultCallback
    (JNIEnv * env, jclass clazz, jint code, jstring joinedDeclinedPerms, jstring joinedGrantedPerms, jstring errorMsg, jlong cbAddress)
{
    int resultCodeCpp = code;
    std::string joinedDeclinedPermsCpp = aex::fb::getCPPString(env, joinedDeclinedPerms);
    std::string joinedGrantedPermsCpp = aex::fb::getCPPString(env, joinedGrantedPerms);
    std::string errorMsgCpp = aex::fb::getCPPString(env, errorMsg);

    if( cbAddress )
    {
        aex::fb::FBLoginManager::NativeCallback *nativeCb = (aex::fb::FBLoginManager::NativeCallback*)cbAddress;
        (*nativeCb)(resultCodeCpp, joinedDeclinedPermsCpp, joinedGrantedPermsCpp, errorMsgCpp);
        delete nativeCb;
    }
}

namespace aex
{
    namespace fb
    {
        void FBLoginManager::nativeLogin(const NativeCallback& nativeCb)
        {
            // will be deleted after cb invocation (15 lines above)
            NativeCallback* stupdCb = new NativeCallback(nativeCb);
            getJniClass("com/aexolgl/android/fb/FBBridge").staticCallVoid("openLoginActivity", getPermissionsJoined(), (int64_t)stupdCb);
        }

        bool FBLoginManager::nativeIsLogged() const
        {
            int result = 0;
            result = getJniClass("com/aexolgl/android/fb/FBBridge").staticCall<int>("isLogged", result);

            if(result != 0)
                return true;
            else
                return false;
        }
    }
}
