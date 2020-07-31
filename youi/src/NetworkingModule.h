#ifndef _YOUIREACT_NETWORKING_MODULE_H_
#define _YOUIREACT_NETWORKING_MODULE_H_

#include <youireact/modules/NetworkingRequestProps.h>

#include <network/YiHTTPService.h>
#include <youireact/modules/EventEmitter.h>

namespace yi
{
namespace react
{
class YI_RN_MODULE(MyNetworkingModule, EventEmitterModule)
{
public:
    MyNetworkingModule();
    virtual ~MyNetworkingModule() final;

    YI_RN_EXPORT_NAME(Networking);

    YI_RN_EXPORT_METHOD(sendRequest)
    (Callback callback, const NetworkingRequestProps &request);
    YI_RN_EXPORT_METHOD(abortRequest)
    (uint64_t requestId);
    YI_RN_EXPORT_METHOD(clearCookies)
    ();

    static CYINetworkConfiguration GetNetworkConfiguration();
    static CYIString GetRootCertificateBundlePath();

private:
    void OnRequestComplete(const std::shared_ptr<CYIHTTPRequest> &pRequest, const std::shared_ptr<CYIHTTPResponse> &pResponse, CYIHTTPService::HTTPStatusCode statusCode, NetworkingRequestProps::ResponseType responseType);

    std::vector<std::shared_ptr<CYIHTTPRequest>> m_requests;
};

} // namespace react
} // namespace yi

#endif
