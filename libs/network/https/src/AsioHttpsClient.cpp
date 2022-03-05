#include <asio/write.hpp>
#include <asio/read.hpp>

#include <RequestHelper.hpp>

#include "AsioHttpsClient.hpp"

namespace network::https
{
    AsioHttpsClient::AsioHttpsClient(asio::io_context &io_context, std::string const &certificatePath) :
        _sslContext(asio::ssl::context::tls),
        _sslSocket(io_context, _sslContext),
        _resolver(io_context)
    {
        try {
            _sslSocket.handshake(asio::ssl::stream_base::client);
        } catch (std::system_error const &e) {
            throw std::runtime_error("Error on handshake: " + std::string(e.what()));
        }
    }

    AsioHttpsClient::AsioHttpsClient(asio::io_context &io_context, SslSocket &&socket) :
        _sslContext(asio::ssl::context::tls),
        _sslSocket(std::move(socket)),
        _resolver(io_context)
    {
        try {
            _sslSocket.handshake(asio::ssl::stream_base::server);
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
            return asio::write(_sslSocket, asio::buffer(data.data(), sizeof(char) * data.size()));
        } catch (std::system_error const &err) {
            std::cerr << "ERROR(network/AsioHttpsClient): " << err.what() << std::endl;
            return 0;
        }
    }

    std::string AsioHttpsClient::receive() noexcept
    {
        std::string header;
        std::string body;
        std::size_t bodyLength = 0;

        while(header.find("\r\n\r\n") == std::string::npos)
            _rec(header);
        http::RequestHelper::cleanHeader(header, body);
        try {
            bodyLength = http::RequestHelper::getContentLength(header);
        } catch (std::runtime_error const &e) {
            std::cerr << e.what() << std::endl;
            asyncSend("HTTP/1.1 400 Bad request\r\nContent-Length: 0\r\n\r\n", [](error::ErrorSocket const &){});
            return {};
        }
        while (body.size() < bodyLength)
            _rec(body);
        return (header + body);
    }


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

    void AsioHttpsClient::_asyncRec(asio::error_code ec, std::function<void(error::ErrorSocket const &, std::string &)> &&cb, std::size_t bytesRead)
    {
        _requestBuffer += std::string{_buffer, _buffer + bytesRead};
        std::cout << "RECEIVING: " << _buffer << std::endl;
        if (ec) {
            try {
                cb(error::AsioErrorTranslator.at(ec), _requestBuffer);
            } catch (std::out_of_range const &) {
                std::cerr << "ERROR(network/AsioHttpsClient): " << ec << std::endl;
            }
        } else {
            if (_requestBuffer.find("\r\n\r\n") != std::string::npos && _bodyLength == 0) {
                try {
                    _bodyLength = http::RequestHelper::getContentLength(_requestBuffer); // body length is now known
                } catch (std::runtime_error &) { // DISCARDS BODY
                     cb(error::SOCKET_NO_ERROR, _requestBuffer);
                } catch (std::invalid_argument &) { // invalid header
                    send("HTTP/1.1 400 Bad request\r\nContent-Length: 0\r\n\r\n");
                }
            } else
                if (_bodyLength != 0) {
                    _totalBytesRead = bytesRead;
                    _requestBuffer += _buffer;
                    if (_totalBytesRead != _bodyLength)
                        asyncReceive(std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb));
                    else
                        cb(error::SOCKET_NO_ERROR, _requestBuffer);
                } else
                    asyncReceive(std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb));
        }
        asyncReceive(std::move(cb));
    }

}