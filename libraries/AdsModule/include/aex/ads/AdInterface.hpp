/*
 * File:   AdInterface.hpp
 * Author: mritke
 *
 * Created on 12 sierpnia 2015, 11:30
 */

#ifndef ADINTERFACE_HPP
#define ADINTERFACE_HPP

#include <aex/Common.hpp>
#include <string>
#include <memory>
#include <mutex>
#include <functional>


namespace aex {
    namespace ads{
        class AEX_PUBLIC_API AdInterface {

        public:

            AdInterface() {
            }
            virtual ~AdInterface() = 0;

            virtual bool init() = 0;
            virtual void loadBanner() = 0;
            virtual void showBanner() = 0;
            virtual void endBanner() = 0;
            virtual void showFullScreen() = 0;
            virtual void loadFullScreen() = 0;
            virtual void endFullScreen() = 0;
            virtual void closedFullScreen() = 0;
            virtual void loadedFullScreen() = 0;
            virtual void shownFullScreen() = 0;
            virtual void failedToLoadFullScreen() = 0;
            virtual void closedBanner() = 0;
            virtual void loadedBanner() = 0;
            virtual void shownBanner() = 0;
            virtual void setBannerUnitID() = 0;
            virtual void setTesting(bool state) = 0;
            virtual void setInterstitialUnitID(const std::string& id) = 0;
            virtual void addTestingDevice(std::string deviceId) = 0;
            virtual void update(double elapsedTime) = 0;


            void resetInterstitial();

            bool isShowAds();
            void setShowAds(bool value);
            bool isInterstitialAdShown() const;
            bool isInterstitialAdRequested() const;
            virtual void setAdBannerUnitID(const std::string& adBannerUnitID);
            std::string getAdBannerUnitID() const;
            void setAdInterstitialUnitID(const std::string& adInterstitialUnitID);
            std::string getAdInterstitialUnitID() const;
            void disableAds();
            void enableAds();
            bool isInterstitialAdLoaded() const;
            bool isBannerAdLoaded() const;
            bool isInterstitialShowInRequest() const;


            template<class T>
            void setOnInterstitialLoadFunc(T func);

            template<class T>
            void setOnInterstitialFailLoadFunc(T func);

            template<class T>
            void setOnBannerLoadFunc(T func);

            template<class T>
            void setOnBannerFailLoadFunc(T func);

            template<class T>
            void setOnInterstitialShowFunc(T func);

            template<class T>
            void setOnBannerShowFunc(T func);

            template<class T>
            void setOnInterstitialCloseFunc(T func);

            template<class T>
            void setOnBannerCloseFunc(T func);

            template<class T>
            void setOnInterstitialLoadFunc(T* obj, void (T::*memFn)());

            template<class T>
            void setOnInterstitialFailLoadFunc(T* obj, void (T::*memFn)());

            template<class T>
            void setOnBannerLoadFunc(T* obj, void (T::*memFn)());

            template<class T>
            void setOnBannerFailLoadFunc(T* obj, void (T::*memFn)());

            template<class T>
            void setOnInterstitialShowFunc(T* obj, void (T::*memFn)());

            template<class T>
            void setOnBannerShowFunc(T* obj, void (T::*memFn)());

            template<class T>
            void setOnInterstitialCloseFunc(T* obj, void (T::*memFn)());

            template<class T>
            void setOnBannerCloseFunc(T* obj, void (T::*memFn)());
            
            
        protected:
            std::string m_adInterstitialUnitID = "";
            std::string m_adBannerUnitID = "";

            bool m_interstitialAdRequested = false;
            bool m_interstitialAdShown = false;
            bool m_interstitialAdLoaded = false;

            bool m_interstitiaShowInRequest = false;

            bool m_bannerAdRequested = false;
            bool m_bannerAdShown = false;
            bool m_bannerAdLoaded = false;

            bool m_showAds = true;
            bool m_adsEnabled = true;

            double m_bannerStartTime = 0.0;
            double m_bannerUpTime = 0.0;
            double m_lastBannerEndTime = 0.0;

            double m_interstitialbannerStartTime = 0.0;
            double m_interstitialUpTime = 0.0;
            double m_lastInterstitialEndTime = 0.0;


            std::function<void() > m_onInterstitialLoadFunc;
            std::function<void() > m_onBannerLoadFunc;

            std::function<void() > m_onInterstitialFailLoadFunc;
            std::function<void() > m_onBannerFailLoadFunc;

            std::function<void() > m_onInterstitialShowFunc;
            std::function<void() > m_onBannerShowFunc;

            std::function<void() > m_onInterstitialCloseFunc;
            std::function<void() > m_onBannerCloseFunc;

        };

        inline AdInterface::~AdInterface() {
        }

        template<class T>
        void AdInterface::setOnInterstitialLoadFunc(T func) {
            m_onInterstitialLoadFunc = func;
        }

        template<class T>
        void AdInterface::setOnInterstitialFailLoadFunc(T func) {
            m_onInterstitialFailLoadFunc = func;
        }

        template<class T>
        void AdInterface::setOnBannerLoadFunc(T func) {
            m_onBannerLoadFunc = func;
        }

        template<class T>
        void AdInterface::setOnBannerFailLoadFunc(T func) {
            m_onBannerFailLoadFunc = func;
        }

        template<class T>
        void AdInterface::setOnInterstitialShowFunc(T func) {
            m_onInterstitialShowFunc = func;
        }

        template<class T>
        void AdInterface::setOnBannerShowFunc(T func) {
            m_onBannerShowFunc = func;
        }

        template<class T>
        void AdInterface::setOnInterstitialCloseFunc(T func) {
            m_onInterstitialCloseFunc = func;
        }

        template<class T>
        void AdInterface::setOnBannerCloseFunc(T func) {
            m_onBannerCloseFunc = func;
        }

        template<class T>
        void AdInterface::setOnInterstitialLoadFunc(T* obj, void (T::*memFn)()) {
            m_onInterstitialLoadFunc = std::bind(memFn, obj);
        };

        template<class T>
        void AdInterface::setOnInterstitialFailLoadFunc(T* obj, void (T::*memFn)()) {
            m_onInterstitialFailLoadFunc = std::bind(memFn, obj);
        }

        template<class T>
        void AdInterface::setOnBannerLoadFunc(T* obj, void (T::*memFn)()) {
            m_onBannerLoadFunc = std::bind(memFn, obj);
        };

        template<class T>
        void AdInterface::setOnBannerFailLoadFunc(T* obj, void (T::*memFn)()) {
            m_onBannerFailLoadFunc = std::bind(memFn, obj);
        }

        template<class T>
        void AdInterface::setOnInterstitialShowFunc(T* obj, void (T::*memFn)()) {
            m_onInterstitialShowFunc = std::bind(memFn, obj);
        };

        template<class T>
        void AdInterface::setOnBannerShowFunc(T* obj, void (T::*memFn)()) {
            m_onBannerShowFunc = std::bind(memFn, obj);
        };

        template<class T>
        void AdInterface::setOnInterstitialCloseFunc(T* obj, void (T::*memFn)()) {
            m_onInterstitialCloseFunc = std::bind(memFn, obj);
        };

        template<class T>
        void AdInterface::setOnBannerCloseFunc(T* obj, void (T::*memFn)()) {
            m_onBannerCloseFunc = std::bind(memFn, obj);
        }
        
        

    }
}
#endif /* ADINTERFACE_HPP */
