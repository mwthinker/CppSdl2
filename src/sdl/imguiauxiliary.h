#ifndef CPPSDL2_SDL_IMGUIUSER_H
#define CPPSDL2_SDL_IMGUIUSER_H

#include <imgui.h>
#include <glm/glm.hpp>

namespace ImGui {

	template <class T>
	bool Window(const char* name, bool* p_open, ImGuiWindowFlags flags, T&& t) {
		bool success = ImGui::Begin(name, p_open, flags);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	template <class T>
	bool Window(const char* name, bool* p_open, T&& t) {
		bool success = ImGui::Begin(name, p_open);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	template <class T>
	bool Window(const char* name, T&& t) {
		bool success = ImGui::Begin(name);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	template <class T>
	bool ChildWindow(const char* name, const ImVec2& size, bool border, ImGuiWindowFlags flags, T&& t) {
		bool success = ImGui::BeginChild(name, size, border, flags);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

	template <class T>
	bool ChildWindow(const char* name, const ImVec2& size, bool border, T&& t) {
		bool success = ImGui::BeginChild(name, size, border);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

	template <class T>
	bool ChildWindow(const char* name, const ImVec2& size, T&& t) {
		bool success = ImGui::BeginChild(name, size);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

	template <class T>
	bool ChildWindow(const char* name, T&& t) {
		bool success = ImGui::BeginChild(name);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

} // Namespace ImGui.

#endif // CPPSDL2_SDL_IMGUIUSER_H
