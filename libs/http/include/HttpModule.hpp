#ifndef HTTPMODULE_HPP
#define HTTPMODULE_HPP

#include <string>
#include <asio.hpp>

#include <ziapi/Module.hpp>

//#include "../../network/include/socket/tcp/AsioTcpListener.hpp"
#include "../../network/include/socket/tcp/AsioTCPListener.hpp"

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
            bool _run;
            ziapi::Version _version;
            ziapi::Version _compatibleApiVersion;
            std::string _name;
            std::string _description;
            std::uint16_t _port;
            asio::io_service _service;
            network::socket::AsioTCPListener<int> _listener;

            void _listen(ziapi::http::IRequestOutputQueue &requests);

            void _send(ziapi::http::IResponseInputQueue &requests);

    };
}

#endif //HTTPMODULE_HPP
