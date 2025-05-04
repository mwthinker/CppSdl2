project(ImGui
	DESCRIPTION
		"ImGui Docking branch: https://github.com/ocornut/imgui"
	LANGUAGES
		CXX
	VERSION 1.91.9
)

include(FetchContent)
FetchContent_Declare(RemoteImGui
	GIT_REPOSITORY
		https://github.com/ocornut/imgui.git
	GIT_TAG
		4806a1924ff6181180bf5e4b8b79ab4394118875
)
FetchContent_MakeAvailable(RemoteImGui)
FetchContent_GetProperties(RemoteImGui
	SOURCE_DIR
		RemoteImGui_SOURCE_DIR
)

set(IMGUI_USER_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/src/imconfig.h)

add_library(ImGui STATIC
	${RemoteImGui_SOURCE_DIR}/imgui.cpp
	${RemoteImGui_SOURCE_DIR}/imgui.h
	${RemoteImGui_SOURCE_DIR}/imgui_demo.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_draw.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_internal.h
	${RemoteImGui_SOURCE_DIR}/imgui_tables.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_widgets.cpp
	${RemoteImGui_SOURCE_DIR}/imstb_rectpack.h
	${RemoteImGui_SOURCE_DIR}/imstb_textedit.h
	${RemoteImGui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
	${RemoteImGui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h
	${RemoteImGui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
	${RemoteImGui_SOURCE_DIR}/misc/freetype/imgui_freetype.h

	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
	${RemoteImGui_SOURCE_DIR}/misc/debuggers/imgui.natvis
	${RemoteImGui_SOURCE_DIR}/misc/debuggers/imgui.natstepfilter
	${IMGUI_USER_CONFIG}
	${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_user.h
)
add_library(CppSdl2::ImGui ALIAS ImGui)

target_include_directories(ImGui
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<BUILD_INTERFACE:${RemoteImGui_SOURCE_DIR}>
		$<BUILD_INTERFACE:${RemoteImGui_SOURCE_DIR}/backends>
		$<INSTALL_INTERFACE:include/CppSdl2/Imgui>
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(glbinding CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Compile error in GCC 2023-09-13
# find_package(unofficial-lunasvg CONFIG REQUIRED)
# Link to unofficial::lunasvg::lunasvg
# vcpkg "lunasvg"
# IMGUI_ENABLE_FREETYPE_LUNASVG

target_link_libraries(ImGui
	PUBLIC
		$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
		$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
		$<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
		$<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
		$<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
		freetype
		glbinding::glbinding
		glm::glm
		fmt::fmt
)

target_compile_definitions(ImGui
	PUBLIC
		IMGUI_DISABLE_OBSOLETE_KEYIO
		IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		IMGUI_INCLUDE_IMGUI_USER_H
		IMGUI_ENABLE_FREETYPE
		IMGUI_USER_CONFIG=$<BUILD_INTERFACE:\"${CMAKE_CURRENT_SOURCE_DIR}/src/imconfig.h\">$<INSTALL_INTERFACE:\"imconfig.h\">
		IMGUI_DEFINE_MATH_OPERATORS
		GLM_ENABLE_EXPERIMENTAL
)

set_target_properties(ImGui
	PROPERTIES
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

install(TARGETS ImGui
	EXPORT CppSdl2Targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)

# Install specific directories and files from RemoteImGui_SOURCE_DIR
install(DIRECTORY
	${RemoteImGui_SOURCE_DIR}/misc
	DESTINATION include/CppSdl2/Imgui/misc
	FILES_MATCHING PATTERN "*.h"
)

# Install specific files from the backends directory
install(FILES
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_sdl2.h
	DESTINATION include/CppSdl2/Imgui/backends
)

# Install specific files from the root of RemoteImGui_SOURCE_DIR
install(FILES
	${RemoteImGui_SOURCE_DIR}/imgui.h
	${RemoteImGui_SOURCE_DIR}/imgui_internal.h
	${RemoteImGui_SOURCE_DIR}/imstb_rectpack.h
	${RemoteImGui_SOURCE_DIR}/imstb_textedit.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/imconfig.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_user.h
	DESTINATION include/CppSdl2/Imgui
)

install(EXPORT CppSdl2Targets
	FILE CppSdl2Targets.cmake
	NAMESPACE CppSdl2::
	DESTINATION share/cmake/CppSdl2
)
