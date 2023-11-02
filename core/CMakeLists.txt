cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME TengineCore)

project(${PROJECT_NAME})

set(DIRS_SRC 	
	"Core/*.cpp"                
	"ECS/*.cpp"                 
	"Components/*.cpp"      	
	"Systems/*.cpp"     		
	"Renderer/*.cpp"            
	"Renderer/OpenGL/*.cpp"
	"Renderer/Shaders/*.cpp"	
	"Renderer/Shaders/GLSL/*.cpp"          
	"Scene/*.cpp"               
	"Utils/*.cpp"               
	"Utils/EditorElements/*.cpp"       
	)
	set(DIRS_INCLUDES	
	"Core/*.h"			
	"ECS/*.h" 			
	"Components/*.h" 			
	"Systems/*.h" 			
	"Renderer/*.h"		
	"Renderer/OpenGL/*.h"
	"Renderer/Shaders/*.h"	
	"Renderer/Shaders/GLSL/*.h"	
	"Scene/*.h" 		
	"Utils/*.h"          
	"Utils/EditorElements/*.h"          
	)
file(GLOB TARGET_SRC ${DIRS_SRC})
file(GLOB TARGET_INCLUDES ${DIRS_INCLUDES})

add_library(${PROJECT_NAME} SHARED ${TARGET_SRC} ${TARGET_INCLUDES})

target_compile_options(${PROJECT_NAME} PRIVATE /wd4251 /wd4996)

target_include_directories(${PROJECT_NAME} PUBLIC "../core")

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/bin/glfw)
add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/bin/glad)
add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/bin/spdlog)
add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/bin/glm)
add_subdirectory(../external/stbi ${CMAKE_CURRENT_BINARY_DIR}/bin/stbi)
add_subdirectory(../external/Assimp ${CMAKE_CURRENT_BINARY_DIR}/bin/Assimp)
target_include_directories(${PROJECT_NAME} PUBLIC ../external/Assimp/include)

add_subdirectory(../external/nlohmann ${CMAKE_CURRENT_BINARY_DIR}/bin/nlohmann)

target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw spdlog glm stbi assimp nlohmann_json)