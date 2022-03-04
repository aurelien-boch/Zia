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
    /**
     * @class Executor
     * @brief Executes a binary and allows you to do several actions with it.
     */
    class DllExport Executor
    {
        public:
#if _WIN32
            using pid = HANDLE;
#else
            using pid = pid_t;
#endif

            /**
             * @param[in] binaryPath The path to the binary.
             * @param[in] args The args to provide to the binary.
             * @param[in] environmentManager The env of the binary.
             */
            Executor(
                std::string binaryPath,
                std::vector <std::string> args,
                env::Manager const &environmentManager
            );

            /**
             * @param[in] binaryPath The path to the binary.
             * @param[in] environmentManager The env of the binary.
             */
            Executor(
                std::string binaryPath,
                env::Manager const &environmentManager
            );

            /**
             * @param[in] binaryPath The path to the binary.
             * @param[in] args The args to provide to the binary.
             */
            Executor(
                std::string binaryPath,
                std::vector <std::string> args
            );

            /**
             * @param[in] binaryPath The path to the binary.
             */
            explicit Executor(std::string const &binaryPath);

            ~Executor();

            /**
             * @brief Waits until the process is stopped.
             */
            void wait() const;

            /**
             * @brief Kills the process.
             * @param[in] exitCode The exit code to end the process with (only works on windows
             * sigkill will always be send on linux)
             */
            void kill(std::uint32_t exitCode) const;

            /**
             * @brief Gets the pid of the process.
             * @return The pid of the process.
             */
            [[nodiscard]] pid getPid() const;

            /**
             * Returns a boolean that refers to the state of the process (running or not).
             */
            [[nodiscard]] bool isRunning() const;

            /**
             * @brief Writes a string to the process stdin
             * @param[in] str The string to write to the child stdin.
             * @return The number of bytes written.
             */
            std::uint32_t operator<<(std::string const &str) const;

            /**
             * @brief Reads a string from the process stdout
             * @param[out] str a reference to a string that will be populated with content of process stdout.
             */
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
