add_library(
    reverse_proxy SHARED
        include/ReverseProxy.hpp
        include/Converter.hpp
        src/ReverseProxy.cpp
        src/Converter.cpp
        entrypoint.cpp
)

target_include_directories(
    reverse_proxy PRIVATE
        include/
)

target_link_libraries(
    reverse_proxy PRIVATE
        ziapi
        http_network
)
