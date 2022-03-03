#ifndef CORE_HPP
#define CORE_HPP

#include <ConfigParser.hpp>
#include <Loader.hpp>
#include <RequestOutputQueue.hpp>
#include <ResponseInputQueue.hpp>
#include <memory>
#include <string>
#include <ziapi/Config.hpp>
#include <ziapi/Module.hpp>

namespace core
{
    /**
     * @class Core
     * @brief This class manages a pipeline
     */
    class Core
    {
        public:
            /**
             * @param[in] filepath Refers to the config of the pipeline
             */
            Core(std::string &name, std::string &filepath) noexcept;

            /**
             * @brief Starts the pipeline
             */
            void run();

            /**
             * @brief Stops the pipeline
             */
            void stop();

            /**
             * @brief Reloads the pipeline
             */
            void config() noexcept;

            /**
             * @brief Activates or deactivates the hot reloading
             */
            bool hotReload() noexcept;

            /**
             * @brief Get the state of the pipeline
             */
            bool isRunning() const noexcept;

        private:
            bool _running;
            bool _configLoaded;
            std::string _filepath;
            bool _hotReload;

            parser::ConfigParser _parser;
            std::shared_ptr<ziapi::INetworkModule> _networkModule;
            std::vector<std::shared_ptr<ziapi::IPreProcessorModule>> _preProcessors;
            std::vector<std::shared_ptr<ziapi::IHandlerModule>> _handlers;
            std::vector<std::shared_ptr<ziapi::IPostProcessorModule>> _postProcessors;
            std::vector<loader::Loader> _libs;

            void _serveRequest(modules::RequestOutputQueue &requests, modules::ResponseInputQueue &responses);

            void _purgeData();

            void _loadModule(ziapi::config::Node const &cfg, std::string const &path, std::string const &name);
    };
} // namespace core

#endif // CORE_HPP
