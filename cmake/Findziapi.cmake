if (NOT ziapi_found)
    include(ExternalProject)
    ExternalProject_Add(
            ziapi_git
            GIT_REPOSITORY  https://github.com/martin-olivier/ZiAPI.git
            GIT_TAG         v3.1.1
            INSTALL_COMMAND ""
            TEST_COMMAND    ""
    )
    add_library(ziapi INTERFACE)

    add_dependencies(ziapi INTERFACE ziapi_git)
    ExternalProject_Get_Property(ziapi_git SOURCE_DIR)
    target_include_directories(ziapi INTERFACE "${SOURCE_DIR}/include")
    set(ziapi_found)
endif()
