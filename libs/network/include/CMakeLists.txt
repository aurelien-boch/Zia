add_subdirectory(http)

set(
    INCLUDES
        ${HTTP_INCLUDES}
        ${CMAKE_CURRENT_SOURCE_DIR}/ITCPListener.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ITCPClient.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Utility.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Errors.hpp
        PARENT_SCOPE
)
