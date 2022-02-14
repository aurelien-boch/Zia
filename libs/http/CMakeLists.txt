project(http_module LANGUAGES CXX)

set(NAME http)

add_subdirectory(src/)
add_subdirectory(include/)

add_library(
        ${NAME}
        SHARED
            ${SRCS}
            ${INCLUDES}
)

target_include_directories(
        ${NAME}
            PUBLIC
            include/
)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(COMPILE_FLAGS  "/W3")
elseif (
(CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR
(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)
    set(COMPILE_FLAGS -Wall -Wextra -Weffc++)
endif()

target_compile_options(
        ${NAME}
        PRIVATE
        ${COMPILE_FLAGS}
)

conan_target_link_libraries(
        ${NAME}
        CONAN_PKG::asio
)

target_link_libraries(
        ${NAME}
        ziapi
        network
)

#add_subdirectory(include/)
#add_subdirectory(src/)
#
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)
#
#add_library(
#        http
#        ${INCLUDES}
#        ${SRCS}
#)
#
#target_include_directories(
#        http
#        PUBLIC
#        include/
#)
#
##set_target_properties(http PROPERTIES LINKER_LANGUAGE CXX)
#
#conan_target_link_libraries(
#        http
#        ziapi
#        network
#)