cmake_minimum_required(VERSION 3.21)
project(ZIA CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(include/)
add_subdirectory(libs/)
add_subdirectory(src/)

add_executable(
    zia
        ${INCLUDES}
        ${SRCS}
        main.cpp
)

target_include_directories(
    zia
        PRIVATE
        include/
)
