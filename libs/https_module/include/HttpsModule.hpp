#ifndef HTTPSMODULE_HPP
#define HTTPSMODULE_HPP

#include <ziapi/Module.hpp>

#include <HttpModule.hpp>

namespace modules
{
    class HttpsModule : public HttpModule
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

        private:

            static const ziapi::Version _version;
            static const ziapi::Version _compatibleApiVersion;

            std::string _certificatePath;
    };
}

#endif //HTTPSMODULE_HPP
