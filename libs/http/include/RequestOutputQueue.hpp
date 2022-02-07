#ifndef RESPONSEINPUTQUEUE_HPP
#define RESPONSEINPUTQUEUE_HPP

#include <queue>

#include <ziapi/Http.hpp>

namespace modules
{
    class RequestOutputQueue : public ziapi::http::IRequestOutputQueue
    {
        public:

            RequestOutputQueue();

            ~RequestOutputQueue() = default;

            [[nodiscard]] std::optional<std::pair<ziapi::http::Request, ziapi::http::Context>> Pop();

            [[nodiscard]] std::size_t Size() const noexcept;

            [[nodiscard]] bool Empty() const noexcept;

            void Push(std::pair<ziapi::http::Request, ziapi::http::Context> &&req);

        private:
            std::queue<std::pair<ziapi::http::Request, ziapi::http::Context>> _requests;
    };
}

#endif //RESPONSEINPUTQUEUE_HPP
