add_library(
    executor
        include/Executor.hpp
        src/Executor.cpp
)

target_include_directories(
    executor PUBLIC
        include
)

set_target_properties(
    executor PROPERTIES
        CXX_STANDARD 20
)

target_link_libraries(
    executor PUBLIC
        env_manager
)
