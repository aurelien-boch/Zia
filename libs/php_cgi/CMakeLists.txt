add_library(
    php_cgi
        include/Cgi.hpp
        src/Cgi.cpp
        entrypoint.cpp
)

target_include_directories(
    php_cgi
        PRIVATE
        include/
)

target_link_libraries(
    php_cgi
        loader
        ziapi
        env_manager
        executor
)
