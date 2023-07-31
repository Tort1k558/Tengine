cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME_EDITOR TengineEditor)

set(DIRS_SRC_EDITOR 
	"src/*.h" "src/*.cpp" 
	)
file(GLOB TARGET_SRC_EDITOR ${DIRS_SRC_EDITOR})

include_directories(src)
add_subdirectory(../external/nfd ${CMAKE_CURRENT_BINARY_DIR}/bin/nfd)

add_executable(${PROJECT_NAME_EDITOR} ${TARGET_SRC_EDITOR} main.cpp)

target_link_libraries(${PROJECT_NAME_EDITOR} TengineCore glm imgui spdlog assimp glfw nfd)