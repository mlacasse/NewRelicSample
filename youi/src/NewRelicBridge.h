#ifndef _NEW_RELIC_BRIDGE_H_
#define _NEW_RELIC_BRIDGE_H_

#include "youireact/NativeModule.h"

#include <folly/dynamic.h>

class YI_RN_MODULE(NewRelicBridgeModule)
{
public:
    YI_RN_EXPORT_NAME(NewRelicBridge);

    // NOTE: Override the reported version by calling this method *before* calling startWithApplicationToken:.
    YI_RN_EXPORT_METHOD(setApplicationVersion)(std::string version);
    // YI_RN_EXPORT_METHOD(startWithApplicationToken)(std::string token);
    YI_RN_EXPORT_METHOD(setMaxEventBufferTime)(int32_t time);
    YI_RN_EXPORT_METHOD(setMaxEventPoolSize)(int32_t size);
    YI_RN_EXPORT_METHOD(setLogLevels)(int32_t level);
    YI_RN_EXPORT_METHOD(recordCustomEvent)(std::string eventName, folly::dynamic attributes);
    YI_RN_EXPORT_METHOD(setAttribute)(std::string name, std::string value);
    YI_RN_EXPORT_METHOD(crashNow)();
    YI_RN_EXPORT_METHOD(crashSync)(std::string message);
    YI_RN_EXPORT_METHOD(crashAsync)(std::string message);
    YI_RN_EXPORT_METHOD(crashJS)(std::string name,std::string message, std::string line, std::string column, std::string isFatal, std::string stack, std::string sourceURL);
};

#endif // _NEW_RELIC_BRIDGE_H_
