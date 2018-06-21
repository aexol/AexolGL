#include "AdInterface.hpp"


namespace aex {
    namespace ads{
        class AEX_PUBLIC_API IAds : public AdInterface {
        public:
            static IAds& GetInstance();
            ~IAds();
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
            virtual void addTestingDevice(std::string deviceId);
            virtual void update(double elapsedTime);
            virtual void endBanner();
            virtual void endFullScreen();
            void setWindowForOverlay(void* window);
            
        private:
            IAds();
            IAds(const IAds& rs) = delete;
            IAds& operator=(const IAds& rs) = delete;

        };
    }
}
