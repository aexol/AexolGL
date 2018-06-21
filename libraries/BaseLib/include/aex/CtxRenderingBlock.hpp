#ifndef __AEX_CTXRENDERINGBLOCK_HPP
#define __AEX_CTXRENDERINGBLOCK_HPP

#include <aex/RenderingBlock.hpp>

namespace aex
{
    // Forward declarations.
    class AexRenderer;

    /**
     * @brief Block handling setting up and tearing down renderer on each frame.
     */
    class AEX_PUBLIC_API RenderContextBlockImpl
    {
        public:
            using TraitsType = RunInitOnlyBlock<RenderContextBlockImpl>;
            typedef RenderingBlock<RenderContextBlockImpl, TraitsType> RenderContextBlock;
            /**
             * @brief Default constructor.
             */
            RenderContextBlockImpl() = default;

            /**
             * @brief Create and wrap gl.
             *
             * @param AexRender* Pointer to AexRenderer object.
             */
            RenderContextBlockImpl(AexRenderer* gl);

            /**
             * @brief Set new context.
             *
             * @param AexRenderer* New context.
             */
            void setContext(AexRenderer* gl);

            /**
             * @brief Get current context.
             *
             * @returns AexRenderer* Current context.
             */
            AexRenderer* getContext();
        private:
            friend RenderContextBlock;
            friend TraitsType;
            /**
             * @brief Init RenderContextBlockImpl.
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
    };
    typedef RenderContextBlockImpl::RenderContextBlock RenderContextBlock;
} // end of aex

#endif // __AEX_CTXRENDERINGBLOCK_HPP
