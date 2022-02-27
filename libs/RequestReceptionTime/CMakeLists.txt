cmake_minimum_required(VERSION 3.11)
project(ziaRequestReceptionTime CXX)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

add_library(
        RequestReceptionTime
        SHARED
        src/RequestReceptionTime.cpp
        include/RequestReceptionTime.hpp
)

target_include_directories(
        RequestReceptionTime
        PUBLIC
        include/
)

target_link_libraries(
        RequestReceptionTime
        ziapi
)