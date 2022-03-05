#ifndef REVERSEPROXY_HPP
#define REVERSEPROXY_HPP

#include <ziapi/Module.hpp>
#include <asio/ip/tcp.hpp>

namespace modules
{
    /**
     * @class ReverseProxy
     * @brief Implements a basic reverse proxy
     */
    class ReverseProxy : public ziapi::IHandlerModule
    {
        public:
            void Init(const ziapi::config::Node &cfg) override;

            [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] const char *GetName() const noexcept override;

            [[nodiscard]] const char *GetDescription() const noexcept override;

            void Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res) override;

            [[nodiscard]] double GetHandlerPriority() const noexcept override;

            [[nodiscard]] bool ShouldHandle(
                const ziapi::http::Context &ctx,
                const ziapi::http::Request &req) const override;

        private:
            std::uint16_t _port;
            std::uint32_t _ip;

            static void _send(asio::ip::tcp::socket &socket, std::string const &data) noexcept;

            static std::string _recvReply(asio::ip::tcp::socket &socket) noexcept;
    };
}

#endif //REVERSEPROXY_HPP
