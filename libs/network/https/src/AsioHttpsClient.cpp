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
            return asio::write(_sslSocket, asio::buffer(data.data(), sizeof(char) * data.size()));
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
        std::function<void(error::ErrorSocket const &)> &&cb) noexcept
    {
        asio::async_write(_sslSocket, asio::buffer(packet.data(), sizeof(char) * packet.size()),
            [cb = std::forward<std::function<void (error::ErrorSocket const &)>>(cb)] (asio::error_code const &ec, std::size_t) {
                if (ec) {
                    const auto it = error::AsioErrorTranslator.find(ec);

                    if (it == error::AsioErrorTranslator.end())
                        std::cerr << "ERROR(network/AsioHttpsClient): " << ec << std::endl;
                    else
                        cb(it->second);
                } else
                    cb(error::SOCKET_NO_ERROR);
        });
    }

    void AsioHttpsClient::asyncReceive(
        std::function<void(error::ErrorSocket const &, std::string &)> &&cb) noexcept
    {
        asio::async_read(_sslSocket, asio::buffer(_buffer, sizeof(char) * 256),
            [cb = std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), this] (asio::error_code ec, std::size_t bytesRead) mutable {
                _asyncRec(ec, std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), bytesRead);
        });
    }

    void AsioHttpsClient::_rec(std::string &str)
    {
        char buff[257]{};

        asio::read(_sslSocket, asio::buffer(buff, sizeof(char) * 256));
        str += buff;
    }
}