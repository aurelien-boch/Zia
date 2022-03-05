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
