#include <thread>

#include "ResponseInputQueue.hpp"

namespace modules
{
    ResponseInputQueue::ResponseInputQueue() : _responses{}
    {}

    std::optional<ResponseInputQueue::ResponsePair> ResponseInputQueue::Pop()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        ResponsePair response{};

        if (_responses.empty())
            return std::nullopt;
        response = _responses.front();
        _responses.pop();
        return response;
    }

    std::size_t ResponseInputQueue::Size() const noexcept
    {
        return _responses.size();
    }

    bool ResponseInputQueue::Empty() const noexcept
    {
        return _responses.empty();
    }

    void ResponseInputQueue::Push(ResponsePair &&response) noexcept
    {
        std::lock_guard<std::mutex> lock{_mutex};
        _responses.push(std::forward<ResponsePair>(response));
        _condvar.notify_one();
    }

    void ResponseInputQueue::Wait() noexcept
    {
        std::unique_lock<std::mutex> lock{_mutex};

        if (!_responses.empty())
            return;
        _condvar.wait(lock, [this] { return !_responses.empty(); });
    }

    void ResponseInputQueue::StopWait() noexcept
    {
        _condvar.notify_all();
    }
}

