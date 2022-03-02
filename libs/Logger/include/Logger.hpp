#ifndef ZIA_LOGGER_HPP
#define ZIA_LOGGER_HPP

#include "ziapi/Module.hpp"
#include "ziapi/Http.hpp"

#ifdef _WIN32
    #define DllExport __declspec(dllexport)
#else
    #define DllExport
#endif

namespace post_process_modules {

class DllExport Logger : public ziapi::IPostProcessorModule {

public:

    Logger() = default;

    ~Logger() override = default;

    void Init(const ziapi::config::Node &cfg) override;

    [[nodiscard]] inline ziapi::Version GetVersion() const noexcept override;

    [[nodiscard]] inline ziapi::Version GetCompatibleApiVersion() const noexcept override;

    [[nodiscard]] inline const char *GetName() const noexcept override;

    [[nodiscard]] inline const char *GetDescription() const noexcept override;

    void PostProcess(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res) override;

    [[nodiscard]] inline double GetPostProcessorPriority() const noexcept override;

    [[nodiscard]] inline bool ShouldPostProcess(const ziapi::http::Context &ctx, const ziapi::http::Request &req,
                                                const ziapi::http::Response &res) const noexcept override;

private:

    [[nodiscard]] inline const std::string_view _getStatusColor(const ziapi::http::Code &status) const noexcept;

    [[nodiscard]] inline const std::string_view _getMethodColor(const std::string_view &method) const noexcept;

    [[nodiscard]] static inline const std::string _getDurationWithUnit(
            const std::chrono::system_clock::duration &duration) noexcept;

    [[nodiscard]] inline const std::string _getIpAddress(const std::uint32_t &numericIpAddress) const noexcept;

    std::string_view m_pipelineName{};

    static constexpr std::string_view s_white{"\033[90;47m"};
    static constexpr std::string_view s_green{"\033[97;42m"};
    static constexpr std::string_view s_yellow{"\033[90;43m"};
    static constexpr std::string_view s_red{"\033[97;41m"};
    static constexpr std::string_view s_blue{"\033[97;44m"};
    static constexpr std::string_view s_magenta{"\033[97;45m"};
    static constexpr std::string_view s_cyan{"\033[97;46m"};
    static constexpr std::string_view s_reset{"\033[0m"};

};

}

#endif /* ZIA_LOGGER_HPP */
