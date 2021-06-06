#ifndef CPPSDL2_SDL_GRAPHIC_H
#define CPPSDL2_SDL_GRAPHIC_H

#include "vertex.h"

#include "batch.h"
#include "textureview.h"
#include "vertexarrayobject.h"
#include "vertexbufferobject.h"
#include "shader.h"

#include <glm/gtc/constants.hpp>

#include <array>
#include <type_traits>

namespace sdl::graphic {

	constexpr auto Pi = glm::pi<float>();

	glm::vec2 getHexagonCorner(int nbr, float startAngle = 0.f);

	glm::vec2 getHexagonCorner(const glm::vec2& center, float size, int nbr, float startAngle = 0.f);

	[[nodiscard]] std::array<glm::vec2, 6> getHexagonCorners(const glm::vec2& center, float radius, float startAngle = 0.f);

}

namespace sdl::graphic::indexed {

	void addLine(Batch<Vertex>& batch, const glm::vec2& p1, const glm::vec2& p2, float width, Color color);

	void addRectangle(Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, Color color);

	void addRectangleImage(Batch<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, const TextureView& sprite, Color color = color::White);

	void addHexagonImage(Batch<Vertex>& batch, const glm::vec2& center, float radius, const TextureView& sprite, float startAngle);

	void addHexagon(Batch<Vertex>& batch, const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle);

	void addCircle(Batch<Vertex>& batch, const glm::vec2& center, float radius, Color color, const int iterations, float startAngle);

	void addCircleOutline(Batch<Vertex>& batch, const glm::vec2& center, float radius, float width, Color color, const int iterations, float startAngle);

	void addPolygon(Batch<Vertex>& batch, std::initializer_list<glm::vec2> points, Color color);

}

namespace sdl {

	class Graphic {
	public:
		Graphic();
		virtual ~Graphic() = default;

		void setIdentityMatrix();

		const glm::mat4& getMatrix() const;

		void setMatrix(const glm::mat4& matrix);

		void multMatrix(const glm::mat4& matrix);

		void rotate(float angle);

		void translate(const glm::vec2& pos);

		void popMatrix();

		void pushMatrix();

		template <typename T>
		void pushMatrix(T&& t) {
			static_assert(std::is_invocable_v<void()>, "type T must be callable void() .");
			pushMatrix();
			t();
			popMatrix();
		}

		void addPixel(const glm::vec2& point, Color color, float size = 1.f);

		void addPixelLine(std::initializer_list<glm::vec2> points, Color color);

		void addLine(const glm::vec2& p1, const glm::vec2& p2, float width, Color color);

		void addFilledHexagon(const glm::vec2& center, float radius, Color color, float startAngle = 0);

		void addRectangle(const glm::vec2& pos, const glm::vec2& size, Color color);

		void addRectangleImage(const glm::vec2& pos, const glm::vec2& size, const sdl::TextureView& textureView, Color color = sdl::color::White);

		void addCircle(const glm::vec2& center, float radius, Color color, const int iterations = 30, float startAngle = 0);

		void addCircleOutline(const glm::vec2& center, float radius, float width, Color color, const int iterations = 30, float startAngle = 0);

		void addPolygon(std::initializer_list<glm::vec2> points, Color color);
		
		void addHexagonImage(const glm::vec2& center, float radius, const sdl::TextureView& sprite, float startAngle = 0);

		void addHexagon(const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle = 0);

		void upload(sdl::Shader& shader);

		void clear();

	protected:
		using Batch = sdl::Batch<sdl::Vertex>;
		using BatchView = sdl::BatchView<sdl::Vertex>;

		glm::mat4& matrix() noexcept;

		int getMatrixIndex() const noexcept;

		void add(BatchView&& batchView, const sdl::TextureView& texture = {});

	private:
		struct BatchData {
			BatchView batchView;
			GLuint texture;
			int matrixIndex;
		};

		struct MatrixPair {
			glm::mat4 matrix;
			int lastIndex;
		};

		void bind(sdl::Shader& shader);

		void draw(sdl::Shader& shader, const BatchData& batchData);

		std::vector<MatrixPair> matrixes_;
		Batch batch_{GL_DYNAMIC_DRAW};
		std::vector<BatchData> batches_;
		sdl::VertexArrayObject vao_;
		int currentMatrixIndex_ = 0;
		bool initiated_ = false;
		bool dirty_ = true;
	};

	inline const glm::mat4& Graphic::getMatrix() const {
		assert(!matrixes_.empty());
		return matrixes_.back().matrix;
	}

	inline glm::mat4& Graphic::matrix() noexcept {
		assert(!matrixes_.empty());
		return matrixes_.back().matrix;
	}

	inline int Graphic::getMatrixIndex() const noexcept {
		return static_cast<int>(matrixes_.size() - 1);
	}

}

#endif
