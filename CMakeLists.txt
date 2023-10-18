cmake_minimum_required(VERSION 3.5)

project(channel)

add_library(channel STATIC src/channel.c)
target_include_directories(channel PUBLIC include)
