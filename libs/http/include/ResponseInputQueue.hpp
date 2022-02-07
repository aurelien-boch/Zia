#ifndef RESPONSEINPUTQUEUE_HPP
#define RESPONSEINPUTQUEUE_HPP

#include <queue>

#include <ziapi/Http.hpp>

namespace modules
{
    class ResponseInputQueue : public ziapi::http::IResponseInputQueue
    {
        public:

            ResponseInputQueue();

            ~ResponseInputQueue() = default;

            [[nodiscard]] std::optional<std::pair<ziapi::http::Response, ziapi::http::Context>> Pop();

            [[nodiscard]] std::size_t Size() const noexcept;

            [[nodiscard]] bool Empty() const noexcept;

            void Push(std::pair<ziapi::http::Response, ziapi::http::Context> &&response) noexcept;


        private:
            std::queue<std::pair<ziapi::http::Response, ziapi::http::Context>> _responses;
    };
}

#endif //RESPONSEINPUTQUEUE_HPP
