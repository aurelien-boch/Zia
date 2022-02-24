#include "HttpsModule.hpp"

namespace modules
{
    HttpsModule::HttpsModule()
    {}

    void HttpsModule::Init(const ziapi::config::Node &cfg)
    {}

    ziapi::Version HttpsModule::GetVersion() const noexcept
    {}

    ziapi::Version HttpsModule::GetCompatibleApiVersion() const noexcept
    {}

    const char *HttpsModule::GetName() const noexcept
    {}

    const char *HttpsModule::GetDescription() const noexcept
    {}

    void HttpsModule::Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses)
    {}

    void HttpsModule::Terminate()
    {}
}