#include "AConfiguration.hpp"
#include <aex/AndroidAPI.hpp>
#include <SDL.h>
#include <mutex>


namespace aex
{
    const char* sNoParamIntRet = "()I";
    const char* mComAexolglAppAexActivityGetDefaultRotation[] = {"getDefaultRotation", sNoParamIntRet};

    static std::once_flag orientationOnceFlag;
    static ADeviceRotation deviceRotation;
    ADeviceRotation
    AConfiguration::getDefaultRotation()
    {
        std::call_once(orientationOnceFlag, []()
        {
            JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
            {
                JniSafeEnv localEnv(env, 16);
                jclass activityClass = localEnv([&, this](JNIEnv & env)
                {
                    return env.GetObjectClass(activityObject);
                });
                jmethodID jDefRotMID = env.GetMethodID(activityClass,
                                                       mComAexolglAppAexActivityGetDefaultRotation[0], mComAexolglAppAexActivityGetDefaultRotation[1]);

                jobject ActivityObject = static_cast<jobject>(SDL_AndroidGetActivity());
                jint rotInt = localEnv([&](JNIEnv & env)
                {
                    return env.CallIntMethod(ActivityObject, jDefRotMID);
                });

                if(rotInt == 0)
                {
                    deviceRotation = ADeviceRotation::ROTATION_0;
                }

                if(rotInt == 90)
                {
                    deviceRotation = ADeviceRotation::ROTATION_90;
                }

                if(rotInt == 180)
                {
                    deviceRotation = ADeviceRotation::ROTATION_180;
                }

                if(rotInt == 270)
                {
                    deviceRotation = ADeviceRotation::ROTATION_270;
                }

            }
        }
                      );
        return deviceRotation;
    }
}
