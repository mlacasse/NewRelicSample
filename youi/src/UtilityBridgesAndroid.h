#ifndef _UTILITY_BRIDGES_ANDROID_H_
#define _UTILITY_BRIDGES_ANDROID_H_

#ifdef YI_ANDROID

#include "JniGlobalRefsManager.h"

#include <jni.h>
#include <android/log.h>

struct JniBridges
{
    virtual ~JniBridges(){}
    virtual void initRef(const jobject& thiz) = 0;
    static JniGlobal::Ptr getDictionaryObject(const folly::dynamic &dyn);
    static std::shared_ptr<std::string> getStringFromJString(const jstring& jStr);
    static void resetPtr(const jobject& mRef);
    static void initJavaBridge(const std::string& bridgeName);
    static void setRef(long long ptr, const std::string& fieldName, const std::string& className);
    static void set(JNIEnv *env, const jobject& obj, long long value);
};

# define LOGD(tag,...) __android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
# define LOGE(tag,...) __android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__)

#endif

#endif  //_UTILITY_BRIDGES_ANDROID_H_

