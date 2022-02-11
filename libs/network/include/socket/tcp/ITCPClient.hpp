#ifndef ITCPCLIENT_HPP
#define ITCPCLIENT_HPP

#include "socket/Errors.hpp"
#include "socket/Utility.hpp"

namespace network::socket
{
    /**
     * @brief This class represents a TCP Client
     */
     template <typename Request, typename Response>
    class ITCPClient
    {
        public:

            virtual ~ITCPClient() = default;

            /**
             * @brief Method that connects to a TCP server
             * @param peer This parameter refers to the address of the peer
             */
            virtual void connect(const Address &peer) noexcept = 0;

            /**
             * @brief Method that sends a packet over TCP
             * @param data This parameter refers to the data to be send over tcp
             */
            virtual std::size_t send(const Response &data) noexcept = 0;

            /**
             * @brief Methods that blocks until a message is received
             * @return Returns the received packet
             */
            [[nodiscard]] virtual Request receive() noexcept = 0;

            /**
             * @brief Method that sends data asynchronously
             * @param packet This parameter refers to the data to be sent.
             * @param callback This parameter refers to the function to be called when a packet is successfully sent
             */
            virtual void asyncSend(
                const Response &packet,
                const std::function<void (const error::ErrorSocket &)> &callback) noexcept = 0;

            /**
             * @brief Method that asynchronously received a message
             * @param callback This parameter refers to the function to be called each time a message is received
             */
            virtual void asyncRead(
                const std::function<void(const error::ErrorSocket &, Request &)> &callback) noexcept = 0;
    };
}

#endif //ITCPCLIENT_HPP
