#ifndef _JNI_GLOBALREFS_MANAGER_H_
#define _JNI_GLOBALREFS_MANAGER_H_

#ifdef YI_ANDROID

#include <folly/json.h>
#include <folly/dynamic.h>
#include <jni.h>

class JniGlobalRefsManager
{
public:
    JniGlobalRefsManager(const folly::dynamic &dyn);
    ~JniGlobalRefsManager();
    typedef std::shared_ptr<JniGlobalRefsManager> Ptr;

    jobject get() const;
    bool isJsonKind() const;
    static folly::dynamic filterArrayEllement(const folly::dynamic &dyn);
    static folly::dynamic filterArrayEllement0(const folly::dynamic &dyn);
private:
    void follyDynamicObjectToJavaHashMap(const folly::dynamic &dyn);

    jobject mGlobalRef;
    bool mIsJsonKind;
};
typedef JniGlobalRefsManager JniGlobal;

#endif

#endif  //_JNI_GLOBALREFS_MANAGER_H_

