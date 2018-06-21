#include <aex/os/OSUtils.hpp>

#ifdef __linux__
  #include <unistd.h>
  #include <stdlib.h>
  void launch(const std::string &url)
  {
    std::string browser = getenv("BROWSER");
    if(browser == "") return;

    char *args[3];
    args[0] = (char*)browser.c_str();
    args[1] = (char*)url.c_str();
    args[2] = 0;

    pid_t pid = fork();
    if(!pid)
      execvp(browser.c_str(), args);
  }
#elif __APPLE__
  #include "TargetConditionals.h"
  #if TARGET_OS_MAC
    #include <unistd.h>
    #include <stdlib.h>
    void launch(const std::string &url)
    {
      char *args[3];
      args[0] = "open";
      args[1] = (char*)url.c_str();
      args[2] = 0;

      pid_t pid = fork();
      if(!pid)
        execvp("open", args);
    }
  #else
    void launch(const std::string &url){}
  #endif
#else
  void launch(const std::string &url){}
#endif

namespace aex {
    namespace os {

        void showErrorMsgDialog(const std::string& msg)
        {
        }

        void killApp()
        {
        }

        void showAppInStore(const std::string& packageName){
        }

        bool showYesOrNoDialog(const std::string& title, const std::string& message, const std::string& yesBttnTxt, const std::string& noBttnTxt)
        {
            return false;
        }

        void doInBackground(const std::function<void()>& func)
        {
        }

        int getNumberOfCores()
        {
          return 1;
        }

        std::string getUniqueDeviceID()
        {
            return "dummy-device-id-1234567890";
        }

        void openURL(const std::string& url)
        {
            launch(url);
        }

        void MoreApps::openMoreApps(const std::string& developerName) {
          launch(developerName);
        }

        void AppSettings::setSleepMode(bool value) {
        }

        Locale::LANGUAGE Locale::getLanguage() {
            return Locale::LANGUAGE::ENGLISH;
        }

        bool Permissions::checkCameraPermissions() {
            return true;
        }

        bool Permissions::checkAudioPermissions() {
            return true;
        }

        bool Permissions::checkPhotoPermissions() {
            return true;
        }

        bool Permissions::showGoToSettingsAlert(const std::string& message, const std::string& okButtonText, const std::string& cancelButtonText) {
            return true;
        }
    }
}
