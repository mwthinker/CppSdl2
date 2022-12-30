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

	void addLine(BatchIndexed<Vertex>& batch, const glm::vec2& p1, const glm::vec2& p2, float width, Color color);

	void addRectangle(BatchIndexed<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, Color color);

	void addRectangleImage(BatchIndexed<Vertex>& batch, const glm::vec2& pos, const glm::vec2& size, const TextureView& sprite, Color color = color::White);

	void addHexagonImage(BatchIndexed<Vertex>& batch, const glm::vec2& center, float radius, const TextureView& sprite, float startAngle);

	void addHexagon(BatchIndexed<Vertex>& batch, const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle);

	void addCircle(BatchIndexed<Vertex>& batch, const glm::vec2& center, float radius, Color color, const int iterations, float startAngle);

	void addCircleOutline(BatchIndexed<Vertex>& batch, const glm::vec2& center, float radius, float width, Color color, const int iterations, float startAngle);

	void addPolygon(BatchIndexed<Vertex>& batch, std::initializer_list<glm::vec2> points, Color color);

	void addPolygon(BatchIndexed<Vertex>& batch, std::input_iterator auto begin, std::input_iterator auto end, Color color);

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

		void pushMatrix(std::invocable auto&& t) {
			pushMatrix();
			t();
			popMatrix();
		}

		void addPixel(const glm::vec2& point, Color color, float size = 1.f);

		void addPixelLine(std::initializer_list<glm::vec2> points, Color color);

		void addPixelLine(std::input_iterator auto begin, std::input_iterator auto end, Color color);

		void addLine(const glm::vec2& p1, const glm::vec2& p2, float width, Color color);

		void addFilledHexagon(const glm::vec2& center, float radius, Color color, float startAngle = 0);

		void addRectangle(const glm::vec2& pos, const glm::vec2& size, Color color);

		void addRectangleImage(const glm::vec2& pos, const glm::vec2& size, const sdl::TextureView& textureView, Color color = sdl::color::White);

		void addCircle(const glm::vec2& center, float radius, Color color, const int iterations = 30, float startAngle = 0);

		void addCircleOutline(const glm::vec2& center, float radius, float width, Color color, const int iterations = 30, float startAngle = 0);

		void addPolygon(std::initializer_list<glm::vec2> points, Color color);

		void addPolygon(std::input_iterator auto begin, std::input_iterator auto end, Color color);
		
		void addHexagonImage(const glm::vec2& center, float radius, const sdl::TextureView& sprite, float startAngle = 0);

		void addHexagon(const glm::vec2& center, float innerRadius, float outerRadius, Color color, float startAngle = 0);

		void upload(sdl::Shader& shader);

		void clear();

	protected:
		using Batch = sdl::BatchIndexed<sdl::Vertex>;
		using BatchView = sdl::BatchView<sdl::Vertex>;

		glm::mat4& matrix() noexcept;

		int getMatrixIndex() const noexcept;

		void add(BatchView&& batchView, const sdl::TextureView& texture = {});

	private:
		struct BatchData {
			BatchView batchView;
			gl::GLuint texture;
			int matrixIndex;
		};

		struct MatrixPair {
			glm::mat4 matrix;
			int lastIndex;
		};

		void bind(sdl::Shader& shader);

		void draw(sdl::Shader& shader, const BatchData& batchData);

		std::vector<MatrixPair> matrixes_;
		Batch batch_{gl::GL_DYNAMIC_DRAW};
		std::vector<BatchData> batches_;
		sdl::VertexArrayObject vao_;
		int currentMatrixIndex_ = 0;
		bool initiated_ = false;
		bool dirty_ = true;
	};

	inline void Graphic::addPolygon(std::initializer_list<glm::vec2> points, Color color) {
		addPolygon(points.begin(), points.end(), color);
	}

	void Graphic::addPolygon(std::input_iterator auto begin, std::input_iterator auto end, Color color) {
		batch_.startBatchView();
		sdl::graphic::addPolygon(batch_, begin, end, color);
		add(batch_.getBatchView(gl::GL_LINES));
	}

	inline void Graphic::addPixelLine(std::initializer_list<glm::vec2> points, Color color) {
		addPixelLine(points.begin(), points.end(), color);
	}

	void Graphic::addPixelLine(std::input_iterator auto begin, std::input_iterator auto end, Color color) {
		batch_.startBatchView();
		batch_.startAdding();

		for (auto it = begin; it != end; ++it) {
			batch_.pushBack({*it, {}, color});
		}

		const auto size = std::distance(begin, end);
		for (int i = 1; i < size; ++i) {
			batch_.pushBackIndex(i - 1);
			batch_.pushBackIndex(i);
		}

		add(batch_.getBatchView(gl::GL_LINES));
	}

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

namespace sdl::graphic {

	inline void addPolygon(BatchIndexed<Vertex>& batch, std::initializer_list<glm::vec2> points, Color color) {
		addPolygon(batch, points.begin(), points.end(), color);
	}

	void addPolygon(BatchIndexed<Vertex>& batch, std::input_iterator auto begin, std::input_iterator auto end, Color color) {
		batch.startAdding();
		for (auto it = begin; it != end; ++it) {
			batch.pushBack({*it, {}, color});
		}
		const auto size = std::distance(begin, end);
		for (int i = 1; i < size - 1; ++i) {
			batch.insertIndexes({0, i, i + 1});
		}
	}

}

#endif
