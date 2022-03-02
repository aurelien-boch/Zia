add_library(
    env_manager SHARED
        include/EnvManager.hpp
        include/VariableNotFoundException.hpp
        include/VariableAlreadyDefinedException.hpp
        src/EnvManager.cpp
)

target_include_directories(
    env_manager PUBLIC
        include
)

set_target_properties(
    env_manager PROPERTIES
        CXX_STANDARD 20
)
