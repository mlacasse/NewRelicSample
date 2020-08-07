#if defined(YI_IOS) || defined(YI_TVOS)

#import "AppDelegate.h"

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#import <NewRelicAgent/NewRelic.h>

#define LOG_TAG "AppDelegate"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions {
    [NewRelic enableFeatures:NRFeatureFlag_NetworkRequestEvents | NRFeatureFlag_NSURLSessionInstrumentation | NRFeatureFlag_RequestErrorEvents | NRFeatureFlag_HttpResponseBodyCapture];
       [NewRelicAgent startWithApplicationToken:@"AA662f1e28999aa0681eb791431280129167a15886-NRMA"];
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
    return true;
    }

- (void)applicationDidBecomeActive:(UIApplication *)application{
}

- (void)applicationWillResignActive:(UIApplication *)application{
}

- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions {
    return true;
}

@end

#endif
