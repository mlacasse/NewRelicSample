#if !defined(YI_IOS) && !defined(YI_TVOS) && !defined(YI_ANDROID)

#include "NewRelicBridge.h"

#include <youireact/NativeModuleRegistry.h>

YI_RN_INSTANTIATE_MODULE(NewRelicBridgeModule);

YI_RN_REGISTER_MODULE(NewRelicBridgeModule);

// YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, startWithApplicationToken)(std::string)
// {
//     YI_LOGI("NewRelic", "Stubbed startWithApplicationToken.");
// }

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, recordCustomEvent)(std::string, folly::dynamic)
{
    YI_LOGI("NewRelic", "Stubbed recordCustomEvent.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setAttribute)(std::string name, std::string value)
{
    YI_LOGI("NewRelic", "Stubbed setAttribute.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setApplicationVersion)(std::string version)
{
    YI_LOGI("NewRelic", "Stubbed setApplicationVersion.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setMaxEventBufferTime)(int32_t time)
{
    YI_LOGI("NewRelic", "Stubbed setMaxEventBufferTime.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setMaxEventPoolSize)(int32_t size)
{
    YI_LOGI("NewRelic", "Stubbed setMaxEventPoolSize.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setLogLevels)(int32_t level)
{
    YI_LOGI("NewRelic", "Stubbed setLogLevels.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashNow)()
{
    YI_LOGI("NewRelic", "Stubbed crashNow.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashSync)(std::string)
{
    YI_LOGI("NewRelic", "Stubbed crashSync.");
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashAsync)(std::string)
{
    YI_LOGI("NewRelic", "Stubbed crashAsync.");
}

#endif
