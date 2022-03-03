#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <vector>

#if _WIN32
    #include <Windows.h>
    #define DllExport   __declspec( dllexport )
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #define DllExport
#endif

#include <EnvManager.hpp>

namespace execution
{
    class DllExport Executor
    {
        public:
#if _WIN32
            using pid = HANDLE;
#else
            using pid = pid_t;
#endif

            Executor(
                std::string binaryPath,
                std::vector <std::string> args,
                env::Manager const &environmentManager
            );

            Executor(
                std::string binaryPath,
                env::Manager const &environmentManager
            );

            Executor(
                std::string binaryPath,
                std::vector <std::string> args
            );

            explicit Executor(std::string const &binaryPath);

            ~Executor();

            void wait() const;

            void kill(std::uint32_t exitCode) const;

            [[nodiscard]] pid getPid() const;

            [[nodiscard]] bool isRunning() const;

            std::uint32_t operator<<(std::string const &str) const;

            void operator>>(std::string &str) const;

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
#else
            pid _processPid;
#endif
            void _invokeProcess(
                pipeDescriptor stdinDescriptor,
                pipeDescriptor stdoutDescriptor,
                const env::Manager &env,
                std::string commandLine,
                std::vector<std::string> args);

            static std::pair<Executor::pipeDescriptor, Executor::pipeDescriptor> _createPipe();
    };
}

#endif //EXECUTOR_HPP
