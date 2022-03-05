#include <thread>

#include <HttpsModule.hpp>
#include <RequestOutputQueue.hpp>
#include <ResponseInputQueue.hpp>

#include <ConfigParser.hpp>

int main()
{
    parser::ConfigParser parser("path/to/config/file.yml");
    modules::HttpModules https;
    modules::ResponseInputQueue responses{};
    modules::RequestOutputQueue requests{};
    std::thread httpsThread;

    https.Init(http.Init(parser.getConfigMap()));
    httpsThread = std::thread{https.Run(requests, responses)};
    while (/* condition */) {
        /* ... */
    }
    https.Terminate();
    if (httpsThread.joinable())
        httpsThread.join();
    return 0;
}
