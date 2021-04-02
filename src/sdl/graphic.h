#ifndef CPPSDL2_SDL_GRAPHIC_H
#define CPPSDL2_SDL_GRAPHIC_H

#include "vertex.h"

#include <sdl/batch.h>
#include <sdl/textureview.h>
#include <sdl/vertexarrayobject.h>
#include <sdl/vertexbufferobject.h>
#include <sdl/shader.h>

#include <glm/gtc/constants.hpp>

#include <array>

namespace sdl::graphic {

	constexpr auto Pi = glm::pi<float>();

	glm::vec2 getHexagonCorner(int nbr, float startAngle = 0.f);

	glm::vec2 getHexagonCorner(const glm::vec2& center, float size, int nbr, float startAngle = 0.f);

	[[nodiscard]] std::array<glm::vec2, 6> getHexagonCorners(const glm::vec2& center, float radius, float startAngle = 0.f);

}

namespace sdl::graphic::indexed {

	BatchView<Vertex> addRectangle(Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, Color color);

	BatchView<Vertex> addRectangleImage(Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, const TextureView& sprite, Color color = color::White);

	BatchView<Vertex> addHexagonImage(Batch<Vertex>& batch, const glm::vec2& center, float radius, const TextureView& sprite, float startAngle);

	BatchView<Vertex> addHexagon(Batch<Vertex>& batch, const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle);

	BatchView<Vertex> addCircle(Batch<Vertex>& batch, const glm::vec2& center, float radius, Color color, const int iterations, float startAngle);

}

namespace sdl {

	class Graphic {
	public:
		Graphic();
		virtual ~Graphic() = default;

		void loadIdentityMatrix();

		const glm::mat4& getMatrix() const;

		void setMatrix(const glm::mat4& matrix);

		void multMatrix(const glm::mat4& matrix);

		void rotate(float angle);

		void translate(const glm::vec2& pos);
		
		void popMatrix();

		void pushMatrix();

		void addFilledHexagon(const glm::vec2& center, float radius, Color color, float startAngle = 0);

		void addRectangle(const glm::vec2& pos, const glm::vec2& size, Color color);

		void addRectangleImage(const glm::vec2& pos, const glm::vec2& size, const sdl::TextureView& textureView, Color color = sdl::color::White);

		void addCircle(const glm::vec2& center, float radius, Color color, const int iterations = 40, float startAngle = 0);

		void addHexagonImage(const glm::vec2& center, float radius, const sdl::TextureView& sprite, float startAngle = 0);

		void addHexagon(const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle = 0);

		void draw(const sdl::Shader& shader);

		void clearDraw();

	protected:
		glm::mat4& getMatrix();

	private:
		using Batch = sdl::Batch<sdl::Vertex>;
		using BatchView = sdl::BatchView<sdl::Vertex>;

		struct BatchData {
			BatchData() = default;
			BatchData(BatchView&& batchView, int matrixIndex);
			BatchData(BatchView&& batchView, const sdl::TextureView& texture, int matrixIndex);

			BatchView batchView;
			GLuint texture{};
			int matrixIndex{};
		};

		void bind(const sdl::Shader& shader);

		void draw(const sdl::Shader& shader, const BatchData& batchData);

		std::vector<glm::mat4> matrixes_;
		Batch batch_{GL_DYNAMIC_DRAW};
		std::vector<BatchData> batches_;
		sdl::VertexArrayObject vao_;
		int currentMatrixIndex_{};
		bool initiated_{};
	};

	inline const glm::mat4& Graphic::getMatrix() const {
		assert(!matrixes_.empty());
		return matrixes_.back();
	}

	inline glm::mat4& Graphic::getMatrix() {
		assert(!matrixes_.empty());
		return matrixes_.back();
	}

}

#endif
