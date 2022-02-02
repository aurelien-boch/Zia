add_library(loader INTERFACE)

target_sources(
    loader INTERFACE
        include/Loader.hpp
)

target_include_directories(
    loader INTERFACE
        include/
)
