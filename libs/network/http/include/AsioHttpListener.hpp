#ifndef ASIOHTTPLISTENER_HPP_
#define ASIOHTTPLISTENER_HPP_

#include <asio.hpp>
#include <memory>

#include <ITCPListener.hpp>

#include "AsioHttpClient.hpp"

namespace network::http
{
    /**
     * @class An http listener using Asio library
     * Enables to accept new clients connections on the server
     */
    class AsioHttpListener : public ITCPListener<std::string, std::string>
    {
        public:

            using IClient = ITCPClient<std::string, std::string>;

            explicit AsioHttpListener(asio::io_service &ctx, std::uint16_t port);

            ~AsioHttpListener() override = default;

            void run(std::function<void(
                error::ErrorSocket const &,
                std::shared_ptr<IClient>)> &&callback) noexcept override;

        private:
            asio::io_context &_io_context;
            asio::ip::tcp::acceptor _acceptor;
    };
}

#endif /* !ASIOHTTPLISTENER_HPP_ */
