#ifndef HTTPHELPER_HPP
#define HTTPHELPER_HPP

#include <string>

namespace network::http
{
    /**
     * @class HttpHelper
     * @brief Helper for Http requests parsing
     */
    class RequestHelper
    {
        public:

            /**
             * @brief Retrieves the size of the body from the Content-Length header
             * @param[in] header the header to parse
             * @return the length of the body
             */
            [[nodiscard]] static std::size_t getContentLength(std::string const &header);

            /**
             * @brief Cleans the header and fills the body
             * @param[in, out] header the header to clean
             * @param[in, out] body the body to fill
             */
            static void cleanHeader(std::string &header, std::string &body);

    };
}

#endif //HTTPHELPER_HPP
