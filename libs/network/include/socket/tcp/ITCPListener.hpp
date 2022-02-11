#ifndef ITCPSERVER_HPP
#define ITCPSERVER_HPP

#include <memory>

#include "ITCPClient.hpp"

namespace network::socket
{
    /**
     * @brief This class represents a TCP server listening for new connections
     */
     template <typename Request, typename Response>
    class ITCPListener
    {
        public:
            virtual ~ITCPListener() = default;

            /**
             * @brief Method that accepts incoming connections and create a new ITCPClient instance on each connections.
             * @param callback a callback function that will be call on each new connection. It takes as parameter an
             * ITCPClient which corresponds to the caller and the error_code of the operation
             */
            virtual void run(
                const std::function<void (const error::ErrorSocket &, std::shared_ptr<ITCPClient<Request, Response>>)> &callback) noexcept = 0;
    };
}

#endif //ITCPSERVER_HPP
