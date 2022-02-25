find_package(catch2)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W3)
endif ()

add_executable(
        HttpParser_tests
        test_HttpParser.cpp
)

target_link_libraries(
        HttpParser_tests
        PRIVATE
        Catch2::Catch2WithMain
        ziaHttpParser
        ziapi
)
