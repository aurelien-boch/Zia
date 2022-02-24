#ifndef ASIOHTTPSCLIENT_HPP
#define ASIOHTTPSCLIENT_HPP

#include <string>

#include <ITCPClient.hpp>

namespace network::https
{
    class AsioHttpsClient : ITCPClient<std::string, std::string>
    {
        public:
            AsioHttpsClient();

            ~AsioHttpsClient() = default;

            void connect(Address const &peer) noexcept override;

            std::size_t send(std::string const &data) noexcept override;

            [[nodiscard]] std::string receive() noexcept override;

            void asyncSend(
                std::string const &packet,
                std::function<void(error::ErrorSocket const &)> &&callback) noexcept override;

            void asyncReceive(
                std::function<void(error::ErrorSocket const &, std::string &)> &&callback
            ) noexcept override;

        private:
    };
}

#endif //ASIOHTTPSCLIENT_HPP
