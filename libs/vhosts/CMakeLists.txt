add_library(
    vhosts SHARED
        include/Vhosts.hpp
        include/Converter.hpp
        src/Vhosts.cpp
        src/Converter.cpp
        entrypoint.cpp
)

target_include_directories(
    vhosts PRIVATE
        include/
)


conan_target_link_libraries(
    vhosts
        CONAN_PKG::asio
)

target_link_libraries(
    vhosts
        ziapi
)
