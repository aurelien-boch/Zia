#include <cstring>

#include "http/AsioHttpClient.hpp"

namespace network::http
{

    AsioHttpClient::AsioHttpClient(asio::io_context &io_context) :
        ITCPClient(),
        _socket(asio::ip::tcp::socket(io_context)),
        _packet(),
        _buffer{0}
    {}

    AsioHttpClient::AsioHttpClient(asio::ip::tcp::socket &socket) :
        _socket(std::move(socket)),
        _packet(),
        _buffer{0}
    {}

    AsioHttpClient::~AsioHttpClient()
    {
        this->_socket.close();
    }

    void AsioHttpClient::connect(const Address &peer) noexcept
    {
        asio::ip::tcp::endpoint endpoint(asio::ip::address_v4(peer.ipAddress), peer.port);

        this->_socket.connect(endpoint);
    }

    std::size_t AsioHttpClient::send(const std::string &data) noexcept
    {
        try {
            return this->_socket.send(asio::buffer(data.c_str(), sizeof(char) * data.size()));
        } catch (std::system_error &err) {
            std::cerr << "ERROR(network/AsioHttpClient): " << err.what() << std::endl;
            return (-1);
        }
    }

    std::string AsioHttpClient::receive() noexcept
    {
        std::string request;

        while(request.find("\r\n") == std::string::npos) {
            char buff[256] = {0};

            this->_socket.receive(asio::buffer(&buff, sizeof(char) * 256));
            request += std::string(buff);
        }
        return request;
    }

    void AsioHttpClient::asyncSend(
        const std::string &packet,
        std::function<void(const error::ErrorSocket &)> &&cb) noexcept
    {
        this->_socket.async_send(asio::buffer(packet.data(), sizeof(char) * packet.size()),
            [cb = std::forward<std::function<void (const error::ErrorSocket &)>>(cb)] (const asio::error_code &ec, std::size_t) {
            if (ec) {
                auto it = error::AsioErrorTranslator.find(ec);

                if (it == error::AsioErrorTranslator.end()) {
                    std::cerr << "ERROR(network/AsioHttpClient): " << ec << std::endl;
                } else
                    cb(it->second);
            } else
                 cb(error::SOCKET_NO_ERROR);
        });
    }

    void AsioHttpClient::asyncRead(
        std::function<void(const error::ErrorSocket &, std::string &)> &&cb
    ) noexcept
    {
        this->_socket.async_receive(
            asio::buffer(&_buffer, sizeof(char) * 256),
            [cb = std::forward<std::function<void (const error::ErrorSocket &, std::string &)>>(cb), this] (asio::error_code ec, std::size_t) mutable {
                _packet += std::string(_buffer);
                if (ec) {
                    auto it = error::AsioErrorTranslator.find(ec);

                    if (it == error::AsioErrorTranslator.end())
                        std::cerr << "ERROR(network/AsioHttpClient): " << ec << std::endl;
                    else
                        cb(it->second, _packet);
                } else if (_packet.find("\r\n") != std::string::npos) {
                    cb(error::SOCKET_NO_ERROR, _packet);
                    _packet = "";
                }
                asyncRead(std::move(cb));
            }
        );
    }
}