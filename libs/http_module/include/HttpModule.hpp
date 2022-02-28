#ifndef HTTPMODULE_HPP
#define HTTPMODULE_HPP

#include <string>
#include <vector>
#include <memory>

#include <asio.hpp>

#include <ziapi/Module.hpp>

#include <ITCPListener.hpp>

#include <HttpParser.hpp>
#include <HttpFormatter.hpp>

namespace modules
{
    class HttpModule : public ziapi::INetworkModule
    {
        public:

            explicit HttpModule();

            ~HttpModule() override = default;

            void Init(const ziapi::config::Node &cfg) override;

            [[nodiscard]] inline ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] inline ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] inline const char *GetName() const noexcept override;

            [[nodiscard]] inline const char *GetDescription() const noexcept override;

            void Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses) override;

            void Terminate() override;

        protected:

            using IClient = network::ITCPClient<std::string, std::string>;

            bool _run;
            std::uint16_t _port;
            std::unique_ptr<network::ITCPListener<std::string, std::string>> _listener;
            asio::io_service _service;
            std::vector<std::shared_ptr<IClient>> _clients;
            formatter::HttpFormatter _formatter;
            parser::HttpParser _parser;

            static const ziapi::Version _version;
            static const ziapi::Version _compatibleApiVersion;


        private:

            void _onConnect(
                    ziapi::http::IRequestOutputQueue &requests,
                    error::ErrorSocket const &err,
                    std::shared_ptr<IClient> client);

            void _onPacket(
                    ziapi::http::IRequestOutputQueue &requests,
                    error::ErrorSocket const &err,
                    std::string &packet,
                    std::shared_ptr<IClient> &client);

            void _sendResponses(ziapi::http::IResponseInputQueue &responses);
    };
}

#endif //HTTPMODULE_HPP
