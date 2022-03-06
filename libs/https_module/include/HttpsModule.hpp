#ifndef HTTPSMODULE_HPP
#define HTTPSMODULE_HPP

#include <ziapi/Module.hpp>

#include <HttpModule.hpp>

#ifdef _WIN32
    #define DllExport   __declspec( dllexport )
#else
    #define DllExport
#endif


namespace modules
{
    /**
     * @class HttpsModule
     * @brief Implements HTTP Module with SSL
     */
    class DllExport HttpsModule : public HttpModule
    {
        public:

            explicit HttpsModule();

            ~HttpsModule() = default;

            void Init(const ziapi::config::Node &cfg);

            [[nodiscard]] inline ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] inline ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] inline const char *GetName() const noexcept override;

            [[nodiscard]] inline const char *GetDescription() const noexcept override;

        private:

            void _onConnect(
                    ziapi::http::IRequestOutputQueue &requests,
                    error::ErrorSocket const &err,
                    std::shared_ptr<IClient> c) override;

            static const ziapi::Version _version;
            static const ziapi::Version _compatibleApiVersion;

            std::string _certificatePath;
    };
}

#endif //HTTPSMODULE_HPP
