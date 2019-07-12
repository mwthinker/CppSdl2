#ifndef BATCHTRIANGLES_H
#define BATCHTRIANGLES_H

#include "testshader2.h"

#include <sdl/batch.h>
#include <sdl/sprite.h>

class BatchTriangles {
public:
	BatchTriangles(const std::shared_ptr<TestShader2>& shader, int maxVertexes);

	BatchTriangles(const std::shared_ptr<TestShader2>& shader);

	virtual ~BatchTriangles() = default;

	void uploadToGraphicCard();

	void addTriangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3);

	void addRectangle(float x, float y, float w, float h);

	void addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite);

	void addRectangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3, const TestShader2::Vertex& v4);

	void addCircle(float x, float y, float radius, const int iterations = 40);

	void addLine(float x1, float y1, float x2, float y2, float w);

	void addCircle(float x, float y, float outerRadius, float innerRadius, const int iterations = 40);

    void addHexagon(float x, float y, float size);

	void init();

	void draw() const;

private:
	std::shared_ptr<TestShader2> shader_;
	sdl::Batch<TestShader2::Vertex> batch_;
	sdl::VertexArrayObject vao_;
};

#endif // BATCHTRIANGLES_H
