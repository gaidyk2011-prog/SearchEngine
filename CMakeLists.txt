cmake_minimum_required(VERSION 3.14)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)
#
#set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)
#set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
#
#FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(search_engine main.cpp
        JsonConverter.h
        JsonConverter.cpp
        InvertedIndex.h
        InvertedIndex.cpp
        SearchServer.h
        SearchServer.cpp)

target_link_libraries(search_engine
        PRIVATE
        nlohmann_json::nlohmann_json
        #gtest_main
)

#include(GoogleTest)
#gtest_discover_tests(search_engine)