#include "RequestOutputQueue.hpp"

namespace modules
{
    RequestOutputQueue::RequestOutputQueue() : _requests{}
    {}

    std::optional<RequestOutputQueue::RequestPair> RequestOutputQueue::Pop()
    {
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
        _requests.push(std::forward<RequestPair>(req));
    }
}