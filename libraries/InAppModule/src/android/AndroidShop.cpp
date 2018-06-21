#include <aex/AndroidShop.hpp>
#include <aex/JniObject.hpp>
#include "AndroidBridge.hpp"


namespace aex {

        aex::JniObject getBridgeClass() {
        aex::Jni& jni = aex::Jni::get();
        aex::JniClass& cls = jni.getClass("com/aexolgl/inapps/InAppsBridge");
        return aex::JniObject(&cls, nullptr);
    }

    AndroidShop::AndroidShop() {

    }

    void AndroidShop::buyProduct(const std::string& productName, float qaunity, IAPCallback* callback) {

        AndroidInAppBridge::GetInstance().addCallback(productName, callback);
        getBridgeClass().staticCallVoid("purchuaseProduct", productName, qaunity);
    }

    void AndroidShop::setDeveloperPayload(const std::string& payload) {

    }

    void AndroidShop::setPublicKey(const std::string& key) {

    }

    std::unique_ptr<AndroidInAppBridge>& AndroidInAppBridgeInstance() {
        static std::unique_ptr<AndroidInAppBridge> instance_;
        return instance_;
    }

    AndroidInAppBridge&
    AndroidInAppBridge::GetInstance() {
        static std::once_flag only_one;

        std::call_once(only_one,
                []() {
                    AndroidInAppBridgeInstance().reset(new AndroidInAppBridge());
                }
        );
        return *AndroidInAppBridgeInstance();
    }

    void AndroidInAppBridge::addCallback(const std::string& name, IAPCallback* callback) {
        m_callbacks.emplace(name, callback);
    }

    void AndroidInAppBridge::removeCallback(const std::string& name) {
        if (m_callbacks.find(name) != m_callbacks.end()) {
            m_callbacks.erase(m_callbacks.find(name));
        }
    }

    void AndroidInAppBridge::removeLastCallback() {

    }

    IAPCallback* AndroidInAppBridge::getCallback(const std::string& name) {

        aex::write("getCallback");
        if (name == "") {
            if (m_callbacks.size() == 1) {
                IAPCallback* ret = m_callbacks.begin()->second;
                m_callbacks.clear();
                aex::write("return ret;[0]");
                return ret;
            }
        }

        if (m_callbacks.find(name) != m_callbacks.end()) {
            IAPCallback* ret = m_callbacks.at(name);
            m_callbacks.erase(m_callbacks.find(name));
            aex::write("return ret;find(name)");
            return ret;
        } else {

            if (m_callbacks.size() == 1) {
                IAPCallback* ret = m_callbacks.begin()->second;
                m_callbacks.clear();
                aex::write("return ret;[0]");
                return ret;
            }

            return nullptr;
        }
    }

    void AndroidInAppBridge::SetLastCallback(IAPCallback* lastCallback) {
        m_lastCallback = lastCallback;
    }

    IAPCallback* AndroidInAppBridge::GetLastCallback() const {
        return m_lastCallback;
    }
}

std::string convertToCppString(JNIEnv* env, jstring jstr) {
    jboolean iscopy = true;
    aex::write("const char *");
    const char *nativeString = env->GetStringUTFChars(jstr, &iscopy);
    std::string ret="";
    aex::write(" if (strlen");
    if (strlen(nativeString) > 0) {
        ret = std::string(nativeString, strlen(nativeString));
        env->ReleaseStringUTFChars(jstr, nativeString);
    } else {
        aex::write(" ret = null ");
        ret = "";
    }
    return ret;
}

JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_productSubscribedNative
(JNIEnv* env, jclass cls, jstring jstr) {

    aex::IAPCallback* callback = aex::AndroidInAppBridge::GetInstance().getCallback(convertToCppString(env, jstr));
    if (callback != nullptr) {
        aex::write("callback->producSubscribed();");
        callback->productSubscribed();
    }
}

JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_productPurchasedNative
(JNIEnv* env, jclass cls, jstring jstr) {

    aex::IAPCallback* callback = aex::AndroidInAppBridge::GetInstance().getCallback(convertToCppString(env, jstr));
    if (callback != nullptr) {
        aex::write("callback->productPurchased();");
        callback->productPurchased();
    }
}

JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_productRestoredNative
(JNIEnv* env, jclass cls, jstring jstr) {
    aex::IAPCallback* callback = aex::AndroidInAppBridge::GetInstance().getCallback(convertToCppString(env, jstr));
    if (callback != nullptr) {
        aex::write("callback->productRestored();");
        callback->productRestored();
    }
}

JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_inventoryQueryNative
(JNIEnv*, jclass, jstring jstr) {

}

JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_purchaseFailedNative
(JNIEnv* env, jclass cls, jstring jstr) {
    aex::IAPCallback* callback = aex::AndroidInAppBridge::GetInstance().getCallback(convertToCppString(env, jstr));
    if (callback != nullptr) {
         aex::write("callback->purchaseFailed();");
        callback->purchaseFailed();
    }
}

JNIEXPORT void JNICALL Java_com_aexolgl_inapps_InAppsBridge_purchaseCanceledNative
(JNIEnv* env, jclass cls, jstring jstr) {
    aex::IAPCallback* callback = aex::AndroidInAppBridge::GetInstance().getCallback(convertToCppString(env, jstr));
    if (callback != nullptr) {
        aex::write("callback->purchaseCanceled();");
        callback->purchaseCanceled();
    }
}
