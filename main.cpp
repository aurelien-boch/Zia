#include "CommandLine.hpp"
#include "ConfigParser.hpp"
#include "Core.hpp"

int main()
{
    std::vector<parser::LibInfo> pipelines = parser::ConfigParser::getConfigsPaths("../../pipelines.txt");
    std::unordered_map<std::string, core::Core> pipelinesMap;

    for (int i = 0; i != pipelines.size(); ++i) {
        pipelinesMap.try_emplace(pipelines[i].name, pipelines[i].name, pipelines[i].configPath);
    }
    cli::CommandLine cli(pipelinesMap);

    cli.run();
    return 0;
}
