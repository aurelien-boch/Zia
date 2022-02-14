project(http_module LANGUAGES CXX)

set(NAME network)

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

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(COMPILE_FLAGS  "/W3")
elseif (
(CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR
(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    set(COMPILE_FLAGS -Wall -Wextra -Weffc++ -g -ggdb) # TODO: remove -g
endif()

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
)
