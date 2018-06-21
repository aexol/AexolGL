/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <aex/ads/AdMob.hpp>

namespace aex{
    namespace ads{
        void AdInterface::resetInterstitial() 
        {
            m_interstitialAdRequested = false;
            m_interstitialAdShown = false;
            m_interstitialAdLoaded = false;
            m_interstitiaShowInRequest = false;
        }

        bool AdInterface::isShowAds() {
            return m_showAds;
        }

        void AdInterface::setShowAds(bool value) {
            m_showAds = value;
        }

        bool AdInterface::isInterstitialAdShown() const {
            return m_interstitialAdShown;
        }

        bool AdInterface::isInterstitialAdRequested() const {
            return m_interstitialAdRequested;
        }

        void AdInterface::setAdBannerUnitID(const std::string& adBannerUnitID) {
            m_adBannerUnitID = adBannerUnitID;
        }

        std::string AdInterface::getAdBannerUnitID() const {
            return m_adBannerUnitID;
        }

        void AdInterface::setAdInterstitialUnitID(const std::string& adInterstitialUnitID) {
            m_adInterstitialUnitID = adInterstitialUnitID;
        }

        std::string AdInterface::getAdInterstitialUnitID() const {
            return m_adInterstitialUnitID;
        }

        void AdInterface::disableAds(){
            m_adsEnabled=false;
        }

        void AdInterface::enableAds() {
            m_adsEnabled=true;
        }

        bool AdInterface::isInterstitialAdLoaded() const {
            return m_interstitialAdLoaded;
        }


        bool AdInterface::isBannerAdLoaded() const {
             return m_bannerAdLoaded;
        }

        bool AdInterface::isInterstitialShowInRequest() const {
            return m_interstitiaShowInRequest;
        }
    }
}
