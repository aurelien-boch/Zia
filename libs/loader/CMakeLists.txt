add_library(loader INTERFACE)

if (DEFINED ENABLE_TESTING AND "${ENABLE_TESTING}" STREQUAL "yes")
    message(STATUS "ENABLE_TESTING=yes detected, enabling testing")
    add_subdirectory(tests)
endif()

target_sources(
    loader INTERFACE
        include/Loader.hpp
)

target_include_directories(
    loader INTERFACE
        include/
)

if (UNIX)
    target_link_libraries(
        loader
            INTERFACE
            dl
    )
endif()
