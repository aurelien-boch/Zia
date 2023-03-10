cmake_minimum_required(VERSION 3.11)
project(ziaLogger CXX)

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
        Logger
        SHARED
        src/Logger.cpp
        include/Logger.hpp
)

target_include_directories(
        Logger
        PUBLIC
        include/
)

target_link_libraries(
        Logger
        ziapi
)

conan_target_link_libraries(
        Logger
        CONAN_PKG::fmt
)