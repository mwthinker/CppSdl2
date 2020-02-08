#include "imguiauxiliary.h"
#include "vertex.h"
#include "graphic.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace ImGui {

	namespace Helper {

		void PrimWriteVtx(ImDrawList* drawList, const sdl::Vertex& vertex) {
			drawList->PrimWriteVtx({vertex.pos.x, vertex.pos.y}, {vertex.tex.x, vertex.tex.y}, vertex.color.toImU32());
		}

		void AddHexagonImage(const sdl::TextureView& sprite, glm::vec2 center, float radius, float startAngle) {
			auto drawList = ImGui::GetWindowDrawList();
			for (int i = 1; i <= 6; ++i) {
				drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 0);
				drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + i);
				drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + (i % 6) + 1);
			}

			auto texHalfSize = sprite.getSize() * 0.5f;
			auto texMiddlePos = sprite.getPosition() + texHalfSize;
			sdl::Vertex centerVertex{center, texMiddlePos, sdl::WHITE};
			PrimWriteVtx(drawList, centerVertex);

			for (int i = 0; i < 6; ++i) {
				auto v = sdl::Vertex{sdl::graphic::getHexagonCorner(center, radius, i, 0), texMiddlePos + texHalfSize * sdl::graphic::getHexagonCorner(i, startAngle), sdl::WHITE};
				PrimWriteVtx(drawList, v);
			}
		}

		void AddCenterImageQuad(const sdl::TextureView& texture,
			const glm::vec2& center, const glm::vec2& size, float angle, const sdl::Color& color) {

			glm::vec2 deltaA{-size.x, -size.y};
			glm::vec2 deltaB{size.x, -size.y};
			glm::vec2 deltaC{size.x, size.y};
			glm::vec2 deltaD{-size.x, size.y};

			ImVec2 a = center + glm::rotate(0.5f * deltaA, angle);
			ImVec2 b = center + glm::rotate(0.5f * deltaB, angle);
			ImVec2 c = center + glm::rotate(0.5f * deltaC, angle);
			ImVec2 d = center + glm::rotate(0.5f * deltaD, angle);

			ImVec2 uv_c{texture.getX(), texture.getY()};
			ImVec2 uv_d{texture.getX() + texture.getWidth(), texture.getY()};
			ImVec2 uv_a{texture.getX() + texture.getWidth(), texture.getY() + texture.getHeight()};
			ImVec2 uv_b{texture.getX(), texture.getY() + texture.getHeight()};

			ImGui::GetWindowDrawList()->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color.toImU32());
		}

		void AddImageQuad(const sdl::TextureView& texture,
			const glm::vec2& pos, const glm::vec2& size, const sdl::Color& color) {

			ImVec2 a = pos;
			ImVec2 b = pos + glm::vec2{size.x, 0.f};
			ImVec2 c = pos + glm::vec2{size.x, size.y};
			ImVec2 d = pos + glm::vec2{0.f, size.y};

			ImVec2 uv_c{texture.getX(), texture.getY()};
			ImVec2 uv_d{texture.getX() + texture.getWidth(), texture.getY()};
			ImVec2 uv_a{texture.getX() + texture.getWidth(), texture.getY() + texture.getHeight()};
			ImVec2 uv_b{texture.getX(), texture.getY() + texture.getHeight()};

			ImGui::GetWindowDrawList()->PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, color.toImU32());
		}

	}  // Namespace Helper.

	void Hexagon(const sdl::TextureView& texture, float size, bool flat) {
		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID) (intptr_t) texture);
		drawList->PrimReserve(6 * 3, 7);

		auto angle = flat? 0.f : glm::pi<float>() / 2.f;

		glm::vec2 pos = ImGui::GetCursorScreenPos();
		Helper::AddHexagonImage(texture, pos + glm::vec2{size * 0.5f, size * 0.5f}, size * 0.5f, angle);
		drawList->PopTextureID();
		ImGui::Dummy({size * 0.5f, size * 0.5f});
	}

	void Image(const sdl::TextureView& texture, const glm::vec2& size, const sdl::Color& tintCol, const sdl::Color& borderCol) {
		ImGui::Image((ImTextureID) (intptr_t) texture, size, texture.getPosition(), texture.getPosition() + texture.getSize(), tintCol, borderCol);
	}

	[[nodiscard]] bool ImageButton(const sdl::TextureView& texture, const glm::vec2& size) {
		return ImageButton((ImTextureID) (intptr_t) texture, size, texture.getPosition(), texture.getPosition() + texture.getSize());
	}

	void ImageBackground(const sdl::TextureView& texture) {
		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushTextureID((ImTextureID) (intptr_t) texture);
		drawList->PrimReserve(6, 4);
		Helper::AddImageQuad(texture, {0.f, 0.f}, ImGui::GetWindowSize());
		drawList->PopTextureID();
	}

} // Namespace ImGui.
