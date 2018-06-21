#ifndef __AEX_CAMERARENDERBLOCK_HPP
#define __AEX_CAMERARENDERBLOCK_HPP

#include <aex/RenderingBlock.hpp>

namespace aex
{
    class AexRenderer;
    class QuaternionCamera;
    /**
     * @brief Block handling setting up and tearing down camera on each frame.
     */
    class AEX_PUBLIC_API CameraRenderBlockImpl
    {
        public:
            using TraitsType = RunInitOnlyBlock<CameraRenderBlockImpl>;
            typedef RenderingBlock<CameraRenderBlockImpl, TraitsType> CameraRenderBlock;
            /**
             * @brief Default constructor.
             */
            CameraRenderBlockImpl();

            /**
             * @brief Set new context.
             *
             * @param AexRenderer* New context.
             */
            void setContext(AexRenderer* gl);

            /**
             * @brief Set new camera.
             *
             * @param QuaternionCamera* New camera.
             */
            void setCamera(QuaternionCamera* camera);
        protected:
            friend CameraRenderBlock;
            friend TraitsType;
            /**
             * @brief Init CameraRenderBlockImpl.
             *
             * @returns True if context is set.
             */
            bool init();

            /**
             * @brief Set up context for frame.
             */
            void beginFrame();

            /**
             * @brief Tear down context after frame.
             */
            void endFrame();
            
            /**
             * @brief Drawing context.
             */
            AexRenderer* m_aexRenderer;
            QuaternionCamera* m_camera;
    };
    typedef CameraRenderBlockImpl::CameraRenderBlock CameraRenderBlock;
} // end of aex

#endif // __AEX_CAMERARENDERBLOCK_HPP
