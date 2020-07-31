#ifndef _YOUIREACT_NETWORKING_REQUEST_PROPS_H_
#define _YOUIREACT_NETWORKING_REQUEST_PROPS_H_

#include <youireact/JsEnum.h>
#include <youireact/Predef.h>

#include <network/YiHTTPRequest.h>

#include <folly/dynamic.h>

namespace yi
{
namespace react
{
struct NetworkingRequestProps
{
    // clang-format off
    YI_RN_DEFINE_ENUM(Method,
        GET,
        POST,
        PUT,
        DELETE,
        PATCH,
        HEAD
    );
    // clang-format on

    // clang-format off
    YI_RN_DEFINE_ENUM(ResponseType,
        text,
        base64,
        blob
    );
    // clang-format on

    CYIUrl url;
    Method method;
    ResponseType responseType;
    uint32_t timeout = 0;
    bool withCredentials = false;
    std::map<CYIString, CYIString> headers;
    std::map<CYIString, CYIString> data;
};

bool InitFromValue(NetworkingRequestProps &request, const folly::dynamic &value);

} // namespace react
} // namespace yi

#endif
