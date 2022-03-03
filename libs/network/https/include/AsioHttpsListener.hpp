#ifndef ASIOHTTPSLISTENER_HPP
#define ASIOHTTPSLISTENER_HPP

#include <asio.hpp> // TODO: change
#include <asio/ssl.hpp>

#include <ITCPListener.hpp>

namespace network::https
{
    /**
     * @class Implements a Http Listener with SSL
     */
    class AsioHttpsListener : public ITCPListener<std::string, std::string>
    {
        public:

            using SslSocket = asio::ssl::stream<asio::ip::tcp::socket>;

            using IClient = ITCPClient<std::string, std::string>;

            explicit AsioHttpsListener(asio::io_service &ctx, std::uint16_t port, std::string const &certificatePath);

            void run(std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback) noexcept override;

        private:
            std::string _certificatePath;
            asio::ip::tcp::endpoint _endpoint;
            asio::ip::tcp::acceptor _acceptor;
            asio::ssl::context _sslContext;
            SslSocket _sslSocket;
            asio::io_context &_io_context;

    };
}

#endif //ASIOHTTPSLISTENER_HPP
