cmake_minimum_required(VERSION 3.16)
project(PainPointMiner LANGUAGES CXX)

add_executable(pain_point_miner
    src/main.cpp
    src/http_client.cpp
)

find_package(CURL REQUIRED)

target_include_directories(pain_point_miner PRIVATE include)
target_compile_features(pain_point_miner PRIVATE cxx_std_17)
target_link_libraries(pain_point_miner PRIVATE CURL::libcurl)