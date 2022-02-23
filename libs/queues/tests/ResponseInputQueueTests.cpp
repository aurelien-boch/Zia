#include <catch2/catch_test_macros.hpp>

#include <catch2/catch_test_macros.hpp>

#include <ResponseInputQueue.hpp>

TEST_CASE("Push and Pop", "[ResponseInputQueue]")
{
    modules::ResponseInputQueue queue{};
    ziapi::http::Response response{
        ziapi::http::Version::kV1,
        ziapi::http::Code::kContinue,
        "reason",
        {
            {"header1", "h1"},
            {"header2", "h2"}
        },
        "body",
    };
    queue.Push({response,{}});

    auto r = queue.Pop().value().first;
    REQUIRE(r.version == ziapi::http::Version::kV1);
    REQUIRE(r.status_code == ziapi::http::Code::kContinue);
    REQUIRE(r.reason == "reason");
    REQUIRE(r.body == "body");
}

TEST_CASE("Empty", "[ResponseInputQueue]")
{
    modules::ResponseInputQueue queue{};
    ziapi::http::Response response{
            ziapi::http::Version::kV1,
            ziapi::http::Code::kContinue,
            "reason",
            {
                    {"header1", "h1"},
                    {"header2", "h2"}
            },
            "body",
    };
    REQUIRE(queue.Empty() == true);
    queue.Push({response,{}});
    REQUIRE(queue.Empty() == false);
}

TEST_CASE("Size", "[ResponseInputQueue]")
{
    modules::ResponseInputQueue queue{};
    ziapi::http::Response response{
            ziapi::http::Version::kV1,
            ziapi::http::Code::kContinue,
            "reason",
            {
                    {"header1", "h1"},
                    {"header2", "h2"}
            },
            "body",
    };
    REQUIRE(queue.Size() == 0);
    queue.Push({response,{}});
    REQUIRE(queue.Size() == 1);
    queue.Push({response,{}});
    REQUIRE(queue.Size() == 2);
}