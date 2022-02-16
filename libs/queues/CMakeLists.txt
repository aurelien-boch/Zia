project(queues LANGUAGES CXX)

set(NAME queues)

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
    set(COMPILE_FLAGS -Wall -Wextra -Weffc++)
endif()

target_compile_options(
        ${NAME}
        PRIVATE
        ${COMPILE_FLAGS}
)

target_link_libraries(
        ${NAME}
        ziapi
)