#include "imguiwindow.h"
#include "logger.h"

namespace sdl {

	namespace {

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;

		const ImGuiWindowFlags ImGuiNoWindow2 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		/*
		bool ComboAi(const char* name, int& item, const std::vector<Ai>& ais, ImGuiComboFlags flags = 0) {
			int oldItem = item;
			if (ImGui::BeginCombo(name, ais[item].getName().c_str(), flags))
			{
				size_t size = ais.size();
				for (int n = 0; n < size; ++n)
				{
					bool is_selected = (item == n);
					if (ImGui::Selectable(ais[n].getName().c_str(), is_selected)) {
						item = n;
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			return oldItem != item;
		}
		*/
	}

	ImGuiWindow::ImGuiWindow() {
		show_demo_window = true;
		show_another_window = false;
		initiatedOpenGl_ = false;
		initiatedSdl_ = false;	
	}

	ImGuiWindow::~ImGuiWindow() {
		if (initiatedOpenGl_) {
			//ImGui_ImplOpenGL3_Shutdown();
		}
		if (initiatedSdl_) {
			//ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
		}
	}

	void ImGuiWindow::initOpenGl() {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		Window::initOpenGl();
	}

	void ImGuiWindow::initPreLoop() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		//ImGui_ImplSDL2_Init(sdl::Window::getSdlWindow());
		initiatedSdl_ = true;
		const char* glsl_version = "#version 130";
		//ImGui_ImplOpenGL3_Init(glsl_version);
		initiatedOpenGl_ = true;

		//defaultFont_ = io.Fonts->AddFontFromFileTTF("fonts/Ubuntu-B.ttf", 16);		

		//tetris::TetrisData::getInstance().bindTextureFromAtlas();

		//ImGui::GetStyle().WindowBorderSize = 0;
	}	

	void ImGuiWindow::update(double deltaTime) {
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		auto context = SDL_GL_GetCurrentContext();

		// Start the Dear ImGui frame
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplSDL2_NewFrame(sdl::Window::getSdlWindow());
		ImGui::NewFrame();
	
		//ImGui::PushFont(defaultFont_);
		ImGui::ShowDemoWindow(&show_demo_window);
		//ImGui::PopFont();

		const ImGuiWindowFlags ImGuiNoWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
		const ImGuiWindowFlags ImGuiInnerWindow = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiWindow::eventUpdate(const SDL_Event& windowEvent) {
		//ImGui_ImplSDL2_ProcessEvent(windowEvent);

		switch (windowEvent.type) {
			case SDL_WINDOWEVENT:
				switch (windowEvent.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						resize(windowEvent.window.data1, windowEvent.window.data2);
						break;
					case SDL_WINDOWEVENT_LEAVE:
						break;
					case SDL_WINDOWEVENT_CLOSE:
						sdl::Window::quit();
				}
				break;
			case SDL_QUIT:
				sdl::Window::quit();
				break;
		}
	}

	void ImGuiWindow::resize(int width, int height) {
		glViewport(0, 0, width, height);
	}

} // Namespace tetris.