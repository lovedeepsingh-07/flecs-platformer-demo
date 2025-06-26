cmake_minimum_required(VERSION 3.15)
project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

set(THIRD_PARTY_DIR external)
add_subdirectory(${THIRD_PARTY_DIR}/raylib)
add_subdirectory(${THIRD_PARTY_DIR}/box2d)
add_subdirectory(${THIRD_PARTY_DIR}/ldtk_loader)
add_subdirectory(${THIRD_PARTY_DIR}/flecs)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(game ${SRC_FILES})

target_include_directories(game PRIVATE
    include
    ${THIRD_PARTY_DIR}/clay
    ${THIRD_PARTY_DIR}/raygui/src
)

target_link_libraries(game PRIVATE raylib)
target_link_libraries(game PRIVATE box2d)
target_link_libraries(game PRIVATE LDtkLoader::LDtkLoader)
target_link_libraries(game PRIVATE flecs::flecs_static)

target_compile_definitions(game PRIVATE PLATFORM_DESKTOP)
