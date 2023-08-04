cmake_minimum_required (VERSION 2.8.7)
project (reflang)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR})

include_directories(../clang/include)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(libreflang STATIC lib/reflang.hpp lib/reflang.cpp)

link_directories(../clang/lib)

add_executable(
	reflang
	src/main.cpp
	src/cmdargs.cpp 			src/cmdargs.hpp
	src/parser.class.cpp		src/parser.class.hpp
	src/parser.cpp 				src/parser.hpp
	src/parser.enum.cpp			src/parser.enum.hpp
	src/parser.function.cpp		src/parser.function.hpp
	src/parser.util.cpp			src/parser.util.hpp
	src/serializer.class.cpp	src/serializer.class.hpp
	src/serializer.cpp 			src/serializer.hpp
	src/serializer.enum.cpp		src/serializer.enum.hpp
	src/serializer.function.cpp src/serializer.function.hpp
	src/serializer.util.cpp		src/serializer.util.hpp
	src/types.cpp				src/types.hpp)
	
target_link_libraries(reflang  libreflang clangAST clangASTMatchers clangBasic clangLex clangParse libclang)