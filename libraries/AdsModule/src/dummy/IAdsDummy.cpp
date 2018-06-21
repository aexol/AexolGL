#include<aex/ads/iAds.hpp>
namespace aex {
    namespace ads{
        IAds::IAds()
        {
        }

        IAds::~IAds()
        {
        }

        bool IAds::init()
        {
            return true;
        }

        void IAds::loadFullScreen()
        {
        }

        void IAds::endFullScreen()
        {
        }

        void IAds::endBanner()
        {
        }

        void IAds::loadBanner()
        {
        }

        void IAds::showBanner()
        {
        }

        void IAds::showFullScreen()
        {
        }

        void IAds::setWindowForOverlay(void* window)
        {
        }

        void IAds::setBannerUnitID()
        {
        }

        void IAds::setInterstitialUnitID(const std::string& id)
        {
        }

        void IAds::addTestingDevice(std::string deviceId)
        {
        }

        void IAds::loadedFullScreen()
        {
        }

        void IAds::closedFullScreen()
        {
        }

        void IAds::shownFullScreen()
        {
        }

        void IAds::closedBanner()
        {
        }

        void IAds::loadedBanner()
        {
        }

        void IAds::shownBanner()
        {
        }

        void IAds::update(double elapsedtime)
        {
        }

        void IAds::setTesting(bool istesting)
        {
        }

        void IAds::failedToLoadFullScreen()
        {

        }

        std::unique_ptr<IAds>& iAdsInstance()
        {
            static std::unique_ptr<IAds> instance_;
            return instance_;
        }

        IAds&
        IAds::GetInstance()
        {
            static std::once_flag only_one;

            std::call_once(only_one,
                    []() {
                        iAdsInstance().reset(new IAds());
                    }
            );

            return *iAdsInstance();
        }
    }
}
