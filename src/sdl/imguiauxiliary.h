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

		void AddImageQuad(const sdl::TextureView& sprite, const glm::vec2& pos, const glm::vec2& size,
			float angle = 0.f, const sdl::Color& color = sdl::WHITE);

	}  // Namespace Helper.

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

	void Hexagon(const sdl::TextureView& texture, float size, bool flat = true);

	void Image(const sdl::TextureView& texture, const glm::vec2& size,
		const sdl::Color& tintCol = sdl::WHITE, const sdl::Color& borderCol = {});

	[[nodiscard]] bool ImageButton(const sdl::TextureView& texture, const glm::vec2& size);

	void ImageBackground(const sdl::TextureView& texture);

} // Namespace ImGui.

#endif // CPPSDL2_SDL_IMGUIAUXILIARY_H
