/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-aurelien.boch
** File description:
** AsioTCPClient
*/


#ifndef ASIOHTTPCLIENT_HPP_
#define ASIOHTTPCLIENT_HPP_

#include <string>

#include <asio.hpp>

#include "Utility.hpp"
#include "ITCPClient.hpp"

namespace network::http
{
    class AsioHttpClient : public ITCPClient<std::string, std::string>
    {
        public:
            explicit AsioHttpClient(asio::io_context &io_context);

            explicit AsioHttpClient(asio::ip::tcp::socket &socket);

            ~AsioHttpClient() override;

            void connect(const Address &peer) noexcept override;

            std::size_t send(const std::string &data) noexcept override;

            [[nodiscard]] std::string receive() noexcept override;

            void asyncSend(
                const std::string &packet,
                const std::function<void(const error::ErrorSocket &)> &callback) noexcept override;

            void asyncRead(
                const std::function<void(const error::ErrorSocket &, std::string &)> &callback
            ) noexcept override;

        private:
            asio::ip::tcp::socket _socket;
            std::string _packet;
    };

}

#endif /* !ASIOHTTPCLIENT_HPP_ */
