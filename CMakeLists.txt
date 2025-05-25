cmake_minimum_required(VERSION 3.29)
project(CppSdl2
	DESCRIPTION
		"A C++-Wrapper of SDL2 and OpenGL"
	LANGUAGES
		CXX
	VERSION 0.1.2
)

add_library(CppSdl2 STATIC
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
	CMakePresets.json
	cppsdl2.natstepfilter
)
add_library(CppSdl2::CppSdl2 ALIAS CppSdl2)

add_subdirectory(ImGui)
set_property(GLOBAL PROPERTY USE_FOLDERS On)
set_target_properties(
	ImGui

	PROPERTIES FOLDER
		ExternalDependencies
)

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
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:include/cppsdl2>
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(CppSdl2
	PUBLIC
		CppSdl2::ImGui
		spdlog::spdlog_header_only
)

set_target_properties(CppSdl2
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

message(STATUS "CppSdl2_Example is available to add: -DCppSdl2_Example=1")
option(CppSdl2_Example "Add CppSdl2_Example project." OFF)
if (CppSdl2_Example)
	add_subdirectory(CppSdl2_Example)
endif ()

message(STATUS "MSVC_TOOLSET_VERSION: ${MSVC_VERSION}")

option(CppSdl2_Test "Add CppSdl2_Test to project." OFF)
if (CppSdl2_Test)
	add_subdirectory(CppSdl2_Test)
endif ()

# -------------------------------------------------------------------------
# Install
install(TARGETS CppSdl2
	EXPORT CppSdl2Targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include/cppsdl2
)

install(DIRECTORY src/sdl/
	DESTINATION include/cppsdl2/sdl
	FILES_MATCHING PATTERN "*.h"
)
install(FILES
	${CMAKE_CURRENT_SOURCE_DIR}/cppsdl2.natstepfilter
	DESTINATION include/cppsdl2
)

include(CMakePackageConfigHelpers)

# Export the package configuration
install(EXPORT CppSdl2Targets
	FILE CppSdl2Targets.cmake
	NAMESPACE CppSdl2::
	DESTINATION share/cmake/cppsdl2
)

# Generate the package configuration file
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/CppSdl2Config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/CppSdl2Config.cmake
	INSTALL_DESTINATION share/cmake/cppsdl2
)

# Install the generated package configuration file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CppSdl2Config.cmake
	DESTINATION share/cmake/cppsdl2
)

# Install the package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/CppSdl2ConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CppSdl2ConfigVersion.cmake
	DESTINATION share/cmake/cppsdl2
)

install(FILES LICENSE.txt DESTINATION share/doc/cppsdl2)
