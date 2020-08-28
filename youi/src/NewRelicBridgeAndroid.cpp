#ifdef YI_ANDROID

#include "NewRelicBridge.h"
#include "UtilityBridgesAndroid.h"
#include "JniGlobalRefsManager.h"

#include <youireact/NativeModuleRegistry.h>

#include <string>

extern jobject  cachedActivity;
extern JNIEnv *GetEnv();

YI_RN_INSTANTIATE_MODULE(NewRelicBridgeModule);

YI_RN_REGISTER_MODULE(NewRelicBridgeModule);

// YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, startWithApplicationToken)(std::string token)
// {
//     JNIEnv *pEnv = GetEnv();
//     jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
//     jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "startWithApplicationToken", "(Ljava/lang/String;Landroid/content/Context;)V");
//     jstring jToken = pEnv->NewStringUTF(token.c_str());
//     pEnv->CallStaticVoidMethod(bridgeClass, methodID, jToken, cachedActivity);
// }

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setApplicationVersion)(std::string version)
{
    JNIEnv *pEnv = GetEnv();
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "setApplicationVersion", "(Ljava/lang/String;)V");
    jstring jVersion = pEnv->NewStringUTF(version.c_str());
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, jVersion);
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setMaxEventBufferTime)(int32_t time)
{
    JNIEnv *pEnv = GetEnv();
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "setMaxEventBufferTime", "(I)V");
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, static_cast<jint>(time));
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setMaxEventPoolSize)(int32_t size)
{
    JNIEnv *pEnv = GetEnv();
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "setMaxEventPoolSize", "(I)V");
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, static_cast<jint>(size));
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setLogLevels)(int32_t level)
{
    JNIEnv *pEnv = GetEnv();
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "setLogLevels", "(I)V");
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, static_cast<jint>(level));
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, recordCustomEvent)(std::string eventName, folly::dynamic attributes)
{
    JNIEnv *pEnv = GetEnv();
    JniGlobal::Ptr jniGlobal = JniBridges::getDictionaryObject(attributes);
    if (jniGlobal)
    {
        jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
        jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "recordCustomEvent", "(Ljava/lang/String;Ljava/util/Map;)V");
        jstring jEventName = pEnv->NewStringUTF(eventName.c_str());
        pEnv->CallStaticVoidMethod(bridgeClass, methodID, jEventName, jniGlobal->get());
    }
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setAttribute)(std::string name, std::string value)
{
    JNIEnv *pEnv = GetEnv();
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "setAttribute", "(Ljava/lang/String;Ljava/lang/String;)V");
    jstring jName = pEnv->NewStringUTF(name.c_str());
    jstring jValue = pEnv->NewStringUTF(value.c_str());
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, jName, jValue);
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashNow)()
{
    JNIEnv *pEnv = GetEnv();
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "crashNow", "()V");
    pEnv->CallStaticVoidMethod(bridgeClass, methodID);
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashSync)(std::string message)
{
    JNIEnv *pEnv = GetEnv();
    jstring errorMessage = pEnv->NewStringUTF(message.c_str());
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "crashSync", "(Ljava/lang/String;)V");
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, errorMessage);

}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashAsync)(std::string message)
{
    JNIEnv *pEnv = GetEnv();
    jstring errorMessage = pEnv->NewStringUTF(message.c_str());
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "crashAsync", "(Ljava/lang/String;)V");
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, errorMessage);
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashJS)(std::string name,std::string message, std::string line, std::string column, std::string isFatal, std::string stack, std::string sourceURL)
{
    JNIEnv *pEnv = GetEnv();
    jstring errorName = pEnv->NewStringUTF(name.c_str());
    jstring errorMessage = pEnv->NewStringUTF(message.c_str());
    jstring errorLine = pEnv->NewStringUTF(line.c_str());
    jstring errorColumn = pEnv->NewStringUTF(column.c_str());
    jstring errorIsFatal = pEnv->NewStringUTF(isFatal.c_str());
    jstring errorStack = pEnv->NewStringUTF(stack.c_str());
    jstring errorSourceURL = pEnv->NewStringUTF(sourceURL.c_str());
    jclass bridgeClass = pEnv->FindClass("tv/youi/NewRelicBridge");
    jmethodID methodID = pEnv->GetStaticMethodID(bridgeClass, "crashJS", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    pEnv->CallStaticVoidMethod(bridgeClass, methodID, errorName,errorMessage,errorLine,errorColumn,errorIsFatal,errorStack,errorSourceURL);
}

#endif
