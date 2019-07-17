#include "batchtriangles.h"

#include <glm/gtc/constants.hpp>
#include <sdl/logger.h>

namespace {

    const auto PI = glm::pi<GLfloat>();

    inline Vec2 getHexCorner(Vec2 center, float size, int nbr) {
        auto angleDeg = 60 * nbr - 30;
        auto angleRad = PI / 180 * angleDeg;
        float x = center.x + size * std::cos(angleRad);
        return {center.x + size * std::cos(angleRad), center.y + size * std::sin(angleRad)};
    }

    inline TestShader2::Vertex createHexCornerVertex(const TestShader2::Vertex& vertex, float size, int nbr) {
        TestShader2::Vertex corner = vertex;
        corner.pos_ = getHexCorner(vertex.pos_, size, nbr);
        float x = corner.pos_.x;
        float y = corner.pos_.y;
        sdl::logger()->info("Pos: ({},{})", corner.pos_.x, corner.pos_.y);
        return  corner;
    }

}

BatchTriangles::BatchTriangles(const std::shared_ptr<TestShader2>& shader, int maxVertexes)
    : shader_(shader), batch_(GL_TRIANGLES, GL_DYNAMIC_DRAW, maxVertexes) {

    sdl::logger()->info("[BatchTriangles] {} Mib\n", batch_.getVboSizeInMiB());
}

BatchTriangles::BatchTriangles(const std::shared_ptr<TestShader2>& shader)
    : shader_(shader), batch_(GL_TRIANGLES, GL_STATIC_DRAW) {

}

void BatchTriangles::uploadToGraphicCard() {
    batch_.uploadToGraphicCard();
}

void BatchTriangles::addTriangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3) {
    batch_.add(v1);
    batch_.add(v2);
    batch_.add(v3);
}

void BatchTriangles::addRectangle(float x, float y, float w, float h) {
    addTriangle(TestShader2::Vertex(x, y), TestShader2::Vertex(x + w, y), TestShader2::Vertex(x, y + h));
    addTriangle(TestShader2::Vertex(x, y + h), TestShader2::Vertex(x + w, y), TestShader2::Vertex(x + w, y + h));
}

void BatchTriangles::addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite) {
    int textureW = sprite.getTexture().getWidth();
    int textureH = sprite.getTexture().getHeight();

    addTriangle(TestShader2::Vertex(x, y, sprite.getX() / textureW, sprite.getY() / textureH),
        TestShader2::Vertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH),
        TestShader2::Vertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH));

    addTriangle(TestShader2::Vertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH),
        TestShader2::Vertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH),
        TestShader2::Vertex(x + w, y + h, (sprite.getX() + sprite.getWidth()) / textureW, (sprite.getY() + sprite.getHeight()) / textureH));
}

void BatchTriangles::addRectangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3, const TestShader2::Vertex& v4) {
    addTriangle(v1, v2, v3);
    addTriangle(v3, v4, v1);
}

void BatchTriangles::addCircle(float x, float y, float radius, const int iterations) {
    const TestShader2::Vertex v1(x, y);
    TestShader2::Vertex v2(x + radius, y);
    for (int i = 1; i <= iterations; ++i) {
        TestShader2::Vertex v3(x + radius * std::cos(2 * PI * i / iterations), y + radius * std::sin(2 * PI * i / iterations));
        addTriangle(v1, v2, v3);
        v2 = v3;
    }
}

void BatchTriangles::addLine(float x1, float y1, float x2, float y2, float w) {
    float angle = atan2(x2 - x1, y2 - y1);
    float dx = 0.5f * w * std::cos(angle);
    float dy = 0.5f * w * std::sin(angle);
    addRectangle(TestShader2::Vertex(x1 - dx, y1 + dy), TestShader2::Vertex(x1 + dx, y1 - dy), TestShader2::Vertex(x2 + dx, y2 - dy), TestShader2::Vertex(x2 - dx, y2 + dy));
}

void BatchTriangles::addCircle(float x, float y, float outerRadius, float innerRadius, const int iterations) {
    TestShader2::Vertex v1(x + innerRadius, y);
    TestShader2::Vertex v4(x + outerRadius, y);
    for (int i = 1; i <= iterations; ++i) {
        TestShader2::Vertex v2(x + innerRadius * std::cos(2 * PI * i / iterations), y + innerRadius * std::sin(2 * PI * i / iterations));
        TestShader2::Vertex v3(x + outerRadius * std::cos(2 * PI * i / iterations), y + outerRadius * std::sin(2 * PI * i / iterations));
        addTriangle(v1, v3, v4);
        addTriangle(v1, v2, v3);
        v1 = v2;
        v4 = v3;
    }
}

void BatchTriangles::addHexagon(float x, float y, float size) {
    auto center = TestShader2::Vertex(x, y);
    center.color_ = Color(1.f, 0, 0, 1.f);
    for (int i = 0; i < 6; ++i) {
        batch_.add(center);
        batch_.add(createHexCornerVertex(center, size, i));
        batch_.add(createHexCornerVertex(center, size, (i + 1) % 6));
    }
}

void BatchTriangles::init() {
    shader_->useProgram();
    vao_.create();
    batch_.bindBuffer();
    shader_->setVertexAttribPointer();
}

void BatchTriangles::draw() const {
    shader_->useProgram();
    vao_.bind();
    batch_.draw();
}
