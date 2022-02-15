/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-aurelien.boch
** File description:
** AsioTCPListener
*/


#ifndef ASIOHTTPLISTENER_HPP_
#define ASIOHTTPLISTENER_HPP_

#include <asio.hpp>
#include <memory>

#include "AsioHttpClient.hpp"
#include "ITCPListener.hpp"

namespace network::http
{
    class AsioHttpListener : public ITCPListener<std::string, std::string>
    {
        public:
            explicit AsioHttpListener(asio::io_service &ctx, std::uint16_t port);

            ~AsioHttpListener() override = default;

            void run(const std::function<void(
                const error::ErrorSocket &,
                std::shared_ptr<ITCPClient<std::string, std::string>>)> &callback) noexcept override;

        private:
            asio::io_context &_io_context;
            asio::ip::tcp::acceptor _acceptor;
    };
}

#endif /* !ASIOHTTPLISTENER_HPP_ */