#ifndef ITCPCLIENT_HPP
#define ITCPCLIENT_HPP

#include "Errors.hpp"
#include "Utility.hpp"

namespace network
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
             * @param[in] peer This parameter refers to the address of the peer
             */
            virtual void connect(Address const &peer) noexcept = 0;

            /**
             * @brief Method that sends a packet over TCP
             * @param[in] data This parameter refers to the data to be send over tcp
             */
            virtual std::size_t send(Response const &data) noexcept = 0;

            /**
             * @brief Methods that blocks until a message is received
             * @return Returns the received packet
             */
            [[nodiscard]] virtual Request receive() noexcept = 0;

            /**
             * @brief Method that sends data asynchronously
             * @param[in] packet This parameter refers to the data to be sent.
             * @param[in|out] callback This parameter refers to the function to be called when a packet is successfully sent
             */
            virtual void asyncSend(
                Response const &packet,
                std::function<void (error::ErrorSocket const &)> &&callback) noexcept = 0;

            /**
             * @brief Method that asynchronously received a message
             * @param[in|out] callback This parameter refers to the function to be called each time a message is received
             */
            virtual void asyncReceive(
                std::function<void(error::ErrorSocket const &, Request &)> &&callback) noexcept = 0;
    };
}

#endif //ITCPCLIENT_HPP
