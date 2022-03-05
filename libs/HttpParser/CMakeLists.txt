cmake_minimum_required(VERSION 3.11)
project(ziaHttpParser CXX)

if (DEFINED ENABLE_TESTING AND "${ENABLE_TESTING}" STREQUAL "yes")
    message(STATUS "ENABLE_TESTING=yes detected, enabling testing")
    add_subdirectory(tests)
endif()

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

add_library(
        http_parser SHARED
        include/HttpParser.hpp
        src/HttpParser.cpp
        include/Exception.hpp)

target_include_directories(
        http_parser
        PUBLIC
        include/
)

target_link_libraries(
    http_parser
        ziapi
)
