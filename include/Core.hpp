#ifndef CORE_HPP
#define CORE_HPP

#include <memory>
#include <string>

#include <ziapi/Config.hpp>
#include <ziapi/Module.hpp>

#include <Loader.hpp>
#include <ConfigParser.hpp>

#include <ziapi/Config.hpp>
#include <ziapi/Module.hpp>
#include <string>

namespace core
{
    class Core
    {
        public:
            /**
             * todo
             * @param filepath
             */
            explicit Core(std::string &filepath) noexcept;

            /**
             * todo
             */
            [[noreturn]] void run();

            /**
             * todo
             */
            void stop();

            /**
             * todo
             * @param cfg
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

            void _serveRequest(std::unique_ptr<modules::RequestOutputQueue> &requests,
                               std::unique_ptr<modules::ResponseInputQueue> &responses);

            void _purgeData();

            void _loadModule(const ziapi::config::Node &cfg, std::string &path);
    };
}

#endif //CORE_HPP
