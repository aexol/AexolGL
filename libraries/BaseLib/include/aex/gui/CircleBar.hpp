#ifndef CIRCLE_BAR_AEX_GUI_HEADER
#define CIRCLE_BAR_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"
#include "CircleBarMesh.hpp"

namespace aex {
    namespace gui {

        /**
         * @brief Cricle progress bar.
         */
        class AEX_PUBLIC_API CircleBar : public Component {
        public:
            /**
             * @brief Create new CircleBar
             *
             * @param area Position.
             * @param color Progress circle color.
             * @param backColor Backgroun color.
             * @param thickness Border thicknes.
             * @param percent Current progress in percents.
             * @param startPercent Starting percent.
             */
            CircleBar(const Area& area,
                    const Color& color,
                    const Color& backColor,
                    float thickness,
                    float percent,
                    float startPercent = 0.0f);
            virtual ~CircleBar();

            /**
             * @brief Change starting percent.
             *
             * @param p Starting percent.
             */
            void setStartPercent(float p);

            /**
             * @brief Return current progress.
             *
             * @return Current progress.
             */
            float getPercent() const;

            /**
             * @brief Return starting percent.
             *
             * @return Starting percent.
             */
            float getStartPercent() const;

            /**
             * @brief Set circle color.
             *
             * @param c New color.
             */
            void setBarColor(const Color& c);

            /**
             * @brief Set background color.
             *
             * @param c New color.
             */
            void setBackgroundColor(const Color& c);

            /**
             * @brief Update progres.
             *
             * @param percent0_to_100 New progress.
             */
            void setPercent(float percent0_to_100);

            /**
             * @brief Set blurr on last percent.
             *
             * @param percent0_to_1 New blurr.
             */
            void setBlurrPercent(float percent0_to_1);

            /**
             * @brief Get current blur.
             *
             * @return Current blur.
             */
            float getBlurrPercent() const;

            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
        protected:
            virtual void refresh(int flags);
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);

            float m_percent; // 0 to 100 values
            float m_startPercent;
            std::unique_ptr<CircleBarMesh> m_barMesh;
        };

        inline float CircleBar::getPercent() const {
            return m_percent;
        }

        inline float CircleBar::getStartPercent() const {
            return m_startPercent;
        }
    }
}

#endif
