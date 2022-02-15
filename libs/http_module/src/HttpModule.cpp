#include <iostream>

#include "http/AsioHttpListener.hpp"

#include "HttpModule.hpp"
#include "ResponseInputQueue.hpp"
#include "RequestOutputQueue.hpp"

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
        _service{},
        _listener(std::make_unique<network::http::AsioHttpListener>(_service, port)),
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

    void HttpModule::Run([[maybe_unused]] ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses)
    {
        ResponseInputQueue &res = static_cast<ResponseInputQueue &>(responses);
        _listener->run(
            [this](const error::ErrorSocket &err, std::shared_ptr<IClient> client)
            {
                _onConnect(err, std::move(client));
            }
        );
        _service.run();
        while (_run) {
            try {
                _sendResponses(res);
            } catch(const std::runtime_error &e) {
                std::cerr << "ERROR(modules/http): " << e.what() << std::endl;
            } catch (...) {
                std::cerr << "ERROR(modules/http): Unknown error" << std::endl;
            }
        }
    }

    void HttpModule::Terminate()
    {
        _run = false;
    }

    void HttpModule::_onConnect(
        const error::ErrorSocket &err,
        std::shared_ptr<IClient> client)
    {
        if (err != error::ErrorSocket::SOCKET_NO_ERROR) {
            auto errIt = error::errorMessage.find(err);

            if (errIt == error::errorMessage.end())
                std::cerr << "Unknown error occurred" << err << std::endl;
            else
                std::cerr << "Error occurred" << err << std::endl;
        } else {
            std::shared_ptr<IClient> c = _clients.emplace_back(client);

            c->asyncRead([c, this] (error::ErrorSocket err, std::string &request) mutable {
                _onPacket(err, request, c);
            });
        }
    }
    
    void HttpModule::_onPacket(
        const error::ErrorSocket &err,
        std::string &packet,
        std::shared_ptr<IClient> &client)
    {
        if (err != error::ErrorSocket::SOCKET_NO_ERROR) {
            auto errIt = error::errorMessage.find(err);

            if (errIt == error::errorMessage.end())
                std::cerr << "ERROR(modules/Http): Unknown error occurred" << err << std::endl;
            else
                std::cerr << "ERROR(modules/Http): " << err << std::endl;
        } else {
            // TODO:
            // Call parser
            // Add Packet in queue with client as Context
        }
    }

    void HttpModule::_sendResponses(ResponseInputQueue &responses)
    {
        while (!responses.Empty()) {
            auto res = responses.Pop().value();
            auto client = res.second.find("client");

            if (client == res.second.end())
                throw std::runtime_error("No client specified");
            // TODO: send response to client using http formatter
        }
    }
}
