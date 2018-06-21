#include "aex/fb/FBAppEventsLogger.hpp"
#include "JNIUtils.hpp"

namespace aex
{
    namespace fb
    {
        void FBAppEventsLogger::nativeActivateApp()
        {
            getJniClass("com/aexolgl/android/fb/FBAppEventsLogger").staticCallVoid("activateApp");
        }

    }
}
