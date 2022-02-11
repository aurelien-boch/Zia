/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-aurelien.boch
** File description:
** AsioTCPClient
*/


#ifndef ASIOTCPCLIENT_HPP_
#define ASIOTCPCLIENT_HPP_

#include <asio.hpp>

#include "socket/Utility.hpp"

#include "socket/tcp/ITCPClient.hpp"


namespace network::socket
{
    template <typename T>
    class AsioTCPClient : public ITCPClient<T>
    {
        public:
            explicit AsioTCPClient(asio::io_context &io_context):
                _socket(asio::ip::tcp::socket(io_context))
            {}

            explicit AsioTCPClient(asio::ip::tcp::socket &socket) :
                    // ITCPClient(),
                    _socket(std::move(socket))
            {}

            ~AsioTCPClient() override
            {
                this->_socket.close();
            }

            void connect(const Address &peer) noexcept override
            {
                asio::ip::tcp::endpoint endpoint(asio::ip::address_v4(peer.ipAddress), peer.port);

                this->_socket.connect(endpoint);
            }

            std::size_t send(const T &data) noexcept override
            {
                try {
                    return this->_socket.send(asio::buffer(&data, sizeof(data)));
                } catch (std::system_error &err) {
                    std::cerr << err.what() << std::endl;
                    return (-1);
                }
            }

            [[nodiscard]] T receive() noexcept override
            {
                T p{};

                this->_socket.receive(asio::buffer(&p, sizeof(p)));
                return p;
            }

            void asyncSend(
                const T &packet,
                const std::function<void(const error::ErrorSocket &)> &callback) noexcept override
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
                this->_socket.async_send(asio::buffer(&packet, sizeof(packet)), cb);
            }

            void asyncRead(
                const std::function<void(const error::ErrorSocket &, T &)> &callback
            ) noexcept override
            {
                this->_socket.async_receive(
                    asio::buffer(&_packet, sizeof(_packet)),
                    [callback, this] (asio::error_code ec, std::size_t) {
                        if (ec) {
                            auto it = error::AsioErrorTranslator.find(ec);

                            if (it == error::AsioErrorTranslator.end())
                                std::cerr << "Asio error: " << ec << std::endl;
                            else
                                callback(it->second, _packet);
                        } else
                            callback(error::SOCKET_NO_ERROR, _packet);
                        asyncRead(callback);
                    }
                );
            }

        private:
            asio::ip::tcp::socket _socket;
            T _packet{};
    };

}

#endif /* !ASIOTCPCLIENT_HPP_ */
