#ifndef ZIA_REQUESTRECEPTIONTIME_HPP
#define ZIA_REQUESTRECEPTIONTIME_HPP

#include "ziapi/Module.hpp"

#ifdef _WIN32
    #define DllExport __declspec(dllexport)
#else
    #define DllExport
#endif

namespace pre_process_modules {

/**
 * @Class RequestReceptionTime
 * @brief This module is used to store the time at which the request has been received
 */
class DllExport RequestReceptionTime : public ziapi::IPreProcessorModule {

public:

    RequestReceptionTime() = default;

    ~RequestReceptionTime() override = default;

    void Init(const ziapi::config::Node &cfg) override;

    [[nodiscard]] inline ziapi::Version GetVersion() const noexcept override;

    [[nodiscard]] inline ziapi::Version GetCompatibleApiVersion() const noexcept override;

    [[nodiscard]] inline const char *GetName() const noexcept override;

    [[nodiscard]] inline const char *GetDescription() const noexcept override;

    inline void PreProcess(ziapi::http::Context &ctx, ziapi::http::Request &req) override;

    [[nodiscard]] inline double GetPreProcessorPriority() const noexcept override;

    [[nodiscard]] inline bool ShouldPreProcess(const ziapi::http::Context &ctx, const ziapi::http::Request &req)
    const override;

};

}

#endif /* ZIA_REQUESTRECEPTIONTIME_HPP */
