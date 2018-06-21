/*
 * File:   AdMob.hpp
 * Author: mritke
 *
 * Created on June 23, 2015, 12:47 PM
 */

#ifndef ADMOB_HEADER_HPP
#define	ADMOB_HEADER_HPP

#include <aex/Common.hpp>
#include "AdInterface.hpp"
#include <string>
#include <memory>
#include <mutex>

namespace aex {
    namespace ads{
        class AEX_PUBLIC_API AdMob : public AdInterface
        {
        private:

            AdMob(const AdMob& rs) = delete;
            AdMob& operator=(const AdMob& rs) = delete;

        protected:

            AdMob(){}

        public:

            static AdMob& GetInstance();

            virtual bool init();
            virtual void loadBanner();
            virtual void showBanner();
            virtual void showFullScreen();
            virtual void loadFullScreen();
            virtual void closedFullScreen();
            virtual void loadedFullScreen();
            virtual void shownFullScreen();
            virtual void failedToLoadFullScreen();
            virtual void closedBanner();
            virtual void loadedBanner();
            virtual void shownBanner();
            virtual void setBannerUnitID();
            virtual void setTesting(bool state);
            virtual void setInterstitialUnitID(const std::string& id);
            virtual void setAdBannerUnitID(const std::string& adBannerUnitID);
            virtual void addTestingDevice(std::string deviceId);
            virtual void update(double elapsedTime);
            virtual void endBanner();
            virtual void endFullScreen();



        };

        std::string getDeviceId();
    }
}

#endif	/* ADMOB_HEADER_HPP */
