#include "imguiwindow.h"

#include <spdlog/spdlog.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

namespace sdl {

	namespace {

		void showColorWindow(bool& open) {
			ImGui::SetNextWindowSize({310.f, 400.f});
			ImGui::Window("Html Colors", &open, ImGuiWindowFlags_NoResize, []() {
				ImGui::Text("Copy color to clipboard by clicking");

				static const auto htmlColors = color::html::getHtmlColors();
				static std::string colorStr = "                   ";
				int nbr = 0;
				for (const auto& [name, color] : htmlColors) {
					++nbr;
					colorStr.clear();
					colorStr += name;
					colorStr += "##1";
					if (ImGui::ColorButton(colorStr.c_str(), color)) {
						ImGui::SetClipboardText(name);
					}
					if (nbr % 10 != 0) {
						ImGui::SameLine();
					}
				}
			});
		}

	}

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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			auto& style = ImGui::GetStyle();
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplSDL2_InitForOpenGL(getSdlWindow(), getGlContext());
		ImGui_ImplOpenGL3_Init();
	}

	void ImGuiWindow::update(const DeltaTime& deltaTime) {
		imGuiPreUpdate(deltaTime);
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		imGuiUpdate(deltaTime);

		if (showDemoWindow_) {
			ImGui::ShowDemoWindow(&showDemoWindow_);
		}
		if (showColorWindow_) {
			showColorWindow(showColorWindow_);
		}

		ImGui::Render();
		const auto& io = ImGui::GetIO();
		gl::glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
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
		
		auto& io = ImGui::GetIO();
		bool ioWantCapture = false;
		switch (windowEvent.type) {
			case SDL_MOUSEBUTTONUP:
				[[fallthrough]];
			case SDL_MOUSEBUTTONDOWN:
				[[fallthrough]];
			case SDL_MOUSEMOTION:
				[[fallthrough]];
			case SDL_MOUSEWHEEL:
				ioWantCapture = io.WantCaptureMouse;
				break;
			case SDL_KEYUP:
				[[fallthrough]];
			case SDL_KEYDOWN:
				ioWantCapture = io.WantCaptureKeyboard;
				break;
			case SDL_TEXTEDITING:
				[[fallthrough]];
			case SDL_TEXTINPUT:
				ioWantCapture = io.WantTextInput;
				break;
		}

		if (!ioWantCapture) {
			imGuiEventUpdate(windowEvent);
		}
	}

}
