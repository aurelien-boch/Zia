#include "HttpsModule.hpp"

#include <Loader.hpp>

namespace modules
{
    const ziapi::Version HttpsModule::_version = {1, 1, 1};
    const ziapi::Version HttpsModule::_compatibleApiVersion = {3, 1, 1};

    HttpsModule::HttpsModule() : HttpModule()
    {}

    void HttpsModule::Init(const ziapi::config::Node &cfg)
    {}

    ziapi::Version HttpsModule::GetVersion() const noexcept
    {
        return _version;
    }

    ziapi::Version HttpsModule::GetCompatibleApiVersion() const noexcept
    {
        return _compatibleApiVersion;
    }

    const char *HttpsModule::GetName() const noexcept
    {
        return "Https";
    }

    const char *HttpsModule::GetDescription() const noexcept
    {
        return "A https module that implements Http/1.1 protocol with SSL.";
    }

    void HttpsModule::Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses)
    {}

}

extern "C" {
    DllExport ziapi::IModule * LoadZiaModule()
    {
        return new modules::HttpsModule();
    }
}
