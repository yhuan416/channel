cmake_minimum_required(VERSION 3.5)

project(channel)

# lock_free
file(GLOB_RECURSE src_files src/*.c)
set(src ${src_files})

# posix
file(GLOB_RECURSE src_posix_files src_posix/*.c)

# build library
add_library(channel STATIC ${src})
target_include_directories(channel PUBLIC include)
target_link_libraries(channel PUBLIC pthread rt)

add_subdirectory(example)
