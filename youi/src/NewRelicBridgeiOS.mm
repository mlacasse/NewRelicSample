#include "NewRelicBridge.h"
#include "UtilityiOS.h"

#if defined(YI_IOS)
#import <NewRelicAgent/NewRelic.h>
#else
#import <NewRelicAgentTVOS/NewRelic.h>
#endif

YI_RN_INSTANTIATE_MODULE(NewRelicBridgeModule);

YI_RN_REGISTER_MODULE(NewRelicBridgeModule);

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setApplicationVersion)(std::string version)
{
    [NewRelic setApplicationVersion:convertFollyDynamic(version)];
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, startWithApplicationToken)(std::string token)
{
    [NewRelic enableFeatures:NRFeatureFlag_NetworkRequestEvents | NRFeatureFlag_NSURLSessionInstrumentation | NRFeatureFlag_RequestErrorEvents | NRFeatureFlag_HttpResponseBodyCapture];
    [NewRelicAgent startWithApplicationToken:[NSString stringWithUTF8String:token.c_str()]];
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setMaxEventBufferTime)(int32_t time)
{
    [NewRelic setMaxEventBufferTime:(unsigned int)time];
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setMaxEventPoolSize)(int32_t size)
{
    [NewRelic setMaxEventPoolSize:(unsigned int)size];
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setLogLevels)(int32_t levels)
{
    [NRLogger setLogLevels:(unsigned int)levels];
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, recordCustomEvent)(std::string eventName, folly::dynamic attributes)
{
    [NewRelicAgent recordCustomEvent:@"DFW_Mobile" name:[NSString stringWithUTF8String:eventName.c_str()] attributes:convertFollyDynamic(attributes)];
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, setAttribute)(std::string name, std::string value)
{
    [NewRelic setAttribute:[NSString stringWithUTF8String:name.c_str()] value:[NSString stringWithUTF8String:value.c_str()]];
}

YI_RN_DEFINE_EXPORT_METHOD(NewRelicBridgeModule, crashNow)()
{
    [NewRelicAgent crashNow:@"This is a test crash on iOS/tvOS."];
}


