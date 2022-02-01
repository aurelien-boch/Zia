cmake_minimum_required(VERSION 3.21)
project(ZIA CXX)

set(CMAKE_CXX_STANDARD 20)
set(CONAN_CMAKE_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(include/)
add_subdirectory(libs/)
add_subdirectory(src/)

if (DEFINED ENABLE_TESTING AND "${ENABLE_TESTING}" STREQUAL "yes")
    message(STATUS "ENABLE_TESTING=yes detected, enabling testing")
    add_subdirectory(tests)
endif()

if(EXISTS ${CONAN_CMAKE_OUTPUT})
    include(${CONAN_CMAKE_OUTPUT})
    conan_basic_setup(TARGETS)
else()
    message(FATAL_ERROR "conanbuildinfo.cmake not found.")
endif()

find_package(ziapi)

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
        ziapi
)
