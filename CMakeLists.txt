cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME Tengine)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_DIRS 
	"src/Core/*.h" "src/Core/*.cpp" 
	"src/Renderer/*.h" "src/Renderer/*.cpp"
	"src/Renderer/OpenGL/*.h" "src/Renderer/OpenGL/*.cpp"
	)
file(GLOB TARGET_SRC ${SOURCE_DIRS})

include_directories(src)
  
add_executable(${PROJECT_NAME} ${TARGET_SRC} main.cpp)

add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/external/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad ${CMAKE_CURRENT_BINARY_DIR}/external/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/external/spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(external/imgui ${CMAKE_CURRENT_BINARY_DIR}/external/imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

add_subdirectory(external/glm ${CMAKE_CURRENT_BINARY_DIR}/external/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)