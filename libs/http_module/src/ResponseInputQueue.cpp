#include "ResponseInputQueue.hpp"

namespace modules
{
    ResponseInputQueue::ResponseInputQueue() : _responses{}
    {}

    std::optional<ResponseInputQueue::ResponsePair> ResponseInputQueue::Pop()
    {
        ResponsePair response = {};

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
        _responses.push(std::forward<ResponsePair>(response));
    }

}

