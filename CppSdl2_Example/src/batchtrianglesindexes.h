#ifndef BATCHTRIANGLESINEXES_H
#define BATCHTRIANGLESINEXES_H

#include "testshader.h"

#include <sdl/batch.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

class BatchTrianglesIndexes {
public:
	BatchTrianglesIndexes(std::shared_ptr<TestShader> shader, gl::GLenum usage);

	~BatchTrianglesIndexes() = default;

	float getVboSizeInMiB() const noexcept;

	void uploadToGraphicCard();

	void addTriangle(TestShader::Vertex v1, TestShader::Vertex v2, TestShader::Vertex v3);

	void addRectangle(float x, float y, float w, float h);

	void addRectangle(const TestShader::Vertex& v1, const TestShader::Vertex& v2, const TestShader::Vertex& v3, const TestShader::Vertex& v4);

	void addCircle(float x, float y, float radius, const int iterations = 40);

	void addHexagon(float x, float y, float size);

	void init();

	void draw();

private:
	std::shared_ptr<TestShader> shader_;
	sdl::BatchIndexed<TestShader::Vertex> batch_;
	sdl::VertexArrayObject vao_;
};

#endif
