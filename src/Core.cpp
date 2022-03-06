#include <thread>
#include <future>

#include <RequestOutputQueue.hpp>
#include <ResponseInputQueue.hpp>

#include "Core.hpp"

namespace core
{
    Core::Core(std::string const &name, std::string const &filepath) noexcept :
        _running{false},
        _configLoaded{false},
        _filepath{filepath},
        _parser{_filepath},
        _threadPool{2}
    {}

    void Core::run()
    {
        if (!_configLoaded)
            throw std::runtime_error{"Error, the config isn't loaded"};
        _running = true;
        std::jthread _networkRunThread{[this]() -> void {
            try {
                _networkModule->Run(_requests, _responses);
            } catch (std::exception &err) {
                std::cerr
                << "Error: the network module thrown an exception while running.\n"
                << err.what()
                << std::endl;
            }
            _running = false;
        }};

        while (_running)
        {
            _requests.Wait();
            if (!_running)
                return;
            _threadPool.run([this] { _serveRequest(_requests, _responses); });
        }
    }

    void Core::_serveRequest(
        modules::RequestOutputQueue &requests,
        modules::ResponseInputQueue &responses) const
    {
        ziapi::http::Response response{};
        auto req{requests.Pop()};

        if (!req)
            return;
        auto &[request, ctx] = *req;
        for (auto const &e : _preProcessors)
            if (e->ShouldPreProcess(ctx, request))
                e->PreProcess(ctx, request);
        for (auto const &e : _handlers)
            if (e->ShouldHandle(ctx, request))
                e->Handle(ctx, request, response);
        for (auto const &e : _postProcessors)
            if (e->ShouldPostProcess(ctx, request, response))
                e->PostProcess(ctx, request, response);
        response.version = ziapi::http::Version::kV1;
        responses.Push({std::move(response), std::move(ctx)});
    }

    void Core::stop()
    {
        _running = false;
        _networkModule->Terminate();
        _responses.StopWait();
        _requests.StopWait();
    }

    void Core::config()
    {
        _configLoaded = false;
        _purgeData();
        _parser.parse(_filepath);
        ziapi::config::Dict config = _parser.getConfigMap();
        ziapi::config::Dict modules = config["modules"]->AsDict();

        for (auto const &[moduleName, e] : modules) {
            try {
                auto path = e->AsDict().at("path")->AsString();

                _loadModule(config, path, moduleName);
            } catch (std::out_of_range const &err) {
                std::cerr
                    << "Error, module " + moduleName + " doesn't have a path attribute. The module won't be loaded.\n"
                    << "Error: " << err.what()
                    << std::endl;
            } catch (std::runtime_error const &err) {
                std::cerr << err.what() << std::endl;
            }
        }
        if (_networkModule == nullptr)
            throw std::runtime_error{"Error, the network module is not loaded"};
        else
            _configLoaded = true;
    }

    bool Core::hotReload() noexcept
    {
        _hotReload = !_hotReload;
        return _hotReload;
    }

    bool Core::isRunning() const noexcept
    {
        return _running;
    }

    bool Core::isConfigured() const noexcept
    {
        return _configLoaded;
    }

    void Core::_loadModule(ziapi::config::Node const &cfg, std::string const &path, std::string const &name)
    {
        loader::Loader &loader{_libs.emplace_back(path)};
        std::function<ziapi::IModule *()> symbol{};

        try {
            symbol = loader.getSymbol<ziapi::IModule *()>("LoadZiaModule");
        } catch (std::runtime_error const &e) {
            std::cerr << "Error, cannot find LoadZiaModule method on " << path << " library. " << std::endl;
            return;
        }
        std::shared_ptr<ziapi::IModule> instance{symbol()};

        try {
            instance->Init(cfg);
        } catch (std::exception const &err) {
            std::cerr
                << "Error: " + name + " module thrown during initialization. The module will be skipped.\n"
                << err.what()
                << std::endl;
            return;
        }
        registerModule(instance);
    }

    void Core::registerModule(std::shared_ptr<ziapi::IModule> const &instance)
    {
        if (dynamic_cast<ziapi::IPreProcessorModule *>(instance.get()))
            _preProcessors.emplace_back(std::dynamic_pointer_cast<ziapi::IPreProcessorModule>(instance));
        if (dynamic_cast<ziapi::IHandlerModule *>(instance.get()))
            _handlers.emplace_back(std::dynamic_pointer_cast<ziapi::IHandlerModule>(instance));
        if (dynamic_cast<ziapi::IPostProcessorModule *>(instance.get()))
            _postProcessors.emplace_back(std::dynamic_pointer_cast<ziapi::IPostProcessorModule>(instance));
        if (dynamic_cast<ziapi::INetworkModule *>(instance.get()))
            _networkModule = std::dynamic_pointer_cast<ziapi::INetworkModule>(instance);
    }

    void Core::_purgeData()
    {
        if (_configLoaded)
            throw std::runtime_error{"Error config is in loaded state, cannot perform destruction"};
        _preProcessors.clear();
        _handlers.clear();
        _postProcessors.clear();
        _libs.clear();
    }
}
