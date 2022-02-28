#include "AsioHttpsClient.hpp"

namespace network::https
{
    AsioHttpsClient::AsioHttpsClient(asio::io_context &ctx, std::string const &certificatePath) :
        AsioHttpClient(ctx),
        _sslContext{asio::ssl::context::sslv23},
        _sslSocket{ctx, _sslContext},
         _connectionSocket{_sslSocket.lowest_layer()}
    {
        _sslContext.set_verify_mode(asio::ssl::verify_peer);
        if (certificatePath.empty()) {
            // TODO
        }
        else
            _sslContext.load_verify_file(certificatePath);
    }

    void AsioHttpsClient::connect(Address const &peer) noexcept
    {
        asio::ip::tcp::endpoint endpoint{asio::ip::address_v4{peer.ipAddress}, peer.port};

        _connectionSocket.connect(endpoint);
    }
    std::size_t AsioHttpsClient::send(std::string const &data) noexcept
    {
        return 0;
    }

    std::string receive() noexcept
    {
        return {};
    }

    void asyncSend(
        std::string const &packet,
        std::function<void(error::ErrorSocket const &)> &&callback) noexcept
    {}

    void asyncReceive(
            std::function<void(error::ErrorSocket const &, std::string &)> &&callback
    ) noexcept
    {}
}