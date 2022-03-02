add_library(
    cgi SHARED
        include/Cgi.hpp
        src/Cgi.cpp
        entrypoint.cpp
)

target_include_directories(
    cgi
        PRIVATE
        include/
)

target_link_libraries(
    cgi
        loader
        ziapi
        env_manager
        executor
)
