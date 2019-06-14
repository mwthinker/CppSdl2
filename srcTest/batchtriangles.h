#ifndef BATCHTRIANGLES_H
#define BATCHTRIANGLES_H

#include "testshader2.h"

#include <glm/gtc/constants.hpp>
#include <sdl/batch.h>
#include <sdl/sprite.h>
#include <sdl/window.h>
#include <sdl/logger.h>

class BatchTriangles : public sdl::Batch<TestShader2> {
public:
	BatchTriangles(const std::shared_ptr<TestShader2>& shader, int maxVertexes) : Batch(GL_TRIANGLES, GL_DYNAMIC_DRAW, shader, maxVertexes) {
		sdl::logger()->info("[BatchTriangles] {} Mib\n", getVboSizeInMiB());
	}

	BatchTriangles(const std::shared_ptr<TestShader2>& shader) : Batch(GL_TRIANGLES, shader) {
	}

	virtual ~BatchTriangles() = default;

	void addTriangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3) {
		Batch::add(v1);
		Batch::add(v2);
		Batch::add(v3);
	}

	void addRectangle(float x, float y, float w, float h) {
		addTriangle(TestShader2::Vertex(x, y), TestShader2::Vertex(x + w, y), TestShader2::Vertex(x, y + h));
		addTriangle(TestShader2::Vertex(x, y + h), TestShader2::Vertex(x + w, y), TestShader2::Vertex(x + w, y + h));
	}

	void addRectangle(float x, float y, float w, float h, const sdl::Sprite& sprite) {
		int textureW = sprite.getTexture().getWidth();
		int textureH = sprite.getTexture().getHeight();

		addTriangle(TestShader2::Vertex(x, y, sprite.getX() / textureW, sprite.getY() / textureH),
			TestShader2::Vertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH),
			TestShader2::Vertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH));

		addTriangle(TestShader2::Vertex(x, y + h, sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH),
			TestShader2::Vertex(x + w, y, (sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH),
			TestShader2::Vertex(x + w, y + h, (sprite.getX() + sprite.getWidth()) / textureW, (sprite.getY() + sprite.getHeight()) / textureH));
	}

	void addRectangle(const TestShader2::Vertex& v1, const TestShader2::Vertex& v2, const TestShader2::Vertex& v3, const TestShader2::Vertex& v4) {
		addTriangle(v1, v2, v3);
		addTriangle(v3, v4, v1);
	}

	void addCircle(float x, float y, float radius, const int iterations = 40) {
		const float PI = 3.14159265359f;

		const TestShader2::Vertex v1(x, y);
		TestShader2::Vertex v2(x + radius, y);
		for (int i = 1; i <= iterations; ++i) {
			TestShader2::Vertex v3(x + radius * std::cos(2 * PI * i / iterations), y + radius * std::sin(2 * PI * i / iterations));
			addTriangle(v1, v2, v3);
			v2 = v3;
		}
	}

	void addLine(float x1, float y1, float x2, float y2, float w) {
		float angle = atan2(x2 - x1, y2 - y1);
		float dx = 0.5f * w * std::cos(angle);
		float dy = 0.5f * w * std::sin(angle);
		addRectangle(TestShader2::Vertex(x1 - dx, y1 + dy), TestShader2::Vertex(x1 + dx, y1 - dy), TestShader2::Vertex(x2 + dx, y2 - dy), TestShader2::Vertex(x2 - dx, y2 + dy));
	}

	void addCircle(float x, float y, float outerRadius, float innerRadius, const int iterations = 40) {
		constexpr float PI = glm::pi<GLfloat>();

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

	void draw() const override {
		if (sdl::Window::getOpenGlMajorVersion() >= 2) {
			sdl::Batch<TestShader2>::draw();
		} else {
			int i = 0;
			int nbr = 0;
			bool lastTexture = true;
			for (const TestShader2::Vertex& v : *this) {
				bool texture = v.texture_ > 0.5f;
				if (i == 0) {
					lastTexture = texture;
					if (lastTexture) {
						glEnable(GL_TEXTURE_2D);
					}
					glBegin(GL_TRIANGLES);
				} else if (nbr == 0 && lastTexture != texture) {
					glEnd();
					lastTexture = texture;
					if (lastTexture) {
						glEnable(GL_TEXTURE_2D);
					} else {
						glDisable(GL_TEXTURE_2D);
					}
					glBegin(GL_TRIANGLES);
				}
				
				if (texture) {
					glTexCoord2f(v.tex_.x, v.tex_.y);
				}
				glVertex2f(v.pos_.x, v.pos_.y);
				glColor4f(v.color_.r, v.color_.g, v.color_.b, v.color_.a);
				
				nbr = (nbr + 1) % 3;
				++i;
			}
			glEnd();
			if (lastTexture) {
				glDisable(GL_TEXTURE_2D);
			}
		}
	}

};

#endif // BATCHTRIANGLES_H
