#include "graphic.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <array>

namespace sdl::graphic {
	
	glm::vec2 getHexagonCorner(int nbr, float startAngle) {
		return glm::rotate(glm::vec2{1, 0.f}, Pi / 3 * nbr + startAngle);
	}

	glm::vec2 getHexagonCorner(const glm::vec2& center, float size, int nbr, float startAngle) {
		return center + size * getHexagonCorner(nbr, startAngle);
	}

	std::array<glm::vec2, 6> getHexagonCorners(const glm::vec2& center, float radius, float startAngle) {
		std::array<glm::vec2, 6> corners;
		for (int i = 0; i < 6; ++i) {
			corners[i] = getHexagonCorner(center, radius, i, startAngle);
		}
		return corners;
	}

}

namespace sdl::graphic::indexed {

	void addLine(Batch<Vertex>& batch, const glm::vec2& p1, const glm::vec2& p2, float width, Color color) {
		batch.startAdding();

		auto dp = 0.5f * width * glm::rotate(glm::normalize(p2 - p1), Pi / 2);

		batch.pushBack(Vertex{p1 - dp, {}, color});
		batch.pushBack(Vertex{p2 - dp, {}, color});
		batch.pushBack(Vertex{p2 + dp, {}, color});
		batch.pushBack(Vertex{p1 + dp, {}, color});
		batch.insertIndexes({0, 1, 2, 0, 2, 3});
	}

	void addRectangle(Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, Color color) {
		batch.startAdding();
		batch.pushBack(Vertex{pos, {}, color});
		batch.pushBack(Vertex{pos + glm::vec2{size.x, 0.f}, {}, color});
		batch.pushBack(Vertex{pos + size, {}, color});
		batch.pushBack(Vertex{pos + glm::vec2{0.f, size.y}, {}, color});
		batch.insertIndexes({0, 1, 2, 0, 2, 3});
	}

	void addRectangleImage(Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, const TextureView& texture, Color color) {
		batch.startAdding();

		if (texture) {
			batch.pushBack(Vertex{pos, texture.getPosition() + glm::vec2{0.f, texture.getHeight()}, color});
			batch.pushBack(Vertex{pos + glm::vec2{size.x, 0.f}, texture.getPosition() + glm::vec2{texture.getWidth(), texture.getHeight()}, color});
			batch.pushBack(Vertex{pos + size, texture.getPosition() + glm::vec2{texture.getWidth(), 0.f}, color});
			batch.pushBack(Vertex{pos + glm::vec2{0.f, size.y},  texture.getPosition(), color});

			batch.insertIndexes({0, 1, 2, 0, 2, 3});
		}
	}

	void addHexagonImage(Batch<Vertex>& batch, const glm::vec2& center, float radius, const sdl::TextureView& sprite, float startAngle) {
		batch.startAdding();

		if (sprite) {
			glm::vec2 texHalfSize = sprite.getSize() * 0.5f;
			glm::vec2 texMiddlePos = sprite.getPosition() + texHalfSize;

			Vertex centerVertex{center, texMiddlePos, sdl::color::White};
			batch.pushBack(centerVertex);

			for (int i = 0; i < 6; ++i) {
				auto tex = texHalfSize * getHexagonCorner(i, 0); // Textures are flipped in opengl.
				auto v = Vertex{getHexagonCorner(center, radius, i, startAngle), texMiddlePos + glm::vec2{tex.x, -tex.y}, sdl::color::White};
				batch.pushBack(v);
			}
			for (int i = 1; i <= 6; ++i) {
				batch.insertIndexes({0, i, (i % 6) + 1});
			}
		}
	}

	void addHexagon(Batch<Vertex>& batch, const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle) {
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
	}

	void addCircle(Batch<Vertex>& batch, const glm::vec2& center, float radius, Color color, const int iterations, float startAngle) {
		batch.startAdding();

		batch.pushBack({center, {0.f, 0.f}, color});

		for (int i = 0; i < iterations; ++i) {
			auto rad = 2 * Pi * i / iterations + startAngle;
			auto edge = center + glm::rotate(glm::vec2{radius, 0.f}, rad);

			batch.pushBack({edge, {0.f, 0.f}, color});
		}
		for (int i = 1; i <= iterations; ++i) {
			batch.insertIndexes({0, i, (i % iterations) + 1});
		}
	}

	void addCircleOutline(Batch<Vertex>& batch, const glm::vec2& center, float radius, float width, Color color, const int iterations, float startAngle) {
		batch.startAdding();

		for (int i = 0; i <= iterations; ++i) {
			auto rad = 2 * Pi * i / iterations + startAngle;
			auto outerEdge = center + glm::rotate(glm::vec2{radius + width * 0.5f, 0.f}, rad);
			auto innerEdge = center + glm::rotate(glm::vec2{radius - width * 0.5f, 0.f}, rad);

			batch.pushBack({outerEdge, {0.f, 0.f}, color});
			batch.pushBack({innerEdge, {0.f, 0.f}, color});
		}
		for (int i = 0; i < iterations * 2 - 1; i += 2) {
			batch.insertIndexes({i, i + 2, i + 3, i + 3, i + 1, i});
		}
	}

}

namespace sdl {

	namespace sdlg = sdl::graphic::indexed;

	Graphic::BatchData::BatchData(BatchView&& batchView, int matrixIndex)
		: batchView{batchView}
		, matrixIndex{matrixIndex} {
	}

	Graphic::BatchData::BatchData(BatchView&& batchView, const sdl::TextureView& texture, int matrixIndex)
		: batchView{batchView}
		, texture{texture}
		, matrixIndex{matrixIndex} {
	}

	Graphic::Graphic() {
		matrixes_.push_back({glm::mat4{1}, 0});
	}

	void Graphic::setIdentityMatrix() {
		if (dirty_) {
			matrix() = glm::mat4{1};
		} else {
			dirty_ = true;
			matrixes_.push_back({glm::mat4{1}, matrixes_.back().lastIndex});
		}
	}

	void Graphic::upload(sdl::Shader& shader) {
		glActiveTexture(GL_TEXTURE1);

		auto index = currentMatrixIndex_;
		currentMatrixIndex_ = -1;
		if (batch_.getSize() > 0) {
			shader.useProgram();
			bind(shader);
			batch_.uploadToGraphicCard();
			shader.setMatrix(matrixes_.front().matrix);

			for (auto& batchData : batches_) {
				draw(shader, batchData);
			}
		}
		currentMatrixIndex_ = index;
	}

	void Graphic::addLine(const glm::vec2& p1, const glm::vec2& p2, float width, Color color) {
		batch_.startBatchView();
		sdlg::addLine(batch_, p1, p2, width, color);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::addRectangle(const glm::vec2& pos, const glm::vec2& size, Color color) {
		batch_.startBatchView();
		sdlg::addRectangle(batch_, pos, size, color);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::addRectangleImage(const glm::vec2& pos, const glm::vec2& size, const sdl::TextureView& textureView, Color color) {
		batch_.startBatchView();
		sdlg::addRectangleImage(batch_, pos, size, textureView, color);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::addFilledHexagon(const glm::vec2& center, float radius, Color color, float startAngle) {
		batch_.startBatchView();
		addCircle(center, radius, color, 6, startAngle);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::addHexagonImage(const glm::vec2& center, float radius, const sdl::TextureView& sprite, float startAngle) {
		batch_.startBatchView();
		sdlg::addHexagonImage(batch_, center, radius, sprite, startAngle);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::addHexagon(const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle) {
		batch_.startBatchView();
		sdlg::addHexagon(batch_, center, innerRadius, outerRadius, color, startAngle);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::addCircle(const glm::vec2& center, float radius, Color color, const int iterations, float startAngle) {
		batch_.startBatchView();
		sdlg::addCircle(batch_, center, radius, color, iterations, startAngle);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::addCircleOutline(const glm::vec2& center, float radius, float width, Color color, const int iterations, float startAngle) {
		batch_.startBatchView();
		sdlg::addCircleOutline(batch_, center, radius, width, color, iterations, startAngle);
		add(batch_.getBatchView(GL_TRIANGLES));
	}

	void Graphic::bind(sdl::Shader& shader) {
		if (initiated_) {
			vao_.bind();
		} else {
			initiated_ = true;
			vao_.generate();
			vao_.bind();
			batch_.bind();
			shader.setVertexAttribPointer();
		}
	}

	void Graphic::draw(sdl::Shader& shader, const BatchData& batchData) {
		if (const auto& texture = batchData.texture; texture) {
			shader.setTextureId(1);
			glBindTexture(GL_TEXTURE_2D, texture);
		} else {
			shader.setTextureId(-1);
		}
		if (currentMatrixIndex_ != batchData.matrixIndex) {
			currentMatrixIndex_ = batchData.matrixIndex;
			shader.setMatrix(matrixes_[currentMatrixIndex_].matrix);
		}
		batch_.draw(batchData.batchView);
	}

	void Graphic::clear() {
		batch_.clear();
		batches_.clear();
		matrixes_.clear();
		matrixes_.push_back({glm::mat4{1}, 0});
		currentMatrixIndex_ = 0;
	}

	void Graphic::multMatrix(const glm::mat4& mult) {
		if (dirty_) {
			matrix() *= mult;
		} else {
			matrixes_.push_back({getMatrix() * mult, static_cast<int>(matrixes_.size())});
		}
	}

	void Graphic::pushMatrix() {
		matrixes_.push_back(matrixes_.back());
		dirty_ = true;
	}

	void Graphic::popMatrix() {
		if (matrixes_.size() > 1) {
			dirty_ = true; // Just to be safe, don't know if earlier state weas dirty (Maybe ToDo: make dirty part of matrixes vector).
			matrixes_.push_back(matrixes_[matrixes_.back().lastIndex]);
		} else {
			spdlog::warn("[sdl::Graphic] No matrix to pop");
		}
	}

	void Graphic::setMatrix(const glm::mat4& mat) {
		if (dirty_) {
			matrix() = mat;
		} else {
			dirty_ = true;
			matrixes_.push_back({mat, static_cast<int>(matrixes_.size())});
		}
	}

	void Graphic::rotate(float angle) {
		if (dirty_) {
			matrix() = glm::rotate(getMatrix(), angle, glm::vec3{0, 0, 1});
		} else {
			dirty_ = true;
			matrixes_.push_back({glm::rotate(getMatrix(), angle, glm::vec3{0, 0, 1}), static_cast<int>(matrixes_.size())});
		}
	}

	void Graphic::translate(const glm::vec2& pos) {
		if (dirty_) {
			matrix() = glm::translate(getMatrix(), glm::vec3{pos, 0});
		} else {
			dirty_ = true;
			matrixes_.push_back({glm::translate(getMatrix(), glm::vec3{pos, 0}), static_cast<int>(matrixes_.size())});
		}
	}

}
