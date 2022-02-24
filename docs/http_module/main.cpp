#include <thread>

#include <HttpModule.hpp>
#include <RequestOutputQueue.hpp>
#include <ResponseInputQueue.hpp>

#include <ConfigParser.hpp>

int main()
{
    parser::ConfigParser parser("path/to/config/file.yml");
    modules::HttpModule http;
    modules::ResponseInputQueue responses{};
    modules::RequestOutputQueue requests{};
    std::thread httpThread;

    http.Init(http.Init(parser.getConfigMap()));
    httpThread = std::thread{http.Run(requests, responses)};
    while (/* condition */) {
        /* ... */
    }
    http.Terminate();
    if (httpThread.joinable())
        httpThread.join();
    return 0;
}
