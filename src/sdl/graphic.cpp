#include "graphic.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>
#include <array>

namespace sdl::graphic {
	
	glm::vec2 getHexagonCorner(int nbr, float startAngle) {
		return glm::rotate(glm::vec2{1, 0.f}, PI / 3 * nbr + startAngle);
	}

	glm::vec2 getHexagonCorner(glm::vec2 center, GLfloat size, int nbr, float startAngle) {
		return center + size * getHexagonCorner(nbr, startAngle);
	}

	std::array<glm::vec2, 6> getHexagonCorners(glm::vec2 center, GLfloat radius, float startAngle) {
		std::array<glm::vec2, 6> corners;
		for (int i = 0; i < 6; ++i) {
			corners[i] = getHexagonCorner(center, radius, i, startAngle);
		}
		return corners;
	}

} // Namespace sdl::graphic.

namespace sdl::graphic::indexed {

	sdl::BatchView<Vertex> addRectangle(sdl::Batch<Vertex>& batch, glm::vec2 pos, glm::vec2 size, Color color) {
		batch.startBatchView();
		batch.startAdding();

		batch.pushBack(Vertex{pos, {0.f, 0.f}, color});
		batch.pushBack(Vertex{pos + glm::vec2{size.x, 0.f}, {0.f, 0.f}, color});
		batch.pushBack(Vertex{pos + size, {0.f, 0.f}, color});
		batch.pushBack(Vertex{pos + glm::vec2{0.f, size.y}, {0.f, 0.f}, color});

		batch.pushBackIndex(0);
		batch.pushBackIndex(1);
		batch.pushBackIndex(2);
		batch.pushBackIndex(0);
		batch.pushBackIndex(2);
		batch.pushBackIndex(3);
		return batch.getBatchView(GL_TRIANGLES);
	}

	sdl::BatchView<Vertex> addHexagonImage(sdl::Batch<Vertex>& batch, glm::vec2 center, float radius, const sdl::TextureView& sprite, float startAngle) {
		batch.startBatchView();
		batch.startAdding();

		if (sprite) {
			glm::vec2 texHalfSize = sprite.getSize() * 0.5f;
			glm::vec2 texMiddlePos = sprite.getPosition() + texHalfSize;

			Vertex centerVertex{center, texMiddlePos, WHITE};
			batch.pushBack(centerVertex);

			for (int i = 0; i < 6; ++i) {
				auto v = Vertex{getHexagonCorner(center, radius, i, startAngle), texMiddlePos + texHalfSize * getHexagonCorner(i, 0), WHITE};
				batch.pushBack(v);
			}
			for (int i = 1; i <= 6; ++i) {
				batch.pushBackIndex(0);
				batch.pushBackIndex(i);
				batch.pushBackIndex((i % 6) + 1);
			}
		}

		return batch.getBatchView(GL_TRIANGLES);
	}

	sdl::BatchView<Vertex> addHexagon(sdl::Batch<Vertex>& batch, glm::vec2 center, float innerRadius, float outerRadius, Color color, float startAngle) {
		batch.startBatchView();
		batch.startAdding();

		auto innerCorners = getHexagonCorners(center, innerRadius, startAngle);
		auto outerCorners = getHexagonCorners(center, outerRadius, startAngle);
		
		for (const auto& corner : innerCorners) {
			batch.pushBack({corner, {0.f, 0.f}, color});
		}
		for (const auto& corner : outerCorners) {
			batch.pushBack({corner, {0.f, 0.f}, color});
		}
		
		for (int i = 0; i < 6; ++i) {
			batch.pushBackIndex(i);
			batch.pushBackIndex(6 + i);
			batch.pushBackIndex(6 + (i + 1) % 6);
			batch.pushBackIndex(i);
			batch.pushBackIndex((i + 1) % 6);
			batch.pushBackIndex(6 + (i + 1) % 6);
		}
		return batch.getBatchView(GL_TRIANGLES);
	}

	sdl::BatchView<Vertex> addCircle(sdl::Batch<Vertex>& batch, glm::vec2 center, float radius, Color color, const int iterations, float startAngle) {
		batch.startBatchView();
		batch.startAdding();

		batch.pushBack({center, {0.f, 0.f}, color});

		for (int i = 0; i < iterations; ++i) {
			auto rad = 2 * PI * i / iterations + startAngle;
			auto edge = center + glm::rotate(glm::vec2{radius, 0.f}, rad);

			batch.pushBack({edge, {0.f, 0.f}, color});
		}
		for (int i = 1; i <= iterations; ++i) {
			batch.pushBackIndex(0);
			batch.pushBackIndex(i);
			batch.pushBackIndex((i % iterations) + 1);
		}

		return batch.getBatchView(GL_TRIANGLES);
	}

} // sdl.