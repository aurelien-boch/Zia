add_library(
    thread_pool SHARED
        include/ThreadPool.hpp
        src/ThreadPool.cpp
)

target_include_directories(
    thread_pool PUBLIC
        include/
)
