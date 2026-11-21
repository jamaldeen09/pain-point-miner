cmake_minimum_required(VERSION 3.16)
project(PainPointMiner LANGUAGES CXX)

add_executable(pain_point_miner
    src/main.cpp
)

target_include_directories(pain_point_miner PRIVATE include)
target_compile_features(pain_point_miner PRIVATE cxx_std_17)