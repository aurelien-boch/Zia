find_package(catch2)

set(LINKS_LIBS Catch2::Catch2WithMain config_parser)
set(COMPILE_OPTIONS)

if (DEFINED ENABLE_COVERAGE AND "${ENABLE_COVERAGE}" STREQUAL "yes")
    message(STATUS "ENABLE_COVERAGE=yes detected, enabling coverage")
    list(APPEND LINKS_LIBS gcov)
    list(APPEND COMPILE_OPTIONS --coverage)
endif()

add_executable(
    config_parser_tests
        ${CMAKE_CURRENT_LIST_DIR}/testsConfigParser.cpp
)

target_link_libraries(
    config_parser_tests
        ${LINKS_LIBS}
)

target_compile_options(
    config_parser_tests
        PRIVATE
        ${COMPILE_OPTIONS}
)

set_target_properties(
    config_parser_tests
        PROPERTIES
        CXX_STANDARD 20
)