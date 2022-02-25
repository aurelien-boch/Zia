#include <RequestOutputQueue.hpp>
#include <ResponseInputQueue.hpp>
#include <thread>

#include "Core.hpp"

namespace core
{
    Core::Core(std::string &filepath) noexcept :
        _running{false},
        _configLoaded{false},
        _filepath{filepath},
        _parser{_filepath}
    {}

    void Core::run()
    {
        if (!_configLoaded)
            throw std::runtime_error{"Error, the config isn't loaded"};
        std::unique_ptr<modules::RequestOutputQueue> requests = std::make_unique<modules::RequestOutputQueue>();
        std::unique_ptr<modules::ResponseInputQueue> responses = std::make_unique<modules::ResponseInputQueue>();
        std::jthread _networkRunThread{[this, &requests, &responses]() -> void {
            _networkModule->Run(*requests, *responses);
        }};

        _running = true;
        while (true)
        {
            while (requests->Size() == 0 && _running)
                std::this_thread::yield();
            if (!_running)
                return;
            _serveRequest(requests, responses);
        }
    }

    void Core::_serveRequest(
        std::unique_ptr<modules::RequestOutputQueue> &requests,
        std::unique_ptr<modules::ResponseInputQueue> &responses)
    {
        ziapi::http::Response response{};
//        std::optional<std::pair<ziapi::http::Request, ziapi::http::Context>> req{requests->Pop()};

//        if (!req)
//            return;
//        auto &[request, ctx] = *req;
//        for (auto const &e : _preProcessors)
//            e->PreProcess(ctx, request);
//        for (auto const &e : _handlers)
//            e->Handle(ctx, request, response);
//        for (auto const &e : _postProcessors)
//            e->PostProcess(ctx, response);
//        responses->Push({response, ctx});
    }

    void Core::stop()
    {
        _networkModule->Terminate();
        _running = false;
    }

    void Core::config() noexcept
    {
        _configLoaded = false;
        _purgeData();
        _parser.parse(_filepath);
        ziapi::config::Dict config = _parser.getConfigMap();
        ziapi::config::Dict modules = config["modules"]->AsDict();

        for (auto const &[_, e] : modules) {
            try {
                auto path = e->AsDict().at("path")->AsString();

                _loadModule(config, path);
            } catch (std::out_of_range const &_) {};
        }
        _configLoaded = true;
    }

    void Core::_loadModule(const ziapi::config::Node &cfg, std::string &path)
    {
        loader::Loader &loader = _libs.emplace_back(path);
        std::function<ziapi::IModule *()> symbol;

        try {
            symbol = loader.getSymbol<ziapi::IModule *()>("LoadZiaModule");
        } catch (std::runtime_error &e) {
            std::cerr << "Error, cannot find LoadZiaModule method on " << path << " library. " << std::endl;
        }
        std::shared_ptr<ziapi::IModule> instance{symbol()};

        instance->Init(cfg);
        if (dynamic_cast<ziapi::IPreProcessorModule *>(instance.get()))
            _preProcessors.emplace_back(std::dynamic_pointer_cast<ziapi::IPreProcessorModule>(instance));
        if (dynamic_cast<ziapi::IHandlerModule *>(instance.get()))
            _handlers.emplace_back(std::dynamic_pointer_cast<ziapi::IHandlerModule>(instance));
        if (dynamic_cast<ziapi::IPostProcessorModule *>(instance.get()))
            _postProcessors.emplace_back(std::dynamic_pointer_cast<ziapi::IPostProcessorModule>(instance));
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
