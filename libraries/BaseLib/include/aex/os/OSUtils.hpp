#ifndef OSUTILS_AEX_OS_HEADER
#define OSUTILS_AEX_OS_HEADER

#include <aex/Common.hpp>
#include <string>
#include <thread>

namespace aex
{
    namespace os
    {
        void showErrorMsgDialog(const std::string& msg);
        void killApp();
        void showAppInStore(const std::string& androidPackageName);
        bool showYesOrNoDialog(const std::string& title, const std::string& message, const std::string& yesBttnTxt, const std::string& noBttnTxt);
        void doInBackground(const std::function<void()>& func);
        int  getNumberOfCores();
        std::string getUniqueDeviceID();
        void openURL(const std::string& url);

        class MoreApps {
        public:
            static void openMoreApps(const std::string& developerName);
        };

        class Locale{
        public:
            enum class LANGUAGE{
                ENGLISH = 0,
                RUSSIAN,
                POLISH
            };
            static LANGUAGE getLanguage();
        };

        class Permissions{
        public:
            static bool checkCameraPermissions();
            static bool checkAudioPermissions();
            static bool checkPhotoPermissions();
            static bool showGoToSettingsAlert(const std::string& message, const std::string& okButtonText, const std::string& cancelButtonText);
        };

        class AppSettings{
        public:
            static void setSleepMode(bool value);
        };
    }
}


#endif
