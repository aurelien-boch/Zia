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
        auto &httpsConfig = cfg["modules"]["https"];
        int port = httpsConfig["port"].AsInt();

        if (port < 0)
            throw std::runtime_error("ERROR(modules/Https): Invalid port in configuration file");
        try {
            _certificatePath = httpsConfig["certificatePath"];
        } catch (std::exception const &) {}
        _port = port;
        _listener = std::make_unique<network::https::AsioHttpsListener>(_service, _port, _certificatePath);
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
