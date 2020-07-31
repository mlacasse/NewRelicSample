#if defined(YI_IOS) || defined(YI_TVOS)

// © You i Labs Inc. 2000-2019. All rights reserved.
#undef YI_NETWORK_LOGGER_ENABLED

#import "logging/YiLoggerConfiguration.h"
#import "AppDelegate.h"

int main(int argc, char *argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#endif
