#ifndef CPPSDL2_SDL_IMGUIAUXILIARY_H
#define CPPSDL2_SDL_IMGUIAUXILIARY_H

#include "textureview.h"
#include "color.h"
#include "vertex.h"

#include <imgui.h>
#include <glm/glm.hpp>

namespace ImGui {

	namespace Helper {

		void PrimWriteVtx(ImDrawList* drawList, const sdl::Vertex& vertex);

		void AddHexagonImage(const sdl::TextureView& sprite, glm::vec2 center, float radius, float startAngle);

		void AddCenterImageQuad(const sdl::TextureView& sprite, const glm::vec2& pos, const glm::vec2& size,
			float angle = 0.f, const sdl::Color& color = sdl::WHITE);

		void AddImageQuad(const sdl::TextureView& texture,
			const glm::vec2& pos, const glm::vec2& size, const sdl::Color& color = sdl::WHITE);

	} // Namespace Helper.

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

	void Hexagon(const sdl::TextureView& texture, float size, bool flat = true);

	void Image(const sdl::TextureView& texture, const glm::vec2& size,
		const sdl::Color& tintCol = sdl::WHITE, const sdl::Color& borderCol = {});

	[[nodiscard]] bool ImageButton(const sdl::TextureView& texture, const glm::vec2& size);

	void ImageBackground(const sdl::TextureView& texture);

} // Namespace ImGui.

#endif // CPPSDL2_SDL_IMGUIAUXILIARY_H
