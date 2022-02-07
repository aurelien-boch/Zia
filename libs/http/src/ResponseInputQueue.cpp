#include "ResponseInputQueue.hpp"

namespace modules
{
    ResponseInputQueue::ResponseInputQueue() : _responses{}
    {}

    std::optional<std::pair<ziapi::http::Response, ziapi::http::Context>> ResponseInputQueue::Pop()
    {
        std::pair<ziapi::http::Response, ziapi::http::Context> response = {};

        if (_responses.empty())
            return std::nullopt;
        response = _responses.front();
        _responses.pop();
        return std::optional<std::pair<ziapi::http::Response, ziapi::http::Context>>(response);
    }

    std::size_t ResponseInputQueue::Size() const noexcept
    {
        return _responses.size();
    }

    bool ResponseInputQueue::Empty() const noexcept
    {
        return _responses.empty();
    }

    void ResponseInputQueue::Push(std::pair<ziapi::http::Response, ziapi::http::Context> &&response) noexcept
    {
        _responses.push(response);
    }

}

