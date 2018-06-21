/* 
 * File:   AndroidBridge.hpp
 * Author: mritke
 *
 * Created on 2 września 2015, 16:04
 */


#ifndef ANDROIDBRIDGE_HPP
#define	ANDROIDBRIDGE_HPP


#include <android/log.h>
#include <jni.h>
#include <aex/IAPCallback.hpp>
#include <aex/Logger.hpp>

#ifdef __cplusplus
extern "C" {
#endif
    JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_productPurchasedNative
    (JNIEnv* env, jclass cls, jstring jstr);

    JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_productRestoredNative
    (JNIEnv* env, jclass cls, jstring jstr);

    JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_inventoryQueryNative
    (JNIEnv* env, jclass cls, jstring jstr);

    JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_purchaseFailedNative
    (JNIEnv* env, jclass cls, jstring jstr);

    JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_purchaseCanceledNative
    (JNIEnv* env, jclass cls, jstring jstr);



#ifdef __cplusplus
}
#endif


namespace aex {

    class AndroidInAppBridge {
    private:
        IAPCallback* m_lastCallback = nullptr;
        std::map<std::string, IAPCallback*> m_callbacks;

        AndroidInAppBridge() {
        };
        AndroidInAppBridge(const AndroidInAppBridge& rs) = delete;
        AndroidInAppBridge& operator=(const AndroidInAppBridge& rs) = delete;

    public:
        static AndroidInAppBridge& GetInstance();

        ~AndroidInAppBridge() {
        };

        void addCallback(const std::string& name, IAPCallback* callback);
        IAPCallback* getCallback(const std::string& name);
        void removeCallback(const std::string& name);
        void removeLastCallback();
        void SetLastCallback(IAPCallback* lastCallback);
        IAPCallback* GetLastCallback() const;
    };

    void
    write(const std::string& msg) {
#if __TARGET_PLATFORM__ == __ANDROID_OS__
        
        LOG(msg.c_str());
#else
        std::cout << msg << std::endl;
#endif
    }
}

#endif	/* ANDROIDBRIDGE_HPP */

