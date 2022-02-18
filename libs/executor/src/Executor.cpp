#include <stdexcept>
#include <filesystem>

#include "Executor.hpp"

namespace execution
{
    Executor::Executor(
        std::string const &binaryPath,
        std::vector<std::string> const &args,
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
            const_cast<std::string &>(binaryPath),
            args
        ); //TODO see if const cast is clean
        _childStdin = stdinWrite;
        _childStdout = stdoutRead;
        _childStdinClientDescriptor = stdinRead;
        _childStdoutClientDescriptor = stdoutWrite;
    }

    Executor::Executor(
        const std::string &binaryPath,
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
            const_cast<std::string &>(binaryPath),
            args
        ); //TODO see if const cast is clean
        _childStdin = stdinWrite;
        _childStdout = stdoutRead;
        _childStdinClientDescriptor = stdinRead;
        _childStdoutClientDescriptor = stdoutWrite;
    }

    Executor::Executor(
        const std::string &binaryPath,
        const std::vector<std::string> &args) :
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
            const_cast<std::string &>(binaryPath),
            args
        ); //TODO see if const cast is clean
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
        ); //TODO see if const cast is clean
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
        std::string &commandLine,
        std::vector<std::string> const &args)
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
        if (pipe(handles) == 0)
            throw std::runtime_error({strerror(errno)});
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
#endif
    }

    void Executor::wait() const
    {
#if _WIN32
        if (!this->isRunning())
            throw std::runtime_error("Error, process is not running");
        if (WaitForSingleObject(_processInfo.hProcess, INFINITE) == WAIT_FAILED)
            _throwWindowsError();
#endif
    }

    void Executor::kill(std::uint32_t exitCode) const
    {
#if _WIN32
        if (!this->isRunning())
            throw std::runtime_error("Error, process is not running");
        if (TerminateProcess(_processInfo.hProcess, exitCode) == 0)
            _throwWindowsError();
#endif
    }

    Executor::pid Executor::getPid() const
    {
        return (_processInfo.hProcess);
    }

    std::uint32_t Executor::operator<<(const std::string &str)
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
#endif

    }

    void Executor::operator>>(std::string &str)
    {
        static constexpr std::uint32_t numBytes = 1024;
#if _WIN32
        DWORD numBytesRead;
        std::string buff;

        buff.resize(numBytes);
        do {
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
                str += buff + '\0';
                return;
            } else
                str += buff;
        } while (true);
#endif
    }

    bool Executor::isRunning() const
    {
        DWORD exitCode;

        if (GetExitCodeProcess(_processInfo.hProcess, &exitCode) == 0)
            _throwWindowsError();
        return exitCode == STILL_ACTIVE;
    }
}
