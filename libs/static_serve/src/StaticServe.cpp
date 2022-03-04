#include <filesystem>
#include <iostream>
#include <fstream>
#include "StaticServe.hpp"

namespace modules
{
    const std::array<std::string, 5> StaticServe::_sizes = {"B", "KB", "MB", "GB", "TB"};

    void StaticServe::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
    {
        if (_mayBeServed(req.target)) {
            if (std::filesystem::exists(req.target)) {
                if (std::filesystem::is_directory(req.target))
                    _serveDir(req.target, res);
                else
                    _serveFile(req.target, res);
            } else
                res.status_code = ziapi::http::Code::kNotFound;
        } else
            res.status_code = ziapi::http::Code::kForbidden;
    }

    void StaticServe::Init(const ziapi::config::Node &cfg)
    {
        _serveDirPath = std::filesystem::absolute(cfg["modules"]["staticserve"]["dirpath"].AsString());
    }

    ziapi::Version StaticServe::GetVersion() const noexcept
    {
        return {1, 0, 0};
    }

    ziapi::Version StaticServe::GetCompatibleApiVersion() const noexcept
    {
        return {5, 0, 0};
    }

    const char *StaticServe::GetName() const noexcept
    {
        return "Static Serve";
    }

    const char *StaticServe::GetDescription() const noexcept
    {
        return "Serves a directory and static files";
    }

    double StaticServe::GetHandlerPriority() const noexcept
    {
        return 1;
    }

    bool StaticServe::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
    {
        return true;
    }

    bool StaticServe::_mayBeServed(std::string const &path) const noexcept
    {
        std::filesystem::path p{path};

        while (p != "/" && p.has_parent_path()) {
            if (p.string() == _serveDirPath.string())
                return true;
            p = p.parent_path();
        }
        return false;
    }

    void StaticServe::_serveDir(const std::string &path, ziapi::http::Response &res) const noexcept
    {
        std::string parentPath{std::filesystem::path(path).parent_path()};
        _setupHtml(path, res);
        if (_mayBeServed(parentPath))
            res.body +="<tr><td><a href=\"" + parentPath + "\"/>[parent directory]</a></td></tr>";
        std::filesystem::directory_iterator it;

        try {
            it = std::filesystem::directory_iterator{path};
        } catch (std::filesystem::filesystem_error const &) {
            res.status_code = ziapi::http::Code::kForbidden;
            return;
        }
        for (auto const &e: it)
            try {
                _appendFile(res, e);
            } catch (std::filesystem::filesystem_error const &e) {
                std::cerr << e.what() << std::endl;
            }
        _endHtml(res);
        res.status_code = ziapi::http::Code::kOK;
    }

    void StaticServe::_appendFile(ziapi::http::Response &res, std::filesystem::directory_entry const &e)
    {
        time_t lastWrite{std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(e.last_write_time()))};
        char formattedTime[18]{};
        std::string result{"<tr>"};

        strftime(formattedTime, sizeof(formattedTime), "%d/%m/%y, %H:%M", localtime(&lastWrite));
        if (e.is_directory())
            result += "<td><a href=\"" + e.path().string() +  "\">" + e.path().filename().string() + "/</a></td><td></td>";
        else
            result +=
                "<td><a href=\"" + e.path().string() + "\">" + e.path().filename().string() + "</a></td><td>" +
                _sizeToHuman(e.file_size()) + "</td>";
        result += "<td>" + std::string{formattedTime} + "</td></tr>";
        res.body += result;
    }

    void StaticServe::_setupHtml(std::string const &path, ziapi::http::Response &res) noexcept
    {
        res.body =
            "<html>"
            "   <head>"
            "       <title>Index of " + path + "</title>"
            "   </head>"
            "   <body>"
            "       <h1>Index of " + path + "</h1>"
            "       <hr class=\"solid\">"
            "       <br/>"
            "       <table>"
            "           <thead>"
            "               <tr>"
            "                   <th>Path</th>"
            "                   <th>Size</th>"
            "                   <th>Date Modified</th>"
            "               </tr>"
            "           </thead>"
            "           <tbody>";
    }

    void StaticServe::_endHtml(ziapi::http::Response &res) noexcept
    {
        res.body +=
            "           </tbody>"
            "       </table>"
            "   </body>"
            "</html>";
    }

    std::string StaticServe::_sizeToHuman(uintmax_t size) noexcept
    {
        std::size_t index{};

        while (size >= 1024 && index < _sizes.size()) {
            size /= 1024;
            ++index;
        }
        return {std::to_string(size) + _sizes[index]};
    }

    void StaticServe::_serveFile(const std::string &path, ziapi::http::Response &response)
    {
        std::ifstream stream{path};
        stream.seekg(0, std::ios::end);
        std::size_t size{static_cast<std::size_t>(stream.tellg())};
        std::string buffer(size, ' ');
        stream.seekg(0);
        response.body.resize(size);
        stream.read(response.body.data(), size);
    }
}
