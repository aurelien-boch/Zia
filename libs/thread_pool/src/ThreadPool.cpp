#include <mutex>
#include "ThreadPool.hpp"

namespace thread
{
    thread::ThreadPool::ThreadPool(std::size_t threads) :
        _running{true}
    {
        if (threads < 2)
            throw std::runtime_error{"Error, threadpool can't have less than two threads"};
        for (; threads > 0; --threads)
            _threads.emplace_back([this]() -> void { return _driverProgram(); });
    }

    void ThreadPool::_driverProgram()
    {
        while (_running) {
            std::unique_lock<std::mutex> lock{_mutex};

            _cond_variable.wait(lock, [this] { return !_functions.empty(); });
            callableObject function = std::move(_functions.front());
            _functions.pop();
            lock.unlock();
            function();
            _cond_variable.notify_one();
        }
    }

    void ThreadPool::run(ThreadPool::callableObject &&f) noexcept
    {
        std::lock_guard<std::mutex> lock{_mutex};
        _functions.emplace(std::forward<callableObject>(f));
        _cond_variable.notify_one();
    }

    ThreadPool::~ThreadPool()
    {
        _running = false;
        _cond_variable.notify_all();
    }
}
