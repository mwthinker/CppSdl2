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
			float angle = 0.f, sdl::Color color = sdl::color::White);

		void AddImageQuad(const sdl::TextureView& texture,
			const glm::vec2& pos, const glm::vec2& size, sdl::Color color = sdl::color::White);

	}

	void Hexagon(const sdl::TextureView& texture, float size, bool flat = true);

	void Image(const sdl::TextureView& texture, const glm::vec2& size);

	void ImageWithBg(const sdl::TextureView& texture, const glm::vec2& size, sdl::Color tintCol = sdl::color::White, sdl::Color borderCol = {});

	[[nodiscard]] bool ImageButton(const char* id, const sdl::TextureView& texture, const glm::vec2& size);

	void ImageBackground(const sdl::TextureView& texture);

}

#endif
