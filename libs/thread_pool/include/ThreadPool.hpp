#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <functional>
#include <thread>
#include <condition_variable>
#include <queue>

namespace thread
{
    class ThreadPool
    {
        public:
            /**
             *
             */
            using callableObject = std::function<void (void)>;

            /**
             *
             * @param maxThreads
             */
            explicit ThreadPool(std::size_t maxThreads = std::thread::hardware_concurrency());

            ~ThreadPool();

            /**
             *
             * @param f
             */
            void run(callableObject &&f) noexcept;

        private:
            bool _running;
            std::mutex _mutex;
            std::condition_variable _cond_variable;
            std::queue<callableObject> _functions;
            std::vector<std::jthread> _threads;

            void _driverProgram();

    };
}

#endif //THREADPOOL_HPP
