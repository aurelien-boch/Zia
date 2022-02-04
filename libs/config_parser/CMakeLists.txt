add_library(config_parser SHARED
    ConfigParser.hpp
    ConfigParser.cpp
)

target_include_directories(
    config_parser PUBLIC
        include/
)

set_target_properties(
    config_parser
        PROPERTIES
        CXX_STANDARD 20
)

conan_target_link_libraries(config_parser CONAN_PKG::yaml-cpp)