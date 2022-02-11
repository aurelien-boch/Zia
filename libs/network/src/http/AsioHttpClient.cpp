#include "http/AsioHttpClient.hpp"

namespace network::http
{

    AsioHttpClient::AsioHttpClient(asio::io_context &io_context):
        _socket(asio::ip::tcp::socket(io_context)),
        _packet()
    {}

    AsioHttpClient::AsioHttpClient(asio::ip::tcp::socket &socket) :
        _socket(std::move(socket)),
        _packet()
    {}

    AsioHttpClient::~AsioHttpClient() override
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
            std::cerr << err.what() << std::endl;
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
        const std::function<void(const error::ErrorSocket &)> &callback) noexcept
    {
        auto cb = [&callback] (const asio::error_code &ec, std::size_t) {
            if (ec) {
                auto it = error::AsioErrorTranslator.find(ec);

                if (it == error::AsioErrorTranslator.end()) {
                    std::cerr << "Asio error: " << ec << std::endl;
                } else
                    callback(it->second);
            } else
                callback(error::SOCKET_NO_ERROR);
        };
        this->_socket.async_send(asio::buffer(packet.c_str(), sizeof(char) * packet.size()), cb);
    }

    void AsioHttpClient::asyncRead(
        const std::function<void(const error::ErrorSocket &, std::string &)> &callback
    ) noexcept
    {
        char packet[256] = {0};

        this->_socket.async_receive(
            asio::buffer(&packet, sizeof(char) * 256),
            [packet, callback, this] (asio::error_code ec, std::size_t) {
                _packet += std::string(packet);
                if (ec) {
                    auto it = error::AsioErrorTranslator.find(ec);

                    if (it == error::AsioErrorTranslator.end())
                        std::cerr << "ASIO ERROR: " << ec << std::endl;
                    else
                        callback(it->second, _packet);
                } else if (_packet.find("\r\n") != std::string::npos) {
                    callback(error::SOCKET_NO_ERROR, _packet);
                    _packet = "";
                }
                asyncRead(callback);
            }
        );
    }


}