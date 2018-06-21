/**************************************************************
 *                                                            * 
 *      Filename:   GraphicsDevice.hpp                        * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 15 14:54:15                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef GRAPHICSDEVICE_HPP
#define GRAPHICSDEVICE_HPP 
#include <aex/Common.hpp>
#include <memory>

namespace aex
{
    class AWindow;
    /**
     * @brief Interface for graphics device api.
     *
     * Graphics device api interface.
     */
    class AEX_PUBLIC_API IGraphicsDeviceAPI
    {
        public:
            /**
             * @brief Init API to default state.
             *
             * @param window Window with API context.
             * @param enables Enable flags from GPUApi.hpp.
             * @param disables Disable flags from GPUApi.hpp
             *
             * @return true on success.
             */
            virtual bool initAPI(AWindow* window, uint32_t enables, uint32_t disables) = 0;
            /**
             * @brief API clean up code.
             */
            virtual void finishAPI() = 0;
            /**
             * @brief Called to prepare frame for drawing.
             */
            virtual void beginFrame() = 0;
            /**
             * @brief Called to prepare frame for drawing.
             *
             * @param buffers Which buffers should be prepared for drawing.
             */
            virtual void beginFrame(uint32_t buffers) = 0;
            /**
             * @brief Finish drawing frame
             *
             * @param window 
             */
            virtual void endFrame() = 0;
            virtual ~IGraphicsDeviceAPI() = 0;
    };
    inline IGraphicsDeviceAPI::~IGraphicsDeviceAPI(){}

    class AEX_PUBLIC_API GraphicsDevice
    {
        public:
            GraphicsDevice();
            GraphicsDevice(const GraphicsDevice& gd);
            GraphicsDevice& operator=(const GraphicsDevice& gd);
            GraphicsDevice(GraphicsDevice&& gd);
            GraphicsDevice& operator=(GraphicsDevice&& gd);
            bool initAPI(AWindow* window, uint32_t enables = 0, uint32_t disables = 0);
            void finishAPI();
            void beginFrame();
            void beginFrame(uint32_t buffers);
            void endFrame();
        private:
            std::unique_ptr<IGraphicsDeviceAPI> m_api;
    };
}

#endif /* GRAPHICSDEVICE_HPP */
