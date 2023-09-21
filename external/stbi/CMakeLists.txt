cmake_minimum_required(VERSION 3.2)

project(stbi)

set(STBI_INCLUDES 
	stb_image.h
)
set(STBI_SRC
	stb_image.cpp
)

add_library(stbi STATIC ${STBI_INCLUDES} ${STBI_SRC})

target_include_directories(stbi PUBLIC /)
