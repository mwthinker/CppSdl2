#ifndef CPPSDL2_SDL_GRAPHIC_H
#define CPPSDL2_SDL_GRAPHIC_H

#include "vertex.h"

#include <sdl/batch.h>
#include <sdl/textureview.h>

#include <glm/gtc/constants.hpp>

namespace sdl::graphic {

	constexpr auto PI = glm::pi<float>();

} // Namespace sdl.

namespace sdl::graphic::indexed {

	sdl::BatchView<Vertex> addRectangle(sdl::Batch<Vertex>& batch, glm::vec2 pos, glm::vec2 size, Color color);

	sdl::BatchView<Vertex> addHexagonImage(sdl::Batch<Vertex>& batch, glm::vec2 center, float radius, const sdl::TextureView& sprite, float startAngle);

	sdl::BatchView<Vertex> addHexagon(sdl::Batch<Vertex>& batch, glm::vec2 center, float innerRadius, float outerRadius, Color color, float startAngle);

	sdl::BatchView<Vertex> addCircle(sdl::Batch<Vertex>& batch, glm::vec2 center, float radius, Color color, const int iterations, float startAngle);

} // Namespace sdl.

#endif // CPPSDL2_SDL_GRAPHIC_H
