project(ImGui
	DESCRIPTION
		"ImGui Docking branch: https://github.com/ocornut/imgui"
	LANGUAGES
		CXX
	VERSION 1.88
)

include(FetchContent)
FetchContent_Declare(RemoteImGui
	GIT_REPOSITORY
		https://github.com/ocornut/imgui.git
	GIT_TAG
		9cd9c2eff99877a3f10a7f9c2a3a5b9c15ea36c6
)
FetchContent_MakeAvailable(RemoteImGui)
FetchContent_GetProperties(RemoteImGui
	SOURCE_DIR
		RemoteImGui_SOURCE_DIR
)

set(IMGUI_USER_CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/src/imconfig.h)

add_library(ImGui
	${RemoteImGui_SOURCE_DIR}/imgui.cpp
	${RemoteImGui_SOURCE_DIR}/imgui.h
	${RemoteImGui_SOURCE_DIR}/imgui_demo.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_draw.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_internal.h
	${RemoteImGui_SOURCE_DIR}/imgui_tables.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_widgets.cpp
	${RemoteImGui_SOURCE_DIR}/imstb_rectpack.h
	${RemoteImGui_SOURCE_DIR}/imstb_textedit.h
	${RemoteImGui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
	${RemoteImGui_SOURCE_DIR}/misc/freetype/imgui_freetype.h
		
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_sdl.h
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
	${IMGUI_USER_CONFIG}
	${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_user.h
)

target_include_directories(ImGui
	PUBLIC
		src
		${RemoteImGui_SOURCE_DIR}
		${RemoteImGui_SOURCE_DIR}/backends/
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2-image CONFIG REQUIRED)
find_package(SDL2-mixer CONFIG REQUIRED)
find_package(SDL2-ttf CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(ImGui
	PUBLIC
		SDL2::SDL2 SDL2::SDL2main
		SDL2::SDL2_image
		SDL2::SDL2_ttf
		SDL2::SDL2_mixer
		glad::glad
		freetype
		glm::glm
)

target_compile_definitions(ImGui
	PUBLIC
		IMGUI_DISABLE_OBSOLETE_KEYIO
		IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		IMGUI_INCLUDE_IMGUI_USER_H
		IMGUI_ENABLE_FREETYPE
		IMGUI_USER_CONFIG=\"${IMGUI_USER_CONFIG}\"
)

set_target_properties(ImGui
	PROPERTIES
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)
