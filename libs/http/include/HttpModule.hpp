#ifndef HTTPMODULE_HPP
#define HTTPMODULE_HPP

#include <string>

#include <ziapi/Module.hpp>

namespace modules
{
    class HttpModule : public ziapi::INetworkModule
    {
        public:

            HttpModule();

            ~HttpModule();

            void Init(const ziapi::config::Node &cfg);

            [[nodiscard]] ziapi::Version GetVersion() const noexcept;

            [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept;

            [[nodiscard]] const char *GetName() const noexcept;

            [[nodiscard]] const char *GetDescription() const noexcept;

            void Run(ziapi::http::IRequestOutputQueue &requests, ziapi::http::IResponseInputQueue &responses);

            void Terminate();

        private:
            bool _run;
            ziapi::Version _version;
            ziapi::Version _compatibleApiVersion;
            std::string _name;
            std::string _description;

            void _listen(ziapi::http::IRequestOutputQueue &requests);

            void _send(ziapi::http::IResponseInputQueue &requests);

    };
}

#endif //HTTPMODULE_HPP
