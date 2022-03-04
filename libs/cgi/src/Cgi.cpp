#pragma comment(lib, "Ws2_32.lib")

#include "Cgi.hpp"

#include <EnvManager.hpp>
#include <Executor.hpp>
#include <filesystem>
#include <iostream>

namespace modules
{
    Cgi::Cgi() { _env.loadFromEnvironment(); }

    void Cgi::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
    {
        env::Manager manager{_env};
        std::string cgiResult{};

        _populateEnv(manager, ctx, req);
        res.status_code = ziapi::http::Code::kOK;
        try {
            execution::Executor child{_cgiPath, {manager.getEnvVariable("PATH_TRANSLATED")}, manager};

            if (req.method == ziapi::http::method::kPost || req.method == ziapi::http::method::kPut)
                child << req.body;
            try {
                child.wait();
            } catch (std::runtime_error const &e) {
                std::cerr << e.what() << std::endl;
            }
            child >> cgiResult;
        } catch (std::runtime_error const &e) {
            std::cerr << e.what() << std::endl;
        }
        _parseResponse(cgiResult, res);
    }

    void Cgi::Init(const ziapi::config::Node &cfg)
    {
        _cgiPath = cfg["modules"]["cgi"]["cgiPath"].AsString();
        _rootDirectory = cfg["modules"]["cgi"]["rootDirectory"].AsString();
    }

    ziapi::Version Cgi::GetVersion() const noexcept { return {1, 0, 0}; }

    ziapi::Version Cgi::GetCompatibleApiVersion() const noexcept { return {5, 0, 0}; }

    const char *Cgi::GetName() const noexcept { return "Cgi"; }

    const char *Cgi::GetDescription() const noexcept { return "A simple module that process cgi on the request"; }

    double Cgi::GetHandlerPriority() const noexcept { return 1; }

    bool Cgi::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const { return true; }

    Cgi::Url Cgi::_parseUrl(std::string const &url)
    {
        Url res{};
        std::size_t domainBegin{url.find_first_of("://")};

        if (domainBegin == std::string::npos)
            throw std::runtime_error{"Malformed url"};
        std::size_t domainEnd = url.find_first_of('/', domainBegin);
        if (domainEnd == std::string::npos)
            throw std::runtime_error{"Malformed url"};
        res.host = url.substr(domainBegin, domainEnd - domainBegin);
        std::size_t queryStart = url.find_first_of('?', domainEnd);
        if (queryStart == std::string::npos) {
            res.route = url.substr(domainEnd, domainEnd - queryStart);
            return res;
        }
        res.queryStr = url.substr(queryStart, url.size() - queryStart);
        return res;
    }

    std::string Cgi::_getHostName()
    {
        char hostname[1024]{};

        gethostname(hostname, 1024);
        return {hostname};
    }

    void Cgi::_populateEnv(env::Manager &env, ziapi::http::Context &ctx, const ziapi::http::Request &req)
    {
        std::size_t queryParams{req.target.find('?')};
        std::string filepath{_rootDirectory + req.target.substr(0, queryParams)};
        auto port{std::any_cast<std::uint16_t>(ctx["PORT"])};
        auto addr{std::any_cast<std::uint32_t>(ctx["REMOTE_ADDR"])};
        auto *converted{reinterpret_cast<std::uint8_t *>(&addr)};

        env.pushEnvVariable("AUTH_TYPE", "");
        env.pushEnvVariable("CONTENT_LENGTH", std::to_string(req.body.size()));
        env.pushEnvVariable("CONTENT_TYPE", "");
        env.pushEnvVariable("GATEWAY_INTERFACE", "CGI/1.1");
        env.pushEnvVariable("PATH_INFO", filepath);
        env.pushEnvVariable("PATH_TRANSLATED", std::filesystem::absolute(filepath).string());
        env.pushEnvVariable("QUERY_STRING", queryParams != std::string::npos
                                                ? req.target.substr(queryParams + 1, req.target.size())
                                                : "");
        env.pushEnvVariable("REMOTE_ADDR", std::to_string(converted[0]) + "." + std::to_string(converted[1]) + "."
                                               + std::to_string(converted[2]) + "." + std::to_string(converted[3]));
        env.pushEnvVariable("REMOTE_HOST", "");
        env.pushEnvVariable("REQUEST_METHOD", req.method);
        env.pushEnvVariable("SCRIPT_NAME", filepath);
        env.pushEnvVariable("SERVER_NAME", _getHostName());
        env.pushEnvVariable("SERVER_PORT", std::to_string(port));
        env.pushEnvVariable("SERVER_PROTOCOL", "HTTP/1.1");
        env.pushEnvVariable("SERVER_SOFTWARE", "Zia version (1)");
        env.pushEnvVariable("REDIRECT_STATUS", "200");
    }

    void Cgi::_parseResponse(std::string const &cgiResult, ziapi::http::Response &res) noexcept
    {
        std::size_t bodyOffset{cgiResult.find("\r\n\r\n")};

        if (bodyOffset == std::string::npos)
            res.status_code = ziapi::http::Code::kInternalServerError;
        else {
            for (std::size_t currentIndex{0}; currentIndex < bodyOffset;) {
                const std::size_t headerEnd = cgiResult.find("\r\n", currentIndex);
                const std::size_t headerSeparator = cgiResult.find(':', currentIndex);
                const std::string headerName{cgiResult.substr(currentIndex, headerSeparator - currentIndex)};
                const std::string headerValue{
                    cgiResult.substr(headerSeparator + (cgiResult[headerSeparator + 1] == ' ' ? 2 : 1),
                                     headerEnd - headerSeparator - (cgiResult[headerSeparator + 1] == ' ' ? 2 : 1))};
                auto const &[_, success]{res.headers.try_emplace(headerName, headerValue)};

                if (!success) {
                    res.status_code = ziapi::http::Code::kInternalServerError;
                    std::cerr << "Failed to set " << headerName << " header";
                }
                currentIndex = headerEnd + (cgiResult[headerSeparator + 1] == ' ' ? 2 : 1);
            }
            if (res.headers.contains("Status"))
                res.status_code = static_cast<ziapi::http::Code>(std::stoi(res.headers["Status"]));
            if (res.status_code == ziapi::http::Code::kOK)
                res.body = cgiResult.substr(bodyOffset + 4, cgiResult.size() - bodyOffset);
        }
    }
} // namespace modules
