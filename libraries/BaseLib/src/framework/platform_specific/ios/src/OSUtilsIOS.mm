#include <aex/os/OSUtils.hpp>
#import "../include/IOSUtils.hh"

namespace aex
{
    namespace os
    {
        void showErrorMsgDialog(const std::string& msg)
        {
            // TODO implement with ios UTILS maeybeye ?
        }

        void killApp()
        {
          // TODO implement this line of code
        }

        void showAppInStore(const std::string& packageName)
        {
           [IOSUtils openITunesURL : packageName];
        }

        bool showYesOrNoDialog(const std::string& title, const std::string& message, const std::string& yesBttnTxt, const std::string& noBttnTxt)
        {
            // TODO
            return false;
        }

        void doInBackground(const std::function<void()>& func)
        {
            //dispatch_queue_t myQueue = dispatch_queue_create("unique_queue_name_backGround", NULL);
            //dispatch_async( myQueue, ^{
            dispatch_async( dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
              func();
              //   dispatch_async( dispatch_get_main_queue(), ^{
              //       // Add code here to update the UI/send notifications based on the
              //       // results of the background processing
              // });
            });
        }

        int getNumberOfCores()
        {
          return [[NSProcessInfo processInfo] activeProcessorCount];
        }

        std::string getUniqueDeviceID()
        {
          NSString *uniqueIdentifier = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
          std::string uniqueIdStr([uniqueIdentifier UTF8String]);
          return std::string("iOS-"+uniqueIdStr);
        }

        void openURL(const std::string& url)
        {
            [IOSUtils openURL : url];
        }

        void MoreApps::openMoreApps(const std::string& developerName)
        {
            [IOSUtils openAppStoreURL : developerName];
        }

        void AppSettings::setSleepMode(bool value)
        {
            [IOSUtils setSleepMode : value];
        }

        Locale::LANGUAGE Locale::getLanguage() {
            return Locale::LANGUAGE::ENGLISH;
        }

        bool Permissions::checkCameraPermissions()
        {
            return [IOSUtils checkCameraPermissions];
        }

        bool Permissions::checkAudioPermissions()
        {
            return [IOSUtils checkAudioPermissions];
        }

        bool Permissions::checkPhotoPermissions()
        {
            return [IOSUtils checkPhotoPermissions];
        }

        bool Permissions::showGoToSettingsAlert(const std::string& message, const std::string& okButtonText, const std::string& cancelButtonText)
        {
            return [IOSUtils showGoToSettingsAllert:message :okButtonText :cancelButtonText];
        }
    }
}
