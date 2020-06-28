#ifndef CPPSDL2_SDL_GRAPHIC_H
#define CPPSDL2_SDL_GRAPHIC_H

#include "vertex.h"

#include <sdl/batch.h>
#include <sdl/textureview.h>

#include <glm/gtc/constants.hpp>

#include <array>

namespace sdl::graphic {

	constexpr auto PI = glm::pi<float>();

	glm::vec2 getHexagonCorner(int nbr, float startAngle = 0.f);

	glm::vec2 getHexagonCorner(const glm::vec2& center, float size, int nbr, float startAngle = 0.f);

	[[nodiscard]] std::array<glm::vec2, 6> getHexagonCorners(const glm::vec2& center, float radius, float startAngle = 0.f);

}

namespace sdl::graphic::indexed {

	sdl::BatchView<Vertex> addRectangle(sdl::Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, Color color);

	sdl::BatchView<Vertex> addRectangleImage(sdl::Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, const sdl::TextureView& sprite);

	sdl::BatchView<Vertex> addHexagonImage(sdl::Batch<Vertex>& batch, const glm::vec2& center, float radius, const sdl::TextureView& sprite, float startAngle);

	sdl::BatchView<Vertex> addHexagon(sdl::Batch<Vertex>& batch, const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle);

	sdl::BatchView<Vertex> addCircle(sdl::Batch<Vertex>& batch, const glm::vec2& center, float radius, Color color, const int iterations, float startAngle);

}

#endif
