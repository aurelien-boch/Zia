if (NOT yaml-cpp_found)
    include(FetchContent)

    FetchContent_Declare(
            yaml-cpp
            GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
            GIT_TAG        yaml-cpp-0.7.0
    )
    message(STATUS "Pardon Paul")
    message(STATUS "Cloning yaml-cpp")
    FetchContent_MakeAvailable(yaml-cpp)

    if (UNIX)
        target_compile_options(
            yaml-cpp PRIVATE
                "-fpic"
        )
    endif()

    set(yaml-cpp_found)
endif()
