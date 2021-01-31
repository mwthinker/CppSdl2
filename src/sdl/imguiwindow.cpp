#include "imguiwindow.h"

#include <spdlog/spdlog.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

namespace sdl {

	ImGuiWindow::ImGuiWindow(int majorVersionGl, int minorVersionGl)
		: Window{majorVersionGl, minorVersionGl} {
	}

	ImGuiWindow::~ImGuiWindow() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiWindow::initPreLoop() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		auto& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
			| ImGuiConfigFlags_NavEnableGamepad
			| ImGuiConfigFlags_DockingEnable
			| ImGuiConfigFlags_ViewportsEnable;

		ImGui_ImplSDL2_InitForOpenGL(getSdlWindow(), getGlContext());
		ImGui_ImplOpenGL3_Init();
	}

	void ImGuiWindow::update(const DeltaTime& deltaTime) {
		imGuiPreUpdate(deltaTime);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(getSdlWindow());
		ImGui::NewFrame();

		imGuiUpdate(deltaTime);

		if (showDemoWindow_) {
			ImGui::ShowDemoWindow(&showDemoWindow_);
		}

		ImGui::Render();
		const auto& io = ImGui::GetIO();
		glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			auto backupCurrentWindow = SDL_GL_GetCurrentWindow();
			auto backupCurrentContext = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backupCurrentWindow, backupCurrentContext);
		}

		imGuiPostUpdate(deltaTime);
	}

	void ImGuiWindow::eventUpdate(const SDL_Event& windowEvent) {
		ImGui_ImplSDL2_ProcessEvent(&windowEvent);
	}

}
