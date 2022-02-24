#ifndef ASIOHTTPLISTENER_HPP_
#define ASIOHTTPLISTENER_HPP_

#include <asio.hpp>
#include <memory>

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
     * @tparam Client is the Client's type
     */
    class DllExport AsioHttpListener : public ITCPListener<std::string, std::string>
    {
        public:

            using IClient = ITCPClient<std::string, std::string>;

            explicit AsioHttpListener(asio::io_service &ctx, std::uint16_t port) :
                _io_context(ctx),
                _acceptor(_io_context)
            {
                asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port);

                _acceptor.open(asio::ip::tcp::v4());
                _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
                _acceptor.bind(ep);
                _acceptor.listen();
            }

            ~AsioHttpListener() override = default;

            void run(std::function<void(
                error::ErrorSocket const &,
                std::shared_ptr<IClient>)> &&callback) noexcept override
            {
                    _acceptor.async_accept(
                            this->_io_context,
                            [this, callback = std::forward<std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)>>(callback)]
                                    (asio::error_code const &error, asio::ip::tcp::socket peer) mutable {
                                if (error) {
                                    const auto it = error::AsioErrorTranslator.find(error);

                                    if (it == error::AsioErrorTranslator.end())
                                        std::cerr << "ERROR(network/AsioHttpListener): " << error << std::endl;
                                    else
                                        callback(it->second, nullptr);
                                } else {
                                    auto res = std::make_unique<Client>(peer);

                                    callback(error::SOCKET_NO_ERROR, std::move(res));
                                }
                                run(std::forward<std::function<void(error::ErrorSocket const &,
                                    std::shared_ptr<IClient>)>>(callback));
                            }
                    );
            }

        private:
            asio::io_context &_io_context;
            asio::ip::tcp::acceptor _acceptor;
    };
}

#endif /* !ASIOHTTPLISTENER_HPP_ */
