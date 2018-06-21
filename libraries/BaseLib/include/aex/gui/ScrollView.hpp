#ifndef SCROLL_VIEW_AEX_GUI_HEADER
#define SCROLL_VIEW_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Panel.hpp"

namespace aex {
    namespace gui {
        class AEX_PUBLIC_API ScrollView : public Panel {
        public:
            static const float SCROLL_SAMPLE_TIME_S;
            static const float SCROLL_EPSILON;
            ScrollView();
            virtual ~ScrollView();

            virtual void draw(AexRenderer* gl);
            virtual void finishdraw(AexRenderer* gl);
            void setFriction(float friction);
            void setScroll(float percent0_100);

        protected:
            virtual void touchDown(float x, float y, int fingerId);
            virtual void touchMotion(float x, float y, float dx, float dy, int fingerId);
            virtual void touchUp(float x, float y, int fingerId);
            virtual void touchLine(float x1, float y1, float x2, float y2, int fingerId);
            virtual void touchClick(float x, float y, int fingerId);
            virtual void touchScale(float scale);

            virtual void focusCatched(FOCUS_TYPE type);
            virtual void focusReleased(FOCUS_TYPE type);

            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            virtual void refresh(int flags);
        protected:
            void calcContentSize();
            const Size& getContentSize() const;

            Position getMaxLeft() const;
            Position getMinLeft() const;
            void findVisibleIDIfNeeded();
            void calcSpeed(double deltaTime, Position deltaVec);
            ComponentPtr resendFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);

            Position m_downPos;
            Position m_delta;
            Position m_lastDelta;
            Position m_speedPerSecond;
            Position m_friction;
            float m_frictionMultiplier;
            Size m_contentSize;
            int m_fingerID;
            int m_visibleID;
            std::vector<Position> m_basePositions;
            std::vector<int> m_elemsToDraw;
            double m_drawTime;
            Position m_lastInnerPosition;
            uint m_lastSize;

            double m_touchTimestamp;
            double m_upTimestamp;
            double m_motionTimestamp;
            double m_sampleTimestamp;
            Position m_touchPoint;
            Position m_upPoint;
            Position m_motionPoint;
            Position m_samplePoint;
            float m_maxMotionDelta;
            bool m_touchFocused;
            std::function<void(ComponentPtr) > m_focusMeAction;
            Position m_focusPoint;

            std::map<ComponentPtr, Size> m_sizesMap;
            bool m_recalcBasePositions;
        };
        typedef ScrollView ScrollPanel;
    }
}

#endif
