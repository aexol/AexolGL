/*
 * File:   AdMob.cpp
 * Author: mritke
 *
 * Created on 3 sierpnia 2015, 11:57
 */

#include <aex/ads/AdMob.hpp>

namespace aex{
    namespace ads{
        std::unique_ptr<AdMob>& adMobInstance()
        {
            static std::unique_ptr<AdMob> instance_;
            return instance_;
        }

        AdMob&
        AdMob::GetInstance()
        {
            static std::once_flag only_one;

            std::call_once(only_one,
                    []()
                    {
                        adMobInstance().reset(new AdMob());
                        adMobInstance()->init();
                    }
            );

            return *adMobInstance();
        }



        void AdMob::update(double elapsedTime)
        {
            if(m_showAds)
            {
                if(m_interstitialAdShown)
                {
                    m_interstitialUpTime+=elapsedTime;
                }
                else if(m_bannerAdShown)
                {
                    m_bannerUpTime+=elapsedTime;
                }

            }

        }
    }
}
