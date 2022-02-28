set(NAME httpsClientForTesting)

find_package(catch2)

add_executable(
        ${NAME}
        ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(
        ${NAME}
        Catch2::Catch2WithMain
        https_network
        ziapi
)

set_target_properties(
        ${NAME}
        PROPERTIES
        CXX_STANDARD 20
)

conan_target_link_libraries(
        ${NAME}
        CONAN_PKG::asio
        CONAN_PKG::openssl
)