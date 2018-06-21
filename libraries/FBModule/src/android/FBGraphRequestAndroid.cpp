#include "aex/fb/FBGraphRequest.hpp"
#include <json/json.h>
#include "FBBridge.hpp"
#include "JNIUtils.hpp"
#include <queue>
#include <functional>
#include <aex/Logger.hpp>


JNIEXPORT void JNICALL Java_com_aexolgl_android_fb_FBBridge_fbRequestGETResultCallback
  (JNIEnv *env, jclass clazz, jint code, jstring jsonResult, jstring errMsg, jlong cbAddress)
  {

      int resultCodeCpp = code;
      std::string jsonResultCpp = aex::fb::getCPPString(env, jsonResult);
      std::string errorMsgCpp = aex::fb::getCPPString(env, errMsg);
      if( cbAddress )
      {
          aex::fb::FBGraphRequest::NativeCallback *nativeCb = (aex::fb::FBGraphRequest::NativeCallback*)cbAddress;
          (*nativeCb)(resultCodeCpp, jsonResultCpp, errorMsgCpp);
          delete nativeCb;
      }
  }

JNIEXPORT void JNICALL Java_com_aexolgl_android_fb_FBBridge_fbRequestDELETEResultCallback
  (JNIEnv *env, jclass clazz, jint trueOrFalse, jstring errMsg, jlong cbAddress)
  {
      int resultCodeCpp = trueOrFalse;
      std::string errorMsgCpp = aex::fb::getCPPString(env, errMsg);
      if( cbAddress )
      {
          std::function<void(bool success, const std::string& errMsg)> *nativeCb = (std::function<void(bool success, const std::string& errMsg)>*)cbAddress;
          (*nativeCb)((bool)resultCodeCpp, errorMsgCpp);
          delete nativeCb;
      }
  }

namespace aex
{
    namespace fb
    {
        void FBGraphRequest::nativeRequestGET(
            const std::string& graphPath,
            const std::map<std::string,std::string>& parameters,
            const NativeCallback& nativeCb)
        {
            Json::Value jsonParams;
            for(const auto& elem : parameters)
            {
                jsonParams[elem.first] = elem.second;
            }

            Json::FastWriter writer;

            std::string jsonStr = writer.write(jsonParams);
            NativeCallback* stupdCb = new NativeCallback(nativeCb);
            getJniClass("com/aexolgl/android/fb/FBBridge").staticCallVoid("graphRequestGET", graphPath, jsonStr, (int64_t)stupdCb);
        }

        void FBGraphRequest::nativeRequestDELETE(const std::string& graphPath, const std::function<void(bool success, const std::string& errMsg)>& callback)
        {
            std::function<void(bool success, const std::string& errMsg)>* stupdCb =
                new std::function<void(bool success, const std::string& errMsg)>(callback);

            getJniClass("com/aexolgl/android/fb/FBBridge").staticCallVoid("graphRequestDELETE", graphPath, (int64_t)stupdCb);
        }
    }
}
