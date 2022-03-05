#include <asio/ip/address_v4.hpp>
#include <asio/io_service.hpp>
#include <iostream>

#include "ReverseProxy.hpp"
#include "Converter.hpp"

namespace modules
{

    void ReverseProxy::Init(const ziapi::config::Node &cfg)
    {
        _port = cfg["modules"]["reverse_proxy"]["port"].AsInt();
        _ip = asio::ip::address_v4::from_string(cfg["modules"]["reverse_proxy"]["ip"].AsString()).to_uint();
    }

    ziapi::Version ReverseProxy::GetVersion() const noexcept
    {
        return {1, 0, 0};
    }

    ziapi::Version ReverseProxy::GetCompatibleApiVersion() const noexcept
    {
        return {5, 0, 0};
    }

    const char *ReverseProxy::GetName() const noexcept
    {
        return "Reverse Proxy";
    }

    const char *ReverseProxy::GetDescription() const noexcept
    {
        return "A simple reverse proxy module";
    }

    void ReverseProxy::Handle(ziapi::http::Context &ctx, const ziapi::http::Request &req, ziapi::http::Response &res)
    {
        try {
            asio::io_service io_context;
            asio::ip::tcp::socket sock {io_context};

            sock.connect(asio::ip::tcp::endpoint(asio::ip::address_v4(_ip), _port));
            _send(sock, Converter::toRawRequest(req));
            res = Converter::toResponse(_recvReply(sock));
        } catch (std::exception const &e) {
            res.status_code = ziapi::http::Code::kInternalServerError;
        }
//        char buff[8193]{};
//        std::string zz = Converter::toRawRequest(req);
//        socket.send(asio::buffer(zz.c_str(), zz.size()));
//        socket.receive(asio::buffer(&buff, sizeof(char) * 8192));
//        ziapi::http::Response proxyResponse{Converter::toResponse(sock.receive())};
//        res.body = proxyResponse.body;
//        res.status_code = proxyResponse.status_code;
//        res.headers = proxyResponse.headers;
//        res.reason = proxyResponse.reason;
//        res.version = proxyResponse.version;
    }

    double ReverseProxy::GetHandlerPriority() const noexcept
    {
        return 0;
    }

    bool ReverseProxy::ShouldHandle(const ziapi::http::Context &ctx, const ziapi::http::Request &req) const
    {
        return true;
    }

    void ReverseProxy::_send(asio::ip::tcp::socket &socket, const std::string &data) noexcept
    {
        socket.send(asio::buffer(data.c_str(), data.size()));
    }

    std::string ReverseProxy::_recvReply(asio::ip::tcp::socket &socket) noexcept
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
