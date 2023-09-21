cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME TengineEditor)

set(DIRS_SRC_EDITOR 
	"src/*.h" "src/*.cpp" 
	"src/Scripts/*.h" "src/Scripts/*.cpp" 
	)
file(GLOB TARGET_SRC_EDITOR ${DIRS_SRC_EDITOR})

include_directories(src)
add_subdirectory(../external/nfd ${CMAKE_CURRENT_BINARY_DIR}/bin/nfd)

add_executable(${PROJECT_NAME} ${TARGET_SRC_EDITOR} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE /wd4251)

target_link_libraries(${PROJECT_NAME} TengineCore glm stbi imgui spdlog assimp glfw nfd nlohmann_json)