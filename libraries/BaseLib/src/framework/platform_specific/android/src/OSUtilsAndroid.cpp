#include <aex/os/OSUtils.hpp>
#include <aex/JniObject.hpp>
#include <aex/Logger.hpp>

namespace aex {
    namespace os {
        namespace {
            const char* java_package_slashy = "com/aexolgl/android/app";
        }

        JniObject getOSUtilsClass() {
            Jni& jni = Jni::get();
            JniClass& cls = jni.getClass(aex::string(java_package_slashy)+"/OSUtils");
            return JniObject(&cls, nullptr);
        }

        void showErrorMsgDialog(const std::string& msg) {
            auto activity = GetAndroidActivityAsActivity();
            getOSUtilsClass().staticCallVoid("showErrorDialog", msg, activity);
        }

        void killApp()
        {
            auto activity = GetAndroidActivityAsActivity();
            getOSUtilsClass().staticCallVoid("killApp", activity);
        }

        void showAppInStore(const std::string& packageName)
        {
            auto activity = GetAndroidActivityAsActivity();
            getOSUtilsClass().staticCallVoid("showAppInStore", packageName, activity);
        }

        bool showYesOrNoDialog(const std::string& title, const std::string& message, const std::string& yesBttnTxt, const std::string& noBttnTxt)
        {
            auto activity = GetAndroidActivityAsActivity();
            int retVal;
            retVal = getOSUtilsClass().staticCall("showYesOrNoDialog", retVal, title, message, yesBttnTxt, noBttnTxt, activity);
            return retVal;
        }

        void doInBackground(const std::function<void()>& func)
        {
            std::thread t(func);
            t.detach(); // thread works without t object
        }

        int getNumberOfCores()
        {
          int retVal;
          auto activity = GetAndroidActivityAsActivity();
          retVal = getOSUtilsClass().staticCall("getNumberOfCores", retVal, activity);
          return retVal;
        }

        std::string getUniqueDeviceID()
        {
          std::string retVal;
          auto activity = GetAndroidActivityAsActivity();
          retVal = std::string("android-" + getOSUtilsClass().staticCall("getUniqueDeviceID", retVal, activity));
          return retVal;
        }

        void openURL(const std::string& url)
        {
            auto activity = GetAndroidActivityAsActivity();
            getOSUtilsClass().staticCallVoid("openURL", url, activity);
        }

        void MoreApps::openMoreApps(const std::string& developerName) {
            auto activity = GetAndroidActivityAsActivity();
            getOSUtilsClass().staticCallVoid("openMoreApps", developerName, activity);
        }

        void AppSettings::setSleepMode(bool value) {
            auto activity = GetAndroidActivityAsActivity();
            getOSUtilsClass().staticCallVoid("setSleepMode", (int)value, activity);
        }

        Locale::LANGUAGE Locale::getLanguage() {
            std::string localeStr;
            localeStr = getOSUtilsClass().staticCall<std::string>("getLocaleLanguage", localeStr);
            LOG_DEBUG("LaNgUaGe:", localeStr);

            if (localeStr == "ru")
                return Locale::LANGUAGE::RUSSIAN;
            if (localeStr == "pl")
                return Locale::LANGUAGE::POLISH;

            return Locale::LANGUAGE::ENGLISH;
        }

        bool Permissions::checkCameraPermissions() {
            LOG_DEBUG("FUCK FUCK FUCK 123");
            int retVal;
            auto activity = GetAndroidActivityAsActivity();
            retVal = getOSUtilsClass().staticCall<int>("checkCameraPermissions", retVal, activity);
            LOG_DEBUG("ret Val: ", retVal);
            return retVal != 0;
        }

        bool Permissions::checkAudioPermissions() {
            int retVal;
            auto activity = GetAndroidActivityAsActivity();
            retVal = getOSUtilsClass().staticCall<int>("checkAudioPermissions", retVal, activity);
            LOG_DEBUG("ret Val: ", retVal);
            return retVal != 0;
        }

        bool Permissions::checkPhotoPermissions() {
            int retVal;
            auto activity = GetAndroidActivityAsActivity();
            retVal = getOSUtilsClass().staticCall<int>("checkPhotoPermissions", retVal, activity);
            LOG_DEBUG("ret Val: ", retVal);
            return retVal != 0;
        }

        bool Permissions::showGoToSettingsAlert(const std::string& message, const std::string& okButtonText, const std::string& cancelButtonText) {
            auto activity = GetAndroidActivityAsActivity();
            int retVal;
            retVal = getOSUtilsClass().staticCall<int>("showGoToSettingsDialog", retVal, message, okButtonText, cancelButtonText, activity);
            LOG_DEBUG("Returned: ", retVal);
            return retVal != 0;
        }
    }
}
