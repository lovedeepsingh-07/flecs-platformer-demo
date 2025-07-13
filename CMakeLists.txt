set (PROJECT_NAME game)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME})

set(THIRD_PARTY_DIR external)
add_subdirectory(${THIRD_PARTY_DIR}/raylib)
add_subdirectory(${THIRD_PARTY_DIR}/ldtk_loader)
add_subdirectory(${THIRD_PARTY_DIR}/flecs)

file(GLOB_RECURSE SRC_FILES src/*.cc)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${THIRD_PARTY_DIR}/clay
    ${THIRD_PARTY_DIR}/raygui/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	raylib
	LDtkLoader::LDtkLoader
	flecs::flecs_static
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP)
