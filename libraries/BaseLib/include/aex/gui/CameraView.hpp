#ifndef CAMERA_VIEW_AEX_GUI_HEADER
#define CAMERA_VIEW_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"
#include "aex/Camera.hpp"

namespace aex {
    namespace gui {

        /**
         * @brief Camera GUI component.
         *
         * @details Used to render camera content into application.
         *
         */
        class AEX_PUBLIC_API CameraView : public Component {
        public:
            /**
             * @brief Create new camera view
             *
             * @param area Area in which CameraView should be rendered.
             * @param camera Camera device.
             * @param filter Filter to apply.
             * @param fullFill Fill whole component.
             */
            CameraView(const Area& area,
                    aex::cam::Camera& camera,
                    aex::cam::FILTER filter = aex::cam::FILTER::NONE,
                    bool fullFill = false);

            virtual ~CameraView();
            /**
             * @brief Set full fill mode.
             *
             * @details Enables/Disables full fill mode. In full fill
             * mode, no matter the dimensions of picture, the element will be filled.
             *
             * @param fullFill True to enable, false to disable.
             */
            void setFullFill(bool fullFill);

            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
        protected:
            virtual void refresh(int flags);
            virtual ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
        private:
            aex::cam::Camera& m_cam;
            std::unique_ptr<aex::cam::PreviewView> m_preview;
            bool m_fullFill;
        };
    }
}

#endif
