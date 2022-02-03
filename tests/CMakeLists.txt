find_package(catch2)

add_executable(
    zia_tests
        ${CMAKE_CURRENT_LIST_DIR}/placeholder.cpp
)

target_link_libraries(
    zia_tests
        Catch2::Catch2WithMain
)
