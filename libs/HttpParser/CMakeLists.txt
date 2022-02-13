cmake_minimum_required(VERSION 3.11)
project(ziaHttpParser CXX)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

add_library(
    ziaHttpParser
        include/HttpParser.hpp
        src/HttpParser.cpp
        include/Exception.hpp)

target_include_directories(
    ziaHttpParser
        PUBLIC
        include/
)

target_link_libraries(
    ziaHttpParser
        ziapi
)
