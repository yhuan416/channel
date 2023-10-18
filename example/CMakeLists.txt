cmake_minimum_required(VERSION 3.5)

add_executable(example main.c)
target_link_libraries(example PRIVATE channel)
