add_subdirectory(include/)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(
    network
        ${INCLUDES}
)

target_include_directories(
    network
        PUBLIC
        include/
)

set_target_properties(network PROPERTIES LINKER_LANGUAGE CXX)

conan_target_link_libraries(
    network
        CONAN_PKG::asio
)
