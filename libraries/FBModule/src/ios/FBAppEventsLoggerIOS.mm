#include "aex/fb/FBAppEventsLogger.hpp"

namespace aex{
    namespace fb{
        void FBAppEventsLogger::nativeActivateApp()
        {
            // [FBSDKAppEvents activateApp]; // made in activate app method by swizzling, but user can think that he is launching it
        }
    }
}
