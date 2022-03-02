project(queues LANGUAGES CXX)

set(NAME queues)

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

target_link_libraries(
    ${NAME}
        ziapi
        loader
)
