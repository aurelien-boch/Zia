#ifndef RESPONSEINPUTQUEUE_HPP
#define RESPONSEINPUTQUEUE_HPP

#include <queue>

#include <ziapi/Http.hpp>
#include <Loader.hpp>

namespace modules
{
    /**
     * @class Implements Response queue
     */
    class DllExport ResponseInputQueue : public ziapi::http::IResponseInputQueue
    {
        public:

            using ResponsePair = std::pair<ziapi::http::Response, ziapi::http::Context>;

            ResponseInputQueue();

            ~ResponseInputQueue() override = default;

            [[nodiscard]] std::optional<ResponsePair> Pop() override;

            [[nodiscard]] std::size_t Size() const noexcept override;

            [[nodiscard]] bool Empty() const noexcept;

            void Push(ResponsePair &&response) noexcept;

            void Wait() noexcept override;

        private:
            std::queue<ResponsePair> _responses;
    };
}

#endif //RESPONSEINPUTQUEUE_HPP
