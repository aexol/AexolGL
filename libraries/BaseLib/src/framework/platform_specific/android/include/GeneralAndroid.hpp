#ifndef __AEX_GENERALANDROID_HPP
#define __AEX_GENERALANDROID_HPP

#if __TARGET_PLATFORM__ == __ANDROID_OS__
namespace aex
{
    bool NoJavaCalls();
    void DisableJavaCalls();
    void EnableJavaCalls();
}
#endif // __TARGET_PLATFORM__ == __ANDROID_OS__

#endif // __AEX_GENERALANDROID_HPP
