#include <stdexcept>

#include "RequestHelper.hpp"

namespace network::http
{
    std::size_t RequestHelper::getContentLength(std::string const &header)
    {
        std::string toFind("Content-Length: ");
        std::size_t pos = header.find(toFind);
        std::size_t bodyLength;

        if (pos == std::string::npos)
            throw std::runtime_error{"No Content-Length header"};
        pos += toFind.size();
        std::size_t pos2 = header.substr(pos).find("\r\n");

        if (pos2 == std::string::npos)
            throw std::invalid_argument("Invalid Content-Length header");
        try {
            return std::stol(header.substr(pos, pos2));
        } catch (std::invalid_argument const &) {
            throw std::invalid_argument("Invalid Content-Length header");
        }
    }

    void RequestHelper::cleanHeader(std::string &header, std::string &body)
    {
        std::string toFind = "\r\n\r\n";
        std::size_t pos = header.find(toFind);

        if (pos == std::string::npos)
            throw std::runtime_error("Invalid header");
        body = header.substr(pos + toFind.size());
        header = header.substr(0, pos + toFind.size());
    }
}