#include "HttpsModule.hpp"

#include <Loader.hpp>

#include <AsioHttpsListener.hpp>

namespace modules
{
    const ziapi::Version HttpsModule::_version = {1, 1, 1};
    const ziapi::Version HttpsModule::_compatibleApiVersion = {3, 1, 1};

    HttpsModule::HttpsModule() :
        HttpModule{},
        _certificatePath{}
    {}

    void HttpsModule::Init(const ziapi::config::Node &cfg)
    {
        network::https::AsioHttpsListener::CertificateData data{
            .certificatePath = "../certificate/server.crt",
            .certificateKey = "../certificate/server.key",
            .certificateDhFile = "../certificate/dh2048.pem",
            .certificateKeyPassword = "password",
        };

        _port = 443;
        try {
            auto &httpsConfig = cfg["modules"]["https"];

            try {
                int port = httpsConfig["port"].AsInt();

                _port = (port < 0) ? 443 : port;
            } catch (std::exception const &) {}
            try {
                data.certificatePath = httpsConfig["certificatePath"].AsString();
            } catch (std::exception const &) {}
            try {
                data.certificateKey = httpsConfig["certificateKey"].AsString();
            } catch (std::exception const &) {}
            try {
                data.certificateDhFile = httpsConfig["certificateDhFile"].AsString();
            } catch (std::exception const &) {}
            try {
                .certificateKeyPassword = httpsConfig["certificateKeyPassword"].AsString();
            } catch (std::exception const &) {}
        } catch (std::exception const &) {}
        _listener = std::make_unique<network::https::AsioHttpsListener>(_service, _port, data);
    }

    ziapi::Version HttpsModule::GetVersion() const noexcept
    {
        return _version;
    }

    ziapi::Version HttpsModule::GetCompatibleApiVersion() const noexcept
    {
        return _compatibleApiVersion;
    }

    const char *HttpsModule::GetName() const noexcept
    {
        return "Https";
    }

    const char *HttpsModule::GetDescription() const noexcept
    {
        return "A https module that implements Http/1.1 protocol with SSL.";
    }
}

extern "C" {
    DllExport ziapi::IModule * LoadZiaModule()
    {
        return new modules::HttpsModule();
    }
}
