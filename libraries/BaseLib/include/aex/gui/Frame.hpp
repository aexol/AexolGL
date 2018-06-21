#ifndef FRAME_AEX_GUI_HEADER
#define FRAME_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Input.hpp"
#include <aex/ALoopNode.hpp>
#include <aex/DeviceOrientationObservator.hpp>
#include <aex/AexRenderTree.hpp>
#include <aex/tween/TweenPlayer.hpp>


namespace aex {
    namespace gui {

        class AEX_PUBLIC_API Frame : public ALoopNode, public DeviceOrientationObservator, public TouchInterface, public InputInterface, public MouseInterface {

        protected:
            Frame(aex::AWindow* window, aex::shared_ptr<aex::AexRenderTree> renderTreeRoot);
        public:
            friend class RootPanel;

            virtual ~Frame();
            // ALoop callback's (Event loop invocations)
            virtual bool init();
            virtual void pause();
            virtual void resume();
            virtual void update();
            virtual void finish();

            // TouchInterface
            virtual void touchDown(float x, float y, int fingerId);
            virtual void touchMotion(float x, float y, float dx, float dy, int fingerId);
            virtual void touchUp(float x, float y, int fingerId);
            virtual void touchLine(float x1, float y1, float x2, float y2, int fingerId);
            virtual void touchClick(float x, float y, int fingerId);
            virtual void touchScale(float scale);

            // InputInterface
            virtual void key_down_event(KeyboardEventData keyEvent);
            virtual void key_up_event(KeyboardEventData keyEvent);
            virtual void key_textedit_event(TextEditEventData textEditEvent);
            virtual void key_textinput_event(TextInputEventData textInputEvent);

            // MouseInterface
            virtual void mouse_down_event(MouseButtonEventData e);
            virtual void mouse_up_event(MouseButtonEventData e);
            virtual void mouse_motion_event(MouseMotionEventData e);
            virtual void mouse_wheel_event(MouseWheelEventData e);

            virtual void orientationChanged(aex::ORIENTATION orientation);
            void keyboardHided();

            void simulateTouchClick(const Position& p, int fingerId = 0);

            std::unique_ptr<TouchEventLayer> getTouchEventLayer();
            std::unique_ptr<KeyboardEventLayer> getKeyboardEventLayer();
            std::unique_ptr<MouseEventLayer> getMouseEventLayer();
            std::unique_ptr<MouseEventLayer> getMouseAsTouchEventLayer();



        public:
            aex::shared_ptr<aex::AexRenderTree> getRenderTreeRoot();
            void setMainPanel(Panel* panel);
            AWindow* getWindow();
            tween::TweenPlayer* getTweenPlayer();
        protected:
            bool isTouchFocused() const;
            bool isInputFocused() const;
            const ComponentPtr getTouchFocused() const;
            const ComponentPtr getInputFocused() const;

            // static:
        public:
            static std::unique_ptr<Frame> instanceInit(aex::AWindow* window, aex::shared_ptr<aex::AexRenderTree> renderTreeRoot);
            static Frame* instance();
            static int32_t getWidthPixels();
            static int32_t getHeightPixels();
            static float getWidthNormal();
            static float getHeightNormal();
            static Area getFrameArea();
            static float pix2Norm(int32_t pix);
            static float pix2Norm(int32_t pix,const GSize2D<int>& mockupSize);
            static int32_t norm2Pix(float norm);
            static int32_t norm2Pix(float norm, const GSize2D<int>& mockupSize);
            static float x2N(float x);
            static float y2N(float y);
            static float n2x(float x);
            static float n2y(float y);
            static float w2Pix(float width);
            static float h2Pix(float height);
            static ComponentPtr getGlobalTouchFocused();
            static ComponentPtr getGlobalInputFocused();
            static double timestampSeconds();

        protected:
            static aex::AWindow* g_window;
            static Frame* g_frame;

            // Event Handling
            int m_firstFingerID;
            ComponentPtr m_touchFocused;
            ComponentPtr m_inputFocused;

            // GUI
            aex::shared_ptr<RootPanel> m_rootPanel;
            aex::ObjectRenderNode m_visual;
            aex::AWindow* m_window;
            aex::shared_ptr<aex::AexRenderTree> m_renderTreeRoot;
            Area m_frameArea;
            
            tween::TweenPlayer m_tweenPlayer;
            double m_timestamp;
            
        };


        enum AEX_PUBLIC_API SCALE_PLANE
        {
          HORIZONTAL,
          VERTICAL,
          SHORTER,
          LONGER
        };

        class AEX_PUBLIC_API Scaler{
        public:
          Scaler(GSize2D<int> mockupSize, SCALE_PLANE plane);
          float pix(int pix) const;
        protected:
          GSize2D<int> m_realSize;
          GSize2D<int> m_mockupSize;
          SCALE_PLANE m_scalePlane;
        };
    }
}

#endif
