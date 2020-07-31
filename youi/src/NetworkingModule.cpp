#include "NetworkingModule.h"

#include <framework/YiApp.h>
#include <framework/YiAppContext.h>
#include <network/YiHTTPRequest.h>
#include <network/YiHTTPResponse.h>
#include <network/YiSSLRootCertificateProvider.h>
#include <utility/YiUtilities.h>

#include <youireact/NativeModuleRegistry.h>

using namespace yi::react;

#define TAG "MyNetworkingModule"

static const std::string DID_COMPLETE_NETWORK_RESPONSE_EVENT = "didCompleteNetworkResponse";
static const std::string DID_RECEIVE_NETWORK_RESPONSE_EVENT = "didReceiveNetworkResponse";
static const std::string DID_SEND_NETWORK_DATA_EVENT = "didSendNetworkData";
static const std::string DID_RECEIVE_INCREMENTAL_DATA_EVENT = "didReceiveNetworkIncrementalData";
static const std::string DID_RECEIVE_NETWORK_DATA_PROGRESS_EVENT = "didReceiveNetworkDataProgress";
static const std::string DID_RECEIVE_NETWORK_DATA_EVENT = "didReceiveNetworkData";

static const std::string REQUEST_TIMED_OUT_ERROR_MESSAGE = "Network request timed out";
static const std::string REQUEST_FAILED_FOR_UNKNOWN_REASON_MESSAGE = "Network request failed for unknown reason";

inline bool IsStatusCodeUnknown(CYIHTTPService::HTTPStatusCode statusCode)
{
    return statusCode == CYIHTTPService::HTTPStatusCode::None || statusCode == CYIHTTPService::HTTPStatusCode::Undefined;
}

inline bool IsStatusCodeTimeout(CYIHTTPService::HTTPStatusCode statusCode)
{
    return statusCode == CYIHTTPService::HTTPStatusCode::YouiNetworkTimedOut || statusCode == CYIHTTPService::HTTPStatusCode::YouiConnectionTimedOut;
}

inline bool IsStatusCodeError(CYIHTTPService::HTTPStatusCode statusCode)
{
    return (size_t)statusCode >= 400 && (size_t)statusCode < 600;
}

static void SetUrl(CYIHTTPRequest &request, const NetworkingRequestProps &requestProps)
{
    request.SetURL(CYIUrl(requestProps.url));
}

static void SetMethod(CYIHTTPRequest &request, const NetworkingRequestProps &requestProps)
{
    const static std::vector<std::pair<CYIString, CYIHTTPRequest::Method>> methods =
        {
            {"GET", CYIHTTPRequest::Method::GET},
            {"POST", CYIHTTPRequest::Method::POST},
            {"PUT", CYIHTTPRequest::Method::PUT},
            {"DELETE", CYIHTTPRequest::Method::DELETE},
            {"PATCH", CYIHTTPRequest::Method::PATCH},
            {"HEAD", CYIHTTPRequest::Method::HEAD}};

    request.SetMethod(requestProps.method.TransformTo(methods));
}

static void SetCredentials(CYIHTTPRequest &request, const NetworkingRequestProps &requestProps)
{
    request.SetUseCookies(requestProps.withCredentials);
}

static void SetHeaders(CYIHTTPRequest &request, const NetworkingRequestProps &requestProps)
{
    for (auto const &header : requestProps.headers)
    {
        request.AddHeader(header.first, header.second);
    }
}

static void SetPostData(CYIHTTPRequest &request, const NetworkingRequestProps &requestProps)
{
    for (auto it = requestProps.data.begin(); it != requestProps.data.end(); ++it)
    {
        if (it->first == "string")
        {
            // request data is put in a vector before being passed in to account for null characters embedded in the middle of the string.
            std::vector<char32_t> characters;
            characters.reserve(it->second.GetLength());
            std::transform(it->second.begin(), it->second.end(), back_inserter(characters), [](CYIString::ConstIterator::value_type c) {
                return static_cast<char32_t>(c);
            });
            request.SetPostData(CYIString(characters.data(), characters.size()));
            return;
        }

        if (it->first == "base64")
        {
            std::vector<char> base64Chars = YiBase64Decode(it->second);
            request.SetPostData(CYIString(base64Chars.data(), base64Chars.size()));
            return;
        }
    }

    request.SetPostData(CYIString::EmptyString());
}

static void SetTimeout(CYIHTTPRequest &request, const NetworkingRequestProps &requestProps)
{
    if (requestProps.timeout > 0)
    {
        request.SetNetworkTimeoutMs(requestProps.timeout);
    }
}

static CYIString GetResponseData(const std::shared_ptr<CYIHTTPResponse> &pResponse, const NetworkingRequestProps::ResponseType &responseType)
{
    enum class eResponseType
    {
        TEXT,
        BASE64,
        BLOB
    };

    auto normalizedResponseType = responseType.TransformTo<eResponseType>({{"text", eResponseType::TEXT},
                                                                           {"base64", eResponseType::BASE64},
                                                                           {"blob", eResponseType::BLOB}});

    switch (normalizedResponseType)
    {
        case eResponseType::BASE64:
        {
            const auto &rawData = pResponse->GetRawData();
            const auto &encoded = YiBase64Encode(rawData);

            return CYIString(std::string(encoded.begin(), encoded.end()));
        }
        case eResponseType::BLOB:
        {
            // Blob not supported yet. Won't get here now, as a RSOD is thrown from js since we haven't registered the Blob native module.
            return CYIString::EmptyString();
        }
        case eResponseType::TEXT:
        default:
        {
            return pResponse->GetBody();
        }
    }
}

YI_RN_INSTANTIATE_MODULE(MyNetworkingModule, EventEmitterModule);

YI_RN_REGISTER_MODULE(MyNetworkingModule);

MyNetworkingModule::MyNetworkingModule()
{
    YI_LOGD(TAG, "Added to Bridge");

    SetSupportedEvents({DID_COMPLETE_NETWORK_RESPONSE_EVENT,
                        DID_RECEIVE_NETWORK_RESPONSE_EVENT,
                        DID_SEND_NETWORK_DATA_EVENT,
                        DID_RECEIVE_INCREMENTAL_DATA_EVENT,
                        DID_RECEIVE_NETWORK_DATA_PROGRESS_EVENT,
                        DID_RECEIVE_NETWORK_DATA_EVENT});
}

MyNetworkingModule::~MyNetworkingModule() = default;

YI_RN_DEFINE_EXPORT_METHOD(MyNetworkingModule, sendRequest)
(Callback callback, const NetworkingRequestProps &requestProps)
{
    auto pRequest = *m_requests.emplace(m_requests.end(), std::make_shared<CYIHTTPRequest>());

    SetUrl(*pRequest, requestProps);
    SetMethod(*pRequest, requestProps);
    SetCredentials(*pRequest, requestProps);
    SetHeaders(*pRequest, requestProps);
    SetPostData(*pRequest, requestProps);
    SetTimeout(*pRequest, requestProps);

    pRequest->NotifyComplete.Connect(*this, [this, responseType = std::move(requestProps.responseType)](const std::shared_ptr<CYIHTTPRequest> &pRequest, const std::shared_ptr<CYIHTTPResponse> &pResponse, CYIHTTPService::HTTPStatusCode statusCode) {
        OnRequestComplete(pRequest, pResponse, statusCode, std::move(responseType));
    });

    CYIHTTPService::GetInstance()->EnqueueRequest(pRequest);

    YI_LOGD(TAG, "Enqueued %s", requestProps.url.ToString().GetData());

    callback({pRequest->GetUniqueID()});
}

YI_RN_DEFINE_EXPORT_METHOD(MyNetworkingModule, abortRequest)
(uint64_t requestId)
{
    const auto requestIter = std::find_if(m_requests.begin(), m_requests.end(), [requestId](const std::shared_ptr<CYIHTTPRequest> &pRequest) {
        return pRequest->GetUniqueID() == requestId;
    });

    if (requestIter != m_requests.end())
    {
        CYIHTTPService::GetInstance()->CancelRequest(*requestIter);

        YI_LOGD(TAG, "Cancelled request for %s", (*requestIter)->GetURL().ToString().GetData());

        m_requests.erase(requestIter);
    }
}

YI_RN_DEFINE_EXPORT_METHOD(MyNetworkingModule, clearCookies)
()
{
    CYIHTTPService::GetInstance()->ClearCookies();
}

CYINetworkConfiguration MyNetworkingModule::GetNetworkConfiguration()
{
    CYINetworkConfiguration networkConfiguration;
    networkConfiguration.SetEnableCookieHandling(true);
    return networkConfiguration;
}

CYIString MyNetworkingModule::GetRootCertificateBundlePath()
{
    // Our certificate is sourced from https://curl.haxx.se/docs/caextract.html
    return CYIAppContext::GetInstance()->GetApp()->GetAssetsPath() + "cacert.pem";
}

void MyNetworkingModule::OnRequestComplete(const std::shared_ptr<CYIHTTPRequest> &pRequest, const std::shared_ptr<CYIHTTPResponse> &pResponse, CYIHTTPService::HTTPStatusCode statusCode, NetworkingRequestProps::ResponseType responseType)
{
    const bool statusUnknown = IsStatusCodeUnknown(statusCode);
    const bool statusTimedOut = IsStatusCodeTimeout(statusCode);

    const auto requestId = pRequest->GetUniqueID();

    if (statusUnknown || statusTimedOut)
    {
        const auto pErrorMessage = statusTimedOut ? REQUEST_TIMED_OUT_ERROR_MESSAGE : REQUEST_FAILED_FOR_UNKNOWN_REASON_MESSAGE;

        YI_LOGD(TAG, "ERROR %s", pErrorMessage.c_str());

        EmitEvent(DID_COMPLETE_NETWORK_RESPONSE_EVENT, folly::dynamic::array(requestId, folly::toDynamic(pErrorMessage), statusTimedOut));
    }
    else
    {
        const auto &headers = pResponse->GetHeaders();
        const auto &url = pRequest->GetURL();
        const auto &body = GetResponseData(pResponse, responseType);

        // CYIHTTPRequest Class Reference - https://developer.youi.tv/API_Docs/5.17/core/html/classCYIHTTPRequest.html
        // CYIHTTPResponse Class Reference - https://developer.youi.tv/API_Docs/5.17/core/html/classCYIHTTPResponse.html
        // CYIHTTPRequest::Metrics Struct Reference - https://developer.youi.tv/API_Docs/5.17/core/html/structCYIHTTPRequest_1_1Metrics.html
        // CYIHTTPResponse::Metrics Struct Reference - https://developer.youi.tv/API_Docs/5.17/core/html/structCYIHTTPResponse_1_1Metrics.html

        // std::chrono::high_resolution_clock - https://en.cppreference.com/w/cpp/chrono/high_resolution_clock
        // std::chrono::duration - https://en.cppreference.com/w/cpp/chrono/duration

        // Objective-C: Track custom network requests - https://docs.newrelic.com/docs/mobile-monitoring/new-relic-mobile-ios/api-guides/ios-sdk-api-guide#objc-instrumentation

        YI_LOGD(TAG,
                "\nRequest %s\n" \
                "\tMetrics:\n"
                "\t\tenqueue time: %lld (time since epoch the corresponding request was enqueued)\n" \
                "\t\tcomplete time: %lld (time since epoch the corresponding request was enqueued)\n" \
                "\t\tresponse time: %lld (time since epoch the response was first received)\n" \
                "\t\tcomplete time: %lld (time since epoch the entire response transfer was completed)\n" \
                "\tRequest size: %lu (size of the HTTP request body in bytes)\n" \
                "\tResponse size: %lu (size of the HTTP response body portion in bytes)\n" \
                "\tRequest Time: %s (gets the time when the corresponding network request for this response was sent out)\n" \
                "\tResponse Time: %s (gets the time when we started receiving this network response)\n",
                pRequest->GetURL().ToString().GetData(),
                pRequest->GetMetrics().enqueueTime.count(),
                pRequest->GetMetrics().requestTime.count(),
                pResponse->GetMetrics().initialResponseTime.count(),
                pResponse->GetMetrics().transferCompleteTime.count(),
                pRequest->GetPostData().size(),
                pResponse->GetBody().GetSizeInBytes(),
                pResponse->GetRequestTime().FormatString().GetData(),
                pResponse->GetResponseTime().FormatString().GetData());

        // statusCode returns the origin server response code in the case of re-validation (e.g. sometimes 304)
        // Existing RN framework expect the behaviour, re-validated succesfully (304) or not (200), the response should be 200
        // pResponse is returned from the cache in case of 304 origin server response and contains the original 200 status code.
        // using pResponse->GetHeaderHTTPStatusCode() instead of statusCode
        folly::dynamic headersObject = folly::dynamic::object;
        for (const auto &header : headers)
        {
            headersObject[header.GetName().ToStdString()] = header.GetValue().ToStdString();
        }

        EmitEvent(DID_RECEIVE_NETWORK_RESPONSE_EVENT, folly::dynamic::array(requestId, static_cast<int32_t>(pResponse->GetHTTPStatusCode()), headersObject, ToDynamic(url.ToString())));
        EmitEvent(DID_RECEIVE_NETWORK_DATA_EVENT, folly::dynamic::array(requestId, ToDynamic(body)));
        EmitEvent(DID_COMPLETE_NETWORK_RESPONSE_EVENT, folly::dynamic::array(requestId, nullptr, false));
    }

    const auto requestIter = std::find(m_requests.begin(), m_requests.end(), pRequest);
    if (requestIter != m_requests.end())
    {
        m_requests.erase(requestIter);
    }
}
