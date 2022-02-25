#include "AsioHttpListener.hpp"

namespace network::http
{
    AsioHttpListener::AsioHttpListener(asio::io_service &ctx, std::uint16_t port) :
        _io_context(ctx),
        _acceptor(_io_context)
    {
        asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port);

        _acceptor.open(asio::ip::tcp::v4());
        _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        _acceptor.bind(ep);
        _acceptor.listen();
    }

    void AsioHttpListener::run(std::function<void(
        error::ErrorSocket const &,
        std::shared_ptr<IClient>)> &&callback) noexcept
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
                } else
                    _emplaceClient(peer, std::forward<std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)>>(callback));
                run(std::forward<std::function<void(error::ErrorSocket const &,
                    std::shared_ptr<IClient>)>>(callback));
            }
        );
    }

    void AsioHttpListener::_emplaceClient(asio::ip::tcp::socket &peer,
          std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback)
    {
        auto res = std::make_unique<AsioHttpClient>(peer);

        callback(error::SOCKET_NO_ERROR, std::move(res));
    }
}