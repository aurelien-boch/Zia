#include <chrono>

#include "RequestReceptionTime.hpp"

void pre_process_modules::RequestReceptionTime::Init(const ziapi::config::Node &)
{}

inline ziapi::Version pre_process_modules::RequestReceptionTime::GetVersion() const noexcept
{
    return {1, 0, 0};
}

inline ziapi::Version pre_process_modules::RequestReceptionTime::GetCompatibleApiVersion() const noexcept
{
    return {5, 0, 0};
}

inline const char *pre_process_modules::RequestReceptionTime::GetName() const noexcept
{
    return "RequestReceptionTime";
}

inline const char *pre_process_modules::RequestReceptionTime::GetDescription() const noexcept
{
    return "Retrieves the time at which the request has been received";
}

inline void pre_process_modules::RequestReceptionTime::PreProcess(ziapi::http::Context &ctx, ziapi::http::Request &)
{
    ctx.emplace("received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now()));
}

inline double pre_process_modules::RequestReceptionTime::GetPreProcessorPriority() const noexcept
{
    return 1;
}

inline bool pre_process_modules::RequestReceptionTime::ShouldPreProcess(const ziapi::http::Context &,
                                                                        const ziapi::http::Request &) const
{
    return true;
}