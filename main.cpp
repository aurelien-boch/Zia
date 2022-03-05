#include "CommandLine.hpp"
#include "ConfigParser.hpp"
#include "Core.hpp"

int main()
{
    std::vector<parser::LibInfo> pipelines = parser::ConfigParser::getConfigsPaths("../../pipelines.txt");
    std::unordered_map<std::string, core::Core> pipelinesMap;

    for (auto const &[pipelineName, pipelinePath, _] : pipelines)
        pipelinesMap.try_emplace(pipelineName, pipelineName, pipelinePath);
    cli::CommandLine cli(pipelinesMap);

    cli.run();
    return 0;
}
