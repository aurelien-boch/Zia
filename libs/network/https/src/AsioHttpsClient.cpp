#include <asio/write.hpp>
#include <asio/read.hpp>

#include "AsioHttpsClient.hpp"

namespace network::https
{
    AsioHttpsClient::AsioHttpsClient(asio::io_context &ctx, std::string const &certificatePath) :
        AsioHttpClient(ctx),
        _sslContext{asio::ssl::context::sslv23},
        _sslSocket{ctx, _sslContext},
        _connectionSocket{_sslSocket.lowest_layer()} // ,
        // _resolver{ctx}
    {
        if (certificatePath.empty())
            _sslContext.set_default_verify_paths();
        else
            _sslContext.load_verify_file(certificatePath);
        _sslSocket.set_verify_mode(asio::ssl::verify_peer);
        _sslSocket.handshake(asio::ssl::stream<asio::ip::tcp::socket>::client);
    }

    void AsioHttpsClient::connect(Address const &peer) noexcept
    {
        asio::ip::tcp::endpoint endpoint{asio::ip::address_v4{peer.ipAddress}, peer.port};
        // asio::ip::tcp::resolver::query query("host.name", "https");

        _connectionSocket.connect(endpoint);
    }
    std::size_t AsioHttpsClient::send(std::string const &data) noexcept
    {
        try {
            return asio::write(_sslSocket, asio::buffer(data.c_str(), sizeof(char) * data.size()));
        } catch (std::system_error const &err) {
            std::cerr << "ERROR(network/AsioHttpsClient): " << err.what() << std::endl;
            return 0;
        }
    }

//    std::string AsioHttpsClient::receive() noexcept
//    {
//        try {
//            std::string header;
//            std::string body;
//
//            return (header + body);
//
//        } catch (std::system_error const &err) {
//            std::cerr << "ERROR(network/AsioHttpsClient): " << err.what() << std::endl;
//            return {};
//        }
//    }

    void AsioHttpsClient::asyncSend(
        std::string const &packet,
        std::function<void(error::ErrorSocket const &)> &&callback) noexcept
    {}

    void AsioHttpsClient::asyncReceive(
        std::function<void(error::ErrorSocket const &, std::string &)> &&callback) noexcept
    {}

    void AsioHttpsClient::_rec(std::string &str)
    {
        char buff[257]{};

        asio::read(_sslSocket, asio::buffer(buff, sizeof(char) * 256));
        str += buff;
    }

}