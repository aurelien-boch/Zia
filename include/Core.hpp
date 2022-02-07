#ifndef CORE_HPP
#define CORE_HPP

#include <memory>
#include <ziapi/Config.hpp>
#include <ziapi/Module.hpp>

namespace core
{
    class Core
    {
        public:
            /**
             * todo
             * @param filepath
             */
            Core(std::string &&filepath) noexcept;

            /**
             * todo
             */
            void run();

            /**
             * todo
             */
            void stop();

        private:

            static void _config() noexcept;

            std::shared_ptr<ziapi::INetworkModule> network;
            std::vector<std::shared_ptr<ziapi::IHandlerModule>> _modules;
            std::vector<std::shared_ptr<ziapi::IPreProcessorModule>> _preProcessors;
            std::vector<std::shared_ptr<ziapi::IPostProcessorModule>> _postProcessors;
    };
}

#endif //CORE_HPP
