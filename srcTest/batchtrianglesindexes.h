#ifndef BATCHTRIANGLESINEXES_H
#define BATCHTRIANGLESINEXES_H

#include "testshader2.h"

#include <sdl/batch.h>
#include <sdl/sprite.h>
#include <sdl/vertexarrayobject.h>

class BatchTrianglesIndexes {
public:
	BatchTrianglesIndexes(const std::shared_ptr<TestShader2>& shader, GLenum usage);

	~BatchTrianglesIndexes() = default;

	float getVboSizeInMiB() const noexcept;

	void uploadToGraphicCard();

	void addTriangle(TestShader2::Vertex v1, TestShader2::Vertex v2, TestShader2::Vertex v3);

	void addRectangle(float x, float y, float w, float h);

	void addRectangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3, const TestShader2::Vertex& v4);

	void addCircle(float x, float y, float radius, const int iterations = 40);

    void addHexagon(float x, float y, float size);

	void init();

	void draw() const;

private:
	std::shared_ptr<TestShader2> shader_;
	sdl::Batch<TestShader2::Vertex> batch_;
	sdl::VertexArrayObject vao_;
};

#endif // BATCHTRIANGLESINEXES_H
