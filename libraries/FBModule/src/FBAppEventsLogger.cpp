#include "aex/fb/FBAppEventsLogger.hpp"
#include <aex/Logger.hpp>
namespace aex
{
    namespace fb
    {
        FBAppEventsLogger::FBAppEventsLogger()
        {
        }

        FBAppEventsLogger::~FBAppEventsLogger()
        {
        }

        void FBAppEventsLogger::activateApp()
        {
            nativeActivateApp();
        }
    }
}
