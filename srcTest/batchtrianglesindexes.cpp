#include "batchtrianglesindexes.h"

#include <sdl/color.h>
#include <glm/gtc/constants.hpp>
#include "logger.h"

namespace {

    const auto PI = glm::pi<GLfloat>();

    inline Vec2 getHexCorner(Vec2 center, float size, int nbr) {
        auto angleDeg = 60 * nbr - 30;
        auto angleRad = PI / 180 * angleDeg;
        float x = center.x + size * std::cos(angleRad);
        return {center.x + size * std::cos(angleRad), center.y + size * std::sin(angleRad)};
    }

    inline TestShader::Vertex createHexCornerVertex(const TestShader::Vertex& vertex, float size, int nbr) {
		auto corner = vertex;
        corner.pos_ = getHexCorner(vertex.pos_, size, nbr);
        float x = corner.pos_.x;
        float y = corner.pos_.y;
        //sdl::logger()->info("Pos: ({},{})", corner.pos_.x, corner.pos_.y);
        return  corner;
    }

	const sdl::Color COLOR = {0.5f, 1.f, 0, 1.f};

}

BatchTrianglesIndexes::BatchTrianglesIndexes(const std::shared_ptr<TestShader>& shader, GLenum usage)
	: shader_{shader}, batch_{usage} {

	assert(shader);
}

float BatchTrianglesIndexes::getVboSizeInMiB() const noexcept {
	return batch_.getVboSizeInMiB();
}

void BatchTrianglesIndexes::uploadToGraphicCard() {
    batch_.uploadToGraphicCard();
}

void BatchTrianglesIndexes::addTriangle(TestShader::Vertex v1, TestShader::Vertex v2, TestShader::Vertex v3) {
	batch_.startAdding();

	v1.color_ = COLOR;
	v2.color_ = COLOR;
	v3.color_ = COLOR;

	batch_.add(v1, v2, v3);
	batch_.addIndexes(0, 1, 2);
}

void BatchTrianglesIndexes::addRectangle(float x, float y, float w, float h) {
	addTriangle({x, y}, {x + w, y}, {x, y + h});
	addTriangle({x, y + h}, {x + w, y}, {x + w, y + h});
}

void BatchTrianglesIndexes::addRectangle(const TestShader::Vertex& v1, const TestShader::Vertex& v2, const TestShader::Vertex& v3, const TestShader::Vertex& v4) {
    addTriangle(v1, v2, v3);
    addTriangle(v3, v4, v1);
}

void BatchTrianglesIndexes::addCircle(float x, float y, float radius, const int iterations) {
	batch_.startAdding();
	TestShader::Vertex center{x, y};
	center.color_ = COLOR;
	batch_.add(center);
    for (int i = 0; i < iterations; ++i) {
		float rad = 2 * PI * i / iterations;
		TestShader::Vertex circleVertex{x + radius * std::cos(rad), y + radius * std::sin(rad)};
		circleVertex.color_ = COLOR;
		batch_.add(circleVertex);
    }
	for (int i = 1; i <= iterations; ++i) {
		batch_.addIndexes(0, i, (i % iterations) + 1);
	}
}

void BatchTrianglesIndexes::addHexagon(float x, float y, float size) {
	batch_.startAdding();
	TestShader::Vertex center{x, y};
    center.color_ = COLOR;
	batch_.add(center);
    for (int i = 0; i < 6; ++i) {
		auto vertex = createHexCornerVertex(center, size, i);
		vertex.color_ = COLOR;
        batch_.add(vertex);
    }
	for (int i = 1; i <= 6; ++i) {
		batch_.addIndexes(0, i, (i % 6) + 1);
	}
}

void BatchTrianglesIndexes::init() {
    shader_->useProgram();
    vao_.generate();
    vao_.bind();
    batch_.bind();
    shader_->setVertexAttribPointer();
}

void BatchTrianglesIndexes::draw() const {
    shader_->useProgram();
    vao_.bind();
    batch_.draw(GL_TRIANGLES);
}
