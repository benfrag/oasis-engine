cmake_minimum_required(VERSION 3.10)
project(Renderer VERSION 1.0 LANGUAGES CXX)

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

file(GLOB_RECURSE SOURCES
	src/*.cpp
	lib/*.cpp)

add_executable(renderer WIN32 ${SOURCES})
target_include_directories(renderer PRIVATE include)
