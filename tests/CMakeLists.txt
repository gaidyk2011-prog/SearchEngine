
enable_testing()

add_executable(unit_tests
        index_test.cpp
        server_test.cpp
        ../src/InvertedIndex.cpp
        ../src/SearchServer.cpp
        ../src/JsonConverter.cpp
)


target_include_directories(unit_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
)


target_link_libraries(unit_tests PRIVATE
        gtest_main
        nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(unit_tests)