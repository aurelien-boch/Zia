#ifndef CORE_HPP
#define CORE_HPP

#include <memory>
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
            explicit Core(std::string &&filepath) noexcept;

            /**
             * todo
             */
            [[noreturn]] void run();

            /**
             * todo
             */
            void stop();

        private:

            void _config() noexcept;

            bool _running;
            bool _configLoaded;

            std::shared_ptr<ziapi::INetworkModule> _networkModule;
            std::vector<std::shared_ptr<ziapi::IHandlerModule>> _modules;
            std::vector<std::shared_ptr<ziapi::IPreProcessorModule>> _preProcessors;
            std::vector<std::shared_ptr<ziapi::IPostProcessorModule>> _postProcessors;
    };
}

#endif //CORE_HPP
