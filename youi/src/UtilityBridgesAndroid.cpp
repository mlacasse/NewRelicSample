#ifdef YI_ANDROID

#include "youireact/NativeModule.h"
#include "UtilityBridgesAndroid.h"

#define UNICODE_SUBSTITUTION_CHAR 0xFFFD
#define TAG "UtilityBridgesAndroid"

extern JavaVM  *cachedJVM;
extern jobject cachedActivity;

extern "C"
{
}
/**
 * Helper to get jvm jnienv object to open jni to
 * java channel
 * @return The env jvm environment to be used on jni to java calls
 */
JNIEnv *GetEnv()
{
    JNIEnv *pEnv;
    cachedJVM->GetEnv(reinterpret_cast<void**>(&pEnv), JNI_VERSION_1_6);

    return pEnv;
}
/**
 * Helper method to manipulate folly::dynamic data
 * @param dyn The folly::dynamic data to be manipulated
 * @return An object that will take care of global reference returned object
 */
JniGlobal::Ptr JniBridges::getDictionaryObject(const folly::dynamic &dyn)
{
    JniGlobal::Ptr jniGlobal(new JniGlobal(dyn));
    if (jniGlobal->get() && jniGlobal->isJsonKind())
    {
        return jniGlobal;
    }
    return JniGlobal::Ptr();
}
/**
 * Helper method for jstring to std::string manipulation
 * @param jStr jstring jni object
 * @return Returns shared_ptr of string extracted out of jstring
 * to be held on heap (cached)
 */
std::shared_ptr<std::string> JniBridges::getStringFromJString(const jstring& jStr)
{
    std::shared_ptr<std::string> result;
    if (jStr != nullptr)
    {
        JNIEnv *env = GetEnv();
        const char * utfChar = env->GetStringUTFChars(jStr, 0);
        result.reset(new std::string(utfChar));
        env->ReleaseStringUTFChars(jStr, utfChar);
    }
    return result;
}
/**
 * Helper method to clear java listener muting when cpp objects
 * gets destroyed
 * @param mRef The field reference holding the cpp pointer mirrored on java
 */
void JniBridges::resetPtr(const jobject& mRef)
{
    JNIEnv *env = GetEnv();
    if (env == nullptr || mRef == nullptr)
    {
        return;
    }

    jclass _class = env->GetObjectClass(mRef);
    if (_class == nullptr)
    {
        return;
    }

    jmethodID meth = env->GetMethodID(_class, "resetPtr", "()V");
    if (meth == nullptr)
    {
        return;
    }

    env->CallVoidMethod(mRef, meth, 0);
    env->DeleteGlobalRef(mRef);
}
/**
 * Helper method for setting field references ptr values to be held in java
 * @param env java environment pointer
 * @param obj the JniBridges java object
 * @param value the mapped cpp pointer to be held by java field
 */
void JniBridges::set(JNIEnv *env, const jobject& obj, long long value)
{
    if (env == nullptr)
    {
        return;
    }
    jclass _class = env->GetObjectClass(obj);
    if (_class == nullptr)
    {
        return;
    }

    jmethodID meth = env->GetMethodID(_class, "set", "(J)V");
    if (meth == nullptr)
    {
        return;
    }

    env->CallVoidMethod(obj, meth, (jlong)value);
}
/**
 * Calls inside the java bridge class the method to initialize
 * a specific class on java side(egg JniBridges::initJavaBridge("initCastController")
 * initializes the cast specific java class adapter)
 * @param bridgeName The method to call for initialization (egg initCastController)
 * on cast adapter
 */
void JniBridges::initJavaBridge(const std::string& bridgeName)
{
    JNIEnv *env = GetEnv();
    if (env == nullptr)
    {
        return;
    }

    jclass activityClass = env->GetObjectClass(cachedActivity);
    if (activityClass == nullptr)
    {
        return;
    }

    jfieldID fid = env->GetFieldID(activityClass, "reactBridgePaths", "Ltv/youi/clientapp/ReactBridgePaths;");
    if(fid == nullptr)
    {
        return;
    }
    jobject reactObj = env->GetObjectField(cachedActivity, fid);
    if(reactObj == nullptr)
    {
        return;
    }
    jclass reactClass = env->GetObjectClass(reactObj);
    if (reactClass == nullptr)
    {
        return;
    }
    jmethodID reactMeth = env->GetMethodID(reactClass, bridgeName.c_str(), "()V");
    if (reactMeth == nullptr)
    {
        return;
    }

    env->CallVoidMethod(
            reactObj,
            reactMeth);
}

/**
 * Set a cpp pointer to be referenced in java coresponding class
 * @param ptr The cpp pointer to be referenced
 * @param fieldName The java bridge field name or member data of java class
 * @param className The java class holding the field name or member data
 */
void JniBridges::setRef(long long ptr, const std::string& fieldName, const std::string& className)
{
    JNIEnv *env = GetEnv();
    //env
    if (env == nullptr)
    {
        return;
    }
    //appActivity class
    jclass activityClass = env->GetObjectClass(cachedActivity);
    if (activityClass == nullptr)
    {
        return;
    }
    //reactBridgePaths filed in AppActivity class
    jfieldID fid = env->GetFieldID(activityClass, "reactBridgePaths", "Ltv/youi/clientapp/ReactBridgePaths;");
    if(fid == nullptr)
    {
        return;
    }
    //ReactBridgePaths object
    jobject reactObj = env->GetObjectField(cachedActivity, fid);

    if(reactObj == nullptr)
    {
        return;
    }
    //ReactBridgePaths class
    jclass reactClass = env->GetObjectClass(reactObj);

    if (reactClass == nullptr)
    {
        return;
    }
    //jniBridges field of JniBridges class
    jfieldID fidJniBridges = env->GetFieldID(reactClass, "jniBridges", "Ltv/youi/clientapp/JniBridges;");
    if(fidJniBridges == nullptr)
    {
        return;
    }
    //jniBridges object
    jobject jniBridgesObj = env->GetObjectField(reactObj, fidJniBridges);
    if(jniBridgesObj == nullptr)
    {
        return;
    }
    //JniBridges class
    jclass bridgesClass = env->GetObjectClass(jniBridgesObj);
    if(bridgesClass == nullptr)
    {
        return;
    }
    //extract custom field from JniBridges class
    jfieldID fidBridge = env->GetFieldID(bridgesClass, fieldName.c_str(), className.c_str());
    if(fidBridge == nullptr)
    {
        return;
    }
    //get the object of that field
    jobject jniBridgeObj = env->GetObjectField(jniBridgesObj, fidBridge);
    if(jniBridgeObj == nullptr)
    {
        return;
    }
    //set the ptr in java for class
    set(env, jniBridgeObj, ptr);
}

#endif

