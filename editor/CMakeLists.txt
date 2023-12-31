cmake_policy(VERSION 3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME TengineEditor)

set(DIRS_SRC_EDITOR 
	"src/*.h" "src/*.cpp" 
	"src/Scripts/*.h" "src/Scripts/*.cpp"
	"src/Windows/*.h" "src/Windows/*.cpp"
	)
file(GLOB TARGET_SRC_EDITOR ${DIRS_SRC_EDITOR})

include_directories(src)
include_directories(../external/glfw/include)
include_directories(../external/Assimp/include)
include_directories(../external/spdlog/include)
include_directories(../external/nlohmann/include)
include_directories(../external/stbi)
include_directories(../external/glm)
include_directories(../external/imguizmo)


add_subdirectory(../external/nfd ${CMAKE_CURRENT_BINARY_DIR}/bin/nfd)
add_subdirectory(../external/imgui ${CMAKE_CURRENT_BINARY_DIR}/bin/imgui)
add_subdirectory(../external/imguizmo ${CMAKE_CURRENT_BINARY_DIR}/bin/imguizmo)
add_subdirectory(../core ${CMAKE_CURRENT_BINARY_DIR}/bin/core)

add_executable(${PROJECT_NAME} ${TARGET_SRC_EDITOR} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE /wd4251 /wd4996)

target_link_libraries(${PROJECT_NAME} TengineCore imgui imguizmo nfd)