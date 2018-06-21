/* Thanks to: http://engineering.socialpoint.es/cpp-wrapper-for-jni.html */

#ifdef __ANDROID__
#include <aex/JniObject.hpp>
#include <aex/Logger.hpp>
#include <algorithm>
#include <atomic>
#include <SDL.h>
#include <android/log.h>

namespace aex{

namespace {
    const char* android_app_Activity = "android.app.Activity";
    bool isAssignableFrom(jclass cls, const aex::string& classPath) {
        auto& jni = Jni::get();
        JNIEnv* env = jni.getEnvironment();
        auto superCls = jni.getClass(classPath);
        if(superCls) {
            if(env->IsAssignableFrom(cls, superCls.getClass())) {
                return true;
            }
        }
        env->ExceptionClear();
        return false;
    }
}

JNIEnv*
JniEnviromentAccess::getEnvironment()
{
    return Jni::get().getEnvironment();
}

JniClass::JniClass(const aex::string& classPath) : JniClass()
{
    JNIEnv* env = getEnvironment();
    if(env)
    {
        jclass cls = (jclass)env->FindClass(classPath.c_str());
        if (cls)
        {
            set(cls);
        }
        else
        {
            env->ExceptionClear();
        }
    } else {
        _class = jclass();
    }
}

JniClass::JniClass() : _class(jclass()){}

JniClass::JniClass(const JniClass& jniClass) : JniClass()
{
     *this = jniClass;
}

 JniClass&
 JniClass::operator=(const JniClass& jniClass)
 {
    if(this != &jniClass)
    {
        JNIEnv* env = getEnvironment();
         if(env)
         {
             _staticMethods = jniClass._staticMethods;
             _instanceMethods = jniClass._instanceMethods;
             _staticFields = jniClass._staticFields;
             _instanceFields = jniClass._instanceFields;
             if(jniClass._class)
             {
                 cleanup();
                 set(jniClass._class);
             }
         }
    }
    return *this;
 }
JniClass::JniClass(JniClass&& jniClass) : JniClass()
{
    *this = std::move(jniClass);
}

JniClass&
JniClass::operator=(JniClass&& jniClass)
{
    if(this != &jniClass)
    {
        std::swap(_staticMethods, jniClass._staticMethods);
        std::swap(_instanceMethods, jniClass._instanceMethods);
        std::swap(_staticFields, jniClass._staticFields);
        std::swap(_instanceFields, jniClass._instanceFields);
        std::swap(_class, jniClass._class);
    }
    return *this;
}

JniClass::~JniClass()
{
    cleanup();
}

void
JniClass::cleanup()
{
    JNIEnv* env = getEnvironment();
    if((*this))
    {
        env->DeleteGlobalRef(_class);
    }
}

void
JniClass::set(jclass cls)
{
    JNIEnv* env = getEnvironment();
    if(cls)
    {
        _class = (jclass)env->NewGlobalRef(cls);
    }
}

jclass
JniClass::getClass() const
{
    return _class;
}

JniClass::operator bool() const
{
    return _class != jclass();
}

jmethodID
JniClass::getStaticMethodID(const aex::string& name, const aex::string& signature, bool cache) const
{
    jmethodID methodId{};
    try
    {
        methodId = _staticMethods.at(name+signature);
    } catch(std::out_of_range& e){
        if(!cache)
        {
            JNIEnv* env = getEnvironment();
            if(env)
            {
                methodId = env->GetStaticMethodID(_class, name.c_str(), signature.c_str());
            }
        }
    }
    return methodId;
}

jmethodID
JniClass::getMethodID(const aex::string& name, const aex::string& signature, bool cache) const
{
    jmethodID methodId{};
    try
    {
        methodId = _instanceMethods.at(name+signature);
    } catch(std::out_of_range& e){
        if(!cache)
        {
            JNIEnv* env = getEnvironment();
            if(env)
            {
                methodId = env->GetMethodID(_class, name.c_str(), signature.c_str());
            }
        }
    }
    return methodId;
}

jmethodID
JniClass::getStaticMethodID(const aex::string& name, const aex::string& signature, bool cache)
{
    jmethodID methodId = const_cast<const JniClass*>(this)->getStaticMethodID(name, signature, true);
    if(!methodId)
    {
        JNIEnv* env = getEnvironment();
        if(env)
        {
            methodId = env->GetStaticMethodID(_class, name.c_str(), signature.c_str());
            if(methodId && cache){
                _staticMethods.insert(MethodMap::value_type{name+signature, methodId});
            }
        }
    }
    return methodId;
}
jmethodID
JniClass::getMethodID(const aex::string& name, const aex::string& signature, bool cache)
{
    jmethodID methodId = const_cast<const JniClass*>(this)->getMethodID(name, signature, true);
    if(!methodId)
    {
        JNIEnv* env = getEnvironment();
        if(env)
        {
            methodId = env->GetMethodID(_class, name.c_str(), signature.c_str());
            if(methodId && cache){
                _staticMethods.insert(MethodMap::value_type{name+signature, methodId});
            }
        }
    }
    return methodId;
}

jfieldID
JniClass::getStaticFieldID(const aex::string& name, const aex::string& signature, bool cache) const
{
    jfieldID fieldId{};
    try
    {
        fieldId = _staticFields.at(name+signature);
    } catch(std::out_of_range& e){
        if(!cache)
        {
            JNIEnv* env = getEnvironment();
            if(env)
            {
                fieldId = env->GetStaticFieldID(_class, name.c_str(), signature.c_str());
            }
        }
    }
    return fieldId;
}

jfieldID
JniClass::getFieldID(const aex::string& name, const aex::string& signature, bool cache) const
{
    jfieldID fieldId{};
    try
    {
        fieldId = _instanceFields.at(name+signature);
    } catch(std::out_of_range& e){
        if(!cache)
        {
            JNIEnv* env = getEnvironment();
            if(env)
            {
                fieldId = env->GetFieldID(_class, name.c_str(), signature.c_str());
            }
        }
    }
    return fieldId;
}

jfieldID
JniClass::getStaticFieldID(const aex::string& name, const aex::string& signature, bool cache)
{
  jfieldID fieldId = const_cast<const JniClass*>(this)->getStaticFieldID(name, signature, true);
  if(!fieldId)
  {
      JNIEnv* env = getEnvironment();
      if(env)
      {
          fieldId = env->GetStaticFieldID(_class, name.c_str(), signature.c_str());
          if(fieldId && cache){
            _staticFields.insert(FieldMap::value_type{name+signature, fieldId});
          }
      }
  }
  return fieldId;
}

jfieldID
JniClass::getFieldID(const aex::string& name, const aex::string& signature, bool cache)
{
  jfieldID fieldId = const_cast<const JniClass*>(this)->getFieldID(name, signature, true);
  if(!fieldId)
  {
      JNIEnv* env = getEnvironment();
      if(env)
      {
          fieldId = env->GetFieldID(_class, name.c_str(), signature.c_str());
          if(fieldId && cache){
            _staticFields.insert(FieldMap::value_type{name+signature, fieldId});
          }
      }
  }
  return fieldId;
}
std::mutex Jni::_mutex;
Jni::Jni():
_env(nullptr)
{
}

Jni::Jni(const Jni& other)
{
    assert(false);
}

Jni::~Jni()
{
}

Jni& Jni::get()
{
    static std::atomic<Jni*> jni;
    if(!jni.load())
    {
        std::lock_guard<std::mutex> lock{_mutex};
        if(!jni.load())
        {
            jni = new Jni();
            std::atexit([]{delete &get();});
        }
    }
    return *jni;
}

JNIEnv* Jni::getEnvironment()
{
    if(!_env)
    {
        _env = (JNIEnv*)SDL_AndroidGetJNIEnv();
    }
    return _env;
}

JniClass& Jni::getClass(const aex::string& classPath)
{
    try
    {
        return _classes.at(classPath);
    } catch (std::out_of_range& e){
        _classes.insert(ClassMap::value_type{classPath, JniClass(classPath)});
        return _classes.at(classPath);
    }
}

JniClass Jni::getClassNoCache(const aex::string& classPath) const
{
    try
    {
        return _classes.at(classPath);
    } catch (std::out_of_range& e){
        return JniClass(classPath);
    }
}

#pragma mark - JniObject

JniObject::JniObject(const aex::string& classPath, jobject objId, JniClass* classId) :
_class(nullptr), _instance(nullptr)
{
    init(objId, classId, classPath);
}

JniObject::JniObject(JniClass* classId, jobject objId) :
_class(nullptr), _instance(nullptr)
{
    init(objId, classId);
}

JniObject::JniObject(jobject objId) :
_class(nullptr), _instance(nullptr)
{
    init(objId);
}

JniObject::JniObject(const JniObject& other) :
_class(nullptr), _instance(nullptr)
{
    init(other._instance, other._class, other._classPath);
}

void JniObject::init(jobject objId, JniClass* classId, const aex::string& classPath)
{
    JNIEnv* env = getEnvironment();
    aex::string l_classPath = classPath;
    _classPath = l_classPath;
    std::replace(_classPath.begin(), _classPath.end(), '.', '/');
    if(env)
    {
        if(!classId)
        {
            if(objId)
            {
                jclass cls = env->GetObjectClass(objId);
                if(_classPath.empty() || !isAssignableFrom(cls, _classPath)) {
                    jmethodID mid = env->GetMethodID(cls, "getClass", "()Ljava/lang/Class;");
                    jobject clsObj = env->CallObjectMethod(objId, mid);
                    cls = env->GetObjectClass(clsObj);
                    mid = env->GetMethodID(cls, "getName", "()Ljava/lang/String;");
                    if(mid)
                    {
                        jstring str = (jstring) env->CallObjectMethod(clsObj, mid, NULL);
                        const char * native_className = env->GetStringUTFChars(str, NULL);
                        l_classPath = native_className;
                        _classPath = l_classPath;
                        std::replace(_classPath.begin(), _classPath.end(), '.', '/');
                        env->ReleaseStringUTFChars(str, native_className);
                    } else {
                        if (env->ExceptionCheck()) {
                            LOG_ERR("Could not find classpath of object.");
                        }
                    }
                }
            }
            if(!l_classPath.empty())
            {
                classId = &Jni::get().getClass(_classPath);
            }
        }
        if(classId)
        {
            _class = classId;
        }
        else
        {
            _classPath = "";
        }
        if(objId)
        {
            _instance = env->NewGlobalRef(objId);
        }
    }
    if(_classPath.empty() && _instance && _class && (*_class))
    {
        _classPath = JniObject("java/lang/Class", _class->getClass()).call("getName", aex::string());
    }
    if(!_class)
    {
        aex::string err("Could not find class");
        if(_classPath.empty())
        {
            err += ".";
        }
        else
        {
             err += " '"+_classPath+"'.";
        }
        setError(err);
    }
}

JniObject::~JniObject()
{
    clear();
}

void JniObject::clear()
{
    JNIEnv* env = getEnvironment();
    if(!env)
    {
        return;
    }
    if(_class)
    {
        _class = nullptr;
    }
    if(_instance)
    {
        env->DeleteGlobalRef(_instance);
        _instance = nullptr;
    }
}

aex::string JniObject::getSignature() const
{
    return aex::string("L")+getClassPath()+";";
}

const aex::string& JniObject::getError() const
{
    return _error;
}

bool JniObject::hasError() const
{
    return !_error.empty();
}

void JniObject::setError(const aex::string& msg)
{
    _error = msg;
}

const aex::string& JniObject::getClassPath() const
{
    return _classPath;
}

JniClass* JniObject::getClass() const
{
    return _class;
}

jobject JniObject::getInstance() const
{
    return _instance;
}

jobject JniObject::getNewLocalInstance() const
{
    JNIEnv* env = getEnvironment();
    if(!env)
    {
        return 0;
    }
    return env->NewLocalRef(getInstance());
}

bool JniObject::isInstanceOf(const aex::string& classPath) const
{
    aex::string fclassPath(classPath);
    std::replace(fclassPath.begin(), fclassPath.end(), '.', '/');
    JNIEnv* env = getEnvironment();
    if(!env)
    {
        return false;
    }
    jclass cls = env->FindClass(fclassPath.c_str());
    return env->IsInstanceOf(getInstance(), cls);
}

JniObject JniObject::findSingleton(const aex::string& classPath)
{
    JniObject cls(classPath);

    JniObject obj = cls.staticField("instance", cls);
    if(!obj)
    {
        obj = cls.staticCall("getInstance", cls);
    }
    if(!obj)
    {
        obj.setError("Could not find singleton instance.");
    }
    return obj;
}

JniObject::operator bool() const
{
    return getInstance() != nullptr;
}

JniObject& JniObject::operator=(const JniObject& other)
{
    clear();
    _classPath = other._classPath;
    init(other._instance, other._class);
    return *this;
}

bool JniObject::operator==(const JniObject& other) const
{
    JNIEnv* env = getEnvironment();
    if(!env)
    {
        return false;
    }
    jobject a = getInstance();
    jobject b = other.getInstance();
    if(a && b)
    {
        return env->IsSameObject(a, b);
    }
    if(getClass() && other.getClass())
    {
        a = getClass()->getClass();
        b = other.getClass()->getClass();
        return env->IsSameObject(a, b);
    }
    return false;
}

#pragma mark - JniObject::getSignaturePart

template<>
aex::string JniObject::getSignaturePart<aex::string>(const aex::string& val)
{
    return "Ljava/lang/String;";
}

template<>
aex::string JniObject::getSignaturePart(const JniObject& val)
{
    return val.getSignature();
}

template<>
aex::string JniObject::getSignaturePart(const bool& val)
{
    return "Z";
}

template<>
aex::string JniObject::getSignaturePart(const jboolean& val)
{
    return "Z";
}

template<>
aex::string JniObject::getSignaturePart(const jbyte& val)
{
    return "B";
}

template<>
aex::string JniObject::getSignaturePart(const jchar& val)
{
    return "C";
}

template<>
aex::string JniObject::getSignaturePart(const jshort& val)
{
    return "S";
}

template<>
aex::string JniObject::getSignaturePart(const jlong& val)
{
    return "J";
}

template<>
aex::string JniObject::getSignaturePart(const long& val)
{
    return "J";
}

template<>
aex::string JniObject::getSignaturePart(const jint& val)
{
    return "I";
}

template<>
aex::string JniObject::getSignaturePart(const unsigned int& val)
{
    return "I";
}

template<>
aex::string JniObject::getSignaturePart(const jfloat& val)
{
    return "F";
}

template<>
aex::string JniObject::getSignaturePart(const jobject& val)
{
    return getSignaturePart(JniObject(val));
}

aex::string JniObject::getSignaturePart()
{
    return "V";
}

#pragma mark - JniObject::convertToJavaValue

template<>
jvalue JniObject::convertToJavaValue(const bool& obj)
{
    jvalue val;
    val.z = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jboolean& obj)
{
    jvalue val;
    val.z = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jbyte& obj)
{
    jvalue val;
    val.b = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jchar& obj)
{
    jvalue val;
    val.c = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jshort& obj)
{
    jvalue val;
    val.s = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jint& obj)
{
    jvalue val;
    val.i = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const unsigned int& obj)
{
    jvalue val;
    val.i = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const long& obj)
{
    jvalue val;
    val.j = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jlong& obj)
{
    jvalue val;
    val.j = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jfloat& obj)
{
    jvalue val;
    val.f = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jdouble& obj)
{
    jvalue val;
    val.d = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jobject& obj)
{
    jvalue val;
    val.l = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const JniObject& obj)
{
    return convertToJavaValue(obj.getInstance());
}

template<>
jvalue JniObject::convertToJavaValue(const jarray& obj)
{
    jvalue val;
    val.l = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const jstring& obj)
{
    jvalue val;
    val.l = obj;
    return val;
}

template<>
jvalue JniObject::convertToJavaValue(const aex::string& obj)
{
    JNIEnv* env = getEnvironment();
    if (!env)
    {
        return jvalue();
    }
    return convertToJavaValue(env->NewStringUTF(obj.c_str()));
}

#pragma mark - JniObject::convertFromJavaObject

template<>
bool JniObject::convertFromJavaObject(JNIEnv* env, jobject obj, aex::string& out)
{
    if(!obj)
    {
        out = "";
        return true;
    }
    jstring jstr = (jstring)obj;
    const char* chars = env->GetStringUTFChars(jstr, NULL);
    if(!chars)
    {
        return false;
    }
    out = chars;
    env->ReleaseStringUTFChars(jstr, chars);
    return true;
}

template<>
bool JniObject::convertFromJavaObject(JNIEnv* env, jobject obj, JniObject& out)
{
    out = obj;
    env->DeleteLocalRef(obj);
    return true;
}

#pragma mark - JniObject call jni

template<>
void JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return env->CallStaticVoidMethodA(classId, methodId, args);
}

template<>
jobject JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return env->CallStaticObjectMethodA(classId, methodId, args);
}

template<>
double JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return env->CallStaticDoubleMethodA(classId, methodId, args);
}

template<>
long JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return env->CallStaticLongMethodA(classId, methodId, args);
}


template<>
jlong JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return env->CallStaticLongMethodA(classId, methodId, args);
}

template<>
float JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return env->CallStaticFloatMethodA(classId, methodId, args);
}

template<>
int JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return env->CallStaticIntMethodA(classId, methodId, args);
}

template<>
aex::string JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return convertFromJavaObject<aex::string>(env, callStaticJavaMethod<jobject>(env, classId, methodId, args));
}

template<>
JniObject JniObject::callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args)
{
    return convertFromJavaObject<JniObject>(env, callStaticJavaMethod<jobject>(env, classId, methodId, args));
}

void JniObject::callJavaVoidMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args)
{
    env->CallVoidMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, jboolean& out)
{
    out = env->CallBooleanMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, jobject& out)
{
    out = env->CallObjectMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, double& out)
{
    out = env->CallDoubleMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, long& out)
{
    out = env->CallLongMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, jlong& out)
{
    out = env->CallLongMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, float& out)
{
    out = env->CallFloatMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, int& out)
{
    out = env->CallIntMethodA(objId, methodId, args);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, aex::string& out)
{
    callJavaObjectMethod(env, objId, methodId, args, out);
}

template<>
void JniObject::callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, JniObject& out)
{
    callJavaObjectMethod(env, objId, methodId, args, out);
}

template<>
jobject JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return env->GetStaticObjectField(classId, fieldId);
}

template<>
double JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return env->GetStaticDoubleField(classId, fieldId);
}

template<>
long JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return env->GetStaticLongField(classId, fieldId);
}

template<>
jlong JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return env->GetStaticLongField(classId, fieldId);
}

template<>
float JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return env->GetStaticFloatField(classId, fieldId);
}

template<>
int JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return env->GetStaticIntField(classId, fieldId);
}

template<>
aex::string JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return convertFromJavaObject<aex::string>(getJavaStaticField<jobject>(env, classId, fieldId));
}

template<>
JniObject JniObject::getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId)
{
    return convertFromJavaObject<JniObject>(getJavaStaticField<jobject>(env, classId, fieldId));
}

template<>
jobject JniObject::getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId)
{
    return env->GetObjectField(objId, fieldId);
}

template<>
double JniObject::getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId)
{
    return env->GetDoubleField(objId, fieldId);
}

template<>
long JniObject::getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId)
{
    return env->GetLongField(objId, fieldId);
}

template<>
float JniObject::getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId)
{
    return env->GetFloatField(objId, fieldId);
}

template<>
int JniObject::getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId)
{
    return env->GetIntField(objId, fieldId);
}

template<>
aex::string JniObject::getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId)
{
    return convertFromJavaObject<aex::string>(getJavaField<jobject>(env, objId, fieldId));
}

template<>
JniObject JniObject::getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId)
{
    return convertFromJavaObject<JniObject>(getJavaField<jobject>(env, objId, fieldId));
}

template<>
jarray JniObject::createJavaArray(JNIEnv* env, const jobject& element, size_t size)
{
    jclass elmClass = env->GetObjectClass(element);
    return env->NewObjectArray(size, elmClass, 0);
}

template<>
jarray JniObject::createJavaArray(JNIEnv* env, const double& element, size_t size)
{
    return env->NewDoubleArray(size);
}

template<>
jarray JniObject::createJavaArray(JNIEnv* env, const long& element, size_t size)
{
    return env->NewLongArray(size);
}


template<>
jarray JniObject::createJavaArray(JNIEnv* env, const jlong& element, size_t size)
{
    return env->NewLongArray(size);
}

template<>
jarray JniObject::createJavaArray(JNIEnv* env, const float& element, size_t size)
{
    return env->NewFloatArray(size);
}

template<>
jarray JniObject::createJavaArray(JNIEnv* env, const int& element, size_t size)
{
    return env->NewLongArray(size);
}

template<>
jarray JniObject::createJavaArray(JNIEnv* env, const aex::string& element, size_t size)
{
    jclass elmClass = env->FindClass("java/lang/String");
    return env->NewObjectArray(size, elmClass, 0);
}

template<>
jarray JniObject::createJavaArray(JNIEnv* env, const JniObject& element, size_t size)
{
    jclass elmClass{};
    if(element.getClass())
        elmClass = element.getClass()->getClass();
    return env->NewObjectArray(size, elmClass, 0);
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, jobject& out)
{
    out = env->GetObjectArrayElement((jobjectArray)arr, position);
    return true;
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, double& out)
{
    env->GetDoubleArrayRegion((jdoubleArray)arr, position, 1, &out);
    return true;
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, long& out)
{
    jlong jout = 0;
    env->GetLongArrayRegion((jlongArray)arr, position, 1, &jout);
    out = jout;
    return true;
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, jlong& out)
{
    env->GetLongArrayRegion((jlongArray)arr, position, 1, &out);
    return true;
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, float& out)
{
    env->GetFloatArrayRegion((jfloatArray)arr, position, 1, &out);
    return true;
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, int& out)
{
    env->GetIntArrayRegion((jintArray)arr, position, 1, &out);
    return true;
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, aex::string& out)
{
    jobject obj;
    if(!convertFromJavaArrayElement(env, arr, position, obj))
    {
        return false;
    }
    convertFromJavaObject(env, obj, out);
    return true;
}

template<>
bool JniObject::convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, JniObject& out)
{
    jobject obj;
    if(!convertFromJavaArrayElement(env, arr, position, obj))
    {
        return false;
    }
    convertFromJavaObject(env, obj, out);
    return true;
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const jobject& elm)
{
    env->SetObjectArrayElement((jobjectArray)arr, position, elm);
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const double& elm)
{
    env->SetDoubleArrayRegion((jdoubleArray)arr, position, 1, &elm);
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const long& elm)
{
    jlong jelm = elm;
    env->SetLongArrayRegion((jlongArray)arr, position, 1, &jelm);
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const jlong& elm)
{
    jlong jelm = elm;
    env->SetLongArrayRegion((jlongArray)arr, position, 1, &jelm);
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const float& elm)
{
    env->SetFloatArrayRegion((jfloatArray)arr, position, 1, &elm);
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const int& elm)
{
    env->SetIntArrayRegion((jintArray)arr, position, 1, &elm);
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const aex::string& elm)
{
    jobject obj = env->NewStringUTF(elm.c_str());
    setJavaArrayElement(env, arr, position, obj);
}

template<>
void JniObject::setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const JniObject& elm)
{
    setJavaArrayElement(env, arr, position, elm.getInstance());
}

static int onResume(void* userdata, SDL_Event* event);
static int onPause(void* userdata, SDL_Event* event);
struct ActivityTracker
{
    ActivityTracker() : mVisible(true)
    {
        SDL_AddEventWatch(onResume, this);
        SDL_AddEventWatch(onPause, this);
    }
    ~ActivityTracker()
    {
        SDL_DelEventWatch(onPause, this);
        SDL_DelEventWatch(onResume, this);
    }
    operator bool()
    {
        return mVisible;
    }
    void set(bool new_val)
    {
        mVisible = new_val;
    }
    bool mVisible;
};
static int onResume(void* userdata, SDL_Event* event)
{
    if(event->type == SDL_APP_DIDENTERFOREGROUND)
        reinterpret_cast<ActivityTracker*>(userdata)->set(true);
    return 1;
}
static int onPause(void* userdata, SDL_Event* event)
{
    if(event->type == SDL_APP_DIDENTERBACKGROUND ||
            event->type == SDL_APP_TERMINATING
            )
        reinterpret_cast<ActivityTracker*>(userdata)->set(false);
    return 1;
}

JniObject
JniObject::getAndroidActivityAs(const aex::string& classPath)
{
    jobject androidActivity = nullptr;
    androidActivity = (jobject)SDL_AndroidGetActivity();
    JniObject activity(classPath, androidActivity);
    return activity;
}

JniObject
JniObject::getAndroidActivity()
{
    return getAndroidActivityAs(aex::string());
}

JniObject
JniObject::getAndroidActivityAsActivity()
{
    return getAndroidActivityAs(android_app_Activity);
}

JniObject GetAndroidActivity() {
    return JniObject::getAndroidActivity();
}
JniObject GetAndroidActivityAs(const aex::string& classPath) {
    return JniObject::getAndroidActivityAs(classPath);
}
JniObject GetAndroidActivityAsActivity() {
    return JniObject::getAndroidActivityAsActivity();
}
std::string GetPackageName() {
    auto activity = GetAndroidActivity();
    std::string out;
    return activity.callSigned("getPackageName", "()Ljava/lang/String;", out);
}

}

#endif // __ANDROID__
