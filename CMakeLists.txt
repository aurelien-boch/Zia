cmake_minimum_required(VERSION 3.21)
project(ZIA CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(include/)
add_subdirectory(libs/)
add_subdirectory(src/)

if (DEFINED ENABLE_TESTING AND "${ENABLE_TESTING}" STREQUAL "yes")
    message(STATUS "ENABLE_TESTING=yes detected, enabling testing")
    add_subdirectory(tests)
endif()

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

target_link_libraries(
    zia
        api
)
