#ifndef BATCHTRIANGLES_H
#define BATCHTRIANGLES_H

#include "testshader.h"

#include <sdl/batch.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

class BatchTriangles {
public:
	BatchTriangles(std::shared_ptr<TestShader> shader, GLenum usage);

	virtual ~BatchTriangles() = default;

	float getVboSizeInMiB() const noexcept;

	void uploadToGraphicCard();

	void addTriangle(const TestShader::Vertex& v1, const TestShader::Vertex& v2, const TestShader::Vertex& v3);

	void addRectangle(float x, float y, float w, float h);

	void addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite);

	void addRectangle(const TestShader::Vertex& v1, const TestShader::Vertex& v2, const TestShader::Vertex& v3, const TestShader::Vertex& v4);

	void addCircle(float x, float y, float radius, const int iterations = 40);

	void addLine(float x1, float y1, float x2, float y2, float w);

	void addCircle(float x, float y, float outerRadius, float innerRadius, const int iterations = 40);

	void addHexagon(float x, float y, float size);

	void init();

	void draw();

private:
	std::shared_ptr<TestShader> shader_;
	sdl::Batch<TestShader::Vertex> batch_;
	sdl::VertexArrayObject vao_;
};

#endif
