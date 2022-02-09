#include "Core.hpp"

class A : public ziapi::http::IResponseInputQueue {
    public:
        std::pair<ziapi::http::Request, ziapi::http::Context> Pop() override
        {
            return {};
        }

        [[nodiscard]] std::size_t Size() const noexcept
        {
            return 0;
        }
};
class B : public ziapi::http::IRequestOutputQueue {};

namespace core
{
    Core::Core(std::string &&filepath) noexcept :
        _running(false),
        _configLoaded(false)
    {

    }

    [[noreturn]] void Core::run()
    {
        std::unique_ptr<ziapi::http::IRequestOutputQueue> requests = std::make_unique<A>();
        std::unique_ptr<ziapi::http::IResponseInputQueue> responses;

//        _networkModule->Run(*requests, *responses); -> error => blocking
        _running = true;
        while (_running)
        {
        }
    }

    void Core::stop()
    {
        _networkModule->Terminate();
        _running = false;
    }

    void Core::_config() noexcept
    {
        _configLoaded = false;
        //todo load config
        _configLoaded = true;
    }
}
