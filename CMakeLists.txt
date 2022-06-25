cmake_minimum_required(VERSION 3.22)
project(stream)

set(CMAKE_CXX_STANDARD 23)
include_directories(stream SDL2)

add_executable(stream main.cpp)
target_link_libraries(stream SDL2 SDL2_mixer)
