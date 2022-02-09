#ifndef ASIOTCPLISTENER_HPP
#define ASIOTCPLISTENER_HPP

#include <asio.hpp>

#include <ziapi/Http.hpp>

namespace modules
{
    template <typename T>
    class AsioTCPListener
    {
        public:
            explicit AsioTCPListener(asio::io_service &ctx, std::uint16_t port) :
                _context(ctx),
                _acceptor(asio::ip::tcp::acceptor(ctx))
            {
                asio::ip::tcp::endpoint ep(asio::ip::tcp::v4(), port);

                _acceptor.open(asio::ip::tcp::v4());
                _acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
                _acceptor.bind(ep);
                _acceptor.listen();
            }

            ~AsioTCPListener() = default;

            void run(const std::function<void(T &&data)> &callback) noexcept
            {
                this->_acceptor.async_accept(
                    this->_context,
                    [this, callback] ([[maybe_unused]] const asio::error_code& error, T &&data) {
                        auto res = std::make_unique<T>(data);

                        callback(std::move(res));
                        this->run(callback);
                    }
                );
            }

        private:
            asio::io_context &_context;
            asio::ip::tcp::acceptor _acceptor;
    };
}

#endif //ASIOTCPLISTENER_HPP
