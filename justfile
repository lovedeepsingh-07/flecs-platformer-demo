build_dir := "build"
binary_name := "game"
src_dir := "src"
include_dir := "include"
cmake_toolchain_file := "mingw-w64-toolchain.cmake"
cmake_build_type := "Release"
export EM_CACHE := env_var('HOME') + '/.cache/emscripten'

default:
	just -l

init:
	git submodule update --init --recursive

build:
	mkdir -p {{build_dir}}/windows
	cmake -S . -B {{build_dir}}/windows -DCMAKE_TOOLCHAIN_FILE={{cmake_toolchain_file}} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE={{cmake_build_type}} -DCMAKE_SYSTEM_NAME=Windows
	cmake --build ./{{build_dir}}/windows
	cp {{build_dir}}/windows/compile_commands.json {{build_dir}}/compile_commands.json
run: build
	./{{build_dir}}/windows/{{binary_name}}.exe

build-web:
	mkdir -p {{build_dir}}/web
	emcmake cmake -S . -B {{build_dir}}/web -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE={{cmake_build_type}}
	cmake --build ./{{build_dir}}/web
	cp {{build_dir}}/web/compile_commands.json {{build_dir}}/compile_commands.json
run-web: build-web
	live-server .

fmt:
    find {{src_dir}} {{include_dir}} -regex '.*\.\(cpp\|hpp\)' | xargs clang-format -i
