#ifndef HUD_CONTAINER
#define HUD_CONTAINER

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include <aex/AexolGL.hpp>
#include <aex/Common.hpp>
#include <aex/HudElement.hpp>
#include <aex/Hud.hpp>

#include <aex/Area.hpp>

namespace aex {
    namespace gui {
        
        class AEX_PUBLIC_API HudContainer {
        public:
            typedef aex::shared_ptr<HudContainer> HudContainerPtr;
            
        public:
            HudContainer();
            HudContainer(HudElementPtr hudElem);
            
            static HudContainerPtr readFromJSON(const aex::string& jsonPath, Hud_ptr hud);
            HudContainerPtr getContainerBy(const aex::string& name);
            HudContainerPtr getContainerBy(const HudElementPtr& hudElem);

            bool empty() const;
            void add(const HudContainerPtr& child);
            
            void scale(float scaleX, float scaleY, bool centerX, bool centerY);
            void move(float moveX, float moveY);
            
            
        private:
            void scaleByParent(const NormalArea& oldParentArea, const NormalArea& newParentArea, float scaleX, float scaleY);
            void moveByParent(float moveX, float moveY);
            NormalArea getHudElementArea(HudElementPtr hudEl);
            void setHudElementUsingArea(HudElementPtr hudEl, const NormalArea& area);
            aex::string getHudElemName() const;
            
        private:
            HudElementPtr hudElem;
            std::vector<HudContainerPtr> childs;
        };
        
        
        
        class AEX_PUBLIC_API MoveHudContainerAnimation : public aex::ALoop{
        public:
            MoveHudContainerAnimation(HudContainer& hudContainer);
            virtual ~MoveHudContainerAnimation();

            void startAnimation(const std::function<void()>& onEndCb = []() {});
            bool isAnimating() const;
            void setVectorShiftAndTime(float dX, float dY, float seconds);
            void step();

            // ALoop interface
            virtual bool init();
            virtual void pause();
            virtual void resume();
            virtual void update();
            virtual void finish();

        private:
            HudContainer& m_hudContainer;
            bool m_bIsAnimating;
            float m_deltaX;
            float m_deltaY;
            long long unsigned m_animTimeMills;
            long long unsigned m_lastTimestamp;
            float m_tmpSeconds;
            std::function<void() > m_onAnimationEndCb;
            long long unsigned m_startTimestamp;
        };
    }
}

#endif // HUD_CONTAINER
