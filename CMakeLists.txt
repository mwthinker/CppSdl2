cmake_minimum_required(VERSION 3.1...3.14)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE STRING ""
	)
	message(STATUS "Use VCPKG CMAKE_TOOLCHAIN_FILE")
endif()

project(CppSdl2
	DESCRIPTION
		"A C++ wrapper of SDL2 and OpenGl"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
	src/sdl/batch.h
	src/sdl/font.cpp
	src/sdl/font.h
	src/sdl/gamecontroller.cpp
	src/sdl/gamecontroller.h
	src/sdl/initsdl.cpp
	src/sdl/initsdl.h
	src/sdl/logger.h
	src/sdl/music.cpp
	src/sdl/music.h
	src/sdl/opengl.cpp
	src/sdl/opengl.h
	src/sdl/shaderprogram.cpp
	src/sdl/shaderprogram.h
	src/sdl/sound.cpp
	src/sdl/sound.h
	src/sdl/sprite.cpp
	src/sdl/sprite.h
	src/sdl/text.cpp
	src/sdl/text.h
	src/sdl/textureatlas.cpp
	src/sdl/textureatlas.h
	src/sdl/texture.cpp
	src/sdl/texture.h
	src/sdl/vertexarrayobject.cpp
	src/sdl/vertexarrayobject.h
	src/sdl/vertexbufferobject.cpp
	src/sdl/vertexbufferobject.h
	src/sdl/window.cpp
	src/sdl/window.h
)

add_library(CppSdl2 ${SOURCES})

find_package(GLEW REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2-image CONFIG REQUIRED)
find_package(SDL2-mixer CONFIG REQUIRED)
find_package(SDL2-ttf CONFIG REQUIRED)

find_package(spdlog CONFIG REQUIRED)
find_package(OpenGL REQUIRED)


target_include_directories(CppSdl2
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)

target_link_libraries(CppSdl2
	PUBLIC
		SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static
		SDL2::SDL2_image
		SDL2::SDL2_ttf
		SDL2::SDL2_mixer
		GLEW::GLEW
		${OPENGL_LIBRARIES}
		spdlog::spdlog
)

message(STATUS "CppSdl2Test is available to add: -DCppSdl2Test=1")
option(CppSdl2Test "Add CppSdl2Test project." OFF)

if (CppSdl2Test)
	find_package(glm CONFIG REQUIRED)

	set(SOURCES_TEST
		srcTest/batchtriangles.h
		srcTest/types.h
		srcTest/main.cpp
		srcTest/testshader.cpp
		srcTest/testshader.h
		srcTest/testshader2.h
		srcTest/testshader2.cpp
		srcTest/testwindow2.h
		srcTest/testwindow.cpp
		srcTest/testwindow.h
	)
	
	add_executable(CppSdl2Test ${SOURCES_TEST})

	target_link_libraries(CppSdl2Test
		PRIVATE
			CppSdl2
			glm
	)
	
	file(COPY dataTest/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
endif ()
