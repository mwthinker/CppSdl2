project(ImGui
	DESCRIPTION
		"ImGui: https://github.com/ocornut/imgui"
	LANGUAGES
		CXX
)

include(FetchContent)
FetchContent_Declare(RemoteImGui
	GIT_REPOSITORY
		https://github.com/ocornut/imgui.git
	GIT_TAG
		532aa5a69bbec87560639702d78a2df97a4be2a0
)
FetchContent_MakeAvailable(RemoteImGui)
FetchContent_GetProperties(RemoteImGui
	SOURCE_DIR
		RemoteImGui_SOURCE_DIR
)

find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(ImGui
	${RemoteImGui_SOURCE_DIR}/imconfig.h
	${RemoteImGui_SOURCE_DIR}/imgui.cpp
	${RemoteImGui_SOURCE_DIR}/imgui.h
	${RemoteImGui_SOURCE_DIR}/imgui_demo.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_draw.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_internal.h
	${RemoteImGui_SOURCE_DIR}/imgui_tables.cpp
	${RemoteImGui_SOURCE_DIR}/imgui_widgets.cpp
	${RemoteImGui_SOURCE_DIR}/imstb_rectpack.h
	${RemoteImGui_SOURCE_DIR}/imstb_textedit.h
	${RemoteImGui_SOURCE_DIR}/imstb_truetype.h
		
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_sdl.h
	${RemoteImGui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
)

target_include_directories(ImGui
	PUBLIC
		${RemoteImGui_SOURCE_DIR}
		${RemoteImGui_SOURCE_DIR}/backends/
)

target_link_libraries(ImGui
	PUBLIC
		SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static
		glad::glad
)
