#ifndef FB_APP_EVENT_LOGGER_HPP_FILE
#define FB_APP_EVENT_LOGGER_HPP_FILE

#include <aex/ALoopNode.hpp>

namespace aex
{
    namespace fb
    {
        class FBAppEventsLogger
        {
        public:
            FBAppEventsLogger();
            ~FBAppEventsLogger();

            void activateApp();
            //void deactivateApp(); // not needed in new api
        protected:
            virtual void nativeActivateApp();
            //virtual void nativeDeactivateApp();
        };
    }
}

#endif // FB_APP_EVENT_LOGGER_HPP_FILE
