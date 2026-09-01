
add_executable(search_engine
        main.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        JsonConverter.cpp
)


target_include_directories(search_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)