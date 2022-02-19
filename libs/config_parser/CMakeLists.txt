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

set_target_properties(
    config_parser
        PROPERTIES
        CXX_STANDARD 20
        LINKER_LANGUAGE CXX
)

target_link_libraries(
    config_parser
        ziapi
)

conan_target_link_libraries(
    config_parser
        CONAN_PKG::yaml-cpp
)
