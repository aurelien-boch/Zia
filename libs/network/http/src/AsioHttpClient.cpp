#include <cstring>

#include "AsioHttpClient.hpp"
#include "RequestHelper.hpp"

namespace network::http
{
    AsioHttpClient::AsioHttpClient(asio::io_context &io_context) :
        ITCPClient{},
         _socket{io_context},
        _buffer{},
        _requestBuffer{},
        _bodyLength{},
        _totalBytesRead{},
        _address{} // TODO: fix segfault here
        // static_cast<std::uint32_t>(std::stoul(_socket.remote_endpoint().address().to_string())), static_cast<std::uint8_t>(_socket.remote_endpoint().port())}
    {}

    AsioHttpClient::AsioHttpClient(asio::ip::tcp::socket &socket) :
        _socket{std::move(socket)},
        _buffer{},
        _requestBuffer{},
        _bodyLength{},
        _totalBytesRead{},
        _address{static_cast<std::uint32_t>(std::stoul(_socket.remote_endpoint().address().to_string())), static_cast<std::uint8_t>(_socket.remote_endpoint().port())}
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
        RequestHelper::cleanHeader(header, body);
        try {
            bodyLength = RequestHelper::getContentLength(header);
        } catch (std::runtime_error const &e) {
            std::cerr << e.what() << std::endl;
            asyncSend("HTTP/1.1 400 Bad request\r\nContent-Length: 0\r\n\r\n", [](error::ErrorSocket const &){});
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

                if (it == error::AsioErrorTranslator.end())
                    std::cerr << "ERROR(network/AsioHttpClient): " << ec << std::endl;
                else
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
            [cb = std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), this] (asio::error_code ec, std::size_t bytesRead) mutable {
            [cb = std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), this] (asio::error_code ec, std::size_t) mutable {
                _asyncRec(ec, std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb));
            }
        );
    }

    void AsioHttpClient::_rec(std::string &str)
    {
        char buff[257]{};

        _socket.receive(asio::buffer(&buff, sizeof(char) * 256));
        str += buff;
    }

    Address const &AsioHttpClient::getAddress() const noexcept
    {
        return _address;
    }

    void AsioHttpClient::_asyncRec(asio::error_code ec, std::function<void(error::ErrorSocket const &, std::string &)> &&cb, std::size_t bytesRead)
    {
        _requestBuffer += std::string{_buffer, _buffer + bytesRead};

        if (ec) {
            try {
                cb(error::AsioErrorTranslator.at(ec), _requestBuffer);
            } catch (std::out_of_range const &) {
                std::cerr << "ERROR(network/AsioHttpClient): " << ec << std::endl;
            }
        } else {
            if (_requestBuffer.find("\r\n\r\n") != std::string::npos && _bodyLength == 0) {
                try {
                    _bodyLength = RequestHelper::getContentLength(_requestBuffer); // body length is now known
            } catch (std::runtime_error &) { // DISCARDS BODY
                cb(error::SOCKET_NO_ERROR, _requestBuffer);
            } catch (std::invalid_argument &) { // invalid header
                send("HTTP1/1.1 400 Bad request\r\nContent-Length: 0\r\n\r\n");
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