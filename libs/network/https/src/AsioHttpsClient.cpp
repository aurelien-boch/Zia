#include <asio/write.hpp>
#include <asio/read.hpp>

#include "AsioHttpsClient.hpp"

// openssl s_client -connect localhost:4443

namespace network::https
{
    AsioHttpsClient::AsioHttpsClient(asio::io_context &io_context, std::string const &certificatePath) :
        _sslContext(asio::ssl::context::tls),
        _sslSocket(io_context, _sslContext),
        _resolver(io_context)
    {

    }

    AsioHttpsClient::AsioHttpsClient(asio::io_context &io_context, SslSocket &&socket) :
        _sslContext(asio::ssl::context::tls),
        _sslSocket(std::move(socket)),
        _resolver(io_context)
    {
        try {
            _sslSocket.handshake(asio::ssl::stream_base::server);
            std::cout << "Handshake done" << std::endl;
        } catch (std::system_error const &e) {
            throw std::runtime_error("Error on handshake: " + std::string(e.what()));
        }
    }

    void AsioHttpsClient::connect(Address const &peer) noexcept
    {
        auto endpoints = _resolver.resolve(std::to_string(peer.ipAddress), std::to_string(peer.port));

        asio::connect(_sslSocket.next_layer(), endpoints);
        _sslSocket.handshake(asio::ssl::stream_base::client);
    }

    std::size_t AsioHttpsClient::send(std::string const &data) noexcept
    {
        try {
            std::cout << "Sending data" << std::endl; // TODO: remove line
            return asio::write(_sslSocket, asio::buffer(data.data(), sizeof(char) * data.size()));
        } catch (std::system_error const &err) {
            std::cerr << "ERROR(network/AsioHttpsClient): " << err.what() << std::endl;
            return 0;
        }
    }

    std::string AsioHttpsClient::receive() noexcept // TODO
    {}


void AsioHttpsClient::asyncSend(
        std::string const &packet,
        std::function<void(error::ErrorSocket const &)> &&cb) noexcept
    {
        std::cout << "Sending data" << std::endl; // TODO: remove line
        asio::async_write(_sslSocket, asio::buffer(packet.data(), sizeof(char) * packet.size()),
            [cb = std::forward<std::function<void (error::ErrorSocket const &)>>(cb)] (asio::error_code const &ec, std::size_t) {
                if (ec) {
                    const auto it = error::AsioErrorTranslator.find(ec);

                    if (it == error::AsioErrorTranslator.end())
                        std::cerr << "ERROR(network/AsioHttpsClient): " << ec << std::endl;
                    else
                        cb(it->second);
                } else {
                    std::cout << "Data sent" << std::endl ; // TODO: remove line
                    cb(error::SOCKET_NO_ERROR);
                }
        });
    }

    void AsioHttpsClient::asyncReceive(
        std::function<void(error::ErrorSocket const &, std::string &)> &&cb) noexcept
    {
//        asio::async_read(_sslSocket, asio::buffer(_buffer, sizeof(char) * 256),
//            [cb = std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), this] (asio::error_code ec, std::size_t bytesRead) mutable {
//                _asyncRec(ec, std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), bytesRead);
//        });
    }

    Address const &AsioHttpsClient::getAddress() const noexcept
    {
        return _address;
    }


void AsioHttpsClient::_rec(std::string &str)
    {
        char buff[257]{};

        asio::read(_sslSocket, asio::buffer(buff, sizeof(char) * 256));
        str += buff;
    }

}