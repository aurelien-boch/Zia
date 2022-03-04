#include "CommandLine.hpp"
#include "ConfigParser.hpp"
#include "Core.hpp"


int main()
{
    std::vector<parser::LibInfo> pipelines = parser::ConfigParser::getConfigsPaths("../../pipelines.txt");
    std::unordered_map<std::string, core::Core> pipelinesMap;
    pipelinesMap.try_emplace(pipelines[0].name, pipelines[0].name, pipelines[0].configPath);
    cli::CommandLine cli(pipelinesMap);

    cli.run();
    return 0;
}
//     std::cout << "Hello World!" << std::endl;
//      parser::ConfigParser conf("../docs/https_module/validConfig.yml");
//      modules::HttpsModule https;
//      modules::ResponseInputQueue responses{};
//      modules::RequestOutputQueue requests{};
//      static const ziapi::http::Response responseStruct{
//              .version{ziapi::http::Version::kV1_1},
//              .status_code{ziapi::http::Code::kOK},
//              .reason{ziapi::http::reason::kOK},
//              .headers{
//                      {"Content-Length", "6"},
//                      {"Content-Type", "text/html"},
//              },
//              .body{"bite !"
//              },
//      };
//      ziapi::http::Context context{};

//      parser::HttpParser parser;
//      formatter::HttpFormatter formatter;

//      https.Init(conf.getConfigMap());
//      std::cout << "Https module config done." << std::endl;
//      std::thread httpsThread([&https, requests, responses] () mutable { https.Run(requests, responses);});
//      std::cout << "Https module running." << std::endl;
//      while (true) {
//          // responses.Push({responseStruct, context});
//          while (!requests.Empty()) {
//              auto res = requests.Pop().value().first;

//               std::cout << "RECEIVED: " << res.body << std::endl;
//          }
//      }
//      https.Terminate();
//      if (httpsThread.joinable())
//          httpsThread.join();
//      return 0;
// }
