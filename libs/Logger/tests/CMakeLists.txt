find_package(catch2)

set(CMAKE_CXX_STANDARD 20)

if (UNIX)
    add_compile_options(-Wall -Wextra -Weffc++)
else ()
    add_compile_options(/W4)
endif ()

add_executable(
        Logger_tests
        test_Logger.cpp
)

target_link_libraries(
        Logger_tests
        PRIVATE
        Catch2::Catch2WithMain
        Logger
        ziapi
)