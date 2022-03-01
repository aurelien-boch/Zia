set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W3)
endif ()

add_executable(
        HttpFormatter_tests
        test_HttpFormatter.cpp
)

target_link_libraries(
        HttpFormatter_tests
        PRIVATE
        Catch2::Catch2WithMain
        HttpFormatter
        ziapi
)
