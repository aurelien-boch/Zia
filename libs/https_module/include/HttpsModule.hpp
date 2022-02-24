#ifndef HTTPSMODULE_HPP
#define HTTPSMODULE_HPP

#include <ziapi/Module.hpp>

namespace modules
{
    class HttpsModule : ziapi::INetworkModule
    {
        public:

            explicit HttpsModule();

            ~HttpsModule() = default;

            void Init(const ziapi::config::Node &cfg);

            [[nodiscard]] inline ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] inline ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] inline const char *GetName() const noexcept override;

            [[nodiscard]] inline const char *GetDescription() const noexcept override;

            void Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses) override;

            void Terminate() override;
    };
}

#endif //HTTPSMODULE_HPP
