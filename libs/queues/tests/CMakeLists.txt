set(NAME queues_tests)

find_package(catch2)

add_executable(
        ${NAME}
        ${CMAKE_CURRENT_LIST_DIR}/RequestOutputQueueTests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/ResponseInputQueueTests.cpp
)

target_link_libraries(
        ${NAME}
        Catch2::Catch2WithMain
        queues
)

set_target_properties(
        ${NAME}
        PROPERTIES
        CXX_STANDARD 20
)
