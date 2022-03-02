#include <chrono>
#include <thread>

#include <catch2/catch_test_macros.hpp>

#include "ziapi/Config.hpp"
#include "ziapi/Http.hpp"
#include "ziapi/HttpConstants.hpp"

#include "Logger.hpp"

TEST_CASE("Logging Successful Get", "[Logger]")
{
    ziapi::config::Dict cfg{{"pipelineName", std::make_shared<ziapi::config::Node>("Successful Get")}};
    post_process_modules::Logger logger{};
    ziapi::http::Request req{
        .version{ziapi::http::Version::kV1_1},
        .target{"/user?id=6942"},
        .method{ziapi::http::method::kGet},
        .headers{
            {"Accept", "*/*"},
        },
        .body{"toto"},
    };
    ziapi::http::Response res{
        .version{ziapi::http::Version::kV1_1},
        .status_code{ziapi::http::Code::kOK},
        .reason{ziapi::http::reason::kOK},
        .headers{
            {"Content-Type", "application/json"},
        },
        .body{"toto"},
    };
    ziapi::http::Context ctx{
        {
            "received_at",
            std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now())},
        {"REMOTE_ADDR", std::make_any<std::uint32_t>(0xC0A8002A)},
    };

    logger.Init(cfg);
    logger.PostProcess(ctx, req, res);
    SUCCEED();
}

TEST_CASE("Logging Redirection Post", "[Logger]")
{
    ziapi::config::Dict cfg{{"pipelineName", std::make_shared<ziapi::config::Node>("Redirection Post")}};
    post_process_modules::Logger logger{};
    ziapi::http::Request req{
            .version{ziapi::http::Version::kV1_1},
            .target{"/signup"},
            .method{ziapi::http::method::kPost},
            .headers{
                    {"Accept", "*/*"},
            },
            .body{"toto"},
    };
    ziapi::http::Response res{
            .version{ziapi::http::Version::kV1_1},
            .status_code{ziapi::http::Code::kMultipleChoices},
            .reason{ziapi::http::reason::kMultipleChoices},
            .headers{
                    {"Content-Type", "application/json"},
            },
            .body{"toto"},
    };
    ziapi::http::Context ctx{
            {
                "received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now())},
            {"REMOTE_ADDR", std::make_any<std::uint32_t>(0xAC8E00A9)},
    };
    std::this_thread::sleep_for(std::chrono::nanoseconds(569));

    logger.Init(cfg);
    logger.PostProcess(ctx, req, res);
    SUCCEED();
}

TEST_CASE("Logging User Error Put", "[Logger]")
{
    ziapi::config::Dict cfg{{"pipelineName", std::make_shared<ziapi::config::Node>("User Error Put")}};
    post_process_modules::Logger logger{};
    ziapi::http::Request req{
            .version{ziapi::http::Version::kV1_1},
            .target{"/profile?id=not-an-id"},
            .method{ziapi::http::method::kPut},
            .headers{
                    {"Accept", "*/*"},
            },
            .body{"toto"},
    };
    ziapi::http::Response res{
            .version{ziapi::http::Version::kV1_1},
            .status_code{ziapi::http::Code::kBadRequest},
            .reason{ziapi::http::reason::kBadRequest},
            .headers{
                    {"Content-Type", "application/json"},
            },
            .body{"toto"},
    };
    ziapi::http::Context ctx{
            {
                "received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now())},
            {"REMOTE_ADDR", std::make_any<std::uint32_t>(0xFFFFFFFF)},
    };
    std::this_thread::sleep_for(std::chrono::microseconds(342));

    logger.Init(cfg);
    logger.PostProcess(ctx, req, res);
    SUCCEED();
}

TEST_CASE("Logging Server Error Delete", "[Logger]")
{
    ziapi::config::Dict cfg{{"pipelineName", std::make_shared<ziapi::config::Node>("Server Error Delete")}};
    post_process_modules::Logger logger{};
    ziapi::http::Request req{
            .version{ziapi::http::Version::kV1_1},
            .target{"/account/delete?id=6942"},
            .method{ziapi::http::method::kDelete},
            .headers{
                    {"Accept", "*/*"},
            },
            .body{"toto"},
    };
    ziapi::http::Response res{
            .version{ziapi::http::Version::kV1_1},
            .status_code{ziapi::http::Code::kInternalServerError},
            .reason{ziapi::http::reason::kInternalServerError},
            .headers{
                    {"Content-Type", "application/json"},
            },
            .body{"toto"},
    };
    ziapi::http::Context ctx{
            {
                "received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now())},
            {"REMOTE_ADDR", std::make_any<std::uint32_t>(0x2A2A2A2A)},
    };
    std::this_thread::sleep_for(std::chrono::milliseconds(69));

    logger.Init(cfg);
    logger.PostProcess(ctx, req, res);
    SUCCEED();
}

TEST_CASE("Logging Continue Patch", "[Logger]")
{
    ziapi::config::Dict cfg{{"pipelineName", std::make_shared<ziapi::config::Node>("Continue Patch")}};
    post_process_modules::Logger logger{};
    ziapi::http::Request req{
            .version{ziapi::http::Version::kV1_1},
            .target{"/account?username=toto"},
            .method{ziapi::http::method::kPatch},
            .headers{
                    {"Accept", "*/*"},
            },
            .body{"toto"},
    };
    ziapi::http::Response res{
            .version{ziapi::http::Version::kV1_1},
            .status_code{ziapi::http::Code::kContinue},
            .reason{ziapi::http::reason::kContinue},
            .headers{
                    {"Content-Type", "application/json"},
            },
            .body{"toto"},
    };
    ziapi::http::Context ctx{
            {
                "received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now())},
            {"REMOTE_ADDR", std::make_any<std::uint32_t>(0x45454545)},
    };
    std::this_thread::sleep_for(std::chrono::milliseconds(342));

    logger.Init(cfg);
    logger.PostProcess(ctx, req, res);
    SUCCEED();
}

TEST_CASE("Logging No Content Options", "[Logger]")
{
    ziapi::config::Dict cfg{{"pipelineName", std::make_shared<ziapi::config::Node>("No Content Options")}};
    post_process_modules::Logger logger{};
    ziapi::http::Request req{
            .version{ziapi::http::Version::kV1_1},
            .target{"/health"},
            .method{ziapi::http::method::kOptions},
            .headers{
                    {"Accept", "*/*"},
            },
            .body{"toto"},
    };
    ziapi::http::Response res{
            .version{ziapi::http::Version::kV1_1},
            .status_code{ziapi::http::Code::kNoContent},
            .reason{ziapi::http::reason::kNoContent},
            .headers{
                    {"Content-Type", "application/json"},
            },
            .body{"toto"},
    };
    ziapi::http::Context ctx{
            {
                "received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now())},
            {"REMOTE_ADDR", std::make_any<std::uint32_t>(0x7F7F7F7F)},
    };
    std::this_thread::sleep_for(std::chrono::milliseconds(1426));

    logger.Init(cfg);
    logger.PostProcess(ctx, req, res);
    SUCCEED();
}

TEST_CASE("Logging Not Found Head", "[Logger]")
{
    ziapi::config::Dict cfg{{"pipelineName", std::make_shared<ziapi::config::Node>("Not Found Head")}};
    post_process_modules::Logger logger{};
    ziapi::http::Request req{
            .version{ziapi::http::Version::kV1_1},
            .target{"/toto"},
            .method{ziapi::http::method::kHead},
            .headers{
                    {"Accept", "*/*"},
            },
            .body{"toto"},
    };
    ziapi::http::Response res{
            .version{ziapi::http::Version::kV1_1},
            .status_code{ziapi::http::Code::kNotFound},
            .reason{ziapi::http::reason::kNotFound},
            .headers{
                    {"Content-Type", "application/json"},
            },
            .body{"toto"},
    };
    ziapi::http::Context ctx{
            {
                "received_at",
                std::make_any<std::chrono::time_point<std::chrono::system_clock>>(std::chrono::system_clock::now())},
            {"REMOTE_ADDR", std::make_any<std::uint32_t>(0xACF248B5)},
    };

    logger.Init(cfg);
    logger.PostProcess(ctx, req, res);
    SUCCEED();
}