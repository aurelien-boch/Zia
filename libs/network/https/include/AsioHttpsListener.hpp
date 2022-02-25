#ifndef ASIOHTTPSLISTENER_HPP
#define ASIOHTTPSLISTENER_HPP

#include <AsioHttpListener.hpp>

namespace network::https
{
    /**
     * @class Implements a Http Listener with SSL
     */
    class AsioHttpsListener : public http::AsioHttpListener
    {
        public:

            using IClient = ITCPClient<std::string, std::string>;

            explicit AsioHttpsListener(asio::io_service &ctx, std::uint16_t port, std::string const &certificatePath);

        protected:
            void _emplaceClient(asio::ip::tcp::socket &peer,
                std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback) override;

        private:
            std::string _certificatePath;

    };
}

#endif //ASIOHTTPSLISTENER_HPP
