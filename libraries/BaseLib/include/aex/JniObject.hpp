/* Thanks to: http://engineering.socialpoint.es/cpp-wrapper-for-jni.html */

#ifndef __JniObject__
#define __JniObject__

#ifdef __ANDROID__

#include <aex/Common.hpp>
#include <jni.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <array>
#include <list>
#include <set>
#include <cassert>
#include <mutex>
namespace aex{
    

class AEX_PUBLIC_API JniEnviromentAccess
{
    public:
        static JNIEnv* getEnvironment();
};

class AEX_PUBLIC_API JniClass : public JniEnviromentAccess {
    private:
        typedef std::map<aex::string, jmethodID> MethodMap;
        typedef std::map<aex::string, jfieldID> FieldMap;
        MethodMap _staticMethods;
        MethodMap _instanceMethods;
        FieldMap _staticFields;
        FieldMap _instanceFields;
        jclass _class;
        aex::string _err;
        JniClass(const aex::string& classPath);
        friend class Jni;
        void cleanup();
        void set(jclass cls);
    public:
        JniClass();
        JniClass(const JniClass& jniClass);
        JniClass& operator=(const JniClass& jniClass);
        JniClass(JniClass&& jniClass);
        JniClass& operator=(JniClass&& jniClass);
        ~JniClass();
        /* Get jclass object represented */
        jclass getClass() const;

        /* Is valid java class */
        operator bool() const;

        /* Get jMethoodIDs for supplied signature */
        jmethodID getStaticMethodID(const aex::string& name, const aex::string& signature, bool cache = false) const;
        jmethodID getMethodID(const aex::string& name, const aex::string& singature, bool cache = false) const;

        jmethodID getStaticMethodID(const aex::string& name, const aex::string& signature, bool cache = true);
        jmethodID getMethodID(const aex::string& name, const aex::string& singature, bool cache = true);

        /* Get jFieldIDs for supplied singature */
        jfieldID getStaticFieldID(const aex::string& name, const aex::string& signature, bool cache = false) const;
        jfieldID getFieldID(const aex::string& name, const aex::string& singature, bool cache = false) const;

        jfieldID getStaticFieldID(const aex::string& name, const aex::string& singature, bool cache = true);
        jfieldID getFieldID(const aex::string& name, const aex::string& signature, bool cache = true);
};

class AEX_PUBLIC_API Jni
{
private:
    typedef std::map<aex::string, JniClass> ClassMap;
    JNIEnv* _env;
    ClassMap _classes;
    static std::mutex _mutex;

    Jni();
    Jni(const Jni& other);
public:
    ~Jni();

    /**
     * This class is a singleton
     */
    static Jni& get();

    /**
     * Get the java environment pointer
     * Will attatch to the current thread automatically
     */
    JNIEnv* getEnvironment();

    /**
     * get a class, will be stored in64 the class cache
     */
    JniClass& getClass(const aex::string& classPath);
    JniClass getClassNoCache(const aex::string& classPath) const;
};

/**
 * This class represents a jni object
 */
class AEX_PUBLIC_API JniObject : public JniEnviromentAccess
{
private:

    JniClass* _class;
    jobject _instance;
    aex::string _error;
    aex::string _classPath;

    template<typename Arg, typename... Args>
    static void buildSignature(std::ostringstream& os, const Arg& arg, const Args&... args)
    {
        os << getSignaturePart(arg);
        buildSignature(os, args...);
    }

    static void buildSignature(std::ostringstream& os)
    {
    }

    template<typename Return, typename... Args>
    static aex::string createSignature(const Return& ret, const Args&... args)
    {
        std::ostringstream os;
        os << "(";
        buildSignature(os, args...);
        os << ")" << getSignaturePart(ret);
        return os.str();
    }

    template<typename... Args>
    static aex::string createVoidSignature(const Args&... args)
    {
        std::ostringstream os;
        os << "(";
        buildSignature(os, args...);
        os << ")" << getSignaturePart();
        return os.str();
    }

    template<typename... Args>
    static jvalue* createArguments(const Args&... args)
    {
        jvalue* jargs = (jvalue*)malloc(sizeof(jvalue)*sizeof...(Args));
        buildArguments(jargs, 0, args...);
        return jargs;
    }

    static jvalue* createArguments()
    {
        return nullptr;
    }

    template<typename Arg, typename... Args>
    static jvalue* buildArguments(jvalue* jargs, unsigned pos, const Arg& arg, const Args&... args)
    {
        jargs[pos] = convertToJavaValue(arg);
        return buildArguments(jargs, pos+1, args...);
    }

    static jvalue* buildArguments(jvalue* jargs, unsigned pos)
    {
        return jargs;
    }

    /**
     * Return the signature for the given type
     */
    template<typename Type>
    static aex::string getSignaturePart(const Type& type);

    /**
    * Return the signature for the given container element
    */
    template<typename Type>
    static aex::string getContainerElementSignaturePart(const Type& container)
    {
        if(container.empty())
        {
            return getSignaturePart(typename Type::value_type());
        }
        else
        {
            return getSignaturePart(*container.begin());
        }
    }

    // template specialization for pointers
    template<typename Type>
    static aex::string getSignaturePart(Type* val)
    {
        return getSignaturePart((jlong)val);
    }

    // template specialization for containers
    template<typename Type>
    static aex::string getSignaturePart(const std::vector<Type>& val)
    {
        return aex::string("[")+getContainerElementSignaturePart(val);
    }
    template<typename Type>
    static aex::string getSignaturePart(const std::set<Type>& val)
    {
        return aex::string("[")+getContainerElementSignaturePart(val);
    }
    template<typename Type, int Size>
    static aex::string getSignaturePart(const std::array<Type, Size>& val)
    {
        return aex::string("[")+getContainerElementSignaturePart(val);
    }
    template<typename Type>
    static aex::string getSignaturePart(const std::list<Type>& val)
    {
        return aex::string("[")+getContainerElementSignaturePart(val);
    }
    template<typename Key, typename Value>
    static aex::string getSignaturePart(const std::map<Key, Value>& val)
    {
        return "Ljava/util/Map;";
    }

    /**
     * Return the signature for the void type
     */
    static aex::string getSignaturePart();

    template<typename Return>
    Return callStaticJavaMethod(JNIEnv* env, jclass classId, jmethodID methodId, jvalue* args);

    void callJavaVoidMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args);

    template<typename Return>
    void callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, Return& out);

    template<typename Return>
    void callJavaObjectMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, Return& out)
    {
        jobject jout = nullptr;
        callJavaMethod(env, objId, methodId, args, jout);
        out = convertFromJavaObject<Return>(jout);
    }

    template<typename Type>
    void callJavaMethod(JNIEnv* env, jobject objId, jmethodID methodId, jvalue* args, std::vector<Type>& out)
    {
        callJavaObjectMethod(env, objId, methodId, args, out);
    }

    template<typename Return>
    Return getJavaStaticField(JNIEnv* env, jclass classId, jfieldID fieldId);

    template<typename Return>
    Return getJavaField(JNIEnv* env, jobject objId, jfieldID fieldId);

    void setError(const aex::string& msg);

public:
    JniObject(const aex::string& classPath, jobject javaObj=nullptr, JniClass* classId=nullptr);
    JniObject(JniClass* classId, jobject javaObj);
    JniObject(jobject javaObj=nullptr);
    JniObject(const JniObject& other);
    void init(jobject javaObj=nullptr, JniClass* classId=nullptr, const aex::string& classPath="");
    ~JniObject();

    /**
     * Clear the retained global references
     */
    void clear();

    /**
     * Find a singleton instance
     * will try the `instance` static field and a `getInstance` static method
     */
    static JniObject findSingleton(const aex::string& classPath);

    /**
     * Create a new JniObject
     */
    template<typename... Args>
    static JniObject createNew(const aex::string& classPath, Args&&... args)
    {
        JniObject defRet(classPath);
        JNIEnv* env = getEnvironment();
        if(!env)
        {
            return defRet;
        }
        JniClass& classId = Jni::get().getClass(classPath);
        if(!classId.getClass())
        {
            return defRet;
        }
        aex::string signature(createVoidSignature<Args...>(args...));
        jmethodID methodId = classId.getMethodID("<init>", signature.c_str());
        if (!methodId || env->ExceptionCheck())
        {
            env->ExceptionClear();
            defRet.setError(aex::string("Failed to find constructor '"+classPath+"' with signature '"+signature+"'."));
        }
        else
        {
            jvalue* jargs = createArguments(args...);
            jobject obj = env->NewObjectA(classId.getClass(), methodId, jargs);
            if (env->ExceptionCheck())
            {
                env->ExceptionClear();
                defRet.setError(aex::string("Failed to call constructor '"+classPath+"' with signature '"+signature+"'."));
            }
            else
            {
                defRet = JniObject(classPath, obj, &classId);
            }
        }
        return defRet;
    }

    /**
     * Calls an object method
     */
    template<typename Return, typename... Args>
    Return call(const aex::string& name, const Return& defRet, Args&&... args)
    {
        aex::string signature(createSignature(defRet, args...));
        return callSigned(name, signature, defRet, args...);
    }

    template<typename Return, typename... Args>
    Return callSigned(const aex::string& name, const aex::string& signature, const Return& defRet, Args&&... args)
    {
        JNIEnv* env = getEnvironment();
        if(!env)
        {
            return defRet;
        }
        JniClass* classId = getClass();
        if(!classId && !(*classId))
        {
            return defRet;
        }
        jobject objId = getInstance();
        if(!objId)
        {
            return defRet;
        }
        jmethodID methodId = classId->getMethodID(name.c_str(), signature.c_str());
        if (!methodId || env->ExceptionCheck())
        {
            env->ExceptionClear();
            setError(aex::string("Failed to find method '")+name+"' with signature '"+signature+"'.");
            return defRet;
        }
        else
        {
            jvalue* jargs = createArguments(args...);
            Return result;
            callJavaMethod(env, objId, methodId, jargs, result);
            if (env->ExceptionCheck())
            {
                env->ExceptionClear();
                setError(aex::string("Failed to call method '")+name+" with signature '"+signature+"'.");
                return defRet;
            }
            else
            {
                return result;
            }
        }
    }

    /**
     * Calls an object void method
     */
    template<typename... Args>
    void callVoid(const aex::string& name, Args&&... args)
    {
        aex::string signature(createVoidSignature(args...));
        return callSignedVoid(name, signature, args...);
    }

    template<typename... Args>
    void callSignedVoid(const aex::string& name, const aex::string& signature, Args&&... args)
    {
        JNIEnv* env = getEnvironment();
        if(!env)
        {
            return;
        }
        JniClass* classId = getClass();
        if(!classId && !(*classId))
        {
        	setError(aex::string("Could not invoke '")+name+"': class not found.");
            return;
        }
        jobject objId = getInstance();
        if(!objId)
        {
            return;
        }
        jmethodID methodId = classId->getMethodID(name.c_str(), signature.c_str());
        if (!methodId || env->ExceptionCheck())
        {
            env->ExceptionClear();
            setError(aex::string("Failed to find method '")+name+"' with signature '"+signature+"'.");
        }
        else
        {
            jvalue* jargs = createArguments(args...);
            callJavaVoidMethod(env, objId, methodId, jargs);
            if (env->ExceptionCheck())
            {
                env->ExceptionClear();
                setError(aex::string("Failed to call method '")+name+"' with signature '"+signature+"'.");
            }
        }
    }

    /**
     * Calls a class method
     */
    template<typename Return, typename... Args>
    Return staticCall(const aex::string& name, const Return& defRet, Args&&... args)
    {
        aex::string signature(createSignature(defRet, args...));
        return staticCallSigned(name, signature, defRet, args...);
    }

    template<typename Return, typename... Args>
    Return staticCallSigned(const aex::string& name, const aex::string& signature, const Return& defRet, Args&&... args)
    {
        JNIEnv* env = getEnvironment();
        if(!env)
        {
            return defRet;
        }
        JniClass* classId = getClass();
        if(!classId && !(*classId))
        {
            return defRet;
        }
        jmethodID methodId = classId->getStaticMethodID(name.c_str(), signature.c_str());
        if (!methodId || env->ExceptionCheck())
        {
            env->ExceptionClear();
            setError(aex::string("Failed to find static method '")+name+"'.");
            return defRet;
        }
        else
        {
            jvalue* jargs = createArguments(args...);
            Return result = callStaticJavaMethod<Return>(env, classId->getClass(), methodId, jargs);
            if (env->ExceptionCheck())
            {
                env->ExceptionClear();
                setError(aex::string("Failed to call static method '")+name+"'.");
                return defRet;
            }
            else
            {
                return result;
            }
        }
    }

    /**
     * Calls a class void method
     */
    template<typename... Args>
    void staticCallVoid(const aex::string& name, Args&&... args)
    {
        aex::string signature(createVoidSignature(args...));
        return staticCallSignedVoid(name, signature, args...);
    }

    template<typename... Args>
    void staticCallSignedVoid(const aex::string& name, const aex::string& signature, Args&&... args)
    {
        JNIEnv* env = getEnvironment();
        if(!env)
        {
            return;
        }
        JniClass* classId = getClass();
        if(!classId && !(*classId))
        {
            return;
        }
        jmethodID methodId = classId->getStaticMethodID(name.c_str(), signature.c_str());
        if (!methodId || env->ExceptionCheck())
        {
            env->ExceptionClear();
            setError(aex::string("Failed to find static method '")+name+"'.");
            return;
        }
        else
        {
            jvalue* jargs = createArguments(args...);
            callStaticJavaMethod<void>(env, classId->getClass(), methodId, jargs);
            if (env->ExceptionCheck())
            {
                env->ExceptionClear();
                setError(aex::string("Failed to call static method '")+name+"'.");
            }
        }
    }

    /**
     * Get a static class field
     * @param name the field name
     */
    template<typename Return>
    Return staticField(const aex::string& name, const Return& defRet)
    {
        aex::string signature(getSignaturePart<Return>(defRet));
        return staticFieldSigned(name, signature, defRet);
    }

    template<typename Return>
    Return staticFieldSigned(const aex::string& name, const aex::string& signature, const Return& defRet)
    {
        JNIEnv* env = getEnvironment();
        if(!env)
        {
            return defRet;
        }

        JniClass* classId = getClass();
        if(!classId && !(*classId))
        {
            return defRet;
        }

        jfieldID fieldId = classId->getStaticFieldID(name.c_str(), signature.c_str());
        if (!fieldId || env->ExceptionCheck())
        {
            env->ExceptionClear();
            setError(aex::string("Failed to find static field '")+name+"' with signature '"+signature+"'.");
            return defRet;

        }
        else
        {
            Return result = getJavaStaticField<Return>(env, classId->getClass(), fieldId);
            if (env->ExceptionCheck())
            {
                env->ExceptionClear();
                setError(aex::string("Failed to read static field '")+name+"' with signature '"+signature+"'.");
                return defRet;
            }
            else
            {
                return result;
            }
        }
    }

    /**
     * Get a object field
     * @param name the field name
     */
    template<typename Return>
    Return field(const aex::string& name, const Return& defRet)
    {
        aex::string signature(getSignaturePart<Return>(defRet));
        return fieldSigned(name, signature, defRet);
    }

    template<typename Return>
    Return fieldSigned(const aex::string& name, const aex::string& signature, const Return& defRet)
    {
        JNIEnv* env = getEnvironment();
        if(!env)
        {
            return defRet;
        }

        JniClass* classId = getClass();
        if(!classId && !(*classId))
        {
            return defRet;
        }

        jfieldID fieldId = classId->getFieldID(name.c_str(), signature.c_str());
        if (!fieldId || env->ExceptionCheck())
        {
            env->ExceptionClear();
            setError(aex::string("Failed to find field '")+name+"' with signature '"+signature+"'.");
            return defRet;
        }
        else
        {
            Return result = getJavaField<Return>(env, classId->getClass(), fieldId);
            if (env->ExceptionCheck())
            {
                env->ExceptionClear();
                setError(aex::string("Failed to read field '")+name+"' with signature '"+signature+"'.");
                return defRet;

            }
            else
            {
                return result;
            }
        }
    }

    /**
     * Return the signature for the object
     */
    aex::string getSignature() const;

    /**
     * Return the error
     */
    const aex::string& getError() const;

    /**
     * Return true of there is an error
     */
    bool hasError() const;

    /**
     * create an java array of the given type
     */
    template<typename Type>
    static jarray createJavaArray(JNIEnv* env, const Type& element, size_t size);

    /**
     * Convert a jobject array to a container
     */
    template<typename Type>
    static bool convertFromJavaArray(JNIEnv* env, jarray arr, Type& container)
    {
        if(!arr)
        {
            return false;
        }
        jsize arraySize = env->GetArrayLength(arr);
        for(size_t i=0; i<arraySize; i++)
        {
            typename Type::value_type elm;
            convertFromJavaArrayElement(env, arr, i, elm);
            container.insert(container.end(), elm);
        }
        return true;
    }

    template<typename Type>
    static bool convertFromJavaArray(jarray arr, Type& container)
    {
        JNIEnv* env = getEnvironment();
        assert(env);
        return convertFromJavaArray(env, arr, container);
    }

    /**
     * Get an element of a java array
     */
    template<typename Type>
    static bool convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, Type& out);

    template<typename Type>
    static bool convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, std::vector<Type>& out)
    {
        jobject elm;
        if(!convertFromJavaArrayElement(env, arr, position, elm))
        {
            return false;
        }
        return convertFromJavaArray(env, (jarray)elm, out);
    }

    template<typename Key, typename Value>
    static bool convertFromJavaArrayElement(JNIEnv* env, jarray arr, size_t position, std::map<Key, Value>& out)
    {
        jobject elm;
        if(!convertFromJavaArrayElement(env, arr, position, elm))
        {
            return false;
        }
        return convertFromJavaMap(env, elm, out);
    }

    /**
    * Set an element of a java array
    */
    template<typename Type>
    static void setJavaArrayElement(JNIEnv* env, jarray arr, size_t position, const Type& elm);

    template<typename Type>
    static bool convertFromJavaCollection(JNIEnv* env, jobject obj, Type& out)
    {
        if(!obj)
        {
            return false;
        }
        JniObject jcontainer(obj);
        if(!jcontainer.isInstanceOf("java.util.Collection"))
        {
            return false;
        }
        out = jcontainer.call<Type>("toArray", out, out);
        return true;
    }

    template<typename Key, typename Value>
    static bool convertFromJavaMap(JNIEnv* env, jobject obj, std::map<Key, Value>& out)
    {
        if(!obj)
        {
            return false;
        }
        JniObject jmap(obj);
        if(!jmap.isInstanceOf("java.util.Map"))
        {
            return false;
        }
        JniObject jkeys = jmap.call<JniObject>("keySet", JniObject("java.util.Set"));
        if(jkeys.hasError())
        {
            return false;
        }
        std::vector<Key> keys = jkeys.callSigned<std::vector<Key>>("toArray", "()[Ljava/lang/Object;", std::vector<Key>());
        for(typename std::vector<Key>::const_iterator itr = keys.begin(); itr != keys.end(); ++itr)
        {
            Value v = jmap.callSigned<Value>("get", "(Ljava/lang/Object;)Ljava/lang/Object;", Value(), *itr);
            out[*itr] = v;
        }
        return true;
    }

    template<typename Key, typename Value>
    static bool convertToMapFromJavaArray(JNIEnv* env, jarray arr, std::map<Key, Value>& out)
    {
        if(!arr)
        {
            return false;
        }
        jsize mapSize = env->GetArrayLength(arr) / 2;
        for(size_t i=0; i<mapSize; ++i)
        {
            Key k;
            if(convertFromJavaArrayElement(env, arr, i*2, k))
            {
                Value v;
                if(convertFromJavaArrayElement(env, arr, i*2+1, v))
                {
                    out[k] = v;
                }
            }
        }
        return true;
    }

    /**
     * Convert a jobject to the c++ representation
     */
    template<typename Type>
    static bool convertFromJavaObject(JNIEnv* env, jobject obj, Type& out);

    // template specialization for containers
    template<typename Type>
    static bool convertFromJavaObject(JNIEnv* env, jobject obj, std::vector<Type>& out)
    {
        if(convertFromJavaCollection(env, obj, out))
        {
            return true;
        }
        if(convertFromJavaArray(env, (jarray)obj, out))
        {
            return true;
        }
        return false;
    }
    template<typename Type>
    static bool convertFromJavaObject(JNIEnv* env, jobject obj, std::set<Type>& out)
    {
        if(convertFromJavaCollection(env, obj, out))
        {
            return true;
        }
        if(convertFromJavaArray(env, (jarray)obj, out))
        {
            return true;
        }
        return false;
    }
    template<typename Type, int Size>
    static bool convertFromJavaObject(JNIEnv* env, jobject obj, std::array<Type, Size>& out)
    {
        if(convertFromJavaCollection(env, obj, out))
        {
            return true;
        }
        if(convertFromJavaArray(env, (jarray)obj, out))
        {
            return true;
        }
        return false;
    }
    template<typename Type>
    static bool convertFromJavaObject(JNIEnv* env, jobject obj, std::list<Type>& out)
    {
        if(convertFromJavaCollection(env, obj, out))
        {
            return true;
        }
        if(convertFromJavaArray(env, (jarray)obj, out))
        {
            return true;
        }
        return false;
    }

    template<typename Key, typename Type>
    static bool convertFromJavaObject(JNIEnv* env, jobject obj, std::map<Key, Type>& out)
    {
        if(convertFromJavaMap(env, obj, out))
        {
            return true;
        }
        if(convertToMapFromJavaArray(env, (jarray)obj, out))
        {
            return true;
        }
        return false;
    }

    static bool convertFromJavaObject(JNIEnv* env, jobject obj, std::string& out) {
        if(!obj) {
            return false;
        }

        if(!(JniObject(obj).isInstanceOf("java.lang.String"))) {
            return false;
        }
        jstring jstr = jstring(obj);
        const char *nStr = env->GetStringUTFChars(jstr, JNI_FALSE);
        out = std::string(nStr);
        env->ReleaseStringUTFChars(jstr, nStr);
        return true;
    }


    // utility methods that return the object
    template<typename Type>
    static Type convertFromJavaObject(JNIEnv* env, jobject obj)
    {
        Type out;
        bool result = convertFromJavaObject(env, obj, out);
        (void)(result);
        assert(result);
        return out;
    }
    template<typename Type>
    static Type convertFromJavaObject(jobject obj)
    {
        JNIEnv* env = getEnvironment();
        assert(env);
        return convertFromJavaObject<Type>(env, obj);
    }
    template<typename Type>
    static bool convertFromJavaObject(jobject obj, Type& out)
    {
        JNIEnv* env = getEnvironment();
        assert(env);
        return convertFromJavaObject(env, obj, out);
    }

    /**
     * Convert a c++ list container to a jarray
     */
    template<typename Type>
    static jarray createJavaArray(const Type& obj)
    {
        JNIEnv* env = getEnvironment();
        if (!env)
        {
            return nullptr;
        }
        jarray arr = nullptr;
        if(obj.empty())
        {
            arr = createJavaArray(env, typename Type::value_type(), 0);
        }
        else
        {
            arr = createJavaArray(env, *obj.begin(), obj.size());
        }
        size_t i = 0;
        for(typename Type::const_iterator itr = obj.begin(); itr != obj.end(); ++itr)
        {
            setJavaArrayElement(env, arr, i, *itr);
            i++;
        }
        return arr;
    }

    template<typename Key, typename Value>
    static JniObject createJavaMap(const std::map<Key, Value>& obj, const aex::string& classPath="java/util/HashMap")
    {
        JniObject jmap(JniObject::createNew(classPath));
        for(typename std::map<Key, Value>::const_iterator itr = obj.begin(); itr != obj.end(); ++itr)
        {
        	Value v = jmap.callSigned("put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;", Value(), itr->first, itr->second);
        }
        return jmap;
    }

    template<typename Type>
    static JniObject createJavaList(const Type& obj, const aex::string& classPath="java/util/ArrayList")
    {
        JniObject jlist(JniObject::createNew(classPath));
        for(typename Type::const_iterator itr = obj.begin(); itr != obj.end(); ++itr)
        {
            jlist.callSignedVoid("add", "(Ljava/lang/Object;)Z", *itr);
        }
        return jlist;
    }

    template<typename Type>
    static JniObject createJavaSet(const Type& obj, const aex::string& classPath="java/util/HashSet")
    {
        return createJavaList(obj, classPath);
    }

    /**
     * Convert a c++ type to the jvalue representation
     * This is called on all jni arguments
     */
    template<typename Type>
    static jvalue convertToJavaValue(const Type& obj);

    // template specialization for pointers
    template<typename Type>
    static jvalue convertToJavaValue(Type* obj)
    {
        return convertToJavaValue((jlong)obj);
    }

    // template specialization for containers
    template<typename Type>
    static jvalue convertToJavaValue(const std::vector<Type>& obj)
    {
        return convertToJavaValue(createJavaArray(obj));
    }
    template<typename Type>
    static jvalue convertToJavaValue(const std::set<Type>& obj)
    {
        return convertToJavaValue(createJavaArray(obj));
    }
    template<typename Type, int Size>
    static jvalue convertToJavaValue(const std::array<Type, Size>& obj)
    {
        return convertToJavaValue(createJavaArray(obj));
    }
    template<typename Type>
    static jvalue convertToJavaValue(const std::list<Type>& obj)
    {
        return convertToJavaValue(createJavaArray(obj));
    }
    template<typename Key, typename Value>
    static jvalue convertToJavaValue(const std::map<Key, Value>& obj)
    {
        return convertToJavaValue(createJavaMap(obj).getNewLocalInstance());
    }

    /**
     * Convert JniObject back to jobject.
     */
    static inline jvalue convertToJavaValue(const JniObject& obj) {
        return jvalue{.l = obj.getInstance()};
    }

    /**
     * Returns the class reference. This is a global ref that will be removed
     * when the JniObject is destroyed
     */
    JniClass* getClass() const;

    /**
     * Returns the class path. If it is not there it tries to call
     * `getClass().getName()` on the object to get the class
     */
    const aex::string& getClassPath() const;

    /**
     * Returns the jobject reference. This is a global ref that will be removed
     * when the JniObject is destroyed
     */
    jobject getInstance() const;

    /**
    * Returns the jobject reference. This is a new local ref
    */
    jobject getNewLocalInstance() const;

    /**
     * Return true if class path and class ref match
     */
    bool isInstanceOf(const aex::string& classPath) const;

    /**
     * Returns true if there is an object instance
     */
    operator bool() const;

    /**
     * Copy a jni object
     */
    JniObject& operator=(const JniObject& other);

    /**
     * Compare two jni objects
     */
    bool operator==(const JniObject& other) const;

    /**
     * Get Android Activity object.
     */
    static JniObject getAndroidActivity();
    static JniObject getAndroidActivityAs(const aex::string& classPath);
    static JniObject getAndroidActivityAsActivity();
};

AEX_PUBLIC_API JniObject GetAndroidActivity();
AEX_PUBLIC_API JniObject GetAndroidActivityAs(const aex::string& classPath);
AEX_PUBLIC_API JniObject GetAndroidActivityAsActivity();
AEX_PUBLIC_API std::string GetPackageName();

}

#endif // __ANDROID__
#endif
