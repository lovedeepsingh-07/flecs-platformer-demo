cmake_minimum_required(VERSION 3.15)
project(physics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_library(physics ${SRC_FILES})

target_include_directories(physics PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
