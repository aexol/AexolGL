#include <aex/ads/AdMob.hpp>
#include <aex/JniObject.hpp>
#include <aex/Common.hpp>
#include <memory>
#include <sstream>
#include <aex/Logger.hpp>

namespace aex {
    namespace ads{
        namespace {
            const char* ads_package_slashy = "com/aexolgl/android/ads";
        }
        /*AdMob::AdMob()
        {
            //LOG_DEBUG(" AdMob::AdMob()");
            init();
        }*/

        JniObject getJniClass()
        {
            Jni& jni = Jni::get();
            JniClass& cls = jni.getClass(aex::string(ads_package_slashy)+"/AdMobHandler");
            return JniObject(&cls, nullptr);
        }

        bool AdMob::init()
        {

            auto activity = GetAndroidActivityAsActivity();
            getJniClass().staticCallVoid("initRequest", activity);

    #ifndef NDEBUG
            setTesting(true);
            LOG("AdMob::init::setTesting: true.");
    #else
            setTesting(false);
            LOG("AdMob::init::setTesting: false.");
    #endif
            return true;
        }

        void AdMob::loadBanner()
        {
            auto activity = GetAndroidActivityAsActivity();
            getJniClass().staticCallVoid("loadBannerAd", activity);
        }

        void AdMob::showBanner()
        {
            //LOG_DEBUG("showBanner");
            auto activity = GetAndroidActivityAsActivity();
            getJniClass().staticCallVoid("showBannerAd", activity);

        }

        void AdMob::loadFullScreen()
        {
            //LOG_DEBUG("loadFullScreen");

            if (m_adsEnabled)
            {
                auto activity = GetAndroidActivityAsActivity();
                getJniClass().staticCallVoid("loadInterstitialAd", activity);
                m_interstitialAdRequested = true;
            }
        }

        void AdMob::setTesting(bool state)
        {
            //LOG_DEBUG("setTesting: ",state);
            getJniClass().staticCallVoid("setTesting", state);
        }

        void AdMob::addTestingDevice(std::string deviceId)
        {
            //LOG_DEBUG("addTestDeviceId: ", deviceId);
            getJniClass().staticCallVoid("addTestDeviceId", deviceId);
        }

        void AdMob::closedFullScreen()
        {
            m_interstitialAdShown = false;
            m_interstitialAdLoaded = false;
            m_interstitialAdRequested = false;
            m_interstitiaShowInRequest = false;

            if (m_onInterstitialCloseFunc)
            {
                m_onInterstitialCloseFunc();
            }
        }

        void AdMob::loadedFullScreen()
        {
            m_interstitialAdRequested = false;
            m_interstitialAdLoaded = true;

            if (m_onInterstitialLoadFunc)
            {
                m_onInterstitialLoadFunc();
            }
        }

        void AdMob::shownFullScreen()
        {

            m_interstitialAdShown = true;
            m_interstitiaShowInRequest = false;
            if (m_onInterstitialShowFunc)
            {
                m_onInterstitialCloseFunc();
            }
        }

        void AdMob::failedToLoadFullScreen()
        {
            resetInterstitial();

            if (m_onInterstitialFailLoadFunc)
            {
                m_onInterstitialFailLoadFunc();
            }
        }

        void AdMob::closedBanner()
        {
            m_bannerAdShown = false;
            m_bannerAdLoaded = false;

            if (m_onBannerCloseFunc)
            {
                m_onBannerCloseFunc();
            }
        }

        void AdMob::loadedBanner()
        {
            m_bannerAdRequested = false;
            m_bannerAdLoaded = true;

            if (m_onBannerLoadFunc)
            {
                m_onBannerLoadFunc();
            }
        }

        void AdMob::shownBanner()
        {
            m_bannerAdShown = true;

            if (m_onBannerShowFunc)
            {
                m_onBannerShowFunc();
            }
        }

        void AdMob::showFullScreen()
        {
            if (!m_interstitiaShowInRequest)
            {
                auto activity = GetAndroidActivityAsActivity();
                m_interstitiaShowInRequest = true;
                getJniClass().staticCallVoid("showInterstitialAd", activity);
            }
        }

        void AdMob::endBanner()
        {

        }

        void AdMob::endFullScreen()
        {

        }

        void AdMob::setInterstitialUnitID(const std::string& id)
        {
            // LOG_DEBUG("setInterstitialUnitID");
            m_adInterstitialUnitID = id;
            getJniClass().staticCallVoid("setInterstitialUnitID", m_adInterstitialUnitID);
        }

        void AdMob::setAdBannerUnitID(const std::string& adBannerUnitID)
        {
            m_adBannerUnitID = adBannerUnitID;
            setBannerUnitID();
        }

        void AdMob::setBannerUnitID()
        {
            //LOG_DEBUG("setBannerUnitID");
            getJniClass().staticCallVoid("setBannerUnitID", m_adBannerUnitID);
        }
    }
}

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_com_aexolgl_android_ads_AdMobHandler_adMobInitialized
(JNIEnv*, jclass)
{
    //LOG_DEBUG("adMobInitialized");
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_ads_AdMobHandler_nativeInterstitialAdLoaded
(JNIEnv*, jclass)
{
    aex::ads::AdMob::GetInstance().loadedFullScreen();
    //LOG_DEBUG("Java_com_aexolgl_android_ads_AdMobHandler_nativeInterstitialAdLoaded");
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_ads_AdMobHandler_nativeInterstitialAdShown
(JNIEnv*, jclass)
{
    aex::ads::AdMob::GetInstance().shownFullScreen();
    //LOG_DEBUG("Java_com_aexolgl_android_ads_AdMobHandler_nativeInterstitialAdShown");
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_ads_AdMobHandler_nativeInterstitialAdClosed
(JNIEnv*, jclass)
{
    aex::ads::AdMob::GetInstance().closedFullScreen();
    //LOG_DEBUG("Java_com_aexolgl_android_ads_AdMobHandler_nativeInterstitialAdClosed");
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_ads_AdMobHandler_nativeInterstitialFailedToLoad
(JNIEnv*, jclass)
{
    aex::ads::AdMob::GetInstance().failedToLoadFullScreen();
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_ads_AdMobHandler_nativeBannerAdLoaded
(JNIEnv*, jclass)
{
    aex::ads::AdMob::GetInstance().loadedBanner();
}

JNIEXPORT void JNICALL Java_com_aexolgl_android_ads_AdMobHandler_nativeBannerAdShown
(JNIEnv*, jclass)
{
    aex::ads::AdMob::GetInstance().shownBanner();
}
#ifdef __cplusplus
}
#endif
