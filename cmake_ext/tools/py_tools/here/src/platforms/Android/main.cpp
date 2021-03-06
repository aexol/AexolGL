//Android main
#include <aex/AexolGL.hpp>
#include <aex/AndroidAPI.hpp>
#include <aex/Logger.hpp>
#include <aex/JniObject.hpp>
#include <SDL.h>
#include <python2.7/Python.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include <jni.h>
#include <android/log.h>
#include <cstring>
typedef std::vector<char> char_vector;
std::string python_native_lib_path();

//Some paths
constexpr const char *python_home_suffix="APP/python";
constexpr const char *python_lib_path_suffix="APP/python/lib/python2.7/";
constexpr const char *app_home_suffix="APP/src";
constexpr const char *app_main_suffix="APP/app.zip";
const char *sub_paths[]
{
    python_home_suffix,
    python_lib_path_suffix,
    "APP/python/lib/python2.7.zip",
    "APP/python/usr/local/lib/python2.7/",
    "APP/python/usr/local/lib/python2.7/site-packages",
    "APP/python/usr/local/lib/python2.7/site-packages.zip",
    "APP/python/usr/local/lib/python2.7.zip",
    python_native_lib_path().c_str(),
    app_home_suffix
};
PyObject *_pPyGetValFunc;
PyObject *_pPyobStringIO;

void unpack_python()
{
    JniObject activity = JniObject::getAndroidActivity();
    if(!activity.hasError())
        activity.callVoid("buildPythonData");
}


int set_stderr()
{
    PyObject *modStringIO = nullptr;
    PyObject *obFuncStringIO = nullptr;
    int ret = 0;

    // cStringIO import.
    modStringIO = PyImport_ImportModule("cStringIO");
    if(PyErr_Occurred() || modStringIO == nullptr)
        ret = 1;

    obFuncStringIO = PyObject_GetAttrString(modStringIO, "StringIO");
    if(ret || PyErr_Occurred() || obFuncStringIO == nullptr)
        ret = 1;

    _pPyobStringIO = PyObject_CallObject(obFuncStringIO, NULL);
    if(ret || PyErr_Occurred() || _pPyobStringIO==NULL)
        ret = 1;

    _pPyGetValFunc = PyObject_GetAttrString(_pPyobStringIO, "getvalue");
    if(ret || PyErr_Occurred() || _pPyGetValFunc==NULL)
        ret = 1;

    if(ret == 0)
        ret = PySys_SetObject("stderr", _pPyobStringIO);

    if(ret != 0 )
    {
        LOG_ERR("cStringIO import failed. Errors will not be visible in logcat.");
        Py_XDECREF(modStringIO);
        Py_XDECREF(obFuncStringIO);
        Py_XDECREF(_pPyobStringIO);
        Py_XDECREF(_pPyGetValFunc);
    }
    return ret;
}

std::string getPythonErrorMessage()
{
    int ret = 0;
    std::string error_msg;
    PyObject *obResult=nullptr;
    char *sresult = nullptr;
    obResult = PyObject_CallObject(_pPyGetValFunc, NULL);
    if(PyErr_Occurred() || obResult==NULL)
        ret = 1;
    // did getvalue return a string?
    if(!PyString_Check(obResult))
        ret = 1;
    // retrieve error message string from this object
    if(ret == 0 && nullptr != (sresult = PyString_AsString(obResult)))
        error_msg = sresult;
    if(obResult)
        Py_XDECREF(obResult);
    return error_msg;
}

//Helper methods declarations
template<typename T, std::size_t N> char_vector init_pythonpath(T (&paths)[N], aexFramework::FileManager& fileManager);
char_vector app_abspath(const char* p, aexFramework::FileManager& fileManager);
const char* get_library_path();


#define PYARGC 3

int main(int argc, char **argv)
{
    int ret = 0;
    aexFramework::FileManager fileManager;
    const char_vector python_path = init_pythonpath(sub_paths, fileManager);
    const char_vector main_pyo = app_abspath(app_main_suffix, fileManager);
    const char* main_file;
    const char* python_cmd[PYARGC] = {"python", "-m", "src"};
    char** pyargv = new char*[PYARGC];
    for(std::size_t ii = 0; ii < PYARGC; ii++)
    {
        int len = strlen(python_cmd[ii]);
        pyargv[ii] = new char[64];
        strncpy(pyargv[ii], python_cmd[ii], len);
        pyargv[ii][len] = ' ';
    }
    char_vector python_home = app_abspath(python_home_suffix, fileManager);
    unpack_python();
    
    // Enviroment variables for interpreter
    SDL_setenv("PYTHONOPTIMIZE", "2", 1);
    SDL_setenv("PYTHONDONTWRITEBYTECODE", "1", 1);
    SDL_setenv("PYTHONNOUSERSITE", "1", 1);
    SDL_setenv("PYTHONPATH", python_path.data(), 1);
    
    Py_SetPythonHome(python_home.data());
    Py_Initialize();
    //PySys_SetArgv(argc, argv);
    //set_stderr();
    //
    //PyEval_InitThreads();
    //main_file = main_pyo.data();
    //AEX_FILE* fd = AEX_FILEFromFile(main_file, "r");
    //if(!fd)
    //{
    //    main_file = app_main_suffix;
    //    fd = AEX_FILEFromFile(main_file, "r");
    //}
    //if(!fd)
    //{
    //    ret = 1;
    //    LOG("Unable to open ", app_main_suffix);
    //} else {
    //    ret = PyRun_SimpleFileEx(fd, main_file, 1);
    //    LOG(getPythonErrorMessage());
    //}
    Py_Main(PYARGC, pyargv);
    Py_Finalize();
    for(std::size_t ii = 0; ii < PYARGC; ii++)
        delete [] pyargv[ii];
    delete [] pyargv;
    return ret;
}

template<typename T, size_t N>
char_vector init_pythonpath(T (&paths)[N], aexFramework::FileManager& fileManager)
{
    char_vector ret;
    std::stringstream ss;
    const std::string& write_dir = fileManager.write_dir().native();
    for(int ii=0; ii < N; ii++)
    {
        ss<<write_dir<<"/"<<sub_paths[ii]<<":";
    }
    std::string str = ss.str();
    str.pop_back();
    ret.resize(str.size()+1);
    std::copy(str.c_str(), str.c_str()+str.size(), ret.begin());
    ret.back() = ' ';
    return ret;
}

char_vector app_abspath(const char* p, aexFramework::FileManager& fileManager)
{
    char_vector ret;
    std::stringstream ss;
    const std::string& write_dir = fileManager.write_dir().native();
    ss<<write_dir<<"/"<<p;
    std::string str = ss.str();
    ret.resize(str.size()+1);
    std::copy(str.c_str(), str.c_str()+str.size(), ret.begin());
    ret.back() = ' ';
    return ret;
}

const char* get_library_path()
{
    static char* s_AndroidTemporaryFilesPath = NULL;

    if(!s_AndroidTemporaryFilesPath)
    {
        jobject activity;
        jstring pathString;
        jmethodID mid;
        const char* path;

        JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
        aex::JNIFrameGuard jni_frame(*env, 16);
        activity = static_cast<jobject>(SDL_AndroidGetActivity());

        // Call getLibDir() on AexActivity.
        mid = env->GetMethodID(env->GetObjectClass(activity),
                "getLibDir", "()Ljava/lang/String;");
        pathString = static_cast<jstring>(env->CallObjectMethod(activity, mid));
        path = env->GetStringUTFChars(pathString, NULL);
        s_AndroidTemporaryFilesPath = SDL_strdup(path);
        env->ReleaseStringUTFChars(pathString, path);
    }

    return s_AndroidTemporaryFilesPath;
}

std::string python_native_lib_path()
{
    std::string rt(get_library_path());
    return rt+"/python2.7";
}
