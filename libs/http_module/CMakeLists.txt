project(http_module LANGUAGES CXX)

set(NAME http_module)

add_subdirectory(src/)
add_subdirectory(include/)

if (DEFINED ENABLE_TESTING AND "${ENABLE_TESTING}" STREQUAL "yes")
    message(STATUS "ENABLE_TESTING=yes detected, enabling testing")
    add_subdirectory(tests)
endif()

add_library(
        ${NAME}
        SHARED
            ${SRCS}
            ${INCLUDES}
)

target_include_directories(
        ${NAME}
            PUBLIC
            include/
)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

target_compile_options(
        ${NAME}
        PRIVATE
        ${COMPILE_FLAGS}
)

conan_target_link_libraries(
        ${NAME}
        CONAN_PKG::asio
)

target_link_libraries(
        ${NAME}
        ziapi
        http_network
        queues
        http_parser
        http_formatter
        loader
)
