
#include<aex/ads/AdMob.hpp>

namespace aex
{
    namespace ads{
        bool AdMob::init() { return false;}

        void AdMob::loadBanner() { }

        void AdMob::showBanner() { }

        void AdMob::showFullScreen() { }

        void AdMob::loadFullScreen() { }

        void AdMob::setBannerUnitID() { }

        void AdMob::setInterstitialUnitID(const std::string& id) { }

        void AdMob::addTestingDevice(std::string deviceId) { }

        void AdMob::loadedFullScreen() { }

        void AdMob::closedFullScreen() { }
 
        void AdMob::shownFullScreen() { }

        void AdMob::failedToLoadFullScreen() {}

        void AdMob::setAdBannerUnitID(const std::string& adBannerUnitID) {}

        void AdMob::setTesting(bool) { }

        void AdMob::endBanner() {}

        void AdMob::endFullScreen() {}

        void AdMob::closedBanner()
        {
        }

        void AdMob::loadedBanner()
        {
        }

        void AdMob::shownBanner()
        {
        }

        std::string getDeviceId()
        {
            return "DUMMY_ID";
        }
    }
}
