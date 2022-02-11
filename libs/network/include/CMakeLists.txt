set(
    SOCKET
        ${CMAKE_CURRENT_LIST_DIR}/socket/tcp/ITCPListener.hpp
        ${CMAKE_CURRENT_LIST_DIR}/socket/tcp/ITCPClient.hpp
        ${CMAKE_CURRENT_LIST_DIR}/socket/tcp/AsioTCPClient.hpp
        ${CMAKE_CURRENT_LIST_DIR}/socket/tcp/AsioTCPListener.hpp
)

set(
    INCLUDES
        ${SOCKET}
        ${CMAKE_CURRENT_LIST_DIR}/socket/Utility.hpp
        ${CMAKE_CURRENT_LIST_DIR}/socket/Errors.hpp
        PARENT_SCOPE
)
