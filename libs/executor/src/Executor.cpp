#include <stdexcept>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include "Executor.hpp"

namespace execution
{
    Executor::Executor(
        std::string binaryPath,
        std::vector<std::string> args,
        env::Manager const &environmentManager) :
#ifdef _WIN32
        _processInfo {}
#else
        _processPid {}
#endif
    {
        auto[stdinRead, stdinWrite] = _createPipe();
        auto[stdoutRead, stdoutWrite] = _createPipe();

        _invokeProcess(
            stdinRead,
            stdoutWrite,
            environmentManager,
            std::move(binaryPath),
            std::move(args)
        );
        _childStdin = stdinWrite;
        _childStdout = stdoutRead;
        _childStdinClientDescriptor = stdinRead;
        _childStdoutClientDescriptor = stdoutWrite;
    }

    Executor::Executor(
        std::string binaryPath,
        const env::Manager &environmentManager) :
#ifdef _WIN32
        _processInfo {}
#else
        _processPid {}
#endif
    {
        auto[stdinRead, stdinWrite] = _createPipe();
        auto[stdoutRead, stdoutWrite] = _createPipe();
        std::vector<std::string> args;

        _invokeProcess(
            stdinRead,
            stdoutWrite,
            environmentManager,
            std::move(binaryPath),
            std::move(args)
        );
        _childStdin = stdinWrite;
        _childStdout = stdoutRead;
        _childStdinClientDescriptor = stdinRead;
        _childStdoutClientDescriptor = stdoutWrite;
    }

    Executor::Executor(
        std::string binaryPath,
        std::vector<std::string> args) :
#ifdef _WIN32
        _processInfo {}
#else
        _processPid {}
#endif
    {
        auto[stdinRead, stdinWrite] = _createPipe();
        auto[stdoutRead, stdoutWrite] = _createPipe();
        env::Manager environmentManager;

        _invokeProcess(
            stdinRead,
            stdoutWrite,
            environmentManager,
            std::move(binaryPath),
            std::move(args)
        );
        _childStdin = stdinWrite;
        _childStdout = stdoutRead;
        _childStdinClientDescriptor = stdinRead;
        _childStdoutClientDescriptor = stdoutWrite;
    }

    Executor::Executor(const std::string &binaryPath) :
#ifdef _WIN32
    _processInfo {}
#else
    _processPid {}
    _processPid {}
#endif
    {
        auto [stdinRead, stdinWrite] = _createPipe();
        auto [stdoutRead, stdoutWrite] = _createPipe();
        std::vector<std::string> args;
        env::Manager environmentManager;

        _invokeProcess(
            stdinRead,
            stdoutWrite,
            environmentManager,
            const_cast<std::string &>(binaryPath),
            args
        );
        _childStdin = stdinWrite;
        _childStdout = stdoutRead;
        _childStdinClientDescriptor = stdinRead;
        _childStdoutClientDescriptor = stdoutWrite;
    }

#if _WIN32
    void Executor::_throwWindowsError()
    {
        DWORD code = GetLastError();
        LPSTR message_buffer = nullptr;
        std::size_t message_size = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&message_buffer),
            0,
            nullptr
        );

        throw std::runtime_error({message_buffer, message_size});
    }
#endif

    void Executor::_invokeProcess(
        Executor::pipeDescriptor stdinDescriptor,
        Executor::pipeDescriptor stdoutDescriptor,
        const env::Manager &env,
        std::string commandLine,
        std::vector<std::string> args)
    {
#if _WIN32
        std::string convertedArgs {commandLine + " "};
        std::string convertedEnv = env;
        std::string path{std::filesystem::current_path().string()};
        BOOL res;
        STARTUPINFOA startupInfo {
            .cb = sizeof(STARTUPINFOA),
            .dwFlags = STARTF_USESTDHANDLES,
            .hStdInput = stdinDescriptor,
            .hStdOutput = stdoutDescriptor,
        };

        for (auto const &e: args)
            convertedArgs += " \"" + e + "\"";
        res = CreateProcessA(
            nullptr,
            convertedArgs.data(),
            nullptr,
            nullptr,
            true,
            0,
            convertedEnv.data(),
            path.c_str(),
            &startupInfo,
            &_processInfo
        );
        if (res == 0)
            _throwWindowsError();
#else
        if (access(commandLine.c_str(), X_OK) != 0)
            throw std::runtime_error{"Cannot access binary: " + commandLine};
        pid_t pid = fork();
        std::vector<char *> convertedArgs;

        convertedArgs.emplace_back(commandLine.data());
        for (auto &e : args)
            convertedArgs.emplace_back(e.data());
        convertedArgs.emplace_back(nullptr);
        if (pid == 0) {
            dup2(stdinDescriptor, STDIN_FILENO);
            dup2(stdoutDescriptor, STDOUT_FILENO);
            execve(commandLine.c_str(), convertedArgs.data(), _env);
        } else
            _processPid = pid;
#endif
    }

    std::pair<Executor::pipeDescriptor, Executor::pipeDescriptor> Executor::_createPipe()
    {
        pipeDescriptor handles[2];

#ifdef _WIN32
        SECURITY_ATTRIBUTES securityAttributes {
            .nLength = sizeof(SECURITY_ATTRIBUTES),
            .lpSecurityDescriptor = nullptr,
            .bInheritHandle = true
        };

        if (CreatePipe(&(handles[0]), &(handles[1]), &securityAttributes, 0) == 0)
            _throwWindowsError();
#else
        if (pipe(handles) == -1)
            throw std::runtime_error(strerror(errno));
#endif
        return {handles[0], handles[1]};
    }

    Executor::~Executor()
    {
#if _WIN32
        CloseHandle(_childStdin);
        CloseHandle(_childStdout);
        CloseHandle(_childStdoutClientDescriptor);
        CloseHandle(_childStdinClientDescriptor);
        CloseHandle(_processInfo.hProcess);
        CloseHandle(_processInfo.hThread);
#else
        pid_t res = waitpid(_processPid, nullptr, {});

        close(_childStdin);
        close(_childStdout);
        close(_childStdinClientDescriptor);
        close(_childStdoutClientDescriptor);
#endif
    }

    void Executor::wait() const
    {
        if (!this->isRunning())
            throw std::runtime_error{"Error, process is not running"};
#if _WIN32
        if (WaitForSingleObject(_processInfo.hProcess, INFINITE) == WAIT_FAILED)
            _throwWindowsError();
#else
        waitpid(_processPid, nullptr, {});
#endif
    }

    void Executor::kill(std::uint32_t exitCode) const
    {
#if _WIN32
        if (!this->isRunning())
            throw std::runtime_error("Error, process is not running");
        if (TerminateProcess(_processInfo.hProcess, exitCode) == 0)
            _throwWindowsError();
#else
        ::kill(_processPid, SIGKILL);
#endif
    }

    Executor::pid Executor::getPid() const
    {
#if _WIN32
        return (_processInfo.hProcess);
#else
        return _processPid;
#endif
    }

    std::uint32_t Executor::operator<<(const std::string &str) const
    {
#if _WIN32
        DWORD numBytesWritten;

        if (WriteFile(
            _childStdin,
            str.data(),
            str.size(),
            &numBytesWritten,
            nullptr
        ) == 0)
            _throwWindowsError();
        return numBytesWritten;
#else
        std::int64_t res = write(
            _childStdin,
            str.c_str(),
            str.size()
        );

        if (res == -1)
            throw std::runtime_error{strerror(errno)};
        return res;
#endif
    }

    void Executor::operator>>(std::string &str) const
    {
        static constexpr std::uint32_t numBytes = 1024;
        std::string buff;

        buff.resize(numBytes + 1);
#if _WIN32
        DWORD numBytesRead;

        while (true) {
            if (ReadFile(
                _childStdout,
                buff.data(),
                numBytes,
                &numBytesRead,
                nullptr
            ) == 0)
                _throwWindowsError();
            if (numBytes != numBytesRead) {
                buff.resize(numBytesRead);
                str += buff;
                return;
            } else
                str += buff;
        }
#else
        std::int64_t numBytesRead;

        while (true) {
            numBytesRead = read(_childStdout, buff.data(), numBytes);
            if (numBytesRead == -1)
                throw std::runtime_error{strerror(errno)};
            if (numBytes != numBytesRead) {
                buff.resize(numBytesRead);
                str += buff;
                return;
            } else
                str += buff;
        }
#endif
    }

    bool Executor::isRunning() const
    {
#if _WIN32
        DWORD exitCode;

        if (GetExitCodeProcess(_processInfo.hProcess, &exitCode) == 0)
            _throwWindowsError();
        return exitCode == STILL_ACTIVE;
#else
        pid_t res = waitpid(_processPid, nullptr, WNOHANG);

        if (res == -1)
            throw std::runtime_error{strerror(errno)};
        return (res == 0);
#endif
    }
}
