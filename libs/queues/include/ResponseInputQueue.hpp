#ifndef RESPONSEINPUTQUEUE_HPP
#define RESPONSEINPUTQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

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

            /**
             * @brief Stops the Wait method.
             */
            void StopWait() noexcept;

        private:
            std::queue<ResponsePair> _responses;
            std::mutex _mutex;
            std::condition_variable _condvar;
    };
}

#endif //RESPONSEINPUTQUEUE_HPP
