cmake_policy(VERSION 3.2)

project(glad)

add_library(glad STATIC src/glad.c include/glad/glad.h)
target_include_directories(glad PUBLIC include)
