#include "CommandLine.hpp"
#include "ConfigParser.hpp"
#include "Core.hpp"

int main()
{
    std::vector<parser::LibInfo> pipelines = parser::ConfigParser::getConfigsPaths("../../pipelines.txt");
    std::unordered_map<std::string, core::Core> pipelinesMap;

    for (auto & pipeline : pipelines) {
        pipelinesMap.try_emplace(pipeline.name, pipeline.name, pipeline.configPath);
    }
    cli::CommandLine cli(pipelinesMap);

    cli.run();
    return 0;
}
