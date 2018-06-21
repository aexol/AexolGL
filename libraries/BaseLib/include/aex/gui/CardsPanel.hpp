#ifndef CARDS_PANEL_AEX_GUI_HEADER
#define CARDS_PANEL_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Panel.hpp"
#include "TextureComponent.hpp"
#include <aex/FrameBuffer.hpp>
#include <aex/Easing.hpp>

namespace aex {
    namespace gui {

        /**
         * @brief Allows grouping components in cards.
         *
         * @details This panel allows grouping components in cards.
         * Card is premade set of components. On this panel only one
         * card can be visible at once.
         */
        class AEX_PUBLIC_API CardsPanel : public Panel {
        public:
            /**
             * @brief Animation direction.
             */
            enum class SIDE {
                LEFT,
                UP,
                RIGHT,
                DOWN
            };

            /**
             * @brief New CardsPanel object.
             *
             * @param area CardsPanel position.
             * @param swappingTimeSec Swap animation time.
             * @param fboWidthPix Framebuffer width for swap animation.
             * @param fboHeightPix Framebuffer height for swap animation.
             */
            CardsPanel(const Area& area,
                    float swappingTimeSec = 1.0f,
                    int fboWidthPix = 512,
                    int fboHeightPix = 512);
            virtual ~CardsPanel();

            /**
             * @brief Swap card.
             *
             * @details Begins card swap animation.
             */
            void next();

            /**
             * @brief Swap card.
             *
             * @details Begins card swap animation.
             *
             * @param onMoved Execute this action when move finished.
             */
            void next(const std::function<void()>& onMoved);

            /**
             * @brief Get currently displayed card, if any.
             *
             * @return Pointer to currently displayed card.
             */
            ComponentPtr getCurrentVisible() const;

            /**
             * @brief Set card to display.
             *
             * @id Card ID.
             */
            void setCurrentVisible(uint id);

            /**
             * @brief Set animation direction.
             *
             * @param side Direction.
             */
            void setSide(SIDE side);

            /**
             * @brief Reset panel state.
             *
             * @details Resets panel state to initial state.
             */
            void reset();

            virtual void addComponent(ComponentPtr component);
            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
            
        protected:
            virtual void refresh(int flags);
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);

            unsigned m_currentCardID;
            float m_swappingTimeSec;
            bool m_showNext;
            double m_timestamp;
            float m_currentDelta;
            std::function<void() > m_onMovedAction;

            FrameBufferOGL m_fbo1;
            FrameBufferOGL m_fbo2;
            bool m_viewsBuffered;
            ComponentPtr m_firstComponent;
            ComponentPtr m_secondComponent;
            TextureComponent m_firstTex;
            TextureComponent m_secondTex;
            bool m_movingFinished;
            aex::math::Easings::EasingWrap<float> m_easing;
            SIDE m_side;
        };
    }
}

#endif
