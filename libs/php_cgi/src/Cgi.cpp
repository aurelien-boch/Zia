#include <filesystem>
#include <EnvManager.hpp>
#include <Executor.hpp>

#include "Cgi.hpp"

namespace modules
{
    Cgi::Cgi()
    {
        _env.loadFromEnvironment();
    }

    void Cgi::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
    {
        env::Manager manager{_env};
        std::string cgiResult;
        _populateEnv(manager, ctx, req);

        {
            execution::Executor child {_cgiPath, {}, manager};
            child.wait();
            child >> cgiResult;
        }
        //TODO, need core

    }

    void Cgi::Init(const ziapi::config::Node &cfg)
    {
        _cgiPath = cfg["modules"]["CGI"]["CgiPath"].AsString();
    }

    ziapi::Version Cgi::GetVersion() const noexcept
    {
        return {1, 0, 0};
    }

    ziapi::Version Cgi::GetCompatibleApiVersion() const noexcept
    {
        return {4, 0, 0};
    }

    const char *Cgi::GetName() const noexcept
    {
        return "Cgi";
    }

    const char *Cgi::GetDescription() const noexcept
    {
        return "A simple module that process cgi on the request";
    }

    double Cgi::GetHandlerPriority() const noexcept
    {
        return 1;
    }

    bool Cgi::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
    {
        return true;
    }

    Cgi::Url Cgi::_parseUrl(const std::string &url)
    {
        Url res{};

        std::size_t domainBegin = url.find_first_of("://");

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
        char hostname[1024];

        gethostname(hostname, 1024);
        return {hostname};
    }

    void Cgi::_populateEnv(env::Manager &env, ziapi::http::Context &ctx, const ziapi::http::Request &req)
    {
        Url url{_parseUrl(req.target)};
        std::uint16_t port = std::any_cast<std::uint16_t>(ctx["PORT"]);
        std::uint32_t addr = std::any_cast<std::uint32_t>(ctx["REMOTE_ADDR"]);
        auto *converted = reinterpret_cast<std::uint8_t *>(&addr);

        env.pushEnvVariable("AUTH_TYPE", "");
        env.pushEnvVariable("CONTENT_LENGTH", std::to_string(req.body.size()));
        env.pushEnvVariable("CONTENT_TYPE", "");
        env.pushEnvVariable("GATEWAY_INTERFACE", "CGI/1.1");
        env.pushEnvVariable("PATH_INFO", url.route);
        env.pushEnvVariable("PATH_TRANSLATED", std::filesystem::path(url.route).relative_path().string());
        env.pushEnvVariable("QUERY_STRING", url.queryStr);
        env.pushEnvVariable("REMOTE_ADDR",
                                std::to_string(converted[0]) + "." +
                                std::to_string(converted[1]) + "." +
                                std::to_string(converted[2]) + "." +
                                std::to_string(converted[3]));
        env.pushEnvVariable("REMOTE_HOST", "");
        env.pushEnvVariable("REQUEST_METHOD", req.method);
        env.pushEnvVariable("SCRIPT_NAME", url.route);
        env.pushEnvVariable("SERVER_NAME", _getHostName());
        env.pushEnvVariable("SERVER_PORT", std::to_string(port));
        env.pushEnvVariable("SERVER_PROTOCOL", "HTTP/1.1");
        env.pushEnvVariable("SERVER_SOFTWARE", "Zia version (1)");
    }
}
