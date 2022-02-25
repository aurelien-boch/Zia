#ifndef ASIOHTTPSCLIENT_HPP
#define ASIOHTTPSCLIENT_HPP

#include <string>

#include <asio/ssl.hpp>

#include <ITCPClient.hpp>

namespace network::https
{
    /**
     * @class Implements an Http client with SSL using Asio library
     */
    class AsioHttpsClient : ITCPClient<std::string, std::string>
    {
        public:
            explicit AsioHttpsClient(asio::ssl::context &ctx, std::string const &certificatePath = "");

            explicit AsioHttpsClient(asio::ip::tcp::socket &socket);

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
            asio::ssl::context &_context;
            Address _address;
    };
}

#endif //ASIOHTTPSCLIENT_HPP
