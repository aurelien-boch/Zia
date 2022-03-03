#ifndef ASIOHTTPLISTENER_HPP_
#define ASIOHTTPLISTENER_HPP_

#include <memory>

#include <asio/ip/tcp.hpp>

#include <ITCPListener.hpp>

#include "AsioHttpClient.hpp"

#ifdef _WIN32
    #define DllExport   __declspec( dllexport )
#else
    #define DllExport
#endif

namespace network::http
{
    /**
     * @class An http listener using Asio library
     * Enables to accept new clients connections on the server
     */
    class DllExport AsioHttpListener : public ITCPListener<std::string, std::string>
    {
        public:

            using IClient = ITCPClient<std::string, std::string>;

            explicit AsioHttpListener(asio::io_service &ctx, std::uint16_t port);

            ~AsioHttpListener() override = default;

            void run(std::function<void(
                error::ErrorSocket const &,
                std::shared_ptr<IClient>)> &&callback) noexcept override;

        protected:

            asio::io_context &_io_context;

            virtual void _emplaceClient(asio::ip::tcp::socket &peer,
                std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback);

        private:
            asio::ip::tcp::acceptor _acceptor;
    };
}

#endif /* !ASIOHTTPLISTENER_HPP_ */
