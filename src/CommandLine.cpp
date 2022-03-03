#include "CommandLine.hpp"

namespace cli
{
    CommandLine::CommandLine(std::unordered_map<std::string, core::Core> &pipelines) : _pipelines(pipelines) {}

    void CommandLine::run()
    {
        while (std::getline(std::cin, _line))
            _checkCommand();
    }

    void CommandLine::_checkCommand()
    {
        std::size_t pos = _line.find(" ");

        _command = _line.substr(0, pos);
        if (_commands.find(_command) != _commands.end()) {
            _pipelineName = _line.substr(pos + 1, _line.find('\n'));
            _commands.at(_command)(*this);
        } else
            std::cerr << "Command not found, try typing help for available commands" << std::endl;
    }

    void CommandLine::_help()
    {
        std::cout << "Available commands: " << std::endl;
        for (auto const &command : _helpCommands)
            std::cout << command.first << std::setw(6) << "\t" << command.second << std::endl;
    }

    void CommandLine::_startPipeline()
    {
        auto it = _pipelines.find(_pipelineName);

        if (it != _pipelines.end())
            if (it->second.isRunning())
                std::cerr << "Error: the pipeline is already running" << std::endl;
            else
                _pipelinesThreads.try_emplace(_pipelineName, [it]() {
                    try {
                        it->second.run();
                    } catch (std::runtime_error) {
                        std::cerr << "Error: the pipeline has crashed" << std::endl;
                    }
                });
        else
            std::cerr << "Error: the pipeline " << _pipelineName << " doesn't exist" << std::endl;
    }

    void CommandLine::_stopPipeline()
    {
        auto it = _pipelines.find(_pipelineName);

        if (it != _pipelines.end()) {
            try {
                it->second.stop();
                _pipelinesThreads.erase(_pipelineName);
            } catch (std::runtime_error) {
                std::cerr << "Error: the pipeline has crashed" << std::endl;
            }
        } else
            std::cerr << "Error: the pipeline " << _pipelineName << " doesn't exist" << std::endl;
    }

    void CommandLine::_configPipeline()
    {
        auto it = _pipelines.find(_pipelineName);

        if (it != _pipelines.end())
            it->second.config();
        else
            std::cerr << "Error: the pipeline " << _pipelineName << " doesn't exist" << std::endl;
    }

    void CommandLine::_hotReloadPipeline()
    {
        auto it = _pipelines.find(_pipelineName);

        if (it != _pipelines.end())
            if (it->second.hotReload())
                std::cout << "Hot reload activated" << std::endl;
            else
                std::cout << "Hot reload deactivated" << std::endl;
        else
            std::cerr << "Pipeline " << _pipelineName << " not found" << std::endl;
    }

    void CommandLine::_exit()
    {
        for (auto &[name, pipeline] : _pipelines)
            if (pipeline.isRunning())
                pipeline.stop();
        std::exit(0);
    }
} // namespace cli
