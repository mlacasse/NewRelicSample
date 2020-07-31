#include <youireact/modules/NetworkingRequestProps.h>

#include <utility/FollyDynamicUtilities.h>

bool yi::react::InitFromValue(NetworkingRequestProps &request, const folly::dynamic &value)
{
    bool ok = true;
    ok = ok && InitFromOptionalField(request.responseType, value, "responseType", NetworkingRequestProps::ResponseType("text"));
    ok = ok && InitFromMandatoryField(request.url, value, "url");
    ok = ok && InitFromMandatoryField(request.method, value, "method");
    ok = ok && InitFromOptionalField(request.timeout, value, "timeout");
    ok = ok && InitFromMandatoryField(request.withCredentials, value, "withCredentials");
    ok = ok && InitFromMandatoryField(request.headers, value, "headers");
    ok = ok && InitFromMandatoryField(request.data, value, "data");

    return ok;
}
