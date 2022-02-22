#include <cstring>

#include "AsioHttpClient.hpp"

namespace network::http
{
    AsioHttpClient::AsioHttpClient(asio::io_context &io_context) :
        ITCPClient{},
        _socket{asio::ip::tcp::socket{io_context}},
        _header{},
        _body{},
        _buffer{}
    {}

    AsioHttpClient::AsioHttpClient(asio::ip::tcp::socket &socket) :
        _socket{std::move(socket)},
        _header{},
        _body{},
        _buffer{}
    {}

    AsioHttpClient::~AsioHttpClient()
    {
        _socket.close();
    }

    void AsioHttpClient::connect(Address const &peer) noexcept
    {
        asio::ip::tcp::endpoint endpoint{asio::ip::address_v4{peer.ipAddress}, peer.port};

        _socket.connect(endpoint);
    }

    std::size_t AsioHttpClient::send(std::string const &data) noexcept
    {
        try {
            return _socket.send(asio::buffer(data.c_str(), sizeof(char) * data.size()));
        } catch (std::system_error &err) {
            std::cerr << "ERROR(network/AsioHttpClient): " << err.what() << std::endl;
            return (0);
        }
    }

    std::string AsioHttpClient::receive() noexcept
    {
        std::string header;
        std::string body;
        std::size_t bodyLength = 0;

        while(header.find("\r\n\r\n") == std::string::npos)
            _rec(header);
        _cleanHeader(header, body);
        try {
            bodyLength = _getContentLength(header);
        } catch (std::runtime_error const &e) {
            std::cerr << e.what() << std::endl;
            asyncSend("411 Length Required", [](error::ErrorSocket const &){});
            return {};
        }
        if (bodyLength == 0) {
            asyncSend("400 Bad Request", [](error::ErrorSocket const &){});
            return {};
        }
        while (body.size() < bodyLength)
            _rec(body);
        return (header + body);
    }

    void AsioHttpClient::asyncSend(
        std::string const &packet,
        std::function<void(error::ErrorSocket const &)> &&cb) noexcept
    {
        _socket.async_send(asio::buffer(packet.data(), sizeof(char) * packet.size()),
            [cb = std::forward<std::function<void (error::ErrorSocket const &)>>(cb)] (asio::error_code const &ec, std::size_t) {
            if (ec) {
                const auto it = error::AsioErrorTranslator.find(ec);

                if (it == error::AsioErrorTranslator.end()) {
                    std::cerr << "ERROR(network/AsioHttpClient): " << ec << std::endl;
                } else
                    cb(it->second);
            } else
                 cb(error::SOCKET_NO_ERROR);
        });
    }

    void AsioHttpClient::asyncReceive(
        std::function<void(error::ErrorSocket const &, std::string &)> &&cb
    ) noexcept
    {
        _socket.async_receive(
            asio::buffer(&_buffer, sizeof(char) * 256),
            [cb = std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), this] (asio::error_code ec, std::size_t) mutable {
                _header += _buffer;
                if (ec) {
                    const auto it = error::AsioErrorTranslator.find(ec);

                    if (it == error::AsioErrorTranslator.end())
                        std::cerr << "ERROR(network/AsioHttpClient): " << ec << std::endl;
//                    else
//                        cb(it->second, _header + _body);}
                }
//                else if (_header.find("\r\n") != std::string::npos) {
//                    // Check content length header<
////                    cb(error::SOCKET_NO_ERROR, _packet);
////                    _packet = "";
//                }
                asyncReceive(std::move(cb));
            }
        );
    }

    std::size_t AsioHttpClient::_getContentLength(std::string const &header)
    {
        std::string toFind = "Content-Length:";
        std::size_t pos = header.find(toFind);
        std::size_t bodyLength = 0;

        if (pos == std::string::npos)
            throw std::runtime_error("ERROR(network/AsioHttpClient): No Content-Length header");
        try {
            pos += toFind.size() + 1;
            std::size_t pos2 = header.substr(pos).find("\r\n");

            if (pos2 == std::string::npos)
                throw std::runtime_error("ERROR(network/AsioHttpClient): Invalid Content-Length header");
            bodyLength = std::stol(header.substr(pos, pos2));
        } catch (std::exception const &) {
            throw std::runtime_error("ERROR(network/AsioHttpClient): Invalid Content-Length header");
        }
        return bodyLength;
    }

    void AsioHttpClient::_rec(std::string &str)
    {
        char buff[257]{};

        _socket.receive(asio::buffer(&buff, sizeof(char) * 256));
        str += buff;
    }

    void AsioHttpClient::_cleanHeader(std::string &header, std::string &body)
    {
        std::string toFind = "\r\n\r\n";
        std::size_t pos = header.find(toFind);

        if (pos == std::string::npos)
            throw std::runtime_error("ERROR(network/AsioHttpClient): Invalid header");
        body = header.substr(pos + toFind.size());
        header = header.substr(0, pos + toFind.size());
    }

}