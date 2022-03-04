#ifndef ZIA_LOGGER_HPP
#define ZIA_LOGGER_HPP

#include <unordered_map>

#include "ziapi/Module.hpp"
#include "ziapi/Http.hpp"

#ifdef _WIN32
    #define DllExport __declspec(dllexport)
#else
    #define DllExport
#endif

namespace modules {

/**
 * @Class Logger
 * @brief Logs the requests received by the server.
 */
class DllExport Logger : public ziapi::IPreProcessorModule, public ziapi::IPostProcessorModule {

public:

    Logger() = default;
    ~Logger() override = default;

    /* Functions from IModule */
    void Init(const ziapi::config::Node &cfg) override;
    [[nodiscard]] inline ziapi::Version GetVersion() const noexcept override;
    [[nodiscard]] inline ziapi::Version GetCompatibleApiVersion() const noexcept override;
    [[nodiscard]] inline const char *GetName() const noexcept override;
    [[nodiscard]] inline const char *GetDescription() const noexcept override;

    /* Functions from IPreProcessorModule */
    inline void PreProcess(ziapi::http::Context &ctx, ziapi::http::Request &req) override;
    [[nodiscard]] inline double GetPreProcessorPriority() const noexcept override;
    [[nodiscard]] inline bool ShouldPreProcess(const ziapi::http::Context &ctx, const ziapi::http::Request &req)
    const override;

    /* Functions from IPostProcessorModule */
    void PostProcess(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res) override;
    [[nodiscard]] inline double GetPostProcessorPriority() const noexcept override;
    [[nodiscard]] inline bool ShouldPostProcess(const ziapi::http::Context &ctx, const ziapi::http::Request &req,
                                                const ziapi::http::Response &res) const noexcept override;

private:

    /**
     * @brief Returns the color of the background of the status for the logger
     * @param status The status of the response
     * @return The color of the background of the status
     */
    [[nodiscard]] inline const std::string_view _getStatusColor(const ziapi::http::Code &status) const noexcept;

    /**
     * @brief Returns the color of the background of the method for the logger
     * @param method The method of the request
     * @return The color of the background of the method for the logger
     */
    [[nodiscard]] inline const std::string_view _getMethodColor(const std::string_view &method) const noexcept;

    /**
     * @brief Returns the time the request took with its unit
     * @param path The duration of the request
     * @return The time the request took with its unit
     */
    [[nodiscard]] static inline const std::string _getDurationWithUnit(
            const std::chrono::system_clock::duration &duration) noexcept;

    /**
     * @brief Returns the IP of the client formatted as string
     * @param numericIpAddress The IP of the client
     * @return the IP of the client formatted as string
     */
    [[nodiscard]] inline const std::string _getIpAddress(const std::uint32_t &numericIpAddress) const noexcept;

    // The pipeline name that the logs are prefixed with
    std::string_view m_pipelineName{};

    // The colors used by the logger
    static constexpr std::string_view s_white{"\033[90;47m"};
    static constexpr std::string_view s_green{"\033[97;42m"};
    static constexpr std::string_view s_yellow{"\033[90;43m"};
    static constexpr std::string_view s_red{"\033[97;41m"};
    static constexpr std::string_view s_blue{"\033[97;44m"};
    static constexpr std::string_view s_magenta{"\033[97;45m"};
    static constexpr std::string_view s_cyan{"\033[97;46m"};
    static constexpr std::string_view s_reset{"\033[0m"};

    static const std::unordered_map<std::string_view, std::string_view> s_methodColors;

};

}

#endif /* ZIA_LOGGER_HPP */
