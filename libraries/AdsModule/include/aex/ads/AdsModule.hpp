/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AdsModule.hpp
 * Author: rafa
 *
 * Created on November 4, 2016, 11:30 AM
 */

#ifndef ADSMODULE_HPP
#define ADSMODULE_HPP
#include "AdInterface.hpp"
namespace aex{
    namespace ads{
        class AEX_PUBLIC_API AdModule : AdInterface
        {
        public:
            AdModule();

            bool init();
            void loadBanner();
            void showBanner();
            void endBanner();
            void showFullScreen();
            void loadFullScreen();
            void endFullScreen();
            void closedFullScreen();
            void loadedFullScreen();
            void shownFullScreen();
            void setBannerUnitID();
            void setTesting(bool state);
            void setInterstitialUnitID();
            void addTestingDevice(std::string deviceId);
            void update(double elapsedTime);
            
        private:
            AdInterface* m_impl;

        };
    }
}
#endif /* ADSMODULE_HPP */
