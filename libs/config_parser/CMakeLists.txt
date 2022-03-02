find_package(yaml-cpp)

add_library(
    config_parser SHARED
        include/ConfigParser.hpp
        src/ConfigParser.cpp
)

if (DEFINED ENABLE_TESTING AND "${ENABLE_TESTING}" STREQUAL "yes")
    message(STATUS "ENABLE_TESTING=yes detected, enabling testing")
    add_subdirectory(tests)
endif()

target_include_directories(
    config_parser PUBLIC
        include/
)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

target_compile_options(
    config_parser
        PRIVATE
        ${COMPILE_FLAGS}
)

target_link_libraries(
    config_parser
        ziapi
        loader
        yaml-cpp
)
