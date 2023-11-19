cmake_minimum_required(VERSION 3.2)

project(imguizmo)

set(IMGUIZMO_INCLUDES 
	imguizmo/ImGuizmo.h
)
set(IMGUIZMO_SRC
	imguizmo/ImGuizmo.cpp
)
add_library(imguizmo STATIC ${IMGUIZMO_INCLUDES} ${IMGUIZMO_SRC})

include_directories(../imgui/imgui)
