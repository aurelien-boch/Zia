#ifndef CORE_HPP
#define CORE_HPP

#include <memory>
#include <string>

#include <ziapi/Config.hpp>
#include <ziapi/Module.hpp>

#include <Loader.hpp>
#include <ConfigParser.hpp>
#include <RequestOutputQueue.hpp>
#include <ResponseInputQueue.hpp>

namespace core
{
    /**
     * @class This class manages a pipeline
     */
    class Core
    {
        public:
            /**
             * @param [in]filepath this parameter refers to the config of the pipeline
             */
            explicit Core(std::string &&filepath) noexcept;

            /**
             * @brief Starts the pipeline
             */
            void run();

            /**
             * @brief Stops the pipeline
             */
            void stop();

            /**
             * @brief Reload the pipeline
             */
            void config() noexcept;

        private:
            bool _running;
            bool _configLoaded;
            std::string _filepath;

            parser::ConfigParser _parser;
            std::shared_ptr<ziapi::INetworkModule> _networkModule;
            std::vector<std::shared_ptr<ziapi::IPreProcessorModule>> _preProcessors;
            std::vector<std::shared_ptr<ziapi::IHandlerModule>> _handlers;
            std::vector<std::shared_ptr<ziapi::IPostProcessorModule>> _postProcessors;
            std::vector<loader::Loader> _libs;

            void _serveRequest(modules::RequestOutputQueue &requests,
                               modules::ResponseInputQueue &responses);

            void _purgeData();

            void _loadModule(ziapi::config::Node const &cfg, std::string const &path, std::string const &name);
    };
}

#endif //CORE_HPP
