if (NOT Catch2_found)
    include(FetchContent)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.0.0-preview4
    )
    message(STATUS "Cloning Catch2")
    FetchContent_MakeAvailable(Catch2)
    set(Catch2_found)
endif()
