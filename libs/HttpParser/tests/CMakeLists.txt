set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

add_executable(
        HttpParser_tests
        test_HttpParser.cpp
)

target_link_libraries(
        HttpParser_tests
        PRIVATE
        Catch2::Catch2WithMain
        http_parser
        ziapi
)
