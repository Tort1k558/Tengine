cmake_minimum_required(VERSION 3.2)

project(imgui)

set(IMGUI_INCLUDES 
	imgui/imgui.h
	imgui/backends/imgui_impl_glfw.h
	imgui/backends/imgui_impl_opengl3.h
	imgui/misc/cpp/imgui_stdlib.h
)
set(IMGUI_SRC
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui_draw.cpp
	imgui/backends/imgui_impl_glfw.cpp
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/misc/cpp/imgui_stdlib.cpp
)
add_library(imgui STATIC ${IMGUI_INCLUDES} ${IMGUI_SRC})

target_include_directories(imgui PUBLIC imgui)
target_include_directories(imgui PUBLIC /)
target_link_libraries(imgui PRIVATE glfw)
