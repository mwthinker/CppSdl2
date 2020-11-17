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
		ImGui::GetIO();

		ImGui_ImplSDL2_InitForOpenGL(getSdlWindow(), getGlContext());
		ImGui_ImplOpenGL3_Init();
	}

	void ImGuiWindow::update(const DeltaTime& deltaTime) {
		ImGui_ImplOpenGL3_NewFrame();
		
		imGuiPreUpdate(deltaTime);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(getSdlWindow());
		ImGui::NewFrame();
	
		if (showDemoWindow_) {
			ImGui::ShowDemoWindow(&showDemoWindow_);
		}

		imGuiUpdate(deltaTime);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		imGuiPostUpdate(deltaTime);
	}

	void ImGuiWindow::eventUpdate(const SDL_Event& windowEvent) {
		ImGui_ImplSDL2_ProcessEvent(&windowEvent);
	}

}
