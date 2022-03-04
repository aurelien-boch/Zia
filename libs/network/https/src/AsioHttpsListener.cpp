#include <asio/ssl.hpp>

#include "AsioHttpsListener.hpp"

#include "AsioHttpsClient.hpp"

namespace network::https
{
    AsioHttpsListener::AsioHttpsListener(asio::io_service &ctx, std::uint16_t port, CertificateData const &data) :
        _certificateData{data},
        _endpoint(asio::ip::tcp::v4(), port),
        _acceptor(ctx, _endpoint),
        _sslContext(asio::ssl::context::tls),
        _sslSocket(ctx, _sslContext),
        _io_context(ctx)
    {
        _sslContext.set_options(
                asio::ssl::context::default_workarounds
                | asio::ssl::context::no_sslv2);
        _sslContext.set_password_callback(std::bind(&AsioHttpsListener::_getPassword, this));
        try {
            std::cout << std::endl << "[ASIO HTTPS LISTENER] Loading certificate \t" << _certificateData.certificatePath << std::endl;
            _sslContext.use_certificate_chain_file(_certificateData.certificatePath);
            std::cout << "[ASIO HTTPS LISTENER] Loading key \t\t" << _certificateData.certificateKey << std::endl;
            _sslContext.use_private_key_file(_certificateData.certificateKey, asio::ssl::context::pem);
            std::cout << "[ASIO HTTPS LISTENER] Loading dh file \t\t" << _certificateData.certificateDhFile <<std::endl << std::endl;
            _sslContext.use_tmp_dh_file(_certificateData.certificateDhFile);
        } catch (std::system_error const &e) {
            throw std::runtime_error("Error while setting up SSL certificate: " + std::string(e.what()));
        }
    }

    void AsioHttpsListener::run(std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)> &&callback) noexcept
    {
        _acceptor.async_accept([this, callback = std::forward<std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)>>(callback)] (asio::error_code const &error, asio::ip::tcp::socket peer) mutable
        {
            if (error) {
                const auto it = error::AsioErrorTranslator.find(error);

                if (it == error::AsioErrorTranslator.end())
                    std::cerr << "ERROR(network/AsioHttpListener): " << error << std::endl;
                else
                    callback(it->second, nullptr);
            } else {
                try {
                    SslSocket s(std::move(peer), _sslContext);
                    auto res = std::make_unique<AsioHttpsClient>(_io_context, std::move(s));

                    callback(error::ErrorSocket::SOCKET_NO_ERROR, std::move(res));
                    std::cout << "New client connected" << std::endl;
                } catch (std::runtime_error const &e) {
                    std::cerr << "ERROR(network/AsioHttpsListener): " << e.what() << std::endl;
                }
            }
            run(std::forward<std::function<void (error::ErrorSocket const &, std::shared_ptr<IClient>)>>(callback));
        });
    }

    std::string AsioHttpsListener::_getPassword() const
    {
        return _certificateData.certificateKeyPassword;
    }

}

