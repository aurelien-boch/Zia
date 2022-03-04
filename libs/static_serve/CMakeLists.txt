add_library(
    static_serve SHARED
        entrypoint.cpp
        include/StaticServe.hpp
        src/StaticServe.cpp
)

target_include_directories(
    static_serve PRIVATE
        include/
)

target_link_libraries(
    static_serve PRIVATE
        ziapi
        loader
)
