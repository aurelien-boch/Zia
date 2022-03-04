#ifndef STATICSERVE_HPP
#define STATICSERVE_HPP

#include <filesystem>
#include <ziapi/Module.hpp>

namespace modules
{
    class StaticServe : public ziapi::IHandlerModule
    {
        public:
            StaticServe() = default;

            ~StaticServe() override = default;

            void Handle(ziapi::http::Context &ctx, ziapi::http::Request const &req, ziapi::http::Response &res) override;

            void Init(const ziapi::config::Node &cfg) override;

            [[nodiscard]] ziapi::Version GetVersion() const noexcept override;

            [[nodiscard]] ziapi::Version GetCompatibleApiVersion() const noexcept override;

            [[nodiscard]] const char *GetName() const noexcept override;

            [[nodiscard]] const char *GetDescription() const noexcept override;

            [[nodiscard]] double GetHandlerPriority() const noexcept override;

            [[nodiscard]] bool ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const override;

        private:
            std::filesystem::path _serveDirPath;

            static const std::array<std::string, 5> _sizes;

            [[nodiscard]] bool _mayBeServed(std::string const &path) const noexcept;

            void _serveDir(std::string const &path, ziapi::http::Response &res) const noexcept;

            static void _setupHtml(std::string const &path, ziapi::http::Response &res) noexcept;

            static void _endHtml(ziapi::http::Response &res) noexcept;

            static std::string _sizeToHuman(uintmax_t size) noexcept;

            static void _serveFile(std::string const &path, ziapi::http::Response &response);

            static void _appendFile(ziapi::http::Response &res, std::filesystem::directory_entry const &e) ;
    };
}

#endif //STATICSERVE_HPP
