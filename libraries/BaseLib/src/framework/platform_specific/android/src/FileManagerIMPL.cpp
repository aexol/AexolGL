#include <aex/FileManagerBase.hpp>
#include <aex/AndroidAPI.hpp>
#include "../../include/FileManagerIMPL.hpp"
#include <sys/stat.h>
#include <aex/Logger.hpp>
#include <SDL.h>
#include <jni.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <vector>
#include <atomic>
#include <mutex>
#include <sys/stat.h>
namespace aex
{
    namespace {
        // Each lambda is unique type.
        // Using this as a workaround for
        // lack of support of lambda in atexit.
        template<class F>
        struct _atexit{
            static std::function<void(void)> _f;
            static void exec(void);
            _atexit(F&& f);
        };
        template<class F>
        std::function<void(void)> _atexit<F>::_f = std::function<void(void)>();
        template<class F>
        void _atexit<F>::exec(void) {
            if(_atexit::_f){
                _atexit::_f();
            }
            _atexit::_f = std::function<void(void)>();
        }
        template<class F>
        _atexit<F>::_atexit(F&& f) {
            _atexit::_f = std::function<void(void)>(std::move(f));
        }
        template<class F>
        void atexit(F&& f) {
            // Create an _atexit object to move new lambda.
            // This is only safe done _once_ per init.
            _atexit<F>(std::move(f));
            std::atexit(_atexit<F>::exec);
        }
    }

    /******TODO: Reimplement file type to use AssetManager instead of SDL_RWops******/
    const char* AndroidGetTemporaryStoragePath();
    using path = IFileManagerIMPL::path;
    using path_list = IFileManagerIMPL::path_list;

    static std::mutex init_sync;
    static std::atomic<char*> work_dir(nullptr);
    static std::atomic<char*> write_dir(nullptr);
    static std::atomic<char*> alternative_dir(nullptr);
    static std::atomic<char*> temporary_dir(nullptr);
    static const char* android_work_dir()
    {
        const char* _work_dir = work_dir.load();
        if(!_work_dir)
        {
            std::lock_guard<std::mutex> lock{init_sync};
            _work_dir = work_dir.load();
            if(!_work_dir)
            {
                work_dir = SDL_strdup("");
                _work_dir = work_dir.load();
                atexit(
                        [&]{SDL_free(work_dir.load());}
                );
            }
        }
        return _work_dir;
    }

    static const char* android_write_dir()
    {
        const char* _write_dir = write_dir.load();
        if(!_write_dir)
        {
            std::lock_guard<std::mutex> lock{init_sync};
            _write_dir = write_dir.load();
            if(!_write_dir)
            {
                _write_dir = SDL_AndroidGetInternalStoragePath();
                if(_write_dir)
                {
                    write_dir = SDL_strdup(_write_dir);
                    atexit([&]{SDL_free(write_dir.load());});
                }
            }
        }
        return _write_dir;
    }

    static const char* android_alternative_dir()
    {
        const char* _alternative_dir = alternative_dir.load();
        if(!_alternative_dir)
        {
            std::lock_guard<std::mutex> lock{init_sync};
            _alternative_dir = alternative_dir.load();
            if(!_alternative_dir)
            {
                _alternative_dir = SDL_AndroidGetExternalStoragePath();
                if(_alternative_dir)
                {
                    alternative_dir = SDL_strdup(_alternative_dir);
                    atexit([&]{SDL_free(alternative_dir.load());});
                }
            }
        }
        return _alternative_dir;
    }

    static const char* android_temporary_dir()
    {
        const char* _temporary_dir = temporary_dir.load();
        if(!_temporary_dir)
        {
            std::lock_guard<std::mutex> lock{init_sync};
            _temporary_dir = temporary_dir.load();
            if(!_temporary_dir)
            {
                _temporary_dir = AndroidGetTemporaryStoragePath();
                if(_temporary_dir)
                {
                    temporary_dir = SDL_strdup(_temporary_dir);
                    atexit([&]{SDL_free(temporary_dir.load());});
                }
            }
        }
        return _temporary_dir;
    }

    class FileManagerAndroid : public IFileManagerIMPL
    {
        public:
            bool init(void);
            path resourceDir() const
            {
                return m_resource_dir;
            }

            path writeDir() const
            {
                return m_write_dir;
            }

            path alternativeDir() const
            {
                return m_alternative_dir;
            }

            path temporaryDir() const
            {
                return m_temporary_dir;
            }

            path userDir() const
            {
                return m_user_dir;
            }

            path logDir() const
            {
                return m_log_dir;
            }

            path_list getDirFileNames(const path& dir) const;

            size_t file_size(const path& fName) const;

            path m_resource_dir;
            path m_write_dir;
            path m_alternative_dir;
            path m_temporary_dir;
            path m_user_dir;
            path m_log_dir;
    };


    bool FileManagerAndroid::init(void)
    {
        bool success = true;
        const char* tmp = nullptr;
        if(!(tmp = android_work_dir()))
            success = false;
        else
            m_resource_dir = aex::string(tmp);

        if(!(tmp = android_write_dir()))
            success = false;
        else
            m_write_dir = path(aex::string(tmp));

        if((tmp = android_alternative_dir()))
            m_alternative_dir = path(aex::string(tmp));

        if((tmp = android_temporary_dir()))
            m_temporary_dir = path(aex::string(tmp));

        return success;
    }

    path_list FileManagerAndroid::getDirFileNames(const path& dir) const
    {
        path_list ret;

        //LOG("The directory.");
        DIR* dp;
        struct dirent* ep;
        int  count = 0;
        dp = opendir(dir.native().c_str());

        if(dp != NULL)
        {
            while((ep = readdir(dp)))
            {
                ret.push_back(ep->d_name);
                //LOG(ep->d_name);
                count++;
            }

            (void) closedir(dp);
        }
        else
        {
            LOG_ERR("Couldn't open the directory.");
        }

        //Refcount=count;
        return ret;
    }

    const char* AndroidGetTemporaryStoragePath()  //TODO: Implement in Java?
    {
        static char* s_AndroidTemporaryFilesPath = NULL;

        if(!s_AndroidTemporaryFilesPath)
        {
            jobject context;
            jobject fileObject;
            jstring pathString;
            jmethodID mid;
            jmethodID getAbsolutePathMid;
            const char* path;

            JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
            aex::JNIFrameGuard jni_frame(*env, 16);

            /* context = SDLActivity.getContext(); */

            context = static_cast<jobject>(SDL_AndroidGetActivity());

            /* fileObj = context.getFilesDir(); */
            mid = env->GetMethodID(env->GetObjectClass(context),
                    "getCacheDir", "()Ljava/io/File;");
            fileObject = env->CallObjectMethod(context, mid);

            if(!fileObject)
            {
                LOG_ERR();
                throw aex::FileManagerError{"FileManager init failed. Couldn't get temporary directory."};
            }

            getAbsolutePathMid = env->GetMethodID(env->GetObjectClass(fileObject), "getAbsolutePath", "()Ljava/lang/String;");

            /* path = fileObject.getAbsolutePath(); */
            pathString = static_cast<jstring>(env->CallObjectMethod(fileObject, getAbsolutePathMid));

            path = env->GetStringUTFChars(pathString, NULL);
            s_AndroidTemporaryFilesPath = SDL_strdup(path);
            env->ReleaseStringUTFChars(pathString, path);
        }

        return s_AndroidTemporaryFilesPath;
    }

    size_t FileManagerAndroid::file_size(const path& fn) const
    {
        //FIXME: WTH?
        struct stat stat_buf;
        size_t rc = stat(fn.native().c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    }

    IFileManager FileManagerIMPLFactory::getNewFileManagerImplementation() const
    {
        return IFileManager(new FileManagerAndroid);
    }
}
