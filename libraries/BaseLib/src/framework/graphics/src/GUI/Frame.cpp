#include "aex/gui/Frame.hpp"
#include "../../../include/internal_sdl.hpp"
#include "aex/gui/RootPanel.hpp"
#include <aex/AWindow.hpp>
#include <aex/Exceptions.hpp>
#include <aex/BuildInEvents.hpp>
#include "aex/gui/GUIMath.hpp"

namespace aex
{
    namespace gui
    {
        aex::AWindow *Frame::g_window = nullptr;
        Frame *Frame::g_frame = nullptr; // make this just instance (singleton)
        Frame::Frame(aex::AWindow *window, aex::shared_ptr<aex::AexRenderTree> renderTreeRoot) : m_touchFocused(nullptr),
        m_inputFocused(nullptr),
        m_window(window),
        m_renderTreeRoot(renderTreeRoot)
        {
            m_frameArea = getFrameArea();
            // GUI root set
            m_rootPanel = aex::make_shared<RootPanel>();
            m_rootPanel->setArea(getFrameArea());

            m_visual << m_rootPanel;
            m_visual.setRootRenderNode(getRenderTreeRoot()->getRenderRoot());
            m_visual.startDrawing();
        }

        Frame::~Frame()
        {
        }

        aex::shared_ptr<aex::AexRenderTree> Frame::getRenderTreeRoot()
        {
            return m_renderTreeRoot;
        }

        void Frame::setMainPanel(Panel *panel)
        {
            m_rootPanel->clear();
            panel->setArea(Frame::getFrameArea());
            m_rootPanel->addComponent(panel);
        }

        AWindow *Frame::getWindow()
        {
            return m_window;
        }

        tween::TweenPlayer *Frame::getTweenPlayer()
        {
            return &m_tweenPlayer;
        }

        bool Frame::init()
        {
            m_timestamp = Frame::timestampSeconds();
            return true;
        }
        void Frame::pause()
        {
        }
        void Frame::resume()
        {
            m_timestamp = Frame::timestampSeconds();
        }
        void Frame::update()
        {
            double tmpTimestamp = Frame::timestampSeconds();
            m_tweenPlayer.step(tmpTimestamp - m_timestamp);
            m_timestamp = tmpTimestamp;
            if (!SDL_IsTextInputActive())
            {
                keyboardHided();
            }
        }

        void Frame::finish()
        {
            m_done = true;
        }

        void Frame::touchDown(float x, float y, int fingerId)
        {
            x *= m_frameArea.s().w();
            y *= m_frameArea.s().h();
            if (!isTouchFocused())
            {
                m_rootPanel->setFocus(Position(x, y), FOCUS_TYPE::TOUCH, [this](ComponentPtr comp) {
                    if (isTouchFocused())
                    {
                        m_touchFocused->focusReleased(FOCUS_TYPE::TOUCH); // TODO check it
                        m_touchFocused = nullptr;
                    }
                    m_touchFocused = comp;
                    comp->focusCatched(FOCUS_TYPE::TOUCH);
                });
                m_firstFingerID = fingerId;
            }

            if (isTouchFocused())
            {
                m_touchFocused->base_touchDown(x, y, fingerId);
            }
        }

        void Frame::touchMotion(float x, float y, float dx, float dy, int fingerId)
        {
            x *= m_frameArea.s().w();
            y *= m_frameArea.s().h();
            dx *= m_frameArea.s().w();
            dy *= m_frameArea.s().h();

            if (isTouchFocused())
            {
                m_touchFocused->base_touchMotion(x, y, dx, dy, fingerId);
            }
        }

        void Frame::touchUp(float x, float y, int fingerId)
        {
            x *= m_frameArea.s().w();
            y *= m_frameArea.s().h();
            if (isTouchFocused())
            {
                m_touchFocused->base_touchUp(x, y, fingerId);
                if (fingerId == m_firstFingerID)
                {
                    m_touchFocused->focusReleased(FOCUS_TYPE::TOUCH);
                    m_touchFocused = nullptr;
                }
            }
        }

        void Frame::touchLine(float x1, float y1, float x2, float y2, int fingerId)
        {
            x1 *= m_frameArea.s().w();
            y1 *= m_frameArea.s().h();
            x2 *= m_frameArea.s().w();
            y2 *= m_frameArea.s().h();

            if (isTouchFocused())
            {
                m_touchFocused->base_touchLine(x1, y1, x2, y2, fingerId);
            }
        }

        void Frame::touchClick(float x, float y, int fingerId)
        {
            x *= m_frameArea.s().w();
            y *= m_frameArea.s().h();
            if (isTouchFocused())
            {
                m_touchFocused->base_touchClick(x, y, fingerId);
            }

            ComponentPtr success = m_rootPanel->setFocus(Position(x, y), FOCUS_TYPE::INPUT, [this](ComponentPtr comp) {
                if (m_inputFocused != comp)
                {
                    keyboardHided();

                    m_inputFocused = comp;
                    m_inputFocused->focusCatched(FOCUS_TYPE::INPUT);
                }
            });
            if (success != nullptr)
            {
                // Move all content panels
                float keyboardHeight = 0.45f;
                SDL_StartTextInput();
                float focY = m_inputFocused->getArea().leftDown().y();
                if (focY > getHeightNormal() * (1.0f - keyboardHeight))
                {
                    float len = getHeightNormal() - focY;
                    float delta = -(getHeightNormal() * keyboardHeight - len);
                    Position oldPosition = m_rootPanel->getArea().p();
                    Position vector = Position(0.0f, delta);
                    m_rootPanel->setPosition(oldPosition + vector);
                }
            }
            else
            {
                keyboardHided();
                SDL_StopTextInput();
            }
        }

        void Frame::touchScale(float scale)
        {
            if (isTouchFocused())
            {
                m_touchFocused->base_touchScale(scale);
            }
        }

        void Frame::key_down_event(KeyboardEventData e)
        {
            if (isInputFocused())
            {
                m_inputFocused->base_key_down_event(e);
                if (e.scancode == AKEY_AC_BACK)
                {
                    m_inputFocused->focusReleased(FOCUS_TYPE::INPUT);
                    m_inputFocused = nullptr;
                }
            }
        }

        void Frame::key_up_event(KeyboardEventData e)
        {
            if (isInputFocused())
            {
                m_inputFocused->base_key_up_event(e);
                if (e.scancode == AKEY_AC_BACK)
                {
                    m_inputFocused->focusReleased(FOCUS_TYPE::INPUT);
                    m_inputFocused = nullptr;
                }
            }
        }

        void Frame::key_textedit_event(TextEditEventData e)
        {
            if (isInputFocused())
            {
                m_inputFocused->base_key_textedit_event(e);
            }
        }

        void Frame::key_textinput_event(TextInputEventData e)
        {
            if (isInputFocused())
            {
                m_inputFocused->base_key_textinput_event(e);
            }
        }

        void Frame::mouse_down_event(MouseButtonEventData e)
        {
            // TODO implement
        }

        void Frame::mouse_up_event(MouseButtonEventData e)
        {
            // TODO implement
        }

        void Frame::mouse_motion_event(MouseMotionEventData e)
        {
            // TODO implement
        }

        void Frame::mouse_wheel_event(MouseWheelEventData e)
        {
            // TODO implement
        }

        void Frame::orientationChanged(aex::ORIENTATION orientation)
        {
            // TODO implement
        }

        void Frame::keyboardHided()
        {
            if (isInputFocused())
            {
                m_inputFocused->focusReleased(FOCUS_TYPE::INPUT);
                m_inputFocused = nullptr;
                m_rootPanel->setPosition(Position(0, 0));
            }
        }

        void Frame::simulateTouchClick(const Position &p, int fingerId)
        {
            float cartesianX = p.x() / m_frameArea.s().w();
            float cartesianY = p.y() / m_frameArea.s().h();
            touchClick(cartesianX, cartesianY, fingerId);
        }

        std::unique_ptr<TouchEventLayer> Frame::getTouchEventLayer()
        {
            return std::unique_ptr<TouchEventLayer>(new GUITouchEvent(this));
        }

        std::unique_ptr<KeyboardEventLayer> Frame::getKeyboardEventLayer()
        {
            return std::unique_ptr<KeyboardEventLayer>(new GUIKeyboardEvent(this));
        }

        std::unique_ptr<MouseEventLayer> Frame::getMouseEventLayer()
        {
            return std::unique_ptr<MouseEventLayer>(new GUIMouseEvent(this));
        }

        std::unique_ptr<MouseEventLayer> Frame::getMouseAsTouchEventLayer()
        {
            return std::unique_ptr<MouseEventLayer>(new GUIMouseAsTouchEvent(this, getWidthPixels(), getHeightPixels()));
        }

        bool Frame::isTouchFocused() const
        {
            return m_touchFocused != nullptr;
        }

        bool Frame::isInputFocused() const
        {
            return m_inputFocused != nullptr;
        }

        const ComponentPtr Frame::getTouchFocused() const
        {
            return m_touchFocused;
        }
        const ComponentPtr Frame::getInputFocused() const
        {
            return m_inputFocused;
        }

        std::unique_ptr<Frame> Frame::instanceInit(aex::AWindow *window, aex::shared_ptr<aex::AexRenderTree> renderTreeRoot)
        {
            if (g_window == nullptr)
            {
                g_window = window;
                if (g_frame == nullptr)
                {
                    std::unique_ptr<Frame> frameUPtr(new Frame(window, renderTreeRoot));
                    g_frame = frameUPtr.get();
                    return frameUPtr;
                }
            }
            return nullptr;
        }

        Frame *Frame::instance()
        {
            return g_frame;
        }

        int32_t Frame::getWidthPixels()
        {
            return g_window->w();
        }

        int32_t Frame::getHeightPixels()
        {
            return g_window->h();
        }

        float Frame::getWidthNormal()
        {
            if (getWidthPixels() < getHeightPixels())
            {
                return 1.0f;
            }
            else
            {
                return getWidthPixels() / (float)getHeightPixels();
            }
        }

        float Frame::getHeightNormal()
        {
            if (getHeightPixels() < getWidthPixels())
            {
                return 1.0f;
            }
            else
            {
                return getHeightPixels() / (float)getWidthPixels();
            }
        }

        Area Frame::getFrameArea()
        {
            Area area(Position(0, 0), Size(getWidthNormal(), getHeightNormal()));
            return area;
        }

        float Frame::pix2Norm(int32_t pix)
        {
            float smaller = std::min(Frame::getWidthPixels(), Frame::getHeightPixels());
            return pix / smaller;
        }

        float Frame::pix2Norm(int32_t pix, const GSize2D<int> &mockupSize)
        {
            float smaller = std::min(mockupSize.w(), mockupSize.h());
            return pix / smaller;
        }

        int32_t Frame::norm2Pix(float norm)
        {
            return norm * std::min(Frame::getWidthPixels(), Frame::getHeightPixels());
        }

        int32_t Frame::norm2Pix(float norm, const GSize2D<int> &mockupSize)
        {
            return norm * std::min(mockupSize.w(), mockupSize.h());
        }

        float Frame::x2N(float x)
        {
            return x / getWidthNormal();
        }

        float Frame::y2N(float y)
        {
            return y / getHeightNormal();
        }

        float Frame::n2x(float x)
        {
            return x * getWidthNormal();
        }

        float Frame::n2y(float y)
        {
            return y * getHeightNormal();
        }

        float Frame::w2Pix(float width)
        {
            return (width / Frame::getWidthNormal()) * Frame::getWidthPixels();
        }

        float Frame::h2Pix(float height)
        {
            return (height / Frame::getHeightNormal()) * Frame::getHeightPixels();
        }

        ComponentPtr Frame::getGlobalTouchFocused()
        {
            if (g_frame != nullptr)
                return g_frame->getTouchFocused();
            return nullptr;
        }

        ComponentPtr Frame::getGlobalInputFocused()
        {
            if (g_frame != nullptr)
                return g_frame->getInputFocused();
            return nullptr;
        }

        double Frame::timestampSeconds()
        {
            auto duration = std::chrono::system_clock::now().time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            return millis / 1000.0;
        }

        Scaler::Scaler(GSize2D<int> mockupSize, SCALE_PLANE plane) :
        m_realSize(Frame::getWidthPixels(), Frame::getHeightPixels()),
        m_mockupSize(mockupSize),
        m_scalePlane(plane)
        {
        }

        float Scaler::pix(int pix) const
        {
            float normVal = 0;
            switch (m_scalePlane)
            {
            case SCALE_PLANE::HORIZONTAL:
                pix = (pix / (float)m_mockupSize.w()) * m_realSize.w();
                normVal = ((float)pix / m_realSize.w()) * Frame::getWidthNormal();
                break;
            case SCALE_PLANE::VERTICAL:
                pix = (pix / (float)m_mockupSize.h()) * m_realSize.h();
                normVal = ((float)pix / m_realSize.h()) * Frame::getHeightNormal();
                break;
            case SCALE_PLANE::LONGER:
            {
                if (Frame::getWidthNormal() > 1.0)
                {
                    pix = (pix / (float)m_mockupSize.w()) * m_realSize.w();
                    normVal = ((float)pix / m_realSize.w()) * Frame::getWidthNormal();
                }
                else
                {
                    pix = (pix / (float)m_mockupSize.h()) * m_realSize.h();
                    normVal = ((float)pix / m_realSize.h()) * Frame::getHeightNormal();
                }
            }
            break;

            case SCALE_PLANE::SHORTER:
            default:
            {
                if (Frame::getHeightNormal() > 1.0)
                {
                    pix = (pix / (float)m_mockupSize.w()) * m_realSize.w();
                    normVal = ((float)pix / m_realSize.w()) * Frame::getWidthNormal();
                }
                else
                {
                    pix = (pix / (float)m_mockupSize.h()) * m_realSize.h();
                    normVal = ((float)pix / m_realSize.h()) * Frame::getHeightNormal();
                }
            }
            break;
            }
            return normVal;
        }
    }
}
