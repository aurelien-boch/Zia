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

//#include <iostream>
//#include "ConfigParser.hpp"
//
////#include "Core.hpp"
//
//int main()
//{
////    std::string zizi{"../../config.yml"};
////    core::Core zazou{zizi};
////
////    zazou.config();
////    zazou.run();
//    return 0;
//}

//#include <thread>
//
//#include <HttpsModule.hpp>
//#include <RequestOutputQueue.hpp>
//#include <ResponseInputQueue.hpp>
//
//#include <ConfigParser.hpp>
//#include <HttpFormatter.hpp>
//#include <HttpParser.hpp>
//
//int main()
//{
//    parser::ConfigParser conf("../docs/https_module/validConfig.yml");
//    modules::HttpsModule https;
//    modules::ResponseInputQueue responses{};
//    modules::RequestOutputQueue requests{};
//    std::string requestString{"GET toto HTTP/1.1\r\n"
//                              "User-Agent: PostmanRuntime/7.29.0\r\n"
//                              "Accept: */*\r\n"
//                              "Cache-Control: no-cache\r\n"
//                              "Postman-Token: ffa0b8c8-2d43-4805-9af2-7cfac22dc7f3\r\n"
//                              "Host: localhost:8080\r\n"
//                              "Accept-Encoding: gzip, deflate, br\r\n"
//                              "Connection: keep-alive\r\n"
//                              "Content-Length: 12\r\n"
//                              "Cookie: token=github|36404435\r\n\r\n"
//                              "World Hello!"
//    };
//    parser::HttpParser parser;
//    formatter::HttpFormatter formatter;
//
//    https.Init(conf.getConfigMap());
//    std::thread httpsThread([&https, requests, responses] () mutable { https.Run(requests, responses);});
//    while (true) {
//        // responses.Push(formatter.format(requestString));
//        while (requests.Empty()) {
////            auto opt = requests.Pop(); //.value().first;
////
////            std::cout << "RECEIVED: " << res.body << std::endl;
//        }
//    }
//    https.Terminate();
//    if (httpsThread.joinable())
//        httpsThread.join();
//    return 0;
//}

