cmake_minimum_required(VERSION 3.1...3.14)

set(VCPKG_ROOT $ENV{VCPKG_ROOT})
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE STRING ""
	)
	message(STATUS "Use VCPKG CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(CppSdl2
	DESCRIPTION
		"A C++ wrapper of SDL2 and OpenGl"
	LANGUAGES
		CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
endif ()

set(IMGUI_SOURCE
	src/sdl/imguishader.cpp
	src/sdl/imguishader.h
	src/sdl/imguiwindow.h
	src/sdl/imguiwindow.cpp
)

set(SOURCES
	src/sdl/batch.h
	src/sdl/color.h
	src/sdl/font.cpp
	src/sdl/font.h
	src/sdl/gamecontroller.cpp
	src/sdl/gamecontroller.h
	src/sdl/initsdl.cpp
	src/sdl/initsdl.h
	src/sdl/logger.cpp
	src/sdl/logger.h
	src/sdl/music.cpp
	src/sdl/music.h
	src/sdl/opengl.cpp
	src/sdl/opengl.h
	src/sdl/rect.h
	src/sdl/shaderprogram.cpp
	src/sdl/shaderprogram.h
	src/sdl/sound.cpp
	src/sdl/sound.h
	src/sdl/sprite.cpp
	src/sdl/sprite.h
	src/sdl/surface.cpp
	src/sdl/surface.h
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

find_package(GLEW CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2-image CONFIG REQUIRED)
find_package(SDL2-mixer CONFIG REQUIRED)
find_package(SDL2-ttf CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

message(STATUS "Add ImGui as dependencie: -DCppSdl2ImGui=1")
option(CppSdl2ImGui "Add ImGui as dependencie" OFF)
set(ImGuiLib "")
if (CppSdl2ImGui)
	find_package(imgui CONFIG REQUIRED)
	
	set(SOURCES "${SOURCES};${IMGUI_SOURCE}")
	set(ImGuiLib "imgui::imgui")
endif ()

add_library(CppSdl2 ${SOURCES})
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
		spdlog::spdlog spdlog::spdlog_header_only
		glm
		${ImGuiLib}
)

message(STATUS "CppSdl2Test is available to add: -DCppSdl2Test=1")
option(CppSdl2Test "Add CppSdl2Test project." OFF)

if (CppSdl2Test)
	set(IMGUI_SOURCE_TEST
		srcTest/testimguiwindow.cpp
		srcTest/testimguiwindow.h
	)

	set(SOURCES_TEST
		srcTest/batchtriangles.cpp
		srcTest/batchtriangles.h
		srcTest/batchtrianglesindexes.cpp
		srcTest/batchtrianglesindexes.h
		srcTest/logger.h
		srcTest/types.h
		srcTest/main.cpp
		srcTest/testshader.h
		srcTest/testshader.cpp
		srcTest/testwindow2.h
		srcTest/testwindow.cpp
		srcTest/testwindow.h
	)

	if (CppSdl2ImGui)
		set(SOURCES_TEST "${SOURCES_TEST};${IMGUI_SOURCE_TEST}")
		add_compile_definitions(IMGUI_LIB)
	endif ()
	
	add_executable(CppSdl2Test ${SOURCES_TEST})
	
	target_link_libraries(CppSdl2Test
		PRIVATE
			CppSdl2
	)
	
	file(COPY dataTest/. DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) # Copy data to build folder.
endif ()

find_package(Catch2 CONFIG)
if (Catch2_FOUND)
	message(STATUS "CppSdl2CatchTest is available to add: -DCppSdl2CatchTest=1")
	option(CppSdl2CatchTest "Add CppSdl2CatchTest project." OFF)

	if (CppSdl2CatchTest)
		set(SOURCES_TEST
			srcCatchTest/main.cpp
			srcCatchTest/speedtest.cpp
			srcCatchTest/tests.cpp
		)
	
		add_executable(CppSdl2CatchTest ${SOURCES_TEST})
		add_compile_definitions(CATCH_CONFIG_ENABLE_BENCHMARKING)

		target_link_libraries(CppSdl2CatchTest
			PRIVATE
				CppSdl2
				Catch2::Catch2
		)
	endif (CppSdl2CatchTest)
else (Catch2_FOUND)
	message(STATUS "Catch2 not found, CppSdl2CatchTest not created")
endif (Catch2_FOUND)
