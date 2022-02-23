set(NAME asioHttpClient_tests)

find_package(catch2)

add_executable(
        ${NAME}
        ${CMAKE_CURRENT_LIST_DIR}/AsioHttpClientTests.cpp
)

target_link_libraries(
        ${NAME}
        Catch2::Catch2WithMain
        http_network
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