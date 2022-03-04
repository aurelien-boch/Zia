#ifndef ASIOHTTPSCLIENT_HPP
#define ASIOHTTPSCLIENT_HPP

#include <string>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include <ITCPClient.hpp>

#ifdef _WIN32
    #define DllExport   __declspec( dllexport )
#else
    #define DllExport
#endif

namespace network::https
{
    /**
     * @class Implements an Http client with SSL using Asio library
     */
    class DllExport AsioHttpsClient : public ITCPClient<std::string, std::string>
    {
        public:

            using SslSocket = asio::ssl::stream<asio::ip::tcp::socket>;

            explicit AsioHttpsClient(asio::io_context &ctx, std::string const &certificatePath = "");

            explicit AsioHttpsClient(asio::io_context &ctx, SslSocket &&socket);

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

            [[nodiscard]] inline Address const &getAddress() const noexcept override;

        private:

            Address _address;

            asio::ssl::context _sslContext;
            SslSocket _sslSocket;
            asio::ip::tcp::resolver _resolver;

            std::string _requestBuffer;
            std::size_t _bodyLength;
            std::size_t _totalBytesRead;
            char _buffer[257];

            void _rec(std::string &str);

            void _asyncRec(asio::error_code ec, std::function<void(error::ErrorSocket const &, std::string &)> &&cb, std::size_t bytesRead);

    };
}

#endif //ASIOHTTPSCLIENT_HPP
