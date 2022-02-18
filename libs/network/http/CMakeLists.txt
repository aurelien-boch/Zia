project(http_network LANGUAGES CXX)

set(NAME http_network)

add_subdirectory(src/)
add_subdirectory(include/)

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
        network_helper
)
