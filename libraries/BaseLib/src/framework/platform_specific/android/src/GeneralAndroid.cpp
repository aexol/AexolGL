#include "../include/GeneralAndroid.hpp"


#if __TARGET_PLATFORM__ == __ANDROID_OS__
namespace aex
{
    bool JavaCallsEnabled = false;

    bool NoJavaCalls()
    {
        return JavaCallsEnabled;
    }

    void DisableJavaCalls()
    {
        JavaCallsEnabled = true;
    }

    void EnableJavaCalls()
    {
        JavaCallsEnabled = false;
    }
}
#endif // __TARGET_PLATFORM__ == __ANDROID_OS__
