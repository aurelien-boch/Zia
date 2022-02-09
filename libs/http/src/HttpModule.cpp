#include <iostream>

#include "HttpModule.hpp"

namespace modules
{
    HttpModule::HttpModule(std::uint16_t port) :
        ziapi::INetworkModule(),
        _run(true),
        _version(1, 0, 0),
        _compatibleApiVersion(3, 0, 0),
        _name("http"),
        _description("A Http module."),
        _port(port),
        _service(),
        _listener(_service, port)
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
//        _context.run();
        while (_run) {
            try {

            } catch (const std::exception &e) {
                std::cerr << "ERROR " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "ERROR Unknown error" << std::endl;
            }
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
