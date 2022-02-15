#ifndef REQUESTOUTPUTQUEUE_HPP
#define REQUESTOUTPUTQUEUE_HPP

#include <queue>

#include <ziapi/Http.hpp>

namespace modules
{
    class RequestOutputQueue : public ziapi::http::IRequestOutputQueue
    {
        public:
            using RequestPair = std::pair<ziapi::http::Request, ziapi::http::Context>;

            RequestOutputQueue();

            ~RequestOutputQueue() = default;

            [[nodiscard]] std::optional<RequestPair> Pop();

            [[nodiscard]] std::size_t Size() const noexcept;

            [[nodiscard]] bool Empty() const noexcept;

            void Push(std::pair<ziapi::http::Request, ziapi::http::Context> &&req);

        private:
            std::queue<RequestPair> _requests;
    };
}

#endif //REQUESTOUTPUTQUEUE_HPP
