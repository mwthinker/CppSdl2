cmake_minimum_required(VERSION 3.16)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
if (DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE STRING ""
	)
	message(STATUS "Use VCPKG CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
endif ()

project(CppSdl2
	DESCRIPTION
		"A C++-Wrapper of SDL2 and OpenGL"
	LANGUAGES
		CXX
)

find_package(glad CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2-image CONFIG REQUIRED)
find_package(SDL2-mixer CONFIG REQUIRED)
find_package(SDL2-ttf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(CppSdl2 
	src/sdl/batch.h
	src/sdl/color.cpp
	src/sdl/color.h
	src/sdl/framebuffer.cpp
	src/sdl/framebuffer.h
	src/sdl/font.cpp
	src/sdl/font.h
	src/sdl/gamecontroller.cpp
	src/sdl/gamecontroller.h
	src/sdl/graphic.cpp
	src/sdl/graphic.h
	src/sdl/imguiauxiliary.cpp
	src/sdl/imguiauxiliary.h
	src/sdl/imguiwindow.h
	src/sdl/imguiwindow.cpp
	src/sdl/initsdl.cpp
	src/sdl/initsdl.h
	src/sdl/music.cpp
	src/sdl/music.h
	src/sdl/opengl.cpp
	src/sdl/opengl.h
	src/sdl/rect.h
	src/sdl/shader.cpp
	src/sdl/shader.h
	src/sdl/shaderprogram.cpp
	src/sdl/shaderprogram.h
	src/sdl/sound.cpp
	src/sdl/sound.h
	src/sdl/sprite.cpp
	src/sdl/sprite.h
	src/sdl/surface.cpp
	src/sdl/surface.h
	src/sdl/textureatlas.cpp
	src/sdl/textureatlas.h
	src/sdl/texture.cpp
	src/sdl/texture.h
	src/sdl/textureview.cpp
	src/sdl/textureview.h
	src/sdl/vertex.h
	src/sdl/vertexarrayobject.cpp
	src/sdl/vertexarrayobject.h
	src/sdl/vertexbufferobject.cpp
	src/sdl/vertexbufferobject.h
	src/sdl/window.cpp
	src/sdl/window.h
	vcpkg.json
)

add_subdirectory(ImGui)

if (MSVC)
	target_compile_options(CppSdl2
		PRIVATE
			/W3 /WX /permissive-
	)
else()
	target_compile_options(CppSdl2
		PRIVATE
			-Wall -Wextra -Wnon-virtual-dtor -pedantic -Wcast-align -Woverloaded-virtual -Wno-unused-parameter
	)
endif()

target_compile_definitions(CppSdl2
	PUBLIC
		NOMINMAX
)

target_include_directories(CppSdl2
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

target_link_libraries(CppSdl2
	PUBLIC
		ImGui
		SDL2::SDL2_image
		SDL2::SDL2_ttf
		SDL2::SDL2_mixer
		spdlog::spdlog spdlog::spdlog_header_only
)

set_target_properties(CppSdl2
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

message(STATUS "CppSdl2_Example is available to add: -DCppSdl2_Example=1")
option(CppSdl2_Example "Add CppSdl2_Example project." OFF)
if (CppSdl2_Example)
	add_subdirectory(CppSdl2_Example)
endif ()

message(STATUS "CppSdl2_Test is available to add: -DCppSdl2_Test=1")
option(CppSdl2_Test "Add CppSdl2_Test to project." OFF)
if (CppSdl2_Test)
	add_subdirectory(CppSdl2_Test)
endif ()
