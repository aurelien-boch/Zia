#include <iostream>
#include <thread>

#include <AsioHttpListener.hpp>

#include <ResponseInputQueue.hpp>
#include <RequestOutputQueue.hpp>

#include "HttpModule.hpp"


namespace modules
{
    HttpModule::HttpModule() :
        ziapi::INetworkModule(),
        _run(true),
        //_port(port),
        _service{},
        _listener{nullptr},
        _clients{}
    {}

    void HttpModule::Init(const ziapi::config::Node &cfg) // TODO: Check if it works
    {
        auto &httpConfig = cfg["http"];
        int port = httpConfig["port"].AsInt();

        if (port < 0)
            throw std::runtime_error("ERROR(modules/Http): Invalid port in configuration file");
        _listener = std::make_unique<network::http::AsioHttpListener>(_service, port);
    }

    ziapi::Version HttpModule::GetVersion() const noexcept
    {
        return {1, 0, 0};
    }

    ziapi::Version HttpModule::GetCompatibleApiVersion() const noexcept
    {
        return {3, 0, 0};
    }

    const char *HttpModule::GetName() const noexcept
    {
        return "Http";
    }

    const char *HttpModule::GetDescription() const noexcept
    {
        return "A http module that implements HTTP/1.1 protocol.";
    }

    void HttpModule::Run([[maybe_unused]] ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses)
    {
        if (!_listener)
            throw std::runtime_error("ERROR(modules/Http): Module not configured");
        _listener->run(
            [this](const error::ErrorSocket &err, std::shared_ptr<IClient> client)
            {
                _onConnect(err, std::move(client));
            }
        );
        _service.run();
        while (_run) {
            try {
                do {
                    std::this_thread::yield();
                } while (!responses.Size());
                _sendResponses(responses);
            } catch (const std::runtime_error &e) {
                std::cerr << "ERROR(modules/http): " << e.what() << std::endl;
            }
        }
    }

    void HttpModule::Terminate()
    {
        _run = false;
    }

    void HttpModule::_onConnect(
        error::ErrorSocket const &err,
        std::shared_ptr<IClient> client)
    {
        if (err != error::ErrorSocket::SOCKET_NO_ERROR) {
            const auto errIt = error::errorMessage.find(err);

            if (errIt == error::errorMessage.end())
                std::cerr << "Unknown error occurred" << err << std::endl;
            else
                std::cerr << "Error occurred" << err << std::endl;
        } else {
            std::shared_ptr<IClient> c = _clients.emplace_back(client);

            c->asyncReceive([c, this] (error::ErrorSocket err, std::string &request) mutable {
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
            const auto errIt = error::errorMessage.find(err);

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

    void HttpModule::_sendResponses(ziapi::http::IResponseInputQueue &responses)
    {
        while (responses.Size()) {
            const auto res = responses.Pop().value();
            auto client = res.second.find("client");

            if (client == res.second.end())
                throw std::runtime_error("No client specified");
            // TODO: send response to client using http formatter
        }
    }
}
