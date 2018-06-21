#import "iAdsIos.hh"
#import "OverlayViewIOS.hh"
#import "iAdsImplementation/BannerViewController.h"
#import "iAdsImplementation/InterstitialViewController.hh"

namespace aex {
    namespace ads {
        IAds::IAds() : m_interstitialController(NULL) {
        }

        IAds::~IAds() {
            //[(id) m_interstitialController dealloc];
        }

        void IAds::endBanner() {
        }

        void IAds::endFullScreen() {
        }

        bool IAds::init() {
            m_interstitialController = [[InterstitialViewController alloc] init];
            [(id) m_interstitialController setOverlay : m_overlay];
            m_intialized = true;
            return true;
        }

        void IAds::setWindowForOverlay(void* window) {
            m_overlay = [[OverlayViewIOS alloc] initWithSDLWindow : static_cast<SDL_Window*> (window)];
        }

        void IAds::loadBanner() {

        }

        void IAds::showBanner() {
        }

        void IAds::showFullScreen() {
            if (m_adsEnabled) {

                if (!m_intialized) {
                    init();
                }
                [(id) m_interstitialController showFullScreenAd];
            }

        }

        void IAds::loadFullScreen() {
            if (m_adsEnabled) {
                if (!m_intialized) {
                    init();
                }
                [(id) m_interstitialController loadFullScreenAd];
            }
        }

        void IAds::setBannerUnitID() {
        }

        void IAds::setInterstitialUnitID(const std::string& id) {

        }

        void IAds::addTestingDevice(std::string deviceId) {
        }

        void IAds::loadedFullScreen() {

            m_interstitialAdLoaded=true;
            if (m_onInterstitialLoadFunc) {
                m_onInterstitialLoadFunc();
            }
        }

        void IAds::shownFullScreen() {
            m_interstitialAdLoaded=false;
            if (m_onInterstitialShowFunc) {
                m_onInterstitialCloseFunc();
            }
        }

        void IAds::closedFullScreen() {
            if (m_onInterstitialCloseFunc) {
                m_onInterstitialCloseFunc();
            }
        }

        void IAds::setTesting(bool testing) {
        }

        void IAds::update(double elapsedTime) {
            if (m_showAds) {
                if (m_interstitialAdShown) {
                    m_interstitialUpTime += elapsedTime;
                } else if (m_bannerAdShown) {
                    m_bannerUpTime += elapsedTime;
                }

            }

        }

        std::unique_ptr<IAds>& iAdsInstance() {
            static std::unique_ptr<IAds> instance_;
            return instance_;
        }

        IAds&
        IAds::GetInstance() {
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

@implementation IadsCallback

- (void) setOnLoadDelegate {

}

-(void) setonCloseDelegate {

}

-(void) onLoadCallback {
    aex::ads::IAds::GetInstance().loadedFullScreen();
}

-(void) onCloseCallback {
    aex::ads::IAds::GetInstance().closedFullScreen();
}

-(void) onShowCallback;
{
    aex::ads::IAds::GetInstance().shownFullScreen();
}

@end
