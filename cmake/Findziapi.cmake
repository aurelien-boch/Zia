if (NOT ziapi_found)
    include(FetchContent)
    FetchContent_Declare(
            ziapi_git
            GIT_REPOSITORY  https://github.com/martin-olivier/ZiAPI.git
            GIT_TAG         v4.0.0
            INSTALL_COMMAND ""
            TEST_COMMAND    ""
    )

    FetchContent_MakeAvailable(ziapi_git)
    add_library(ziapi INTERFACE)
    add_dependencies(ziapi PUBLIC ziapi_git)
    target_include_directories(ziapi INTERFACE "${ziapi_SOURCE_DIR}/include")
    set(ziapi_found)
endif()
