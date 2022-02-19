cmake_minimum_required(VERSION 2.11)
set(CMAKE_CXX_STANDARD 20)

if (DEFINED ENABLE_TESTING AND "${ENABLE_TESTING}" STREQUAL "yes")
    message(STATUS "ENABLE_TESTING=yes detected, enabling testing")
    add_subdirectory(tests)
endif()

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

add_library(
        HttpFormatter SHARED
        include/HttpFormatter.hpp
        src/HttpFormatter.cpp
)

target_include_directories(
        HttpFormatter PUBLIC
        include
)

target_link_libraries(
        HttpFormatter
        ziapi
)