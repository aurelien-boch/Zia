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
