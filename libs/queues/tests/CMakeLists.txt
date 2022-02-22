set(NAME queues_tests)

find_package(catch2)

#add_library(
#        mock_lib SHARED
#        ${CMAKE_CURRENT_LIST_DIR}/mockLib/code.cpp
#)

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

#target_link_libraries(
#        mock_lib
#        PRIVATE
#        loader
#)

#add_dependencies(loader_tests mock_lib)

#add_compile_definitions(MOCK_LIB_PATH="$<TARGET_FILE:mock_lib>")

set_target_properties(
        ${NAME}
        PROPERTIES
        CXX_STANDARD 20
)
