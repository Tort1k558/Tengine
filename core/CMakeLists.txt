cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME TengineCore)

project(${PROJECT_NAME})

set(DIRS_SRC 	
	"Core/*.cpp"                "Core/*.h"			
	"ECS/*.cpp"                 "ECS/*.h" 			
	"ECS/Components/*.cpp"      "ECS/Components/*.h" 			
	"ECS/Systems/*.cpp"      "ECS/Systems/*.h" 			
	"Renderer/*.cpp"            "Renderer/*.h"		
	"Renderer/OpenGL/*.cpp"     "Renderer/OpenGL/*.h"		
	"Scene/*.cpp"               "Scene/*.h" 		
	"Utils/*.cpp"               "Utils/*.h"          
	)
file(GLOB TARGET_SRC ${DIRS_SRC})

add_library(${PROJECT_NAME} STATIC ${TARGET_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC "../core")

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/bin/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/bin/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/bin/spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/imgui ${CMAKE_CURRENT_BINARY_DIR}/bin/imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/bin/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

add_subdirectory(../external/stbi ${CMAKE_CURRENT_BINARY_DIR}/bin/stbi)
target_link_libraries(${PROJECT_NAME} PRIVATE stbi)

add_subdirectory(../external/Assimp ${CMAKE_CURRENT_BINARY_DIR}/bin/Assimp)
target_include_directories(${PROJECT_NAME} PUBLIC ../external/Assimp/include)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)