#include "AsioHttpsClient.hpp"

namespace network::https
{
    AsioHttpsClient::AsioHttpsClient(asio::ssl::context &ctx, std::string const &certificatePath) :
        _context{ctx}
    {}

    void AsioHttpsClient::connect(Address const &peer) noexcept
    {}

    std::size_t AsioHttpsClient::send(std::string const &data) noexcept
    {}

    std::string receive() noexcept
    {}

    void asyncSend(
            std::string const &packet,
    std::function<void(error::ErrorSocket const &)> &&callback) noexcept
    {}

    void asyncReceive(
            std::function<void(error::ErrorSocket const &, std::string &)> &&callback
    ) noexcept
    {}
}