#ifndef ASIOHTTPCLIENT_HPP_
#define ASIOHTTPCLIENT_HPP_

#include <string>

#include <asio/ip/tcp.hpp>

#include <Utility.hpp>
#include <ITCPClient.hpp>

#ifdef _WIN32
    #define DllExport   __declspec( dllexport )
#else
    #define DllExport
#endif

namespace network::http
{
    /**
     * @class Implements an Http client using the Asio library
     */
    class DllExport AsioHttpClient : public ITCPClient<std::string, std::string>
    {
        public:
            explicit AsioHttpClient(asio::io_context &io_context);

            explicit AsioHttpClient(asio::ip::tcp::socket &socket);

            ~AsioHttpClient() override;

            void connect(Address const &peer) noexcept;

            [[nodiscard]]  std::size_t send(std::string const &data) noexcept override;

            [[nodiscard]] std::string receive() noexcept override;

            void asyncSend(
                std::string const &packet,
                std::function<void(error::ErrorSocket const &)> &&callback) noexcept override;

            void asyncReceive(
                std::function<void(error::ErrorSocket const &, std::string &)> &&callback
            ) noexcept override;

            [[nodiscard]] inline Address const &getAddress() const noexcept override;


        protected:
            asio::ip::tcp::socket _socket;
            Address _address;
<<<<<<< HEAD
            std::string _requestBuffer;
            std::size_t _bodyLength;
            std::size_t _totalBytesRead;
=======
        private:

            std::string _header;
            std::string _body;
>>>>>>> 2a5503a (feat(network/https): Add connect method implementation in AsioHttpsClient)
            char _buffer[257];

            [[nodiscard]] std::size_t _getContentLength(std::string const &header);

            void _rec(std::string &str);

            void _cleanHeader(std::string &header, std::string &body);
    };
}

#endif /* !ASIOHTTPCLIENT_HPP_ */
