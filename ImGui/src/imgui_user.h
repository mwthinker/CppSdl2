#ifndef CPPSDL2_IMGUI_USER_H
#define CPPSDL2_IMGUI_USER_H

#include "misc/cpp/imgui_stdlib.h"

#include <concepts>

namespace ImGui {

	// Used default by MainWindow
	constexpr ImGuiWindowFlags MainWindowFlags
		= ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoScrollWithMouse;

	bool Window(const char* name, bool* p_open, ImGuiWindowFlags flags, std::invocable auto&& t) {
		bool success = ImGui::Begin(name, p_open, flags);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	bool Window(const char* name, bool* p_open, std::invocable auto&& t) {
		bool success = ImGui::Begin(name, p_open);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	bool Window(const char* name, std::invocable auto&& t) {
		bool success = ImGui::Begin(name);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	bool MainWindow(const char* name, ImGuiWindowFlags flags, std::invocable auto&& t) {
		const auto& viewPort = *ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewPort.Pos);
		ImGui::SetNextWindowSize(viewPort.Size);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {0, 0, 0, 0});

		bool result = ImGui::Window(name, nullptr, flags, t);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar(3);
		return result;
	}

	bool MainWindow(const char* name, std::invocable auto&& t) {
		return MainWindow(name, MainWindowFlags, t);
	}

	bool ChildWindow(const char* name, const ImVec2& size, bool border, ImGuiWindowFlags flags, std::invocable auto&& t) {
		bool success = ImGui::BeginChild(name, size, border, flags);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

	bool ChildWindow(const char* name, const ImVec2& size, bool border, std::invocable auto&& t) {
		bool success = ImGui::BeginChild(name, size, border);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

	bool ChildWindow(const char* name, const ImVec2& size, std::invocable auto&& t) {
		bool success = ImGui::BeginChild(name, size);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

	bool ChildWindow(const char* name, std::invocable auto&& t) {
		bool success = ImGui::BeginChild(name);
		if (success) {
			t();
		}
		ImGui::EndChild();
		return success;
	}

	bool Menu(const char* label, bool enabled, std::invocable auto&& t) {
		bool success = ImGui::BeginMenu(label, enabled);
		if (success) {
			t();
			ImGui::EndMenu();
		}
		return success;
	}

	bool Menu(const char* label, std::invocable auto&& t) {
		bool success = ImGui::BeginMenu(label);
		if (success) {
			t();
			ImGui::EndMenu();
		}
		return success;
	}

	bool MenuBar(std::invocable auto&& t) {
		bool success = ImGui::BeginMenuBar();
		if (success) {
			t();
			ImGui::EndMenuBar();
		}
		return success;
	}

	bool MainMenuBar(std::invocable auto&& t) {
		bool success = ImGui::BeginMainMenuBar();
		if (success) {
			t();
			ImGui::EndMainMenuBar();
		}
		return success;
	}

	bool PopupModal(const char* name, bool* p_open, ImGuiWindowFlags flags, std::invocable auto&& t) {
		bool success = ImGui::BeginPopupModal(name, p_open, flags);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	bool PopupModal(const char* name, bool* p_open, std::invocable auto&& t) {
		bool success = ImGui::BeginPopupModal(name, p_open);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	bool Popup(const char* str_id, ImGuiWindowFlags flags, std::invocable auto&& t) {
		bool success = ImGui::BeginPopup(str_id, flags);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	bool Popup(const char* str_id, std::invocable auto&& t) {
		bool success = ImGui::BeginPopup(str_id);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	bool PopupModal(const char* name, std::invocable auto&& t) {
		bool success = ImGui::BeginPopupModal(name);
		if (success) {
			t();
			ImGui::EndPopup();
		}
		return success;
	}

	void Tooltip(std::invocable auto&& t) {
		ImGui::BeginTooltip();
		t();
		ImGui::EndTooltip();
	}

	bool ComboScoped(const char* label, const char* preview_value, ImGuiComboFlags flags, std::invocable auto&& t) {
		bool success = ImGui::BeginCombo(label, preview_value, flags);
		if (success) {
			t();
			ImGui::EndCombo();
		}
		return success;
	}

	bool ComboScoped(const char* label, const char* preview_value, std::invocable auto&& t) {
		bool success = ImGui::BeginCombo(label, preview_value);
		if (success) {
			t();
			ImGui::EndCombo();
		}
		return success;
	}

	void Group(std::invocable auto&& t) {
		ImGui::BeginGroup();
		t();
		ImGui::EndGroup();
	}

	bool Table(const char* str_id, int columns_count, ImGuiTableFlags flags, const ImVec2& outer_size, float inner_width, std::invocable auto&& t) {
		bool success = ImGui::BeginTable(str_id, columns_count, flags, outer_size, inner_width);
		if (success) {
			t();
			ImGui::EndTable();
		}
		return success;
	}

	bool Table(const char* str_id, int columns_count, ImGuiTableFlags flags, const ImVec2& outer_size, std::invocable auto&& t) {
		bool success = ImGui::BeginTable(str_id, columns_count, flags, outer_size);
		if (success) {
			t();
			ImGui::EndTable();
		}
		return success;
	}

	bool Table(const char* str_id, int columns_count, ImGuiTableFlags flags, std::invocable auto&& t) {
		bool success = ImGui::BeginTable(str_id, columns_count, flags);
		if (success) {
			t();
			ImGui::EndTable();
		}
		return success;
	}

	bool Table(const char* str_id, int columns_count, std::invocable auto&& t) {
		bool success = ImGui::BeginTable(str_id, columns_count);
		if (success) {
			t();
			ImGui::EndTable();
		}
		return success;
	}

}

#endif
