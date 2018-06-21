#ifndef CAMERA_PIXEL_BUFFER_AEX_GUI_HEADER
#define CAMERA_PIXEL_BUFFER_AEX_GUI_HEADER

#include "GUIMath.hpp"
#include "Component.hpp"
#include "aex/Camera.hpp"


namespace aex {
    namespace gui {

        /**
         * @brief CameraPixelBuffer is convieniece class for camera data.
         *
         * @details CameraPixelBuffer is meant to be used in rendering loop
         * as an object that gives user access to camera device (if device is
         * available). You can use it for example to take a photo.
         */
        class AEX_PUBLIC_API CameraPixelBuffer : public Component {
            CameraPixelBuffer(aex::cam::Camera& camera,
                    bool isOneShot,
                    bool takeShot,
                    aex::cam::FILTER filter,
                    const aex::cam::Size& fboSize
                    );
        public:
            /**
             * @brief Create new CameraPixelBuffer object.
             *
             * @details Create new CameraPixelBuffer object that continusly
             * captures data from camera device.
             *
             * @param camera Camera device.
             * @param filter Apply filter to buffer (default NONE).
             * @param fboSize Size of framebuffer to create (default [0, 0]).
             */
            CameraPixelBuffer(aex::cam::Camera& camera,
                    aex::cam::FILTER filter = aex::cam::FILTER::NONE,
                    const aex::cam::Size& fboSize = aex::cam::Size(0, 0));

            /**
             * @brief Create new CameraPixelBuffer object.
             *
             * @details Create new CameraPixelBuffer object that depending
             * on isOneShot param will either continusly capture that from camera
             * or take on shot picture.
             *
             * @param camera Camera device.
             * @param isOneShot True if only one picture should be taken.
             */
            CameraPixelBuffer(aex::cam::Camera& camera, bool isOneShot);
            virtual ~CameraPixelBuffer();

            /**
             * @brief Functor object executed when new data is available.
             *
             * @details When new data is available from camera device action
             * will be executed.
             *
             * @param action Action to take.
             */
            void setAction(const std::function<void (aex::cam::PixelBuffer&)>& action);

            /**
             * @brief Get backing framebufer size.
             *
             * @return Framebuffer size.
             */
            aex::cam::Size getPBOSize() const;

            /**
             * @brief Takes a picture in OneShot mode.
             */
            void takeShot();

            virtual void draw(aex::AexRenderer* gl);
            virtual void finishdraw(aex::AexRenderer* gl);
            virtual void needsUpdate();
            virtual void refresh(int flags);
        protected:
            ComponentPtr setFocus(const Position& point, FOCUS_TYPE type, const std::function<void(ComponentPtr)>& focusMe);
            std::unique_ptr<aex::cam::PreviewView> m_preview;
            aex::cam::Size m_pboSize;
            bool m_bIsOneShot;
            bool m_bTakeShot;
        };
    }
}

#endif
