project(network_helper LANGUAGES CXX)

set(NAME network_helper)

add_subdirectory(include/)

add_library(${NAME} INTERFACE)

target_sources(
    ${NAME} INTERFACE
        ${INCLUDES}
)

target_include_directories(
    ${NAME} INTERFACE
        include/
)
