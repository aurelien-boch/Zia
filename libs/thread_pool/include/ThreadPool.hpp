#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <functional>
#include <thread>
#include <condition_variable>
#include <queue>

#ifndef DllExport
    #ifdef _WIN32
        #define DllExport __declspec(dllexport)
    #else
        #define DllExport
    #endif
#endif

namespace thread
{
    /**
     * @class ThreadPool
     * @brief A simple thread pool.
     */
    class DllExport ThreadPool
    {
        public:
            using callableObject = std::function<void (void)>;

            /**
             * @param[in] threads the number of threads the thread pool must use.
             */
            explicit ThreadPool(std::size_t threads = std::thread::hardware_concurrency());

            ~ThreadPool();

            /**
             * @brief Adds a callableObject to the queue of callables that must be ran asynchronously.
             * @param[in] f A rvalue ref to the callableObject that must be ran asynchronously.
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
