#ifndef REQUESTOUTPUTQUEUE_HPP
#define REQUESTOUTPUTQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

#include <ziapi/Http.hpp>
#include <Loader.hpp>

namespace modules
{
    class DllExport RequestOutputQueue : public ziapi::http::IRequestOutputQueue
    {
        public:
            using RequestPair = std::pair<ziapi::http::Request, ziapi::http::Context>;

            RequestOutputQueue();

            ~RequestOutputQueue() override = default;

            [[nodiscard]] std::optional<RequestPair> Pop();

            [[nodiscard]] std::size_t Size() const noexcept override;

            [[nodiscard]] bool Empty() const noexcept;

            void Push(RequestPair &&req) override;

            /**
             * @brief Blocks until a request pushed or StopWait is called.
             */
            void Wait() noexcept;

            /**
             * @brief Stops the Wait method
             */
            void StopWait() noexcept;

        private:
            std::mutex _mutex;
            std::condition_variable _cond_var;
            std::queue<RequestPair> _requests;
    };
}

#endif //REQUESTOUTPUTQUEUE_HPP
