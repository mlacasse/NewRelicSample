#ifndef UtilityiOS_h
#define UtilityiOS_h

#include <folly/dynamic.h>
#import <Foundation/Foundation.h>

@interface UtilityiOS : NSObject

id convertFollyDynamic(const folly::dynamic &dyn);
folly::dynamic convertIdToFollyDynamic(id json);
id convertFollyDynamic2(const folly::dynamic &dyn);
bool isRunningiOSSimulator();

@end

#endif /* UtilityiOS_h */
