#include "AsioHttpsListener.hpp"

namespace network::https
{
    AsioHttpsListener::AsioHttpsListener(asio::io_service &ctx, std::uint16_t port, std::string const &certificatePath) :
        http::AsioHttpListener(ctx, port),
        _certificatePath{certificatePath}
    {}

    void AsioHttpsListener::_emplaceClient(asio::ip::tcp::socket &peer,
        std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback)
    {}
}

