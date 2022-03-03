#include <asio/ssl.hpp>

#include "AsioHttpsListener.hpp"

#include "AsioHttpsClient.hpp"

namespace network::https
{
    AsioHttpsListener::AsioHttpsListener(asio::io_service &ctx, std::uint16_t port, std::string const &certificatePath) :
        _certificatePath{certificatePath},
        _endpoint(asio::ip::tcp::v4(), port),
        _acceptor(ctx, _endpoint),
        _sslContext(asio::ssl::context::tls),
        _sslSocket(ctx, _sslContext),
        _io_context(ctx)
    {}

    void AsioHttpsListener::run(std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback) noexcept
    {
        _acceptor.async_accept([this, callback = std::forward<std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)>>(callback)] (asio::error_code const &error, asio::ip::tcp::socket peer)
        {
            if (error) {
                const auto it = error::AsioErrorTranslator.find(error);

                if (it == error::AsioErrorTranslator.end())
                    std::cerr << "ERROR(network/AsioHttpListener): " << error << std::endl;
                else
                    callback(it->second, nullptr);
            } else {
                SslSocket s(std::move(peer), _sslContext);
                auto res = std::make_unique<AsioHttpsClient>(_io_context, std::move(s));

                // callback(error::ErrorSocket::SOCKET_NO_ERROR, std::move(res));
            }
        });


//                [this, callback = std::forward<std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)>>(callback)]
//                (asio::error_code &error, asio::ip::tcp::socket &peer) mutable {

//                run(std::forward<std::function<void(error::ErrorSocket const &,
//                    std::shared_ptr<IClient>)>>(callback));
//            }
//        );
    }

//    void AsioHttpsListener::_emplaceClient(asio::ip::tcp::socket &peer,
//        std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback)
//    {}
}

