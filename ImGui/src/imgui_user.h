#ifndef CPPSDL2_IMGUI_USER_H
#define CPPSDL2_IMGUI_USER_H

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
	bool MainWindow(const char* name, T&& t) {
		const auto& viewPort = *ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewPort.Pos);
		ImGui::SetNextWindowSize(viewPort.Size);

		constexpr ImGuiWindowFlags ImguiNoWindow
			= ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoBringToFrontOnFocus
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {0,0,0,0});

		bool result = ImGui::Window("Main", nullptr, ImguiNoWindow, t);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(3);
		return result;
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

	template <class T>
	bool Menu(const char* label, bool enabled, T&& t) {
		bool success = ImGui::BeginMenu(label, enabled);
		if (success) {
			t();
			ImGui::EndMenu();
		}
		return success;
	}

	template <class T>
	bool Menu(const char* label, T&& t) {
		bool success = ImGui::BeginMenu(label);
		if (success) {
			t();
			ImGui::EndMenu();
		}
		return success;
	}

	template <class T>
	bool MenuBar(T&& t) {
		bool success = ImGui::BeginMenuBar();
		if (success) {
			t();
			ImGui::EndMenuBar();
		}
		return success;
	}

	template <class T>
	bool MainMenuBar(T&& t) {
		bool success = ImGui::BeginMainMenuBar();
		if (success) {
			t();
			ImGui::EndMainMenuBar();
		}
		return success;
	}

	template <class T>
	bool PopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags, T&& t) {
		bool success = ImGui::BeginPopupModal(name, p_open, flags);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	template <class T>
	bool PopupModal(const char* name, bool* p_open, T&& t) {
		bool success = ImGui::BeginPopupModal(name, p_open);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	template <class T>
	bool Popup(const char* str_id, ImGuiWindowFlags flags, T&& t) {
		bool success = ImGui::BeginPopup(str_id, flags);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	template <class T>
	bool Popup(const char* str_id, T&& t) {
		bool success = ImGui::BeginPopup(str_id);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	template <class T>
	bool PopupModal(const char* name, T&& t) {
		bool success = ImGui::BeginPopupModal(name);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	template <class T>
	void Tooltip(T&& t) {
		ImGui::BeginTooltip();
		t();
		ImGui::EndTooltip();
	}

	template <class T>
	bool ComboScoped(const char* label, const char* preview_value, ImGuiComboFlags flags, T&& t) {
		bool success = ImGui::BeginCombo(label, preview_value, flags);
		if (success) {
			t();
			ImGui::EndCombo();
		}
		return success;
	}

	template <class T>
	bool ComboScoped(const char* label, const char* preview_value, T&& t) {
		bool success = ImGui::BeginCombo(label, preview_value);
		if (success) {
			t();
			ImGui::EndCombo();
		}
		return success;
	}

}

#endif
