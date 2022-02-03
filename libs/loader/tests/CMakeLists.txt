find_package(catch2)

add_library(
    mock_lib SHARED
        ${CMAKE_CURRENT_LIST_DIR}/mockLib/code.cpp
)

add_executable(
    loader_tests
        ${CMAKE_CURRENT_LIST_DIR}/loader.cpp
)

target_link_libraries(
    loader_tests
        Catch2::Catch2WithMain
        loader
)

add_dependencies(loader_tests mock_lib)

add_compile_definitions(MOCK_LIB_PATH="$<TARGET_FILE:mock_lib>")

set_target_properties(
    loader_tests
        PROPERTIES
        CXX_STANDARD 20
)

target_compile_options(
    loader_tests
        PRIVATE
        "/utf-8"
)
