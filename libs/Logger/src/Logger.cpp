#include <chrono>
#include <iostream>
#include <sstream>

#include <fmt/core.h>
#include <fmt/chrono.h>

#include "Logger.hpp"
#include "ziapi/HttpConstants.hpp"

using namespace std::chrono_literals;

const std::unordered_map<std::string_view, std::string_view> modules::Logger::s_methodColors{
    {"GET", s_blue},
    {"POST", s_cyan},
    {"PUT", s_yellow},
    {"DELETE", s_red},
    {"PATCH", s_green},
    {"HEAD", s_magenta},
    {"OPTIONS", s_white},
};

void modules::Logger::Init(const ziapi::config::Node &cfg)
{
    try {
        m_pipelineName = cfg["pipelineName"].AsString();
    } catch (const std::out_of_range &) {
        m_pipelineName = "<unknown>";
    }
}

inline ziapi::Version modules::Logger::GetVersion() const noexcept
{
    return {1, 0, 0};
}

inline ziapi::Version modules::Logger::GetCompatibleApiVersion() const noexcept
{
    return {5, 0, 0};
}

inline const char *modules::Logger::GetName() const noexcept
{
    return "Logger";
}

inline const char *modules::Logger::GetDescription() const noexcept
{
    return "Logs parts of the requests and responses so that they can be analyzed in case of weird behaviour";
}

inline void modules::Logger::PreProcess(ziapi::http::Context &ctx, ziapi::http::Request &)
{
    ctx.emplace("received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now()));
}

inline double modules::Logger::GetPreProcessorPriority() const noexcept
{
    return 1;
}

inline bool modules::Logger::ShouldPreProcess(const ziapi::http::Context &, const ziapi::http::Request &) const
{
    return true;
}

void modules::Logger::PostProcess(ziapi::http::Context &ctx, const ziapi::http::Request &req,
                                  ziapi::http::Response &res)
{
    const std::string_view statusColor{_getStatusColor(res.status_code)};
    const std::string_view methodColor{_getMethodColor(req.method)};
    const auto receivedAtChrono{std::any_cast<std::chrono::time_point<std::chrono::system_clock>>(ctx["received_at"])};
    const std::time_t receivedAt{std::chrono::system_clock::to_time_t(receivedAtChrono)};
    const auto requestDuration{std::chrono::system_clock::now() - receivedAtChrono};
    const std::string requestDurationWithUnit{_getDurationWithUnit(requestDuration)};
    const std::string ipAddress{_getIpAddress(std::any_cast<std::uint32_t>(ctx["REMOTE_ADDR"]))};

    fmt::print("[{}] {:%Y/%m/%d - %H:%M:%S} |{} {} {}| {:>13} | {:>15} |{} {:<8}{} {}\n",
               m_pipelineName, fmt::localtime(receivedAt), statusColor, static_cast<int>(res.status_code), s_reset,
               requestDurationWithUnit, ipAddress, methodColor, req.method, s_reset, req.target);
}

inline double modules::Logger::GetPostProcessorPriority() const noexcept
{
    return 0;
}

inline bool modules::Logger::ShouldPostProcess(const ziapi::http::Context &, const ziapi::http::Request &,
                                               const ziapi::http::Response &) const noexcept
{
    return true;
}

inline const std::string_view modules::Logger::_getStatusColor(const ziapi::http::Code &status) const noexcept
{
    if (status >= ziapi::http::Code::kContinue && status < ziapi::http::Code::kOK)
        return s_blue;
    else if (status >= ziapi::http::Code::kOK && status < ziapi::http::Code::kMultipleChoices)
        return s_green;
    else if (status >= ziapi::http::Code::kMultipleChoices && status < ziapi::http::Code::kBadRequest)
        return s_white;
    else if (status >= ziapi::http::Code::kBadRequest && status < ziapi::http::Code::kInternalServerError)
        return s_yellow;
    return s_red;
}

inline const std::string_view modules::Logger::_getMethodColor(const std::string_view &method) const noexcept
{
    const auto &it = s_methodColors.find(method);

    if (it == s_methodColors.cend())
        return s_reset;
    return it->second;
}

inline const std::string modules::Logger::_getDurationWithUnit(const std::chrono::system_clock::duration &duration)
noexcept
{
    if (duration < 1us)
        return {fmt::format("{:.6f}ns", static_cast<double>(duration.count()))};
    else if (duration < 1ms)
        return {fmt::format("{:.6f}µs", static_cast<double>(duration.count()) / 1000)};
    else if (duration < 1s)
        return {fmt::format("{:.6f}ms", static_cast<double>(duration.count()) / (1000 * 1000))};
    else if (duration < 1min)
        return {fmt::format("{:.6f}s", static_cast<double>(duration.count()) / (1000 * 1000 * 1000))};
    return {fmt::format("{:.6f}min", static_cast<double>(duration.count()) / (1000UL * 1000UL * 1000UL * 60UL))};
}

inline const std::string modules::Logger::_getIpAddress(const std::uint32_t &numericIpAddress) const noexcept
{
    std::ostringstream oss{};

    oss << ((numericIpAddress >> 24) & 0xFF) << '.' <<
    ((numericIpAddress >> 16) & 0xFF) << '.' <<
    ((numericIpAddress >> 8) & 0xFF) << '.' <<
    (numericIpAddress & 0xFF);

    return {oss.str()};
}

extern "C" {
    DllExport ziapi::IModule *LoadZiaModule()
    {
        return new modules::Logger{};
    }
}
