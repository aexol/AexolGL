#include <aex/Logger.hpp>
#include <aex/FileManager.hpp>
#include <mutex>
#include <string>
#if __TARGET_PLATFORM__ == __ANDRID_OS__
#include <aex/AndroidAPI.hpp>
#include "../platform_specific/android/include/GeneralAndroid.hpp"
#include <android/log.h>
#else
#include <iostream>
#endif

namespace aex
{
    void
    file_log_policy::open_ostream()
    {
        FileManager fManager;
        m_outStream->open((aex::string(fManager.log_dir().string()) + m_name).c_str(), std::ios_base::binary | std::ios_base::out);

        if(!m_outStream->is_open())
        {
            throw(std::runtime_error("LOGGER: Failed to open out stream."));
        }
    }

    void
    file_log_policy::close_ostream()
    {
        if(m_outStream)
        {
            m_outStream->close();
        }
    }

    void
    file_log_policy::write(const aex::string& msg)
    {
        (*m_outStream) << msg << std::endl;
    }

    file_log_policy::~file_log_policy()
    {
        close_ostream();
    }

    void
    stdout_log_policy::open_ostream()
    {
    }

    void
    stdout_log_policy::close_ostream()
    {
    }

    void
    stdout_log_policy::write(const aex::string& msg)
    {
#if __TARGET_PLATFORM__ == __ANDROID_OS__
        static aex::string appName;
        static std::once_flag flag;

        if(NoJavaCalls())
        {
            if(appName.empty()){
                __android_log_write(ANDROID_LOG_INFO, "AEX", msg.c_str());
            } else {
                __android_log_write(ANDROID_LOG_INFO, appName.c_str(), msg.c_str());
            }
        } else {
            std::call_once(flag, [&]()
            {
                    try{
                        appName = AndroidGetResourceString("string.app_name");
                    } catch (JavaExceptionOccured& cxx_exc){
                        __android_log_print(ANDROID_LOG_ERROR, "AEX", "Could not find app name with error:");
                        __android_log_print(ANDROID_LOG_ERROR,
                                "AEX",
                                "%s",
                                cxx_exc.what());
                        appName = "AEX";
                    }
            });
            __android_log_write(ANDROID_LOG_INFO, appName.c_str(), msg.c_str());
        }
#else
        std::cout << msg << std::endl;
#endif
    }

    stdout_log_policy::~stdout_log_policy()
    {
        close_ostream();
    }
}
