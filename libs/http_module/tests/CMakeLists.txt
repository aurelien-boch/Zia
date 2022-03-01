set(NAME httpModule_tests)

add_executable(
        ${NAME}
        ${CMAKE_CURRENT_LIST_DIR}/HttpModuleTests.cpp
)

target_link_libraries(
        ${NAME}
        Catch2::Catch2WithMain
        http_network
        http_module
        config_parser
)

set_target_properties(
        ${NAME}
        PROPERTIES
        CXX_STANDARD 20
)

conan_target_link_libraries(
        ${NAME}
        CONAN_PKG::asio
)
