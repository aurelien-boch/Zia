#include <iostream>
#include <thread>

#include <AsioHttpListener.hpp>

#include <ResponseInputQueue.hpp>
#include <RequestOutputQueue.hpp>

#include <Loader.hpp>

#include "HttpModule.hpp"


namespace modules
{
    const ziapi::Version HttpModule::_version = {1, 0, 0};
    const ziapi::Version HttpModule::_compatibleApiVersion = {3, 0, 0};

    HttpModule::HttpModule() :
        ziapi::INetworkModule(),
        _run(true),
        _port(0),
        _service{},
        _listener{nullptr},
        _clients{},
        _formatter{},
        _parser{}
    {}

    void HttpModule::Init(const ziapi::config::Node &cfg) // TODO: Check if it works
    {
        auto &httpConfig = cfg["http"];
        int port = httpConfig["port"].AsInt();

        if (port < 0)
            throw std::runtime_error("ERROR(modules/Http): Invalid port in configuration file");
        _port = port;
        _listener = std::make_unique<network::http::AsioHttpListener>(_service, port);
    }

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
        return "Http";
    }

    const char *HttpModule::GetDescription() const noexcept
    {
        return "A http module that implements HTTP/1.1 protocol.";
    }

    void HttpModule::Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses)
    {
        if (!_listener)
            throw std::runtime_error("ERROR(modules/Http): Module not configured");
        _listener->run(
            [this, &requests] (const error::ErrorSocket &err, std::shared_ptr<IClient> client) mutable
            {
                _onConnect(requests, err, std::move(client));
            }
        );
        _service.run();
        while (_run) {
            try {
                do {
                    std::this_thread::yield();
                    if (!_run)
                        break;
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
        ziapi::http::IRequestOutputQueue &requests,
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
            c->asyncReceive([c, this, &requests] (error::ErrorSocket err, std::string &request) mutable {
                _onPacket(requests, err, request, c);
            });
        }
    }
    
    void HttpModule::_onPacket(
        ziapi::http::IRequestOutputQueue &requests,
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
            requests.Push({_parser.parse(packet),
                 {{"client", std::make_any<std::shared_ptr<IClient>>(client)}}});
        }
    }

    void HttpModule::_sendResponses(ziapi::http::IResponseInputQueue &responses)
    {
        while (responses.Size()) {
            auto res = responses.Pop().value();
            auto clientIt = res.second.find("client");
            std::shared_ptr<IClient> client = nullptr;

            if (clientIt == res.second.end())
                throw std::runtime_error("ERROR(modules/Http): No client specified");
            try {
                client = std::any_cast<std::shared_ptr<IClient>>(clientIt);
            } catch (std::bad_any_cast const &e) {
                throw std::runtime_error("ERROR(modules/Http): Invalid client");
            }
             client->asyncSend(_formatter.format(res.first), [](error::ErrorSocket const &){});
        }
    }
}

extern "C" {
    DllExport ziapi::IModule *LoadZiaModule()
    {
        return new modules::HttpModule();
    }
}