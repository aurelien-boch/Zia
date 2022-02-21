/*
** EPITECH PROJECT, 2021
** B-CPP-500-PAR-5-1-babel-aurelien.boch
** File description:
** Errors
*/

#ifndef UNIVERSALERRORS_HPP
#define UNIVERSALERRORS_HPP

#include <unordered_map>
#include <asio.hpp>
#include <utility>
#include <iostream>

namespace error {
    enum ErrorSocket {
        SOCKET_NO_ERROR,
        SOCKET_ACCESS_DENIED,
        SOCKET_ADDRESS_FAMILY_NOT_SUPPORTED,
        SOCKET_ADDRESS_IN_USE,
        SOCKET_ALREADY_CONNECTED,
        SOCKET_ALREADY_STARTED,
        SOCKET_BROKEN_PIPE,
        SOCKET_CONNECTION_ABORTED,
        SOCKET_CONNECTION_REFUSED,
        SOCKET_CONNECTION_RESET,
        SOCKET_BAD_DESCRIPTOR,
        SOCKET_FAULT,
        SOCKET_HOST_UNREACHABLE,
        SOCKET_IN_PROGRESS,
        SOCKET_INTERRUPTED,
        SOCKET_INVALID_ARGUMENT,
        SOCKET_MESSAGE_SIZE,
        SOCKET_NAME_TOO_LONG,
        SOCKET_NETWORK_DOWN,
        SOCKET_NETWORK_RESET,
        SOCKET_NETWORK_UNREACHABLE,
        SOCKET_NO_DESCRIPTORS,
        SOCKET_NO_BUFFER_SPACE,
        SOCKET_NO_MEMORY,
        SOCKET_NO_PERMISSION,
        SOCKET_NO_PROTOCOL_OPTION,
        SOCKET_NO_SUCH_DEVICE,
        SOCKET_NOT_CONNECTED,
        SOCKET_NOT_SOCKET,
        SOCKET_OPERATION_ABORTED,
        SOCKET_OPERATION_NOT_SUPPORTED,
        SOCKET_SHUT_DOWN,
        SOCKET_TIMED_OUT,
        SOCKET_TRY_AGAIN,
        SOCKET_WOULD_BLOCK,
        SOCKET_UNKNOWN_ERROR,
    };

    static const std::unordered_map<ErrorSocket, std::string_view> errorMessage{
        {SOCKET_NO_ERROR    , "No error"},
        {SOCKET_ACCESS_DENIED, "Permission denied"},
        {SOCKET_ADDRESS_FAMILY_NOT_SUPPORTED, "Address family not supported by protocol"},
        {SOCKET_ADDRESS_IN_USE, "Address already in use"},
        {SOCKET_ALREADY_CONNECTED, "Transport endpoint is already connected"},
        {SOCKET_ALREADY_STARTED, "Operation already in progress"},
        {SOCKET_BROKEN_PIPE, "Broken pipe"},
        {SOCKET_CONNECTION_ABORTED, "A connection has been aborted"},
        {SOCKET_CONNECTION_REFUSED, "Connection refused"},
        {SOCKET_CONNECTION_RESET, "Connection reset by peer"},
        {SOCKET_BAD_DESCRIPTOR, "Bad file descriptor"},
        {SOCKET_FAULT, "Bad address"},
        {SOCKET_HOST_UNREACHABLE, "No route to host"},
        {SOCKET_IN_PROGRESS, "Operation now in progress"},
        {SOCKET_INTERRUPTED, "Interrupted system call"},
        {SOCKET_INVALID_ARGUMENT, "Invalid argument"},
        {SOCKET_MESSAGE_SIZE, "Message too long"},
        {SOCKET_NAME_TOO_LONG, "The name was too long"},
        {SOCKET_NETWORK_DOWN, "Network is down"},
        {SOCKET_NETWORK_RESET, "Network dropped connection on reset"},
        {SOCKET_NETWORK_UNREACHABLE, "Network is unreachable"},
        {SOCKET_NO_DESCRIPTORS, "Too many open files"},
        {SOCKET_NO_BUFFER_SPACE, "No buffer space available"},
        {SOCKET_NO_MEMORY, "Cannot allocate memory"},
        {SOCKET_NO_PERMISSION, "Operation not permitted"},
        {SOCKET_NO_PROTOCOL_OPTION, "Protocol not available"},
        {SOCKET_NO_SUCH_DEVICE, "No such device"},
        {SOCKET_NOT_CONNECTED, "Transport endpoint is not connected"},
        {SOCKET_NOT_SOCKET, "Socket operation on non-socket"},
        {SOCKET_OPERATION_ABORTED, "Operation cancelled"},
        {SOCKET_OPERATION_NOT_SUPPORTED, "Operation not supported"},
        {SOCKET_SHUT_DOWN, "Cannot send after transport endpoint shutdown"},
        {SOCKET_TIMED_OUT, "Connection timed out"},
        {SOCKET_TRY_AGAIN, "Resource temporarily unavailable"},
        {SOCKET_WOULD_BLOCK, "The socket is marked non-blocking and the requested operation would block"},
        {SOCKET_UNKNOWN_ERROR, "The socket encountered an unknown error"},
    };

    static const std::unordered_map<asio::error_code, ErrorSocket> AsioErrorTranslator {
        {asio::error::access_denied, SOCKET_ACCESS_DENIED},
        {asio::error::address_family_not_supported, SOCKET_ADDRESS_FAMILY_NOT_SUPPORTED},
        {asio::error::address_in_use, SOCKET_ADDRESS_IN_USE},
        {asio::error::already_connected, SOCKET_ALREADY_CONNECTED},
        {asio::error::already_started, SOCKET_ALREADY_STARTED},
        {asio::error::broken_pipe, SOCKET_BROKEN_PIPE},
        {asio::error::connection_aborted, SOCKET_CONNECTION_ABORTED},
        {asio::error::connection_refused, SOCKET_CONNECTION_REFUSED},
        {asio::error::connection_reset, SOCKET_CONNECTION_RESET},
        {asio::error::bad_descriptor, SOCKET_BAD_DESCRIPTOR},
        {asio::error::fault, SOCKET_FAULT},
        {asio::error::host_unreachable, SOCKET_HOST_UNREACHABLE},
        {asio::error::in_progress, SOCKET_IN_PROGRESS},
        {asio::error::interrupted, SOCKET_INTERRUPTED},
        {asio::error::invalid_argument, SOCKET_INVALID_ARGUMENT},
        {asio::error::message_size, SOCKET_MESSAGE_SIZE},
        {asio::error::name_too_long, SOCKET_NAME_TOO_LONG},
        {asio::error::network_down, SOCKET_NETWORK_DOWN},
        {asio::error::network_reset, SOCKET_NETWORK_RESET},
        {asio::error::network_unreachable, SOCKET_NETWORK_UNREACHABLE},
        {asio::error::no_descriptors, SOCKET_NO_DESCRIPTORS},
        {asio::error::no_buffer_space, SOCKET_NO_BUFFER_SPACE},
        {asio::error::no_memory, SOCKET_NO_MEMORY},
        {asio::error::no_permission, SOCKET_NO_PERMISSION},
        {asio::error::no_protocol_option, SOCKET_NO_PROTOCOL_OPTION},
        {asio::error::no_such_device, SOCKET_NO_SUCH_DEVICE},
        {asio::error::not_connected, SOCKET_NOT_CONNECTED},
        {asio::error::not_socket, SOCKET_NOT_SOCKET},
        {asio::error::operation_aborted, SOCKET_OPERATION_ABORTED},
        {asio::error::operation_not_supported, SOCKET_OPERATION_NOT_SUPPORTED},
        {asio::error::shut_down, SOCKET_SHUT_DOWN},
        {asio::error::timed_out, SOCKET_TIMED_OUT},
        {asio::error::try_again, SOCKET_TRY_AGAIN},
        {asio::error::would_block, SOCKET_WOULD_BLOCK}
    };
}

#endif /* !UNIVERSALERRORS_HPP_ */
