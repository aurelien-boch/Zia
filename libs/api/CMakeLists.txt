add_library(api INTERFACE)

target_sources(
    api
        INTERFACE
        Placeholder.hpp
)

target_include_directories(
    api
        INTERFACE
        .
)