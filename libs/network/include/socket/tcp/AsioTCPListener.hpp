/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-aurelien.boch
** File description:
** AsioTCPListener
*/


#ifndef ASIOTCPLISTENER_HPP_
#define ASIOTCPLISTENER_HPP_

#include <asio.hpp>
#include <memory>

#include "AsioTCPClient.hpp"
#include "ITCPListener.hpp"

namespace network::socket
{
    template <typename T>
    class AsioTCPListener : public ITCPListener<T>
    {
        public:
            explicit AsioTCPListener(asio::io_service &ctx, std::uint16_t port) :
//                ITCPListener(),
                _io_context(ctx),
                _acceptor(asio::ip::tcp::acceptor(ctx))
            {
                asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port);

                _acceptor.open(asio::ip::tcp::v4());
                _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
                _acceptor.bind(ep);
                _acceptor.listen();
            }

            ~AsioTCPListener() override = default;

            void run(const std::function<void(
                const error::ErrorSocket &,
                std::shared_ptr<ITCPClient<T>>)> &callback) noexcept override
                {
                    this->_acceptor.async_accept(
                        this->_io_context,
                        [this, callback] (const asio::error_code& error, asio::ip::tcp::socket peer) {
                            if (error) {
                                auto it = error::AsioErrorTranslator.find(error);

                                if (it == error::AsioErrorTranslator.end())
                                    std::cerr << "Asio error: " << error << std::endl;
                                else
                                    callback(it->second, nullptr);
                            } else {
                                auto res = std::make_unique<AsioTCPClient<T>>(peer);

                                callback(error::SOCKET_NO_ERROR, std::move(res));
                            }
                            this->run(callback);
                        }
                    );
                }

        private:
            asio::io_context &_io_context;
            asio::ip::tcp::acceptor _acceptor;
    };

}

#endif /* !ASIOTCPLISTENER_HPP_ */
