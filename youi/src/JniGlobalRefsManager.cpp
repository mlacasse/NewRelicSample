#ifdef YI_ANDROID

#include "JniGlobalRefsManager.h"
#include "UtilityBridgesAndroid.h"

extern JNIEnv *GetEnv();

folly::dynamic JniGlobalRefsManager::filterArrayEllement(const folly::dynamic &dyn) {
    switch (dyn.type()) {
        case folly::dynamic::NULLT:
            return nullptr;
        case folly::dynamic::BOOL:
            return dyn.asBool();
        case folly::dynamic::INT64:
            return dyn.asInt();
        case folly::dynamic::DOUBLE:
            return dyn.asDouble();
        case folly::dynamic::STRING:
            return dyn.asString();
        case folly::dynamic::ARRAY: {
            folly::dynamic array = folly::dynamic::array();
            for (auto &elem : dyn) {
                array.push_back(filterArrayEllement(elem));
            }
            return array;
        }
        case folly::dynamic::OBJECT: {
            folly::dynamic objects = folly::dynamic::object();
            for (auto &elem : dyn.items()) {
                objects[filterArrayEllement(elem.first)] = filterArrayEllement(elem.second);
            }
            return objects;
        }
        default:
            break;
    }
    return dyn;
}

folly::dynamic JniGlobalRefsManager::filterArrayEllement0(const folly::dynamic &dyn) {
    switch (dyn.type()) {
        case folly::dynamic::NULLT:
            return nullptr;
        case folly::dynamic::BOOL:
            return dyn.asBool();
        case folly::dynamic::INT64:
            return dyn.asInt();
        case folly::dynamic::DOUBLE:
            return dyn.asDouble();
        case folly::dynamic::STRING:
            return dyn.asString();
        case folly::dynamic::ARRAY: {
            folly::dynamic array = folly::dynamic::array();
            for (auto &elem : dyn) {
                array.push_back(filterArrayEllement(elem));
            }
            return array[0];
        }
        case folly::dynamic::OBJECT: {
            folly::dynamic objects = folly::dynamic::object();
            for (auto &elem : dyn.items()) {
                objects[filterArrayEllement(elem.first)] = filterArrayEllement(elem.second);
            }
            return objects;
        }
        default:
            break;
    }
    return dyn;
}

JniGlobalRefsManager::JniGlobalRefsManager(const folly::dynamic &dyn):
      mGlobalRef(nullptr)
    , mIsJsonKind(false)
{
    follyDynamicObjectToJavaHashMap(dyn);
}

JniGlobalRefsManager::~JniGlobalRefsManager()
{
    if (mGlobalRef)
    {
        JNIEnv *env = GetEnv();
        env->DeleteGlobalRef(mGlobalRef);
    }
}

bool JniGlobalRefsManager::isJsonKind() const
{
    return mIsJsonKind;
}

jobject JniGlobalRefsManager::get() const
{
    return mGlobalRef;
}

void JniGlobalRefsManager::follyDynamicObjectToJavaHashMap(const folly::dynamic &dyn) {
    JNIEnv *env = GetEnv();
    jclass longClass = env->FindClass("java/lang/Long");
    jmethodID initLong = env->GetMethodID(longClass, "<init>", "(J)V");
    jclass doubleClass = env->FindClass("java/lang/Double");
    jmethodID doubleInit = env->GetMethodID(doubleClass, "<init>", "(D)V");
    jclass boolClass = env->FindClass("java/lang/Boolean");
    jmethodID boolInit = env->GetMethodID(boolClass, "<init>", "(Z)V");

    folly::dynamic result = filterArrayEllement0(dyn);
    mIsJsonKind = (result.type() == folly::dynamic::OBJECT);
    //do not deal with arrays inside objects, if so probably better use java
    if (mIsJsonKind)
    {
        jclass mapClass = env->FindClass("java/util/HashMap");
        jmethodID init = env->GetMethodID(mapClass, "<init>", "()V");
        jobject hashMap = env->NewObject(mapClass, init);
        jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        for (auto &obj : result.items()) {
            jstring jKey = env->NewStringUTF(obj.first.getString().c_str());

            switch (obj.second.type()) {
                case folly::dynamic::INT64: {
                    jobject jValue = env->NewObject(longClass, initLong, static_cast<jlong>(obj.second.getInt()));
                    env->CallObjectMethod(hashMap, put, jKey, jValue);
                    env->DeleteLocalRef(jValue);
                    break;
                }
                case folly::dynamic::DOUBLE: {
                    jobject jValue = env->NewObject(doubleClass, doubleInit, static_cast<jdouble>(obj.second.getDouble()));
                    env->CallObjectMethod(hashMap, put, jKey, jValue);
                    env->DeleteLocalRef(jValue);
                    break;
                }
                case folly::dynamic::BOOL: {
                    jobject jValue = env->NewObject(boolClass, boolInit, static_cast<jboolean>(obj.second.getBool()));
                    env->CallObjectMethod(hashMap, put, jKey, jValue);
                    env->DeleteLocalRef(jValue);
                    break;
                }
                case folly::dynamic::STRING: {
                    jobject jValue = env->NewStringUTF(obj.second.getString().c_str());
                    env->CallObjectMethod(hashMap, put, jKey, jValue);
                    env->DeleteLocalRef(jValue);
                    break;
                }
                case folly::dynamic::ARRAY: {
                    //rely on java
                    jobject jValue = env->NewStringUTF(folly::toJson(obj.second).c_str());
                    env->CallObjectMethod(hashMap, put, jKey, jValue);
                    env->DeleteLocalRef(jValue);
                    break;
                }
                case folly::dynamic::NULLT:
                default:
                    break;
            }
            env->DeleteLocalRef(jKey);
        }
        mGlobalRef = env->NewGlobalRef(hashMap);
        env->DeleteLocalRef(hashMap);
        env->DeleteLocalRef(mapClass);
    }
    else
    {
        //object type
        jobject jValue = nullptr;
        switch (result.type()) {
            case folly::dynamic::INT64: {
                jValue = env->NewObject(longClass, initLong, static_cast<jlong>(result.getInt()));
                break;
            }
            case folly::dynamic::DOUBLE: {
                jValue = env->NewObject(doubleClass, doubleInit, static_cast<jdouble>(result.getDouble()));
                break;
            }
            case folly::dynamic::BOOL: {
                jValue = env->NewObject(boolClass, boolInit, static_cast<jboolean>(result.getBool()));
                break;
            }
            case folly::dynamic::STRING: {
                jValue = env->NewStringUTF(result.getString().c_str());
                break;
            }
            case folly::dynamic::NULLT:
            default:
                break;
        }
        if (jValue != nullptr)
        {
            mGlobalRef = env->NewGlobalRef(jValue);
            env->DeleteLocalRef(jValue);
        }
    }
}

#endif

