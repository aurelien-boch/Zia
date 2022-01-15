if (NOT Catch2_found)
    include("FetchContent")

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.0.0-preview3
    )
    FetchContent_MakeAvailable(Catch2)
    set(Catch2_found)
endif()

add_executable(
    zia_tests
        ${CMAKE_CURRENT_LIST_DIR}/placeholder.cpp
)

target_link_libraries(
    zia_tests
        Catch2::Catch2WithMain
)
