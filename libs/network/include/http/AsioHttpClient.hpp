#ifndef ASIOHTTPCLIENT_HPP_
#define ASIOHTTPCLIENT_HPP_

#include <string>

#include <asio.hpp>

#include "Utility.hpp"
#include "ITCPClient.hpp"

namespace network::http
{
    /**
     * Implements a Http client using the Asio library
     */
    class AsioHttpClient : public ITCPClient<std::string, std::string>
    {
        public:
            explicit AsioHttpClient(asio::io_context &io_context);

            explicit AsioHttpClient(asio::ip::tcp::socket &socket);

            ~AsioHttpClient() override;

            void connect(Address const &peer) noexcept override;

            std::size_t send(std::string const &data) noexcept override;

            [[nodiscard]] std::string receive() noexcept override;

            void asyncSend(
                std::string const &packet,
                std::function<void(error::ErrorSocket const &)> &&callback) noexcept override;

            void asyncReceive(
                std::function<void(error::ErrorSocket const &, std::string &)> &&callback
            ) noexcept override;

        private:
            asio::ip::tcp::socket _socket;
            std::string _packet;
            char _buffer[257];
    };
}

#endif /* !ASIOHTTPCLIENT_HPP_ */
