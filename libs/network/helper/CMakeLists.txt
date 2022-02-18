project(network_helper LANGUAGES CXX)

set(NAME network_helper)

add_subdirectory(include/)

add_library(
    ${NAME}
        SHARED
        ${INCLUDES}
)

set_target_properties(${NAME} PROPERTIES LINKER_LANGUAGE CXX)

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
