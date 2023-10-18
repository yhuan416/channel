cmake_minimum_required(VERSION 3.5)

project(channel)

add_library(channel STATIC src/channel_posix.c)
target_include_directories(channel PUBLIC include)
target_link_libraries(channel PUBLIC pthread rt)

add_subdirectory(example)
