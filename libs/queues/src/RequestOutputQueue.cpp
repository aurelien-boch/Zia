#include "RequestOutputQueue.hpp"

namespace modules
{
    RequestOutputQueue::RequestOutputQueue() :
        _requests{},
        _mutex{}
    {}

    std::optional<RequestOutputQueue::RequestPair> RequestOutputQueue::Pop()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        RequestPair request{};

        if (_requests.empty())
            return std::nullopt;
        request = _requests.front();
        _requests.pop();
        return request;
    }

    std::size_t RequestOutputQueue::Size() const noexcept
    {
        return _requests.size();
    }

    bool RequestOutputQueue::Empty() const noexcept
    {
        return _requests.empty();
    }

    void RequestOutputQueue::Push(RequestPair &&req)
    {
        std::lock_guard<std::mutex> lock{_mutex};
        _requests.push(std::forward<RequestPair>(req));
        _cond_var.notify_one();
    }

    void RequestOutputQueue::Wait() noexcept
    {
        std::unique_lock<std::mutex> lock{_mutex};

        if (!_requests.empty())
            return;
        _cond_var.wait(lock, [this] { return !_requests.empty(); });
    }

    void RequestOutputQueue::StopWait() noexcept
    {
        _cond_var.notify_all();
    }
}
