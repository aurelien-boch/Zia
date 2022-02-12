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
        _listener(_service, port),
        _clients{}
    {}

    HttpModule::~HttpModule()
    {}

    void HttpModule::Init([[maybe_unused]] const ziapi::config::Node &cfg)
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

    void HttpModule::Run([[maybe_unused]] ziapi::http::IRequestOutputQueue &requests, [[maybe_unused]] ziapi::http::IResponseInputQueue &responses)
    {
        _listener.run(
            [this](const error::ErrorSocket &err, IClient client)
            {
                _onConnect(err, std::move(client));
            }
        );
        _service.run();
    }

    void HttpModule::Terminate()
    {
        _run = false;
    }

    void HttpModule::_onConnect(
        const error::ErrorSocket &err,
        IClient client)
    {
        if (err != error::ErrorSocket::SOCKET_NO_ERROR) {
            auto errIt = error::errorMessage.find(err);

            if (errIt == error::errorMessage.end())
                std::cerr << "Unknown error occurred" << err << std::endl;
            else
                std::cerr << "Error occurred" << err << std::endl;
        } else {
            IClient c = _clients.emplace_back(client);

            c->asyncRead([c, this] (error::ErrorSocket err, std::string &request) mutable {
                _onPacket(err, request, c);
            });
        }
        std::cout << "CLIENT CONNECTED" << std::endl; // TODO: remove line
    }
    
    void HttpModule::_onPacket(
        const error::ErrorSocket &err,
        std::string &packet,
        IClient &client)
    {
        std::cout << "GOT DATA" << std::endl; // TODO: remove line
        if (err != error::ErrorSocket::SOCKET_NO_ERROR) {
            auto errIt = error::errorMessage.find(err);

            if (errIt == error::errorMessage.end())
                std::cerr << "ERROR: Unknown error occurred" << err << std::endl;
            else
                std::cerr << "ERROR: " << err << std::endl;
        } else {
            // TODO:
            // Call parser
            // Add Packet in queue
        }
    }
}
