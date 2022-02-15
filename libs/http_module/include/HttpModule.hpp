#ifndef HTTPMODULE_HPP
#define HTTPMODULE_HPP

#include <string>
#include <vector>
#include <memory>

#include <asio.hpp>

#include <ziapi/Module.hpp>

#include "ITCPListener.hpp"

#include "ResponseInputQueue.hpp"

namespace modules
{
    class HttpModule : public ziapi::INetworkModule
    {
        public:

            explicit HttpModule(std::uint16_t port);

            ~HttpModule();

            void Init(const ziapi::config::Node &cfg);

            [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] const char *GetName() const noexcept override;

            [[nodiscard]] const char *GetDescription() const noexcept override;

            void Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses) override;

            void Terminate() override;

        private:

            using IClient = network::ITCPClient<std::string, std::string>;

            bool _run;
            ziapi::Version _version;
            ziapi::Version _compatibleApiVersion;
            std::string _name;
            std::string _description;
            std::uint16_t _port;
            asio::io_service _service;
            std::unique_ptr<network::ITCPListener<std::string, std::string>> _listener;
            std::vector<std::shared_ptr<IClient>> _clients;

            void _onConnect(
                    const error::ErrorSocket &err,
                    std::shared_ptr<IClient> client);

            void _onPacket(
                    const error::ErrorSocket &err,
                    std::string &packet,
                    std::shared_ptr<IClient> &client);

            void _sendResponses(ResponseInputQueue &responses);
    };
}

#endif //HTTPMODULE_HPP
