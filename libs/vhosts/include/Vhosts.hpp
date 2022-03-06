#ifndef VHOSTS_HPP
#define VHOSTS_HPP

#include <asio/io_service.hpp>
#include <asio/ip/address_v4.hpp>
#include <asio/ip/tcp.hpp>

#include "ziapi/Module.hpp"

namespace modules
{
    class Vhosts : public ziapi::IHandlerModule
    {
        public:
            void Init(const ziapi::config::Node &cfg) override;

            [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] const char *GetName() const noexcept override;

            [[nodiscard]] const char *GetDescription() const noexcept override;

            void Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res) override;

            [[nodiscard]] double GetHandlerPriority() const noexcept override;

            [[nodiscard]] bool ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const override;

        private:
            using target = std::pair<std::uint32_t, std::uint16_t>;

            std::unordered_map<std::string, target> _hosts;

            static void _send(asio::ip::tcp::socket &socket, std::string const &data) noexcept;

            static std::string _recvReply(asio::ip::tcp::socket &socket) noexcept;
    };
}

#endif //VHOSTS_HPP
