#include <catch2/catch_test_macros.hpp>

#include <RequestOutputQueue.hpp>

TEST_CASE("Push and Pop", "[RequestOutputQueue]")
{
    modules::RequestOutputQueue queue{};
    ziapi::http::Request request{
        ziapi::http::Version::kV1,
        "target",
        "method",
        {
            {"header1", "h1"},
            {"header2", "h2"}
        },
        "body",
    };
    queue.Push({request,{}});

    auto r = queue.Pop().value().first;
    REQUIRE(r.version == ziapi::http::Version::kV1);
    REQUIRE(r.target == "target");
    REQUIRE(r.method == "method");
    REQUIRE(r.body == "body");
}

TEST_CASE("Empty", "[RequestOutputQueue]")
{
    modules::RequestOutputQueue queue{};

    ziapi::http::Request request{
            ziapi::http::Version::kV1,
            "target",
            "method",
            {
                    {"header1", "h1"},
                    {"header2", "h2"}
            },
            "body",
    };
    REQUIRE(queue.Empty() == true);
    queue.Push({request,{}});
    REQUIRE(queue.Empty() == false);
}

TEST_CASE("Size", "[RequestOutputQueue]")
{
    modules::RequestOutputQueue queue{};

    ziapi::http::Request request{
            ziapi::http::Version::kV1,
            "target",
            "method",
            {
                {"header1", "h1"},
                {"header2", "h2"}
            },
            "body",
    };
    REQUIRE(queue.Size() == 0);
    queue.Push({request,{}});
    REQUIRE(queue.Size() == 1);
    queue.Push({request,{}});
    REQUIRE(queue.Size() == 2);
}