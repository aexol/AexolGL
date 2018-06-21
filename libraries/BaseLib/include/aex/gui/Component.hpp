#ifndef COMPONENT_AEX_GUI_HEADER
#define COMPONENT_AEX_GUI_HEADER

#include <aex/AexDrawable.hpp>
#include "Input.hpp"
#include "GUIMath.hpp"
#include "Color.hpp"
#include "Flags.hpp"

namespace aex {
    namespace gui {

        enum class FOCUS_TYPE {
            NONE,
            TOUCH,
            INPUT
        };

        enum DEBUG_RECT {
            HITBOX,
            NORMAL,
            INNER,
            OUTER
        };

        typedef flags::FLAG FLAG;

        /**
         * @brief Abstract base for all GUI components.
         *
         * @details All GUI elements must inherit from this
         * class.
         */
        class AEX_PUBLIC_API Component : public DrawObject,
            public TouchInterface,
            public InputInterface,
            public MouseInterface 
        {
            public:
                friend class Panel;
                friend class Frame;
                friend class ScrollView;
                friend class LabeledComponent;
                Component();
                Component(const Area& area);
                Component(const Area& area, const Interspace& margin,
                        const Interspace& padding);
                virtual ~Component();

                /**
                 * @brief Set component margin.
                 *
                 * @param margin New margin.
                 */
                void setMargin(const Interspace& margin);

                /**
                 * @brief Set component padding
                 *
                 * @param padding New padding.
                 */
                void setPadding(const Interspace& padding);

                /**
                 * @brief Set component area.
                 *
                 * @param area New area.
                 */
                void setArea(const Area& area);

                /**
                 * @brief Set component hithobx.
                 *
                 * @details Allows user to resize and move component hitbox relative
                 * to it's position.
                 *
                 * @param scale Hitbox scale.
                 * @param movement Hitbox offset.
                 */
                void setHitbox(const Scale& scale,
                        const Position& movement = Position(0, 0));

                /**
                 * @brief Move component.
                 *
                 * @param p New position.
                 */
                void setPosition(const Position& p);

                /**
                 * @brief Resize component.
                 *
                 * @param p New size.
                 */
                void setSize(const Size& s);

                /**
                 * @brief Copy position and size of component.
                 *
                 * @details Copies component size, position, padding, margin
                 * and hitbox settings.
                 *
                 * @param comp Component to be copied.
                 */
                void setUsing(const Component& comp);
                /**
                 * @brief Clear all falgs.
                 */
                void clearFlags();

                /**
                 * @brief Set component flag.
                 *
                 * @param flag aex::gui::flags::FLAG enum
                 */
                void setFlag(FLAG flag);

                /**
                 * @brief Set component flag.
                 *
                 * @param flag New flag.
                 */
                void setFlag(int flag);

                /**
                 * @brief Check if flag is set.
                 *
                 * @param flag Tests if this flag is set.
                 * @param inFlags Set of flags to test.
                 *
                 * @return True if set.
                 */
                bool checkFlag(FLAG flag, int inFlags);

                /**
                 * @brief Check if flag is set.
                 *
                 * @param flag Tests if this flag is set.
                 * @param inFlags Set of flags to test.
                 *
                 * @return True if set.
                 */
                bool checkFlag(int flag, int inFlags);

                /**
                 * @brief Return component flags.
                 *
                 * @return Flags.
                 */
                int getFlags() const;

                /**
                 * @brief Force component refresh.
                 */
                void forceRefresh();

                /**
                 * @brief Get component margins.
                 *
                 * @return Margin.
                 */
                const Interspace& getMargin() const;

                /**
                 * @brief Get component padding.
                 *
                 * @return Padding.
                 */
                const Interspace& getPadding() const;

                /**
                 * @brief Get component position and size.
                 *
                 * @return Area.
                 */
                const Area& getArea() const;

                /**
                 * @brief Should background color be visible.
                 *
                 * @details Each component can have a background color
                 * which is not visible by default.
                 *
                 * @param visible True if background color should be visible.
                 */
                void setBackgroundVisible(bool visible);

                /**
                 * @brief Background color visibility.
                 *
                 * @return Visibility.
                 */
                bool isBackgroundVisible() const;

                /**
                 * @brief Background color.
                 *
                 * @return Color.
                 */
                void setBackgroundColor(const Color& color);

                /**
                 * @brief Get component hitbox scale and offset.
                 *
                 * @return Hitbox.
                 */
                Area getHitbox() const;

                /**
                 * @brief Was this component hit by last touch event.
                 *
                 * @return True if yes.
                 */
                bool isTouchFocus() const;

                /**
                 * @brief Was this component hit by last touch or clickevent.
                 *
                 * @return True if yes.
                 */
                bool isInputFocus() const;

                /**
                 * @brief Is component visible.
                 *
                 * @return True if yes.
                 */
                bool isVisible() const;

                /**
                 * @brief Set component visibility.
                 *
                 * @param value New visibility.
                 */
                void setVisible(bool value);

                /**
                 * @brief Get component unique ID.
                 *
                 * @return ID.
                 */
                long long getID() const;

                /**
                 * @brief Add new listener for touch event.
                 *
                 * @param listener New listener.
                 */
                void addTouchActionListener(TouchActionListener* listener);

                /**
                 * @brief Draw background color.
                 *
                 * @param gl Rendering context.
                 */
                void drawBackground(AexRenderer* gl);

                // Virtual methods.
                
                virtual void setDebugRect(DEBUG_RECT type, bool visible);
                virtual void setDebugColor(DEBUG_RECT type, const Color& color);
                virtual void onRefresh();
                virtual Area getInnerArea() const;
                virtual Area getOuterArea() const;
                virtual bool isHitted(const Position& point) const;
                virtual bool isFocusableFor(FOCUS_TYPE type) const;
                virtual void focusCatched(FOCUS_TYPE type);
                virtual void focusReleased(FOCUS_TYPE type);

            protected:
                /**
                 * @biref Pure virtual action to take on component refresh.
                 */
                virtual void refresh(int flags) = 0; // for gui system

                // TouchInterface
                void base_touchDown(float x, float y, int fingerId);
                void base_touchMotion(float x, float y, float dx, float dy, int fingerId);
                void base_touchUp(float x, float y, int fingerId);
                void base_touchLine(float x1, float y1, float x2, float y2, int fingerId);
                void base_touchClick(float x, float y, int fingerId);
                void base_touchScale(float scale);

                virtual void touchDown(float x, float y, int fingerId);
                virtual void touchMotion(float x, float y, float dx,
                        float dy, int fingerId);
                virtual void touchUp(float x, float y, int fingerId);
                virtual void touchLine(float x1, float y1, float x2, float y2,
                        int fingerId);
                virtual void touchClick(float x, float y, int fingerId);
                virtual void touchScale(float scale);

                // InputInterface
                void base_key_down_event(KeyboardEventData keyEvent);
                void base_key_up_event(KeyboardEventData keyEvent);
                void base_key_textedit_event(TextEditEventData textEditEvent);
                void base_key_textinput_event(TextInputEventData textInputEvent);

                virtual void key_down_event(KeyboardEventData keyEvent);
                virtual void key_up_event(KeyboardEventData keyEvent);
                virtual void key_textedit_event(TextEditEventData textEditEvent);
                virtual void key_textinput_event(TextInputEventData textInputEvent);

                // MouseInterface
                void base_mouse_down_event(MouseButtonEventData e);
                void base_mouse_up_event(MouseButtonEventData e);
                void base_mouse_motion_event(MouseMotionEventData e);
                void base_mouse_wheel_event(MouseWheelEventData e);

                virtual void mouse_down_event(MouseButtonEventData e);
                virtual void mouse_up_event(MouseButtonEventData e);
                virtual void mouse_motion_event(MouseMotionEventData e);
                virtual void mouse_wheel_event(MouseWheelEventData e);

                virtual ComponentPtr setFocus(const Position& point,
                        FOCUS_TYPE type,
                        const std::function<void(ComponentPtr)>& focusMe) = 0;
                void recalcAreas();
                void resetBackground();
                bool needRefresh() const;
                bool sizeChanged() const;
                void setRefresh(bool value);

                Area m_area;
                Area m_outerArea;
                Area m_innerArea;
                Interspace m_margin;
                Interspace m_padding;
                Position m_deltaHitbox;
                Scale m_scaleHitbox;
                bool m_bBackground;
                ColorRectComponentUPtr m_background;
                Color m_backgroundColor;
                bool m_visible;
                bool m_needUpdate;
                int m_flags;
                long long m_id;
                std::vector<TouchActionListener*> m_touchActionListeners;
            private:
                static long long GLOBAL_ID_COUNTER;
        };

        inline bool Component::needRefresh() const
        {
            return m_needUpdate;
        }
        inline void Component::touchDown(float x, float y, int fingerId) {
        }

        inline void Component::touchMotion(float x, float y, float dx, float dy, int fingerId) {
        }

        inline void Component::touchUp(float x, float y, int fingerId) {
        }

        inline void Component::touchLine(float x1, float y1, float x2, float y2, int fingerId) {
        }

        inline void Component::touchClick(float x, float y, int fingerId) {
        }

        inline void Component::touchScale(float scale) {
        }

        inline void Component::key_down_event(KeyboardEventData keyEvent) {
        }

        inline void Component::key_up_event(KeyboardEventData keyEvent) {
        }

        inline void Component::key_textedit_event(TextEditEventData textEditEvent) {
        }

        inline void Component::key_textinput_event(TextInputEventData textInputEvent) {
        }

        inline void Component::mouse_down_event(MouseButtonEventData e) {
            UNUSED(e);
        }

        inline void Component::mouse_up_event(MouseButtonEventData e) {
            UNUSED(e);
        }

        inline void Component::mouse_motion_event(MouseMotionEventData e) {
            UNUSED(e);
        }

        inline void Component::mouse_wheel_event(MouseWheelEventData e) {
            UNUSED(e);
        }
        inline void Component::setDebugRect(DEBUG_RECT type, bool visible) {
        }

        inline void Component::setDebugColor(DEBUG_RECT type, const Color& color) {
        }

        inline void Component::onRefresh() {
        }
    }
}

#endif
