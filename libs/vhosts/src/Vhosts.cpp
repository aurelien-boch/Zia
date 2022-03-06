#include <iostream>
#include "Vhosts.hpp"
#include "Converter.hpp"

namespace modules
{
    void Vhosts::Init(const ziapi::config::Node &cfg)
    {
        ziapi::config::Dict dict{cfg["modules"]["vhosts"].AsDict()};

        for (auto const &[key, val] : dict) {
            const std::string &strVal{val->AsString()};
            std::size_t separator{strVal.find(':')};

            if (separator != std::string::npos)
                _hosts.try_emplace(
                    key,
                    target{
                        asio::ip::address_v4::from_string(strVal.substr(0, separator)).to_uint(),
                        std::stoul(strVal.substr(separator + 1, strVal.size()))
                    }
                );
            else
                std::cerr << "Invalid value for key: " << key << " (" << strVal << ")" << std::endl;
        }
    }

    ziapi::Version Vhosts::GetVersion() const noexcept
    {
        return {1, 0, 0};
    }

    ziapi::Version Vhosts::GetCompatibleApiVersion() const noexcept
    {
        return {5, 0, 0};
    }

    const char *Vhosts::GetName() const noexcept
    {
        return "Vhosts";
    }

    const char *Vhosts::GetDescription() const noexcept
    {
        return "A simple module that allows you to create vhosts";
    }

    void Vhosts::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
    {
        try {
            asio::io_service io_context;
            std::string host{req.headers.at("Host")};
            std::size_t separator{host.find(':')};
            target target{_hosts.at(host.substr(0, separator))};
            asio::ip::tcp::socket sock {io_context};

            sock.connect(asio::ip::tcp::endpoint(asio::ip::address_v4(target.first), target.second));
            _send(sock, Converter::toRawRequest(req));
            res = Converter::toResponse(_recvReply(sock));
        } catch (std::out_of_range const &) {
            res.status_code = ziapi::http::Code::kBadRequest;
        } catch (std::system_error const &) {
            res.status_code = ziapi::http::Code::kInternalServerError;
        } catch (std::exception const &e) {
            std::cerr << e.what() << std::endl;
            res.status_code = ziapi::http::Code::kInternalServerError;
        }
    }

    double Vhosts::GetHandlerPriority() const noexcept
    {
        return 0;
    }

    bool Vhosts::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
    {
        return true;
    }

    void Vhosts::_send(asio::ip::tcp::socket &socket, const std::string &data) noexcept
    {
        socket.send(asio::buffer(data.c_str(), data.size()));
    }

    std::string Vhosts::_recvReply(asio::ip::tcp::socket &socket) noexcept
    {
        std::string res;
        char buff[8193]{};

        while (socket.is_open()) {
            try {
                socket.receive(asio::buffer(&buff, sizeof(buff)));
                res += buff;
            } catch (std::system_error const &err) {
                break;
            }
        }
        return res;
    }
}
