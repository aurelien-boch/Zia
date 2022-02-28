#ifndef ASIOHTTPSCLIENT_HPP
#define ASIOHTTPSCLIENT_HPP

#include <string>

#include <asio/ssl.hpp>

#include <AsioHttpClient.hpp>

namespace network::https
{
    /**
     * @class Implements an Http client with SSL using Asio library
     */
    class AsioHttpsClient : http::AsioHttpClient
    {
        public:
            explicit AsioHttpsClient(asio::io_context &ctx, std::string const &certificatePath = "");

            explicit AsioHttpsClient(asio::ip::tcp::socket &socket);

            ~AsioHttpsClient() = default;

            void connect(Address const &peer) noexcept override;

            [[nodiscard]] std::size_t send(std::string const &data) noexcept override;

            [[nodiscard]] std::string receive() noexcept override;

            void asyncSend(
                std::string const &packet,
                std::function<void(error::ErrorSocket const &)> &&callback) noexcept override;

            void asyncReceive(
                std::function<void(error::ErrorSocket const &, std::string &)> &&callback
            ) noexcept override;

        private:
            asio::ssl::context _sslContext;
            asio::ssl::stream<asio::ip::tcp::socket> _sslSocket;
            asio::ip::tcp::socket::lowest_layer_type& _connectionSocket;

    };
}

#endif //ASIOHTTPSCLIENT_HPP
