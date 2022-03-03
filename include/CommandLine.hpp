#ifndef COMMANDLINE_HPP_
#define COMMANDLINE_HPP_

#include "Core.hpp"

#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

namespace cli
{
    static const std::unordered_map<std::string, std::string> _helpCommands = {
        {"help", "Display this help"},
        {"start", "Start the pipeline"},
        {"stop", "Stop the pipeline"},
        {"config", "Reload the pipeline configuration"},
        {"hotreload", "Activate or deactivate hot reloading"}};

    class CommandLine
    {
        public:
            CommandLine(std::unordered_map<std::string, core::Core> &pipelines);

            ~CommandLine() = default;

            void run();

        private:
            void _checkCommand();

            void _help();

            void _startPipeline();

            void _stopPipeline();

            void _configPipeline();

            void _hotReloadPipeline();

            void _exit();

            static const std::unordered_map<std::string, std::function<void(CommandLine &)>> _commands;

            std::string _line;
            std::string _command;
            std::string _pipelineName;
            std::unordered_map<std::string, core::Core> &_pipelines;
            std::unordered_map<std::string, std::jthread> _pipelinesThreads;
    };

    inline const std::unordered_map<std::string, std::function<void(CommandLine &)>> CommandLine::_commands{
        {"help", [](CommandLine &cli) { cli._help(); }},
        {"config", [](CommandLine &cli) { cli._configPipeline(); }},
        {"hotreload", [](CommandLine &cli) { cli._hotReloadPipeline(); }},
        {"stop", [](CommandLine &cli) { cli._stopPipeline(); }},
        {"start", [](CommandLine &cli) { cli._startPipeline(); }},
        {"exit", [](CommandLine &cli) { cli._exit(); }}};

} // namespace cli

#endif /* !COMMANDLINE_HPP_ */
