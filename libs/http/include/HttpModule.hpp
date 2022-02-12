#ifndef HTTPMODULE_HPP
#define HTTPMODULE_HPP

#include <string>
#include <vector>
#include <memory>

#include <asio.hpp>

#include <ziapi/Module.hpp>

#include "../../network/include/http/AsioHttpListener.hpp"

namespace modules
{
    class HttpModule : public ziapi::INetworkModule
    {
        public:

            explicit HttpModule(std::uint16_t port);

            ~HttpModule();

            void Init(const ziapi::config::Node &cfg);

            [[nodiscard]] ziapi::Version GetVersion() const noexcept;

            [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept;

            [[nodiscard]] const char *GetName() const noexcept;

            [[nodiscard]] const char *GetDescription() const noexcept;

            void Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses);

            void Terminate();

        private:

            using IClient = std::shared_ptr<network::ITCPClient<std::string, std::string>>;

            bool _run;
            ziapi::Version _version;
            ziapi::Version _compatibleApiVersion;
            std::string _name;
            std::string _description;
            std::uint16_t _port;
            asio::io_service _service;
            network::http::AsioHttpListener _listener;
            std::vector<IClient> _clients;

            void _onConnect(
                    const error::ErrorSocket &err,
                    IClient client);

            void _onPacket(
                    const error::ErrorSocket &err,
                    std::string &packet,
                    IClient &client);
    };
}

#endif //HTTPMODULE_HPP
