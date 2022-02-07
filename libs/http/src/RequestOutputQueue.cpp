#include "RequestOutputQueue.hpp"

namespace modules
{
    RequestOutputQueue::RequestOutputQueue() : _requests{}
    {}

    std::optional<std::pair<ziapi::http::Request, ziapi::http::Context>> RequestOutputQueue::Pop()
    {
        std::pair<ziapi::http::Request, ziapi::http::Context> request = {};

        if (_requests.empty())
            return std::nullopt;
        request = _requests.front();
        _requests.pop();
        return std::optional<std::pair<ziapi::http::Request, ziapi::http::Context>>(request);
    }

    std::size_t RequestOutputQueue::Size() const noexcept
    {
        return _requests.size();
    }

    bool RequestOutputQueue::Empty() const noexcept
    {
        return _requests.empty();
    }

    void RequestOutputQueue::Push(std::pair<ziapi::http::Request, ziapi::http::Context> &&req)
    {
        _requests.push(req);
    }
}