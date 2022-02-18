#include <cstring>

#include "AsioHttpClient.hpp"

namespace network::http
{
    AsioHttpClient::AsioHttpClient(asio::io_context &io_context) :
        ITCPClient(),
        _socket(asio::ip::tcp::socket(io_context)),
        _packet(),
        _buffer{}
    {}

    AsioHttpClient::AsioHttpClient(asio::ip::tcp::socket &socket) :
        _socket(std::move(socket)),
        _packet(),
        _buffer{}
    {}

    AsioHttpClient::~AsioHttpClient()
    {
        this->_socket.close();
    }

    void AsioHttpClient::connect(Address const &peer) noexcept
    {
        asio::ip::tcp::endpoint endpoint(asio::ip::address_v4(peer.ipAddress), peer.port);

        this->_socket.connect(endpoint);
    }

    std::size_t AsioHttpClient::send(std::string const &data) noexcept
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
        std::string request{};

        while(request.find("\r\n\r\n") == std::string::npos) {
            char buff[257]{};

            this->_socket.receive(asio::buffer(&buff, sizeof(char) * 256));
            request += std::string(buff);
        }
        return request;
    }

    void AsioHttpClient::asyncSend(
        std::string const &packet,
        std::function<void(error::ErrorSocket const &)> &&cb) noexcept
    {
        this->_socket.async_send(asio::buffer(packet.data(), sizeof(char) * packet.size()),
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
        this->_socket.async_receive(
            asio::buffer(&_buffer, sizeof(char) * 256),
            [cb = std::forward<std::function<void (error::ErrorSocket const &, std::string &)>>(cb), this] (asio::error_code ec, std::size_t) mutable {
                _packet += std::string(_buffer);
                if (ec) {
                    const auto it = error::AsioErrorTranslator.find(ec);

                    if (it == error::AsioErrorTranslator.end())
                        std::cerr << "ERROR(network/AsioHttpClient): " << ec << std::endl;
                    else
                        cb(it->second, _packet);
                } else if (_packet.find("\r\n") != std::string::npos) {
                    cb(error::SOCKET_NO_ERROR, _packet);
                    _packet = "";
                }
                asyncReceive(std::move(cb));
            }
        );
    }
}