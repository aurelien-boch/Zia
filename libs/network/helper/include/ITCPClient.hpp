#ifndef ITCPCLIENT_HPP
#define ITCPCLIENT_HPP

#include "Errors.hpp"
#include "Utility.hpp"

namespace network
{
    /**
     * @brief Represents a TCP Client
     */
    template <typename Request, typename Response>
    class ITCPClient
    {
        public:

            virtual ~ITCPClient() = default;

            /**
             * @brief Connects to a TCP server
             * @param[in] peer Refers to the address of the peer
             */
            virtual void connect(Address const &peer) noexcept = 0;

            /**
             * @brief Sends a packet over TCP
             * @param[in] data Refers to the data to be send over tcp
             */
            virtual std::size_t send(Response const &data) noexcept = 0;

            /**
             * @brief Blocks until a message is received
             * @return The received packet
             */
            [[nodiscard]] virtual Request receive() noexcept = 0;

            /**
             * @brie Sends data asynchronously
             * @param[in] packet Refers to the data to be sent.
             * @param[in|out] callback Refers to the function to be called when a packet is successfully sent
             */
            virtual void asyncSend(
                Response const &packet,
                std::function<void (error::ErrorSocket const &)> &&callback) noexcept = 0;

            /**
             * @brief Asynchronously receives a message
             * @param[in|out] callback Refers to the function to be called each time a message is received
             */
            virtual void asyncReceive(
                std::function<void(error::ErrorSocket const &, Request &)> &&callback) noexcept = 0;
    };
}

#endif //ITCPCLIENT_HPP
