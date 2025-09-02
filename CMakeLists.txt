set (PROJECT_NAME game)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

cmake_minimum_required(VERSION 3.15)
project(${PROJECT_NAME})

set(EXTERNAL_DIR external)
add_subdirectory(${EXTERNAL_DIR}/raylib)
add_subdirectory(${EXTERNAL_DIR}/flecs)
add_subdirectory(${EXTERNAL_DIR}/box2d)
add_subdirectory(${EXTERNAL_DIR}/pugixml)
add_subdirectory(${EXTERNAL_DIR}/rapidcsv)
add_subdirectory(${EXTERNAL_DIR}/expected)
include(${CMAKE_CURRENT_LIST_DIR}/lua.cmake)

set(IMGUI_SOURCES
    ${EXTERNAL_DIR}/imgui/imgui.cpp
    ${EXTERNAL_DIR}/imgui/imgui_draw.cpp
    ${EXTERNAL_DIR}/imgui/imgui_demo.cpp
    ${EXTERNAL_DIR}/imgui/imgui_tables.cpp
    ${EXTERNAL_DIR}/imgui/imgui_widgets.cpp
)
set(RLIMGUI_SOURCES
	${EXTERNAL_DIR}/rlimgui/rlImGui.cpp
)
include_directories(
	${EXTERNAL_DIR}/imgui
	${EXTERNAL_DIR}/rlimgui
)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${IMGUI_SOURCES} ${RLIMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    ${EXTERNAL_DIR}/clay
    ${EXTERNAL_DIR}/raygui/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	raylib
	flecs::flecs_static
	box2d
	pugixml
	rapidcsv
	lua
	expected
)

target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP)
