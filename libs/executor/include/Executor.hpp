#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <vector>

#if _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#include <EnvManager.hpp>

namespace execution
{
    class Executor
    {
        public:
#if _WIN32
            using pid = HANDLE;
#else
            using pid = pid_t;
#endif

            Executor(
                std::string const &binaryPath,
                std::vector <std::string> const &args,
                env::Manager const &environmentManager
            );

            Executor(
                std::string const &binaryPath,
                env::Manager const &environmentManager
            );

            Executor(
                std::string const &binaryPath,
                std::vector <std::string> const &args
            );

            explicit Executor(std::string const &binaryPath);

            ~Executor();

            void wait() const;

            void kill(std::uint32_t exitCode) const;

            [[nodiscard]] pid getPid() const;

            [[nodiscard]] bool isRunning() const;

            std::uint32_t operator<<(std::string const &str);

            void operator>>(std::string &str);

        private:
#if _WIN32
            using pipeDescriptor = HANDLE;
#else
            using pipeDescriptor = int;
#endif

            pipeDescriptor _childStdin;
            pipeDescriptor _childStdout;
            pipeDescriptor _childStdinClientDescriptor;
            pipeDescriptor _childStdoutClientDescriptor;

#if _WIN32
            PROCESS_INFORMATION _processInfo;

            static void _throwWindowsError();

            void _invokeProcess(
                pipeDescriptor stdinDescriptor,
                pipeDescriptor stdoutDescriptor,
                const env::Manager &env,
                std::string &commandLine,
                std::vector<std::string> const &args);
#else
            pid _processPid;
#endif
            static std::pair<Executor::pipeDescriptor, Executor::pipeDescriptor> _createPipe();
    };
}

#endif //EXECUTOR_HPP
