#ifndef ASIOHTTPSLISTENER_HPP
#define ASIOHTTPSLISTENER_HPP

#include <asio/ip/tcp.hpp>
#include <asio/ssl.hpp>

#include <ITCPListener.hpp>

#ifdef _WIN32
    #define DllExport   __declspec( dllexport )
#else
    #define DllExport
#endif


namespace network::https
{
    /**
     * @class AsioHttpsListener
     * @brief Implements a Http Listener with SSL
     */
    class DllExport AsioHttpsListener : public ITCPListener<std::string, std::string>
    {
        public:

            struct CertificateData
            {
                std::string certificatePath;
                std::string certificateKey;
                std::string certificateDhFile;
                std::string certificateKeyPassword;
            };

            using SslSocket = asio::ssl::stream<asio::ip::tcp::socket>;

            using IClient = ITCPClient<std::string, std::string>;

            explicit AsioHttpsListener(asio::io_service &ctx, std::uint16_t port, CertificateData const &data);

            void run(std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback) noexcept override;

        private:
            CertificateData _certificateData;
            asio::ip::tcp::endpoint _endpoint;
            asio::ip::tcp::acceptor _acceptor;
            asio::ssl::context _sslContext;
            SslSocket _sslSocket;
            asio::io_context &_io_context;

            std::string _getPassword() const;

    };
}

#endif //ASIOHTTPSLISTENER_HPP
