#ifndef CGI_HPP
#define CGI_HPP

#include <ziapi/Module.hpp>

namespace modules
{
    class Cgi : public ziapi::IHandlerModule
    {
        public:
            Cgi();

            ~Cgi() override = default;

            void Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res) override;

            void Init(const ziapi::config::Node &cfg) override;

            [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] const char *GetName() const noexcept override;

            [[nodiscard]] const char *GetDescription() const noexcept override;

            [[nodiscard]] double GetHandlerPriority() const noexcept override;

            [[nodiscard]] bool ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const override;

        private:
            std::string _cgiPath;
            env::Manager _env;

            struct Url
            {
                std::string queryStr;
                std::string host;
                std::string route;
            };

            static Url _parseUrl(std::string const &url);

            static std::string _getHostName();

            static void _populateEnv(env::Manager &env, ziapi::http::Context &ctx, const ziapi::http::Request &req);
    };
}

#endif //CGI_HPP
