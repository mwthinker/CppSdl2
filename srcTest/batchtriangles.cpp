#include "batchtriangles.h"

#include <glm/gtc/constants.hpp>
#include <sdl/logger.h>

namespace {

    const auto PI = glm::pi<GLfloat>();

    inline Vec2 getHexCorner(Vec2 center, float size, int nbr) {
        auto angleDeg = 60 * nbr - 30;
        auto angleRad = PI / 180 * angleDeg;
        return {center.x + size * std::cos(angleRad), center.y + size * std::sin(angleRad)};
    }

    inline TestShader::Vertex createHexCornerVertex(const TestShader::Vertex& vertex, float size, int nbr) {
		TestShader::Vertex corner = vertex;
        corner.pos_ = getHexCorner(vertex.pos_, size, nbr);
        float x = corner.pos_.x;
        float y = corner.pos_.y;
        //sdl::logger()->info("Pos: ({},{})", corner.pos_.x, corner.pos_.y);
        return  corner;
    }

}

BatchTriangles::BatchTriangles(const std::shared_ptr<TestShader>& shader, GLenum usage)
	: shader_{shader}, batch_{usage} {

	assert(shader);
}

float BatchTriangles::getVboSizeInMiB() const noexcept {
	return batch_.getVboSizeInMiB();
}

void BatchTriangles::uploadToGraphicCard() {
    batch_.uploadToGraphicCard();
}

void BatchTriangles::addTriangle(const TestShader::Vertex& v1, const TestShader::Vertex& v2, const TestShader::Vertex& v3) {
    batch_.pushBack(v1);
    batch_.pushBack(v2);
    batch_.pushBack(v3);
}

void BatchTriangles::addRectangle(float x, float y, float w, float h) {
	addTriangle({x, y}, {x + w, y}, {x, y + h});
	addTriangle({x, y + h}, {x + w, y}, {x + w, y + h});
}

void BatchTriangles::addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite) {
    int textureW = sprite.getTextureWidth();
    int textureH = sprite.getTextureHeight();

	addTriangle({x, y, (float) sprite.getX() / textureW, (float) sprite.getY() / textureH},
		{x + w, y, (float) (sprite.getX() + sprite.getWidth()) / textureW, (float) sprite.getY() / textureH},
        {x, y + h, (float) sprite.getX() / textureW, (float) (sprite.getY() + sprite.getHeight()) / textureH});

	addTriangle({x, y + h, (float) sprite.getX() / textureW, (float) (sprite.getY() + sprite.getHeight()) / textureH},
		{x + w, y, (float) (sprite.getX() + sprite.getWidth()) / textureW, (float) sprite.getY() / textureH},
		{x + w, y + h, (float) (sprite.getX() + sprite.getWidth()) / textureW, (float) (sprite.getY() + sprite.getHeight()) / textureH});
}

void BatchTriangles::addRectangle(const TestShader::Vertex& v1, const TestShader::Vertex& v2, const TestShader::Vertex& v3, const TestShader::Vertex& v4) {
    addTriangle(v1, v2, v3);
    addTriangle(v3, v4, v1);
}

void BatchTriangles::addCircle(float x, float y, float radius, const int iterations) {
	const TestShader::Vertex v1{x, y};
	TestShader::Vertex v2{x + radius, y};
    for (int i = 1; i <= iterations; ++i) {
		TestShader::Vertex v3{x + radius * std::cos(2 * PI * i / iterations), y + radius * std::sin(2 * PI * i / iterations)};
        addTriangle(v1, v2, v3);
        v2 = v3;
    }
}

void BatchTriangles::addLine(float x1, float y1, float x2, float y2, float w) {
    float angle = atan2(x2 - x1, y2 - y1);
    float dx = 0.5f * w * std::cos(angle);
    float dy = 0.5f * w * std::sin(angle);
	addRectangle({x1 - dx, y1 + dy}, {x1 + dx, y1 - dy}, {x2 + dx, y2 - dy}, {x2 - dx, y2 + dy});
}

void BatchTriangles::addCircle(float x, float y, float outerRadius, float innerRadius, const int iterations) {
	TestShader::Vertex v1{x + innerRadius, y};
	TestShader::Vertex v4{x + outerRadius, y};
    for (int i = 1; i <= iterations; ++i) {
		TestShader::Vertex v2{x + innerRadius * std::cos(2 * PI * i / iterations), y + innerRadius * std::sin(2 * PI * i / iterations)};
		TestShader::Vertex v3{x + outerRadius * std::cos(2 * PI * i / iterations), y + outerRadius * std::sin(2 * PI * i / iterations)};
        addTriangle(v1, v3, v4);
        addTriangle(v1, v2, v3);
        v1 = v2;
        v4 = v3;
    }
}

void BatchTriangles::addHexagon(float x, float y, float size) {
	auto center = TestShader::Vertex{x, y};
	center.color_ = sdl::Color{1.f, 0, 0, 1.f};
    for (int i = 0; i < 6; ++i) {
        batch_.pushBack(center);
        batch_.pushBack(createHexCornerVertex(center, size, i));
        batch_.pushBack(createHexCornerVertex(center, size, (i + 1) % 6));
    }
}

void BatchTriangles::init() {
    shader_->useProgram();
    vao_.generate();
    vao_.bind();
    batch_.bind();
    shader_->setVertexAttribPointer();
}

void BatchTriangles::draw() const {
    shader_->useProgram();
    vao_.bind();
    batch_.draw(GL_TRIANGLES);
}
