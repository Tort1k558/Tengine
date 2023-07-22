cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME Tengine)

project(${PROJECT_NAME})

set(DIRS_SRC 
	"src/Components/*.h" "src/Components/*.cpp" 
	"src/Core/*.h" "src/Core/*.cpp" 
	"src/ECS/*.h" "src/ECS/*.cpp" 
	"src/Renderer/*.h" "src/Renderer/*.cpp"
	"src/Renderer/OpenGL/*.h" "src/Renderer/OpenGL/*.cpp"
	"src/Systems/*.h" "src/Systems/*.cpp"
	"src/Scene/*.h" "src/Scene/*.cpp"
	"src/Utils/*.h" "src/Utils/*.cpp"
	)
file(GLOB TARGET_SRC ${DIRS_SRC})

include_directories(src)
  
add_executable(${PROJECT_NAME} ${TARGET_SRC} ${STBI_SRC} main.cpp)

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

add_subdirectory(external/stbi ${CMAKE_CURRENT_BINARY_DIR}/external/stbi)
target_link_libraries(${PROJECT_NAME} PRIVATE stbi)

add_subdirectory(external/Assimp ${CMAKE_CURRENT_BINARY_DIR}/external/Assimp)
target_include_directories(${PROJECT_NAME} PRIVATE external/Assimp/include)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)