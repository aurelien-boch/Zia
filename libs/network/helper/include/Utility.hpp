#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstdint>

namespace network
{
    struct Address
    {
        std::uint32_t ipAddress;
        std::uint16_t port;

        bool operator==(Address const &other) const noexcept
        {
            return (
                ipAddress == other.ipAddress &&
                port == other.port
            );
        }

        operator bool() const noexcept
        {
            return (
                this->ipAddress != 0 &&
                port != 0
            );
        }
    };
}

#endif /* !UTILITY_HPP */
