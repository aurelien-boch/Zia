#include "http/AsioHttpListener.hpp"

namespace network::http
{
    AsioHttpListener::AsioHttpListener(asio::io_service &ctx, std::uint16_t port) :
        ITCPListener(),
        _io_context(ctx),
        _acceptor(asio::ip::tcp::acceptor(ctx))
    {
        asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port);

        _acceptor.open(asio::ip::tcp::v4());
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(ep);
        _acceptor.listen();
    }

    void AsioHttpListener::run(const std::function<void(
        const error::ErrorSocket &,
        std::shared_ptr<ITCPClient<std::string, std::string>>)> &callback) noexcept
    {
        this->_acceptor.async_accept(
            this->_io_context,
            [this, callback] (const asio::error_code& error, asio::ip::tcp::socket peer) {
                if (error) {
                    auto it = error::AsioErrorTranslator.find(error);

                    if (it == error::AsioErrorTranslator.end())
                        std::cerr << "ERROR(network/AsioHttpListener): " << error << std::endl;
                    else
                        callback(it->second, nullptr);
                } else {
                    auto res = std::make_unique<AsioHttpClient>(peer);

                    callback(error::SOCKET_NO_ERROR, std::move(res));
                }
                this->run(callback);
            }
        );
    }

}