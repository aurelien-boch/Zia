#include "HttpModule.hpp"

namespace modules
{
    HttpModule::HttpModule() : ziapi::INetworkModule(),
        _run(true),
        _version(1, 1),
        _compatibleApiVersion(1, 1),
        _name("http"),
        _description("")
    {}

    HttpModule::~HttpModule()
    {}

    void HttpModule::Init(const ziapi::config::Node &cfg)
    {}

    ziapi::Version HttpModule::GetVersion() const noexcept
    {
        return _version;
    }

    ziapi::Version HttpModule::GetCompatibleApiVersion() const noexcept
    {
        return _compatibleApiVersion;
    }

    const char *HttpModule::GetName() const noexcept
    {
        return _name.c_str();
    }

    const char *HttpModule::GetDescription() const noexcept
    {
        return _description.c_str();
    }

    void HttpModule::Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses)
    {
        while (_run) {

        }
    }

    void HttpModule::Terminate()
    {
        _run = false;
    }

    void HttpModule::_listen(ziapi::http::IRequestOutputQueue &requests)
    {

    }

    void HttpModule::_send(ziapi::http::IResponseInputQueue &requests)
    {

    }
}
