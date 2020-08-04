#if defined(YI_IOS) || defined(YI_TVOS)

#import "AppDelegate.h"

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#define LOG_TAG "AppDelegate"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions {
[NewRelic enableFeatures:NRFeatureFlag_NetworkRequestEvents | NRFeatureFlag_NSURLSessionInstrumentation | NRFeatureFlag_RequestErrorEvents | NRFeatureFlag_HttpResponseBodyCapture];
    [NewRelicAgent startWithApplicationToken:@"AA216ffa362de5f23adaec2ab8c09d1db36102ea91"];
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
