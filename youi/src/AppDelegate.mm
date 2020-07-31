#if defined(YI_IOS) || defined(YI_TVOS)

#import "AppDelegate.h"

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

#define LOG_TAG "AppDelegate"

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions {
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
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
